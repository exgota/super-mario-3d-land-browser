#!/usr/bin/env python3
"""Verify a real bounded gameplay movie, HID/PCM/final pixels and owned browser closure."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import time

WORKTREE = Path(__file__).resolve().parents[2]
from browser_session_policy import BrowserSession
from audit_webassembly_platform import digest
from compare_input_capture import load_input_capture, load_movie, validate_movie_delivery

def require(condition, message):
    if not condition:
        raise RuntimeError(message)


parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('url')
parser.add_argument('output', type=Path)
parser.add_argument('--server-output', type=Path, required=True)
parser.add_argument('--reference', type=Path, required=True)
parser.add_argument('--movie', type=Path, required=True)
parser.add_argument('--dump', type=Path, required=True)
parser.add_argument('--timeout-seconds', type=int, default=600)
args = parser.parse_args()
output = args.output.resolve()
server = args.server_output.resolve()
reference = args.reference.resolve()
require(output.is_relative_to(WORKTREE / 'build') and not output.exists(), 'Browser gameplay validation failed at source line 28')
from urllib.parse import urlsplit
url = urlsplit(args.url)
if (url.scheme != 'http' or url.hostname != '127.0.0.1' or url.username or url.password or url.query or url.fragment or not 1 <= args.timeout_seconds <= 3600):
    raise ValueError('A local server and finite browser deadline are required')
configuration = json.loads((server / 'configuration.json').read_text())
options = configuration['options']
require(options['gameplay_input_mode'] == 'replay', 'Browser gameplay validation failed at source line 35')
require(options['stream_audio_output'] and options['frame_output'], 'Browser gameplay validation failed at source line 36')
require(all(not options.get(k, False) for k in ['live_button_capture', 'live_circle_pad_capture', 'live_touch_capture']), 'Browser gameplay validation failed at source line 37')
dump_hash = digest(args.dump)
output.mkdir()
states = []
with BrowserSession(output) as browser:
    def evaluate(code):
        text = browser.run(['eval', code])
        return json.JSONDecoder().raw_decode(text.split('### Result\n', 1)[1].lstrip())[0]
    browser.run(['open', args.url])
    action = "async (page) => { await page.waitForFunction(() => document.body.dataset.captureState === 'ready'); "
    action += "await page.getByLabel('Game file', {exact:true}).setInputFiles(" + json.dumps(str(args.dump.resolve())) + "); "
    action += "await page.getByRole('button', {name:'Run session', exact:true}).click(); "
    action += "await page.waitForFunction(() => document.body.dataset.captureState === 'running', null, {timeout:60000}); }"
    started = time.monotonic()
    browser.run(['run-code', action], timeout=90)
    before_audio = evaluate("async () => { const player = (await import('./BrowserCapturePage.mjs')).streamedAudio(); "
        "if (!player || player.context || player.state !== 'idle') throw Error('Audio started without gesture'); "
        "return player.statistics(); }")
    browser.run(['run-code', "async (page) => { await page.getByRole('button', {name:'Play preview sound', exact:true}).click(); }"])
    while True:
        state = evaluate("() => ({state:document.body.dataset.captureState, identifier:document.body.dataset.captureIdentifier, "
            "status:document.querySelector('#capture-status').textContent, error:document.querySelector('#capture-error').textContent, "
            "isolated:crossOriginIsolated, renderer_frame:Number(document.body.dataset.sampledRendererFrame ?? 0)})")
        states.append({**state, 'elapsed_seconds':time.monotonic()-started})
        (output / 'observed_states.json').write_text(json.dumps(states, indent=2)+'\n')
        print(json.dumps(states[-1]), flush=True)
        if state['state'] == 'complete': break
        if state['state'] == 'failed' or time.monotonic()-started > args.timeout_seconds:
            raise RuntimeError('Actual gameplay replay refused: '+json.dumps(state))
        time.sleep(10)
    identifier = state['identifier']
    require(re.fullmatch(r'capture_[0-9a-f]{32}', identifier) and state['isolated'], 'Browser gameplay validation failed at source line 68')
    capture = server / identifier
    receipt_path = server / (identifier+'_receipt.json')
    receipt = json.loads(receipt_path.read_text())
    manifest = json.loads((server / (identifier+'_manifest.json')).read_text())
    require(receipt['passed'] and all(digest(p) == h for p,h in receipt['protected_inputs'].items()), 'Browser gameplay validation failed at source line 73')
    require(all(digest(capture/p) == h for p,h in receipt['files'].items()), 'Browser gameplay validation failed at source line 74')
    require(not (capture / 'gpu_events.jsonl').exists(), 'Browser gameplay validation failed at source line 75')
    require(not list(capture.glob('pica_command_list_*.bin')), 'Browser gameplay validation failed at source line 76')
    require(len(manifest['counters']) == 2 and all(v['fallbacks'] == '0' for v in manifest['counters']), 'Both static CPU counters must be present with zero reached fallbacks')
    outcome = json.loads((capture / 'gameplay_session_outcome.json').read_text())
    require(outcome['complete'] and outcome['outcome'] == 'presentation_limit', 'Browser gameplay validation failed at source line 78')
    require(outcome['presentations'] == options['gameplay_session_presentations'], 'Browser gameplay validation failed at source line 79')
    require(outcome['gpu_trace_absent'] and outcome['pica_payload_files_absent'], 'Browser gameplay validation failed at source line 80')
    require(all(outcome[k] is False for k in ['exact_replay_accepted','section_7_accepted','goal_accepted']), 'Browser gameplay validation failed at source line 81')
    names = ['input_events.jsonl','audio_events.jsonl','audio_pcm_s16le.bin',
             'rendered_screen_0.rgba','rendered_screen_2.rgba',
             'framebuffer_screen_0.bin','framebuffer_screen_2.bin']
    equalities = {n:{'reference':digest(reference/n),'browser':digest(capture/n)} for n in names}
    require(all(v['reference'] == v['browser'] for v in equalities.values()), equalities)
    input_capture = load_input_capture(capture)
    movie = load_movie(args.movie)
    validate_movie_delivery(capture, movie, input_capture)
    initial = reference / 'initial_user_state'
    recorded = capture / 'initial_user_state'
    def files(path):
        return {str(p.relative_to(path)):digest(p) for p in path.rglob('*') if p.is_file() and str(p.relative_to(path)) != 'log/reference_capture.log'}
    require(files(initial) == files(recorded), 'Browser gameplay validation failed at source line 94')
    pixels = evaluate("async () => { const result={}; for(const name of ['top-screen','bottom-screen']) { const c=document.getElementById(name); "
        "const data=c.getContext('2d').getImageData(0,0,c.width,c.height).data; const hash=await crypto.subtle.digest('SHA-256',data); "
        "result[name]={width:c.width,height:c.height,hidden:c.hidden,sha256:[...new Uint8Array(hash)].map(v=>v.toString(16).padStart(2,'0')).join('')}; } return result; }")
    for canvas,name in [('top-screen','rendered_screen_0.rgba'),('bottom-screen','rendered_screen_2.rgba')]:
        require(not pixels[canvas]['hidden'] and pixels[canvas]['sha256'] == digest(capture/name), 'Browser gameplay validation failed at source line 99')
    browser.run(['run-code', "async (page) => { await page.waitForFunction(async () => { const p=(await import('./BrowserCapturePage.mjs')).streamedAudio(); "
        "return p?.state === 'ended' && p?.context?.state === 'closed'; }, null, {timeout:60000}); }"])
    audio = evaluate("async () => { const p=(await import('./BrowserCapturePage.mjs')).streamedAudio(); "
        "if (p?.state !== 'ended' || p?.context?.state !== 'closed') throw Error('Audio output did not drain and close'); "
        "return {statistics:p.statistics(),receipt:p.receipt,full_output_waveform_observed:false}; }")
    frames=(capture/'audio_pcm_s16le.bin').stat().st_size//4
    require(audio['statistics']['accepted_source_frames']==frames, 'Browser gameplay validation failed at source line 105')
    require(audio['statistics']['consumed_source_frames']==frames, 'Browser gameplay validation failed at source line 106')
    observation=manifest['observations']['audio_stream']
    pcm=(capture/'audio_pcm_s16le.bin').read_bytes();position=0
    for sequence,packet in enumerate(observation['packets']):
        length=packet['sample_frames']*4
        require(packet['sequence']==sequence and packet['first_sample_frame']*4==position, 'Browser gameplay validation failed at source line 111')
        require(0<length<=8192 and hashlib.sha256(pcm[position:position+length]).hexdigest()==packet['pcm_sha256'], 'Browser gameplay validation failed at source line 112')
        position+=length
    require(position==len(pcm) and observation['consumer']==audio['receipt'], 'Browser gameplay validation failed at source line 114')
    browser.run(['screenshot','--filename='+str(output/'gameplay.png')])
    result={'passed':True,'states':states,'capture':str(capture),'outcome':outcome,'equalities':equalities,
            'pixels':pixels,'audio':audio,'before_audio':before_audio,'capture_receipt_sha256':digest(receipt_path),
            'dump_sha256':dump_hash,'source_sha256':digest(__file__),
            'scope':'Actual bounded browser gameplay replay with exact input/PCM/final pixels/framebuffers and all source-packet identities/consumption counts. Whole audio output waveform, GPU stream, Section 7, goal and playable speed unverified.'}
require(digest(args.dump) == dump_hash, 'Browser gameplay validation failed at source line 120')
result['browser_session_cleanup']=json.loads((output/'cleanup.json').read_text())
(output/'result.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps({'passed':True,'capture':str(capture),'host_diagnostics':outcome['host_diagnostics']}),flush=True)
