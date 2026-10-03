#!/usr/bin/env python3
"""Exercise real browser controls and inspect the delivered HID movie."""
import argparse
import json
from pathlib import Path
import time

WORKTREE=Path(__file__).resolve().parents[2]
from browser_session_policy import BrowserSession
from audit_webassembly_platform import digest
from compare_input_capture import load_input_capture,load_movie,validate_movie_delivery

def require(condition,message):
    if not condition:
        raise RuntimeError(message)


parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('url')
parser.add_argument('output',type=Path)
parser.add_argument('--server-output',type=Path,required=True)
parser.add_argument('--dump',type=Path,required=True)
args=parser.parse_args()
output=args.output.resolve()
server=args.server_output.resolve()
dump=args.dump.resolve()
from urllib.parse import urlsplit
url=urlsplit(args.url)
if url.scheme!='http' or url.hostname!='127.0.0.1' or url.username or url.password or url.query or url.fragment:
    raise ValueError('A local gameplay server is required')
if output.exists() or not output.is_relative_to(WORKTREE/'build'):
    raise ValueError('An absent own ignored output directory is required')
configuration=json.loads((server/'configuration.json').read_text())
if configuration['options'].get('gameplay_input_mode')!='record' or configuration['options']['gameplay_session_presentations']!=60000:
    raise ValueError('A bounded live-control recording session is required')
dump_hash=digest(dump)
output.mkdir()
with BrowserSession(output) as browser:
    def evaluate(code):
        text=browser.run(['eval',code])
        return json.JSONDecoder().raw_decode(text.split('### Result\n',1)[1].lstrip())[0]
    def run_code(code,timeout=90): return browser.run(['run-code',code],timeout=timeout)
    browser.run(['open',args.url])
    started=time.monotonic()
    run_code("async(page)=>{await page.waitForFunction(()=>document.body.dataset.captureState==='ready'); "
        "await page.getByLabel('Game file',{exact:true}).setInputFiles("+json.dumps(str(dump))+"); "
        "await page.getByRole('button',{name:'Run session',exact:true}).click(); "
        "await page.waitForFunction(()=>!document.querySelector('#gameplay-buttons').disabled && "
        "!document.querySelector('#top-screen').hidden,null,{timeout:120000});}",150)
    action="""async(page)=>{
        const evidence=[];
        async function polls(count=8){
            const start=await page.evaluate(()=>Number(document.body.dataset.buttonPollCount));
            await page.waitForFunction(n=>Number(document.body.dataset.buttonPollCount)>=n,start+count,{timeout:30000});
        }
        async function mask(value){await page.waitForFunction(n=>Number(document.body.dataset.gameplayRequestedMask)===n,value,{timeout:10000});}
        async function key(code,bit){
            await page.locator('#top-screen').focus();
            await page.keyboard.down(code);await mask(1<<bit);await polls();
            evidence.push(await page.evaluate(()=>({mask:Number(document.body.dataset.gameplayRequestedMask),polls:Number(document.body.dataset.buttonPollCount)})));
            await page.keyboard.up(code);await mask(0);await polls();
        }
        for(const [code,bit] of [['KeyZ',0],['KeyX',1],['ShiftLeft',2],['Enter',3],['KeyL',4],['KeyJ',5],['KeyI',6],['KeyK',7],['KeyW',8],['KeyQ',9],['KeyS',10],['KeyA',11]])await key(code,bit);
        await page.locator('#top-screen').focus();await page.keyboard.down('KeyX');await mask(2);await polls();
        await page.locator('#run-preview').focus();await mask(0);await polls();await page.keyboard.up('KeyX');
        for(const code of ['ArrowUp','ArrowDown','ArrowLeft','ArrowRight']){
            await page.locator('#top-screen').focus();await page.keyboard.down(code);await polls();await page.keyboard.up(code);await polls();
        }
        await page.locator('[data-hid-bit="1"]').focus();await page.keyboard.down('Space');await mask(2);await polls();await page.keyboard.up('Space');await mask(0);await polls();
        const a=await page.locator('[data-hid-bit="0"]').boundingBox();await page.mouse.move(a.x+a.width/2,a.y+a.height/2);await page.mouse.down();await mask(1);await polls();await page.mouse.up();await mask(0);await polls();
        const touch=await page.locator('#bottom-screen').boundingBox();await page.mouse.move(touch.x+touch.width/3,touch.y+touch.height/3);await page.mouse.down();await polls();await page.mouse.move(touch.x+touch.width*2/3,touch.y+touch.height*2/3);await polls();await page.mouse.up();await polls();
        await page.evaluate(v=>window.rootGameplayControlEvidence=v,evidence);
    }"""
    run_code(action,240)
    evidence=evaluate('()=>window.rootGameplayControlEvidence')
    browser.run(['screenshot','--filename='+str(output/'controls_running.png')])
    run_code("async(page)=>{await page.evaluate(()=>{window.rootGameplayStopStates=[];const collect=()=>{window.rootGameplayStopStates.push({state:document.body.dataset.captureState,text:document.querySelector('#run-preview').textContent,disabled:document.querySelector('#run-preview').disabled});};const observer=new MutationObserver(collect);observer.observe(document.body,{attributes:true,childList:true,subtree:true});collect();});await page.getByRole('button',{name:'Stop session',exact:true}).click();}")
    stop_state=evaluate("()=>({state:document.body.dataset.captureState,button:document.querySelector('#run-preview').textContent,disabled:document.querySelector('#run-preview').disabled})")
    stop_transitions=evaluate('()=>window.rootGameplayStopStates')
    require(any(v['disabled'] and 'Stopping' in v['text'] for v in stop_transitions), 'Browser control validation failed at source line 76')
    states=[]
    while True:
        state=evaluate("()=>({state:document.body.dataset.captureState,identifier:document.body.dataset.captureIdentifier,error:document.querySelector('#capture-error').textContent,stop_accepted:document.body.dataset.gameplayStopAccepted})")
        states.append({**state,'elapsed_seconds':time.monotonic()-started})
        (output/'observed_states.json').write_text(json.dumps(states,indent=2)+'\n')
        print(json.dumps(states[-1]),flush=True)
        if state['state']=='complete':break
        if state['state']=='failed' or time.monotonic()-started>600:raise RuntimeError(json.dumps(state))
        time.sleep(5)
    identifier=state['identifier'];capture=server/identifier
    receipt=json.loads((server/(identifier+'_receipt.json')).read_text())
    manifest=json.loads((server/(identifier+'_manifest.json')).read_text())
    require(receipt['passed'] and all(digest(p)==h for p,h in receipt['protected_inputs'].items()), 'Browser control validation failed at source line 89')
    require(all(digest(capture/p)==h for p,h in receipt['files'].items()), 'Browser control validation failed at source line 90')
    outcome=json.loads((capture/'gameplay_session_outcome.json').read_text())
    require(outcome['complete'] and outcome['outcome']=='requested_stop' and outcome['stop_observed_at_final_presentation'], 'Browser control validation failed at source line 92')
    require(not (capture/'gpu_events.jsonl').exists() and not list(capture.glob('pica_command_list_*.bin')), 'Browser control validation failed at source line 93')
    delivered=load_input_capture(capture);movie=load_movie(capture/'input_movie.ctm')
    validate_movie_delivery(capture,movie,delivered)
    polls=delivered['polls']
    button_evidence={}
    for bit in range(12):
        held=[i for i,v in enumerate(polls) if v['buttons']&(1<<bit)]
        require(held and any(not (v['buttons']&(1<<bit)) for v in polls[held[-1]+1:]), 'Browser control validation failed at source line 100')
        button_evidence[str(bit)]={'first_held_poll':held[0],'last_held_poll':held[-1],'held_polls':len(held),'later_release_delivered':True}
    require(any(v['circle_pad_x']==154 for v in polls) and any(v['circle_pad_x']==-154 for v in polls), 'Browser control validation failed at source line 102')
    require(any(v['circle_pad_y']==154 for v in polls) and any(v['circle_pad_y']==-154 for v in polls), 'Browser control validation failed at source line 103')
    touches=[i for i,v in enumerate(polls) if v['touch_valid']]
    require(touches and any(not v['touch_valid'] for v in polls[touches[-1]+1:]), 'Browser control validation failed at source line 105')
    require(polls[-1]['buttons']==0 and polls[-1]['circle_pad_x']==0 and polls[-1]['circle_pad_y']==0 and polls[-1]['touch_valid']==0, 'Browser control validation failed at source line 106')
    require(all(v['fallbacks']=='0' for v in manifest['counters']), 'Browser control validation failed at source line 107')
    require(manifest['observations']['gameplay_session']['stop_accepted_by_native_bridge'], 'Browser control validation failed at source line 108')
    pixels=evaluate("async()=>{const r={};for(const name of ['top-screen','bottom-screen']){const c=document.getElementById(name);const b=c.getContext('2d').getImageData(0,0,c.width,c.height).data;const h=await crypto.subtle.digest('SHA-256',b);r[name]=[...new Uint8Array(h)].map(v=>v.toString(16).padStart(2,'0')).join('');}return r;}")
    require(pixels['top-screen']==digest(capture/'rendered_screen_0.rgba') and pixels['bottom-screen']==digest(capture/'rendered_screen_2.rgba'), 'Browser control validation failed at source line 110')
    result={'passed':True,'capture':str(capture),'outcome':outcome,'actual_button_delivery':button_evidence,'controller_request_evidence':evidence,'input_polls':len(polls),'movie_sha256':digest(capture/'input_movie.ctm'),'stop_state':stop_state,'stop_transitions':stop_transitions,'counters':manifest['counters'],'pixels':pixels,'source_sha256':digest(__file__),'scope':'All twelve standard HID buttons, four circle directions, pointer/keyboard button controls, focus release, touch and natural Stop grounded in actual HID/movie polls. No full-level goal or GPU equality.'}
require(digest(dump)==dump_hash, 'Browser control validation failed at source line 112')
result['browser_session_cleanup']=json.loads((output/'cleanup.json').read_text())
(output/'result.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps({'passed':True,'capture':str(capture),'input_polls':len(polls),'all_twelve_buttons_delivered':True}),flush=True)
