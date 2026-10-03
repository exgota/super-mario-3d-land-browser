#!/usr/bin/env python3
"""Record actual browser touch controls and verify delivered pixel positions."""
import argparse
import json
from pathlib import Path
import re
import subprocess
import time
from urllib.parse import urlsplit

from audit_webassembly_platform import digest
from browser_session_policy import BrowserSession, reject_keep_open
from compare_input_capture import load_input_capture, load_movie, validate_movie_delivery, validate_observation_boundary
from compare_rendered_capture import load_rendered_capture

ROOT = Path(__file__).resolve().parents[2]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("url")
    parser.add_argument("output", type=Path)
    parser.add_argument("--dump", type=Path, required=True)
    parser.add_argument("--server-output", type=Path, required=True)
    parser.add_argument("--sampled-frame", type=int, default=360)
    parser.add_argument("--keep-open", action="store_true")
    parser.add_argument("--headed", action="store_true", help="explicitly open a visible project preview")
    parser.add_argument("--screenshots", action="store_true")
    args = parser.parse_args()
    reject_keep_open(getattr(args, "keep_open", False))
    source_sha256 = digest(Path(__file__).resolve())
    url = urlsplit(args.url)
    output, server = args.output.resolve(), args.server_output.resolve()
    if (url.scheme != "http" or url.hostname != "127.0.0.1" or url.username or url.password or
            url.query or url.fragment or output.exists() or not output.is_relative_to(ROOT / "build") or
            not server.is_relative_to(ROOT / "build") or not server.is_dir()):
        raise ValueError("Fresh owned ignored output and local server required")
    configuration = json.loads((server / "configuration.json").read_text())
    options = configuration["options"]
    if (options.get("live_touch_capture") is not True or options.get("live_button_capture", False) or options.get("live_circle_pad_capture", False) or options.get("input_capture") is not True or
            options.get("audio_capture") is not True or options.get("frame_output") is not True or not 0 <= args.sampled_frame < options["presentation_limit"]):
        raise ValueError("Finite touch-only recording profile required")
    dump = args.dump.resolve()
    if digest(dump) != configuration["inputs"]["dump"]["expected_sha256"]:
        raise ValueError("Owner dump identity differs")
    output.mkdir(parents=True)
    with BrowserSession(output, headed=args.headed) as browser:
        run = browser.run
        def result(arguments, timeout=60):
            raw = run(arguments, timeout)
            marker = "### Result\n"
            if marker not in raw:
                raise RuntimeError("Actual browser result absent")
            return json.JSONDecoder().raw_decode(raw.split(marker, 1)[1].lstrip())[0]

        run(["open", args.url])
        run(["resize", "1440", "1080"])
        action = "async (page) => { await page.waitForFunction(() => document.body.dataset.captureState === 'ready'); "
        action += "await page.getByLabel('Game file', {exact: true}).setInputFiles(" + json.dumps(str(dump)) + "); "
        action += "await page.getByRole('button', {name:'Run preview',exact:true}).click(); }"
        run(["run-code", action])
        action = r"""async page => {
            const canvas = page.locator('#bottom-screen');
            await page.waitForFunction(threshold => document.body.dataset.captureState === 'failed' ||
                (Number(document.body.dataset.sampledRendererFrame) >= threshold &&
                 document.querySelector('#bottom-screen').getAttribute('aria-disabled') === 'false'),
                __THRESHOLD__, {timeout:240000});
            if (await page.evaluate(() => document.body.dataset.captureState === 'failed')) throw new Error('Capture failed');
            const observations = [];
            const waitPosition = async (name,x,y,pressed) => {
                await page.waitForFunction(({x,y,pressed}) => document.body.dataset.touchRequestedX === String(x) &&
                    document.body.dataset.touchRequestedY === String(y) && document.body.dataset.touchRequestedPressed === String(pressed) &&
                    document.body.dataset.touchRequestAccepted === 'true', {x,y,pressed}, {timeout:30000});
                const before = await page.evaluate(() => Number(document.body.dataset.touchPollCount));
                await page.waitForFunction(count => Number(document.body.dataset.touchPollCount) >= count+8, before, {timeout:30000});
                observations.push({name,x,y,pressed,before,after:await page.evaluate(() => ({...document.body.dataset}))});
            };
            const movePixel = async (x,y) => {
                await canvas.scrollIntoViewIfNeeded();
                const box = await canvas.boundingBox();
                if (!box || box.width<=0 || box.height<=0) throw new Error('Touch target absent');
                await page.mouse.move(box.x+(x+0.5)*box.width/320,box.y+(y+0.5)*box.height/240);
            };
            await movePixel(160,120); await page.mouse.down(); await waitPosition('pointer center',160,120,1);
            const cursor = await page.locator('#touch-position').evaluate(element=>{
                const rectangle=element.getBoundingClientRect();return {hidden:element.hasAttribute('hidden'),
                    display:getComputedStyle(element).display,width:rectangle.width,height:rectangle.height};});
            if(cursor.hidden||cursor.display==='none'||cursor.width!==24||cursor.height!==24)throw new Error('Focused touch cursor did not render');
            observations.push({name:'rendered desktop cursor',measurement:cursor});
            __SCREENSHOTS__
            for (const [name,x,y] of [['top left',0,0],['top right',319,0],['bottom right',319,239],
                                    ['bottom left',0,239],['asymmetric interior',73,191]]) {
                await movePixel(x,y); await waitPosition(name,x,y,1);
            }
            const box = await canvas.boundingBox();
            await page.mouse.move(box.x+box.width+20,box.y-20);
            await waitPosition('captured outside clamp',319,0,1);
            await page.mouse.up(); await waitPosition('pointer release',0,0,0);
            await movePixel(160,120); await page.mouse.down(); await waitPosition('mixed pointer hold',160,120,1);
            await page.keyboard.down('Space'); await page.mouse.up(); await waitPosition('keyboard retains pointer release',160,120,1);
            await page.keyboard.press('ArrowLeft'); await page.keyboard.press('ArrowUp'); await waitPosition('keyboard moved hold',152,112,1);
            await page.keyboard.down('Enter'); await page.keyboard.up('Space'); await waitPosition('partial keyboard release',152,112,1);
            await page.keyboard.up('Enter'); await waitPosition('keyboard release',0,0,0);
            await page.keyboard.down('Space'); await waitPosition('focus-loss hold',152,112,1);
            await page.getByRole('button',{name:'Stop preview',exact:true}).focus(); await waitPosition('focus-loss neutral',0,0,0);
            await page.keyboard.up('Space');
            return {observations,final:await page.evaluate(() => ({...document.body.dataset})),
                pressed:await canvas.getAttribute('aria-pressed'),
                scope:'Actual desktop Chrome mouse and keyboard actions. Mobile is a resized desktop viewport. No physical-device or gameplay claim.'};
        }""".replace('__THRESHOLD__',str(args.sampled_frame))
        screenshots = ''
        if args.screenshots:
            screenshots = ('await page.screenshot({path:' + json.dumps(str(output / 'desktop_running.png')) + ',fullPage:true}); '
                'await page.mouse.up(); await waitPosition("resize pointer release",0,0,0); '
                'await page.setViewportSize({width:390,height:1200}); await page.evaluate(async()=>{await document.fonts.ready; '
                'await new Promise(resolve=>requestAnimationFrame(()=>requestAnimationFrame(resolve)));}); '
                'await movePixel(73,191); await page.mouse.down(); await waitPosition("mobile pointer interior",73,191,1); '
                'await page.waitForTimeout(400); observations.push({name:"settled mobile target",measurement:await canvas.evaluate(element=>{'
                'const style=getComputedStyle(element),r=element.getBoundingClientRect(); return {pressed:element.getAttribute("aria-pressed"),'
                'disabled:element.getAttribute("aria-disabled"),width:r.width,height:r.height,logical_width:element.width,logical_height:element.height,'
                'touch_action:style.touchAction,border:style.border,padding:style.padding,transform:style.transform,'
                'cursor_visible:!document.querySelector("#touch-position").hasAttribute("hidden") && getComputedStyle(document.querySelector("#touch-position")).display!=="none",fonts_loaded:document.fonts.check(\'400 16px "IBM Plex Sans"\')};})}); '
                'await page.screenshot({path:' + json.dumps(str(output / 'mobile_running.png')) + ',fullPage:true}); '
                'await page.mouse.up(); await waitPosition("mobile pointer release",0,0,0); '
                'await page.setViewportSize({width:1440,height:1080}); await page.evaluate(async()=>{await document.fonts.ready; '
                'await new Promise(resolve=>requestAnimationFrame(()=>requestAnimationFrame(resolve)));}); '
                'await movePixel(160,120); await page.mouse.down(); await waitPosition("desktop pointer resumed",160,120,1); ')
        delivery = result(['run-code', action.replace('__SCREENSHOTS__',screenshots)], 270)
        (output / 'browser_action.json').write_text(json.dumps(delivery,indent=2)+'\n')
        deadline = time.monotonic() + 240
        states = []
        while True:
            state = result(["eval", "() => ({...document.body.dataset,status:document.querySelector('#capture-status').textContent,error:document.querySelector('#capture-error').textContent,isolated:crossOriginIsolated})"])
            states.append(state)
            (output / "observed_states.json").write_text(json.dumps(states, indent=2) + "\n")
            print(json.dumps(state), flush=True)
            if state.get("captureState") == "complete":
                break
            if state.get("captureState") == "failed" or time.monotonic() > deadline:
                raise RuntimeError("Actual browser did not finish normally")
            time.sleep(5)
        identifier = state["captureIdentifier"]
        if not re.fullmatch(r"capture_[0-9a-f]{32}", identifier) or state["isolated"] is not True:
            raise RuntimeError("Actual browser identity/isolation differs")
        capture = server / identifier
        receipt_path, manifest_path = server / f"{identifier}_receipt.json", server / f"{identifier}_manifest.json"
        receipt, manifest = json.loads(receipt_path.read_text()), json.loads(manifest_path.read_text())
        if (receipt.get("passed") is not True or receipt["completion"]["exit_status"] != 0 or
                any(digest(path) != seal for path, seal in receipt["protected_inputs"].items()) or
                sorted(str(path.relative_to(capture)) for path in capture.rglob("*") if path.is_dir()) != sorted(manifest["directories"])):
            raise RuntimeError("Capture shutdown, seals or directory inventory differs")
        for item in manifest["files"]:
            path = capture / item["relative_path"]
            if path.stat().st_size != item["size"] or digest(path) != receipt["files"][item["relative_path"]]:
                raise RuntimeError("Exported file extent/identity differs")
        snapshot = capture / "initial_user_state"
        expected_files = {item["relative_path"]: item["expected_sha256"] for item in configuration["inputs"]["initial_user_files"]}
        actual_files = {str(path.relative_to(snapshot)): digest(path) for path in snapshot.rglob("*") if path.is_file()}
        host_log = "log/reference_capture.log"
        if (host_log not in expected_files or host_log not in actual_files or
                {name: seal for name, seal in expected_files.items() if name != host_log} !=
                {name: seal for name, seal in actual_files.items() if name != host_log} or
                sorted(str(path.relative_to(snapshot)) for path in snapshot.rglob("*") if path.is_dir()) !=
                sorted(configuration["inputs"]["initial_user_directories"])):
            raise RuntimeError("Initial guest state inventory/identity differs")
        observations = load_input_capture(capture)
        movie = load_movie(capture / "input_movie.ctm")
        validate_observation_boundary(capture, observations)
        validate_movie_delivery(capture, movie, observations)
        polls = observations["polls"]
        if any(event['buttons'] or event['circle_pad_x'] or event['circle_pad_y'] for event in polls):
            raise RuntimeError('Touch-only capture delivered a button or circle-pad input')
        controls = manifest['touch_controls']
        if controls['before_install'] != {'neutral':2,'out_of_range':1,'invalid_pressed':1,'noncanonical_release':1,'active':0}:
            raise RuntimeError('Inactive native touch refusal controls differ')
        if controls['active'] != {'negative_coordinate':1,'outside_screen':1,'invalid_pressed':1,'noncanonical_release':1,
                                 'button_input_active':0,'button_setter_refused':2,'circle_input_active':0,'circle_setter_refused':2}:
            raise RuntimeError('Active native touch refusal controls differ')
        if manifest['touch_closed_polls'] != len(polls) or any(counter['fallbacks'] != '0' for counter in manifest['counters']):
            raise RuntimeError('Touch cleanup or static execution closure differs')
        requests = manifest['touch_requests']
        if not requests or any(item['sequence'] != index or item['status'] != 0 for index,item in enumerate(requests)):
            raise RuntimeError('Touch request sequence or acceptance differs')
        positions = [(item['touch_x'],item['touch_y'],item['touch_valid']) for item in polls]
        if any(not valid and (x or y) for x,y,valid in positions):
            raise RuntimeError('Delivered touch release is noncanonical')
        plateaus = {}
        for observation in delivery['observations']:
            if 'measurement' in observation: continue
            expected = (observation['x'],observation['y'],observation['pressed'])
            first = max(0,observation['before']-2)
            last = min(len(positions),int(observation['after']['touchPollCount'])+2)
            indices = [index for index in range(first,last-2) if positions[index:index+3] == [expected]*3]
            if not indices: raise RuntimeError('Actual delivered touch plateau absent: '+observation['name'])
            plateaus[observation['name']] = {'first_poll':indices[0],'position':expected,
                'ticks':[polls[index]['ticks'] for index in range(indices[0],indices[0]+3)],
                'diagnostic_interval':[first,last]}
        required = {'pointer center','top left','top right','bottom right','bottom left','asymmetric interior',
                    'captured outside clamp','pointer release','keyboard retains pointer release','keyboard moved hold',
                    'partial keyboard release','keyboard release','focus-loss neutral'}
        if not required <= set(plateaus) or positions[-1] != (0,0,0) or delivery['pressed'] != 'false':
            raise RuntimeError('Required actual touch delivery or final release absent')
        pixels = result(["eval", "async () => Object.fromEntries(await Promise.all([...document.querySelectorAll('canvas')].map(async c => [c.id,{width:c.width,height:c.height,hidden:c.hidden,sha256:[...new Uint8Array(await crypto.subtle.digest('SHA-256',c.getContext('2d').getImageData(0,0,c.width,c.height).data))].map(v=>v.toString(16).padStart(2,'0')).join('')}])))"])
        rendered = load_rendered_capture(capture)
        dimensions = {screen['screen_id']:(screen['width'],screen['height']) for screen in rendered['screens']}
        if rendered['complete'] is not True or set(pixels) != {'top-screen','bottom-screen'}:
            raise RuntimeError('Actual browser did not display both completed screens')
        for name, screen in pixels.items():
            identifier = 0 if name == 'top-screen' else 2
            payload = capture / f'rendered_screen_{identifier}.rgba'
            if screen["hidden"] or (screen['width'],screen['height']) != dimensions[identifier] or screen["sha256"] != digest(payload):
                raise RuntimeError("Actual canvas bytes differ from capture")
        viewports = {}
        if args.screenshots:
            for name, width, height in (("desktop", 1440, 1080), ("mobile", 390, 1200)):
                run(["resize", str(width), str(height)])
                run(["run-code", "async page => {await page.evaluate(() => scrollTo(0,0)); await page.waitForTimeout(400);}"])
                run(["screenshot", "--full-page", "--filename=" + str(output / f"{name}.png")])
                raw = (output / f"{name}.png").read_bytes()
                if raw[:8] != b"\x89PNG\r\n\x1a\n" or int.from_bytes(raw[16:20], "big") != width or int.from_bytes(raw[20:24], "big") < height:
                    raise RuntimeError("Invalid full-page screenshot extent")
                viewports[name] = result(["eval", "() => ({inner_width:innerWidth,client_width:document.documentElement.clientWidth,scroll_height:document.documentElement.scrollHeight,fonts_loaded:document.fonts.check('400 16px \"IBM Plex Sans\"') && document.fonts.check('600 44px \"IBM Plex Sans\"')})"])
        if digest(dump) != configuration["inputs"]["dump"]["expected_sha256"]:
            raise RuntimeError("Readonly owner dump changed")
        if digest(Path(__file__).resolve()) != source_sha256:
            raise RuntimeError('Verifier source changed during browser execution')
        report = {"passed":True,"capture":str(capture),"browser_action":delivery,
            "browser":result(["eval","() => navigator.userAgent"]),"pixels":pixels,"viewports":viewports,
            "input_polls":len(polls),"delivered_touch_plateaus":plateaus,
            "touch_requests":requests,"touch_controls":controls,"touch_closed_polls":manifest['touch_closed_polls'],
            "movie_sha256":digest(capture/'input_movie.ctm'),"movie_pad_count":len(movie['pads']),"movie_record_counts":movie['record_counts'],
            "snapshot_guest_files_exact":True,"snapshot_directories_exact":True,
            "raw_host_log_sha256":{"original":expected_files[host_log],"recorded":actual_files[host_log]},
            "manifest_sha256":digest(manifest_path),"receipt_sha256":digest(receipt_path),"source_sha256":source_sha256,
            "scope":"Actual finite browser touch-only recording, pixel corners, interior and captured drag, keyboard and focus-loss releases in original HID/movie bytes, complete export and displayed pixels. Stock replay and semantic gameplay remain separate checks."}
        cleanup = browser.close()
        report["browser_session_policy"] = {"passed": cleanup["passed"],
                                             "cleanup_sha256": digest(output / "cleanup.json"),
                                             "registration_sha256": digest(output / "registration.json")}
    (output/'result.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({"passed":True,"capture":str(capture),"input_polls":len(polls),"delivered_touch_plateaus":list(plateaus)}),flush=True)


if __name__ == "__main__":
    main()
