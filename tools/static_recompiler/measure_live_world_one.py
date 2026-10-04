#!/usr/bin/env python3
"""Coordinate runtime builds and collect comparable live World 1-1 measurements."""
import argparse
import datetime
import fcntl
import json
import math
import os
import re
from pathlib import Path
import shlex
import statistics
import subprocess
import uuid

COORDINATION = Path('/Users/exgota/super-mario-3d-land-browser/build/runtime_coordination')


def timestamp():
    return datetime.datetime.now(datetime.timezone.utc).isoformat()


def resources():
    rows = subprocess.check_output(['ps', '-ww', '-axo', 'pid=,ppid=,%cpu=,rss=,nice=,args='], text=True)
    inventory, factories, unknown = [], [], []
    for row in rows.splitlines():
        fields = row.strip().split(None, 5)
        if len(fields) != 6:
            continue
        pid, parent, cpu, rss, priority, arguments = fields
        try:
            tokens = shlex.split(arguments)
        except ValueError:
            unknown.append(int(pid))
            continue
        if not tokens:
            continue
        executable = Path(tokens[0]).name
        script = None
        if re.fullmatch(r'python(?:[0-9]+(?:\.[0-9]+)*)?', executable, re.IGNORECASE):
            index = 1
            while index < len(tokens):
                option = tokens[index]
                if option in ('-c', '-m'):
                    break
                if option in ('-W', '-X'):
                    index += 2
                    continue
                if option.startswith(('-W', '-X')) or option in ('-u', '-B', '-E', '-I', '-s', '-S', '-v', '-q', '-O', '-OO'):
                    index += 1
                    continue
                if option == '--':
                    index += 1
                    if index < len(tokens):
                        script = Path(tokens[index]).name
                    break
                if option.startswith('-'):
                    unknown.append(int(pid))
                else:
                    script = Path(option).name
                break
        item = {'pid': int(pid), 'parent_pid': int(parent), 'cpu_percent': float(cpu),
                'resident_kibibytes': int(rss), 'nice_priority': int(priority),
                'executable': executable, 'python_script': script}
        inventory.append(item)
        if script and script.lower() == 'factory.py':
            factories.append(item)
    return {'utc': timestamp(), 'load_averages': os.getloadavg(),
            'factory_processes': factories, 'factory_classification_complete': not unknown,
            'unclassified_process_pids': unknown,
            'factory_process_absence_observed': not factories and not unknown,
            'busiest_processes': sorted(inventory, key=lambda item: item['cpu_percent'], reverse=True)[:30]}


def browser_script():
    # Use this expression through the browser's evaluate tool. It measures displayed frames.
    return """(async () => {
 const page = await import('/BrowserCapturePage.mjs');
 if (window.liveWorldMeasurement?.timer) throw new Error('A live measurement is already running');
 const start = performance.now(), initialFrame = page.previewFrameObservation();
 const measurement = window.liveWorldMeasurement = {startedUtc:new Date().toISOString(),
  timeOrigin:performance.timeOrigin, initialFrame, initialAudio:page.streamedAudio()?.statistics(),
  rows:[], observations:[], timer:null, complete:false,
  scope:'Actual displayed frame observations. World identity must be visually verified. No human acceptance.'};
 let previousFrame = null, previousTime = null;
 measurement.timer = setInterval(() => {
  const now = performance.now(), elapsed = now - start, frame = page.previewFrameObservation();
  if (elapsed >= 40000) {
   clearInterval(measurement.timer); measurement.timer = null; measurement.complete = true;
   measurement.endedUtc = new Date().toISOString(); measurement.finalFrame = frame;
   measurement.finalAudio = page.streamedAudio()?.statistics();
   return;
  }
  if (elapsed < 10000 || !frame || frame.renderer_frame === previousFrame) return;
  if (previousTime !== null) measurement.rows.push({frame:frame.renderer_frame, host:now, interval:now-previousTime});
  measurement.observations.push({frame:frame.renderer_frame, sequence:frame.sequence, host:now});
  previousTime = now; previousFrame = frame.renderer_frame;
 }, 8);
 return {startedUtc:measurement.startedUtc, durationSeconds:40, warmupSeconds:10};
})()"""


def summary(values):
    ordered = sorted(values)
    if not ordered:
        raise ValueError('No live display intervals')
    percentile = lambda fraction: ordered[max(0, math.ceil(len(ordered) * fraction) - 1)]
    return {'intervals': len(values), 'elapsed_milliseconds': sum(values),
            'presentations_per_second': len(values) * 1000 / sum(values),
            'median_milliseconds': statistics.median(values),
            'p95_milliseconds': percentile(.95), 'p99_milliseconds': percentile(.99),
            'maximum_milliseconds': ordered[-1],
            'fraction_over_33_milliseconds': sum(value > 33 for value in values) / len(values)}


parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('operation', choices=('status', 'reserve', 'sample', 'release', 'browser-script', 'analyze'))
parser.add_argument('--lane')
parser.add_argument('--kind', choices=('measurement', 'build'))
parser.add_argument('--token')
parser.add_argument('--observation', type=Path)
parser.add_argument('--output', type=Path)
args = parser.parse_args()
if args.operation == 'browser-script':
    print(browser_script())
elif args.operation == 'analyze':
    observation = json.loads(args.observation.read_text())
    if not observation.get('complete'):
        raise ValueError('Live measurement did not complete')
    report = {'scope': observation['scope'], 'display': summary([row['interval'] for row in observation['rows']]),
              'started_utc': observation['startedUtc'], 'ended_utc': observation['endedUtc'],
              'audio_delta_scope': 'Entire40second observation, including10second warmup',
              'audio_underrun_delta': observation['finalAudio']['underrun_frames'] - observation['initialAudio']['underrun_frames'],
              'human_acceptance': False}
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report))
else:
    COORDINATION.mkdir(parents=True, exist_ok=True)
    with (COORDINATION / 'arbitration.lock').open('a') as stream:
        fcntl.flock(stream, fcntl.LOCK_EX)
        state_path = COORDINATION / 'reservation.json'
        state = json.loads(state_path.read_text()) if state_path.exists() else None
        if args.operation == 'status':
            print(json.dumps({'reservation': state, 'resources': resources()}))
        elif args.operation == 'reserve':
            if state:
                raise RuntimeError('Reservation already held: ' + json.dumps(state))
            if not args.lane or not args.kind:
                raise ValueError('Specify lane and kind')
            if args.kind == 'build' and os.statvfs(COORDINATION).f_bavail * os.statvfs(COORDINATION).f_frsize < 12_000_000_000:
                raise RuntimeError('Free disk below12GB')
            state = {'lane': args.lane, 'kind': args.kind, 'token': uuid.uuid4().hex, 'started_utc': timestamp()}
            state_path.write_text(json.dumps(state, indent=2) + '\n')
            (COORDINATION / (state['token'] + '.json')).write_text(json.dumps({'reservation': state, 'samples': [resources()]}, indent=2) + '\n')
            print(json.dumps(state))
        else:
            if not state or args.token != state['token']:
                raise RuntimeError('Reservation token does not match')
            receipt_path = COORDINATION / (state['token'] + '.json')
            receipt = json.loads(receipt_path.read_text())
            receipt['samples'].append(resources())
            if args.operation == 'release':
                receipt['released_utc'] = timestamp()
                state_path.unlink()
            receipt_path.write_text(json.dumps(receipt, indent=2) + '\n')
            print(json.dumps({'reservation': state, 'released': args.operation == 'release', 'receipt': str(receipt_path)}))
