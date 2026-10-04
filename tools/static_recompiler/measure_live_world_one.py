#!/usr/bin/env python3
"""Run functional boots and serialize live World 1-1 frame-time measurements."""
import argparse
import base64
import datetime
import fcntl
import hashlib
import json
import math
import os
import re
from pathlib import Path
import shlex
import statistics
import subprocess
import signal
import sys
import time
from urllib.request import urlopen
import uuid

COORDINATION = Path('/Users/exgota/super-mario-3d-land-browser/build/runtime_coordination')
ROOT = Path(__file__).resolve().parents[2]
PRIMARY = Path('/Users/exgota/super-mario-3d-land-browser')
QUEUE = COORDINATION / 'measurement_queue'
DEFAULT_REFERENCE = ROOT / 'build/browser_session_preparation/server_reference_9000'
DEFAULT_SCHEDULE = PRIMARY / 'build/root_native_block_scheduling/build/block_scheduled_native/block_schedule.bin'
DEFAULT_DUMP = PRIMARY / 'build/root_port_reference/owned_dump.3ds'
DEFAULT_SERVER = ROOT / 'build/browser_performance_resume/serve_browser_execution_fast_frames.py'
DUMP_SHA256 = 'c83f9175208b3d60898d3a6d60451bee3108521e409cace362f63b46dda8c976'


def write_json(path, value):
    temporary = path.with_suffix('.temporary')
    temporary.write_text(json.dumps(value, indent=2) + '\n')
    temporary.replace(path)


def digest(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


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


def analyze(observation):
    if not observation.get('complete'):
        raise ValueError('Live measurement did not complete')
    return {'scope': observation['scope'], 'display': summary([row['interval'] for row in observation['rows']]),
            'started_utc': observation['startedUtc'], 'ended_utc': observation['endedUtc'],
            'audio_delta_scope': 'Entire40second observation, including10second warmup',
            'audio_underrun_delta': observation['finalAudio']['underrun_frames'] - observation['initialAudio']['underrun_frames'],
            'human_acceptance': False}


def reservation(operation, lane=None, kind=None, token=None):
    COORDINATION.mkdir(parents=True, exist_ok=True)
    with (COORDINATION / 'arbitration.lock').open('a') as stream:
        fcntl.flock(stream, fcntl.LOCK_EX)
        state_path = COORDINATION / 'reservation.json'
        state = json.loads(state_path.read_text()) if state_path.exists() else None
        if operation == 'status':
            return {'reservation': state, 'resources': resources()}
        if operation == 'reserve':
            if state:
                raise RuntimeError('Reservation already held: ' + json.dumps(state))
            if not lane or not kind:
                raise ValueError('Specify lane and kind')
            if os.statvfs(COORDINATION).f_bavail * os.statvfs(COORDINATION).f_frsize < 12_000_000_000:
                raise RuntimeError('Free disk below12GB')
            state = {'lane': lane, 'kind': kind, 'token': uuid.uuid4().hex, 'started_utc': timestamp()}
            write_json(state_path, state)
            write_json(COORDINATION / (state['token'] + '.json'), {'reservation': state, 'samples': [resources()]})
            return state
        if not state or token != state['token']:
            raise RuntimeError('Reservation token does not match')
        receipt_path = COORDINATION / (state['token'] + '.json')
        receipt = json.loads(receipt_path.read_text())
        receipt['samples'].append(resources())
        if operation == 'release':
            receipt['released_utc'] = timestamp()
            state_path.unlink()
        write_json(receipt_path, receipt)
        return {'reservation': state, 'released': operation == 'release', 'receipt': str(receipt_path)}


def enqueue(args):
    repository = (args.repository or ROOT).resolve()
    module = args.module.resolve()
    server = (args.server or DEFAULT_SERVER).resolve()
    reference = (args.reference or DEFAULT_REFERENCE).resolve()
    if not module.is_relative_to(repository / 'build') or not server.is_file():
        raise ValueError('Candidate module must be under its checkout build directory and server must exist')
    manifest = json.loads((module / 'build_manifest.json').read_text())
    if not manifest.get('passed') or not manifest.get('gameplay_session_supported'):
        raise ValueError('Candidate is not a linked gameplay module')
    for key in ('module', 'wasm'):
        path = Path(manifest[key]).resolve()
        if path.parent != module or not path.is_file():
            raise ValueError('Candidate module path differs or is missing')
    initial = reference / 'initial_user_state'
    for path in sorted(initial.rglob('*')):
        if path.is_symlink():
            raise ValueError('Reference tree contains a symbolic link')
    if not any(path.is_file() for path in initial.rglob('*')):
        raise ValueError('Reference tree is empty')
    navigation = json.loads(args.navigation.read_text()) if args.navigation else {'schema': 1, 'strategy': 'calibration', 'deadline_seconds': 120}
    if navigation.get('schema') != 1 or not 20 <= navigation.get('deadline_seconds', 120) <= 600:
        raise ValueError('Navigation recipe/deadline differs')
    if navigation.get('strategy') == 'recorded_navigation':
        if not navigation.get('events') or not navigation.get('world_anchor'):
            raise ValueError('Recorded navigation requires delivered input changes and an observed World anchor')
    elif navigation.get('strategy') == 'smoke' and args.operation != 'functional-boot':
        raise ValueError('Smoke boot cannot admit a frame-time measurement')
    elif navigation.get('strategy') not in ('calibration', 'smoke') and (not navigation.get('map_anchor') or not navigation.get('world_anchor')):
        raise ValueError('World navigation requires both observed scene anchors')
    QUEUE.mkdir(parents=True, exist_ok=True)
    identifier = datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%dT%H%M%S%f') + '_' + uuid.uuid4().hex[:8]
    job = {'schema': 1, 'identifier': identifier, 'lane': args.lane, 'state': 'functional_pending' if args.operation == 'functional-boot' else 'queued', 'queued_utc': timestamp(),
           'repository': str(repository), 'module': str(module), 'server': str(server), 'reference': str(reference),
           'source_commit': subprocess.check_output(['git', 'rev-parse', 'HEAD'], cwd=repository, text=True).strip(),
           'functional_only': args.operation == 'functional-boot', 'navigation': navigation,
           'measurement_protocol': 'functional boot only' if args.operation == 'functional-boot' else '10second warmup plus30second displayed-frame window;40second audio'}
    write_json(QUEUE / (identifier + '.json'), job)
    return job


def evaluate(browser, expression):
    output = browser.run(['eval', expression])
    marker = '### Result\n'
    if marker not in output:
        raise RuntimeError('Browser evaluation has no structured result')
    return json.JSONDecoder().raw_decode(output.split(marker, 1)[1].lstrip())[0]


def navigation_script(recipe):
    # Inputs use the existing page handlers. Scene anchors are local diagnostic data.
    return """(async () => {
 const page = await import('/BrowserCapturePage.mjs'), recipe = RECIPE;
 const canvas = document.querySelector('#top-screen'), started = performance.now();
 const state = window.worldEntry = {startedUtc:new Date().toISOString(), events:[], stage:'title_story', complete:false, elapsedSeconds:0};
 const held = new Set();
 const key = (code, down) => {canvas.dispatchEvent(new KeyboardEvent(down?'keydown':'keyup',{code,bubbles:true})); down?held.add(code):held.delete(code);};
 const sleep = ms => new Promise(resolve=>setTimeout(resolve,ms));
 const press = async (code, ms=300) => {key(code,true);await sleep(ms);key(code,false);};
 const anchor = specification => {
  if(!specification) return false;
  const data=canvas.getContext('2d').getImageData(specification.x,specification.y,specification.width,specification.height).data;
  let error=0;const expected=specification.rgb;
  for(let i=0,j=0;i<data.length;i+=4,j+=3) for(let c=0;c<3;++c) error+=Math.abs(data[i+c]-expected[j+c]);
  return error/expected.length<=specification.maximum_mean_error;
 };
 const record = () => {state.elapsedSeconds=(performance.now()-started)/1000;state.frame=page.previewFrameObservation();};
 state.stop=()=>{state.cancelled=true;for(const code of held)key(code,false);};
 canvas.focus();
 try {
  if(recipe.strategy==='smoke') {
   while(!state.cancelled && performance.now()-started<recipe.deadline_seconds*1000) {
    record();
    if(state.frame){state.stage='first_frame';state.complete=true;break;}
    if(document.body.dataset.captureState==='failed')throw new Error(document.querySelector('#capture-error').textContent);
    await sleep(50);
   }
  } else if(recipe.strategy==='recorded_navigation') {
   const codes=['KeyZ','KeyX','ShiftLeft','Enter','KeyL','KeyJ','KeyI','KeyK','KeyW','KeyQ','KeyS','KeyA'];
   const initial=page.previewFrameObservation();if(!initial)throw new Error('No displayed frame before navigation');
   const shift=BigInt(initial.sampled_ticks)-BigInt(recipe.events[0].ticks);
   let index=0,previousButtons=0,previousDirection='';
   state.stage='recorded_navigation';state.guest_tick_offset=String(shift);
   while(!state.cancelled && performance.now()-started<recipe.deadline_seconds*1000) {
    const frame=page.previewFrameObservation();
    if(frame) while(index<recipe.events.length && BigInt(frame.sampled_ticks)>=BigInt(recipe.events[index].ticks)+shift) {
     const event=recipe.events[index++];
     for(let bit=0;bit<codes.length;++bit) if((event.buttons^previousButtons)&(1<<bit))key(codes[bit],!!(event.buttons&(1<<bit)));
     previousButtons=event.buttons;
     const direction=event.circle_x>40?'ArrowRight':event.circle_x< -40?'ArrowLeft':event.circle_y>40?'ArrowUp':event.circle_y< -40?'ArrowDown':'';
     if(direction!==previousDirection){if(previousDirection)key(previousDirection,false);if(direction)key(direction,true);previousDirection=direction;}
     state.delivered_input_changes=index;
    }
    if(index===recipe.events.length && anchor(recipe.world_anchor)){state.stage='world';state.complete=true;break;}
    if(document.body.dataset.captureState==='failed')throw new Error(document.querySelector('#capture-error').textContent);
    record();await sleep(8);
   }
  } else {
  while(!state.cancelled && performance.now()-started<recipe.deadline_seconds*1000) {
   if(document.body.dataset.captureState==='failed') throw new Error(document.querySelector('#capture-error').textContent);
   if(anchor(recipe.world_anchor)) {state.stage='world';state.complete=true;break;}
   if(state.stage==='title_story' && anchor(recipe.map_anchor)) {
    state.stage='map';state.events.push({stage:'map',elapsedSeconds:(performance.now()-started)/1000});
    await sleep(1200);await press('ArrowRight',800);await sleep(2000);await press('KeyZ',800);state.stage='entering';
   } else if(state.stage==='title_story') {
    await press('KeyZ');
    if(recipe.press_start && performance.now()-started>recipe.start_after_seconds*1000) {await sleep(500);await press('Enter');}
    await sleep(1500);
   } else await sleep(250);
   record();
  }
  }
  record();if(!state.complete && !state.cancelled)state.error='World entry deadline exceeded';
 } catch(error) {state.error=String(error);} finally {for(const code of held)key(code,false);state.finishedUtc=new Date().toISOString();state.finished=true;}
})()""".replace('RECIPE', json.dumps(recipe))


def run_job(job_path):
    from browser_session_policy import BrowserSession
    from browser_process_identity import process_identity, identity_matches
    job = json.loads(job_path.read_text())
    functional = job.get('functional_only', False)
    token = None if functional else reservation('reserve', job['lane'], 'measurement')['token']
    output = ROOT / 'build/runtime_measurements' / job['identifier']
    output.mkdir(parents=True)
    server_output = Path(job['repository']) / 'build/runtime_measurements' / (job['identifier'] + '_server')
    server_output.parent.mkdir(parents=True, exist_ok=True)
    import socket
    with socket.socket() as probe:
        probe.bind(('127.0.0.1', 0)); port = probe.getsockname()[1]
    command = [sys.executable, job['server'], job['module'], str(server_output), '--block-schedule', str(DEFAULT_SCHEDULE),
               '--reference', job['reference'], '--dump-sha256', DUMP_SHA256, '--dump-bytes', '536870912',
               '--gameplay-session-presentations', '60000', '--gameplay-input-mode', 'record', '--stream-audio', '--frame-output',
               '--wall-time-seconds', '900', '--port', str(port)]
    job.update(state='running', started_utc=timestamp(), token=token, output=str(output), server_output=str(server_output),
               origin=f'http://127.0.0.1:{port}', collector_sha256=hashlib.sha256(browser_script().encode()).hexdigest(),
               script_sha256=digest(Path(__file__)), server_command=command)
    write_json(job_path, job)
    server = None
    browser_cleanup = None
    browser_attempted = False
    server_absent = False
    started = time.monotonic()
    try:
        log = (output / 'server.log').open('wb')
        server = subprocess.Popen(command, cwd=job['repository'], stdout=log, stderr=subprocess.STDOUT, start_new_session=True)
        log.close()
        server_identity = process_identity(server.pid)
        write_json(output / 'server_identity.json', server_identity)
        for _ in range(100):
            if server.poll() is not None:
                raise RuntimeError('Owned server exited before admission')
            try:
                with urlopen(job['origin'] + '/configuration.json', timeout=2) as response:
                    configuration = json.load(response)
                    headers = dict(response.headers)
                break
            except OSError:
                time.sleep(.2)
        else:
            raise RuntimeError('Owned server admission deadline exceeded')
        write_json(output / 'configuration.json', {'configuration': configuration, 'headers': headers})
        browser_directory = output / 'browser'
        browser_directory.mkdir()
        browser_attempted = True
        with BrowserSession(browser_directory, headed=True, repository=ROOT) as browser:
            browser.run(['open', job['origin'] + '/'])
            browser.run(['snapshot'])
            action = 'async(page)=>{await page.waitForFunction(()=>document.body.dataset.captureState==="ready");'
            action += 'await page.locator("#game-file").setInputFiles(' + json.dumps(str(DEFAULT_DUMP)) + ');'
            action += 'await page.locator("#run-preview").click();await page.waitForFunction(()=>document.body.dataset.captureState==="running",null,{timeout:90000});'
            action += 'await page.locator("#play-recorded-sound").click();await page.waitForFunction(()=>document.body.dataset.audioState==="running",null,{timeout:30000});'
            action += 'await page.waitForFunction(()=>!document.querySelector("#gameplay-buttons").disabled,null,{timeout:90000});}'
            browser.run(['run-code', action], timeout=180)
            job['navigation_started_utc'] = timestamp()
            # Start asynchronously so progress sampling and evidence do not depend on a long CLI request.
            script = navigation_script(job['navigation'])
            evaluate(browser, '() => {window.worldEntryPromise=' + script + ';return true;}')
            resource_samples = []
            while True:
                state = evaluate(browser, '() => {const {stop,...state}=window.worldEntry;return state;}')
                resource_samples.append(resources())
                write_json(output / 'navigation.json', state)
                write_json(output / 'resources.json', resource_samples)
                screens = evaluate(browser, '() => Object.fromEntries(["top-screen","bottom-screen"].map(id=>[id,document.getElementById(id).toDataURL("image/png")]))')
                for name, data in screens.items():
                    (output / (name + '_' + str(len(resource_samples)) + '.png')).write_bytes(base64.b64decode(data.split(',', 1)[1]))
                if state.get('finished'):
                    break
                time.sleep(5)
            job['entry_seconds_from_queue_admission'] = time.monotonic() - started
            job['entry_under_two_minutes'] = job['entry_seconds_from_queue_admission'] < 120 and state.get('complete') is True
            if not state.get('complete'):
                browser.run(['run-code', 'async(page)=>{await page.locator("#run-preview").click();await page.waitForFunction(()=>["complete","failed"].includes(document.body.dataset.captureState),null,{timeout:90000});}'], timeout=100)
                raise RuntimeError(state.get('error', 'World entry did not complete'))
            job['browser_identity'] = evaluate(browser, '() => ({userAgent:navigator.userAgent,origin:location.origin,crossOriginIsolated,secureContext:isSecureContext})')
            job['startup_diagnostics'] = evaluate(browser, 'async () => {const page=await import("/BrowserCapturePage.mjs");return typeof page.runtimeInitializationObservation==="function"?page.runtimeInitializationObservation():null;}')
            if not functional:
                evaluate(browser, '() => ' + browser_script())
                for _ in range(3):
                    time.sleep(15)
                    resource_samples.append(resources())
                    write_json(output / 'resources.json', resource_samples)
                observation = evaluate(browser, '() => window.liveWorldMeasurement')
                write_json(output / 'observation.json', observation)
                write_json(output / 'analysis.json', analyze(observation))
            browser.run(['run-code', 'async(page)=>{await page.locator("#run-preview").click();await page.waitForFunction(()=>["complete","failed"].includes(document.body.dataset.captureState),null,{timeout:90000});}'], timeout=100)
            job['final_browser_state'] = evaluate(browser, '() => ({state:document.body.dataset.captureState,identifier:document.body.dataset.captureIdentifier,error:document.querySelector("#capture-error").textContent})')
            if job['final_browser_state']['state'] != 'complete':
                raise RuntimeError('Natural Stop failed: ' + job['final_browser_state']['error'])
        browser_cleanup = json.loads((browser_directory / 'cleanup.json').read_text())
        job['state'] = 'complete'
    except BaseException as error:
        job['state'], job['error'] = 'failed', str(error)
        cleanup_path = output / 'browser/cleanup.json'
        if cleanup_path.exists():
            browser_cleanup = json.loads(cleanup_path.read_text())
    finally:
        if server is not None:
            if server.poll() is None:
                # Only this Popen child, after its process identity has been checked.
                if identity_matches(server_identity) is not True:
                    raise RuntimeError('Owned server identity changed; cleanup refused')
                server.send_signal(signal.SIGINT)
                try:
                    server.wait(timeout=10)
                except subprocess.TimeoutExpired:
                    server.terminate(); server.wait(timeout=5)
            server_absent = server.poll() is not None and identity_matches(server_identity) is False
        job.update(ended_utc=timestamp(), browser_cleanup=browser_cleanup, server_absence_observed=server_absent)
        if server_absent and ((not browser_attempted) or (browser_cleanup is not None and browser_cleanup.get('passed') is True)):
            if token is not None:
                job['release'] = reservation('release', token=token)
        else:
            job['cleanup_blocker'] = 'Reservation retained because owned helper absence is unproved'
        write_json(job_path, job)
        write_json(output / 'receipt.json', job)
    return job


def run_queue():
    QUEUE.mkdir(parents=True, exist_ok=True)
    with (QUEUE / 'runner.lock').open('a') as lock:
        fcntl.flock(lock, fcntl.LOCK_EX | fcntl.LOCK_NB)
        while True:
            pending = [path for path in sorted(QUEUE.glob('*.json'))
                       if json.loads(path.read_text()).get('state') == 'queued']
            if not pending:
                break
            if reservation('status')['reservation']:
                return {'state': 'waiting', 'reason': 'Existing frame-time measurement'}
            path = pending[0]
            result = run_job(path)
            print(json.dumps({'job': str(path), 'state': result['state'], 'output': result['output']}), flush=True)
            if result.get('cleanup_blocker'):
                return result
    return {'state': 'drained'}


parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('operation', choices=('status', 'reserve', 'sample', 'release', 'browser-script', 'analyze', 'enqueue', 'functional-boot', 'run-queue', 'queue-status'))
parser.add_argument('--lane')
parser.add_argument('--kind', choices=('measurement', 'build'))
parser.add_argument('--token')
parser.add_argument('--observation', type=Path)
parser.add_argument('--output', type=Path)
parser.add_argument('--repository', type=Path)
parser.add_argument('--module', type=Path)
parser.add_argument('--server', type=Path)
parser.add_argument('--reference', type=Path)
parser.add_argument('--navigation', type=Path)
args = parser.parse_args()
if args.operation == 'browser-script':
    print(browser_script())
elif args.operation == 'analyze':
    observation = json.loads(args.observation.read_text())
    report = analyze(observation)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report))
elif args.operation in ('enqueue', 'functional-boot'):
    if not args.module or not args.lane:
        raise ValueError('Specify lane and module')
    job = enqueue(args)
    if args.operation == 'functional-boot':
        print(json.dumps(run_job(QUEUE / (job['identifier'] + '.json'))))
    else:
        print(json.dumps({'identifier':job['identifier'], 'lane':job['lane'], 'state':job['state'],
                          'job':str(QUEUE / (job['identifier'] + '.json'))}))
elif args.operation == 'run-queue':
    print(json.dumps(run_queue()))
elif args.operation == 'queue-status':
    rows = []
    for path in sorted(QUEUE.glob('*.json')):
        job = json.loads(path.read_text())
        rows.append({key:job.get(key) for key in ('identifier','lane','state','output','error','cleanup_blocker')})
    print(json.dumps(rows))
else:
    print(json.dumps(reservation(args.operation, args.lane, args.kind, args.token)))
