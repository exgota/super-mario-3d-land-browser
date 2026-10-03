#!/usr/bin/env python3
"""Record actual browser circle-pad controls and verify normal HID averaging."""
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
    parser.add_argument("--screenshots", action="store_true")
    parser.add_argument("--headed", action="store_true", help="request the shared visible-preview lease")
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
    if (options.get("live_circle_pad_capture") is not True or options.get("live_button_capture", False) or options.get("input_capture") is not True or
            options.get("audio_capture") is not True or not 0 <= args.sampled_frame < options["presentation_limit"]):
        raise ValueError("Finite circle-only recording profile required")
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
            await page.waitForFunction(threshold => document.body.dataset.captureState === 'failed' ||
                (Number(document.body.dataset.sampledRendererFrame) >= threshold && !document.querySelector('#circle-pad').disabled),
                __THRESHOLD__, {timeout:240000});
            if (await page.evaluate(() => document.body.dataset.captureState === 'failed')) throw new Error('Capture failed');
            const observations = [];
            const waitPosition = async (name,x,y) => {
                await page.waitForFunction(({x,y}) => document.body.dataset.circleRequestedX === String(x) &&
                    document.body.dataset.circleRequestedY === String(y) && document.body.dataset.circleRequestAccepted === 'true',
                    {x,y}, {timeout:30000});
                const before = await page.evaluate(() => Number(document.body.dataset.circlePollCount));
                await page.waitForFunction(count => Number(document.body.dataset.circlePollCount) >= count+8, before, {timeout:30000});
                observations.push({name,x,y,before,after:await page.evaluate(() => ({...document.body.dataset}))});
            };
            const right = page.getByRole('button',{name:'Hold right',exact:true});
            const box = await right.boundingBox();
            await page.mouse.move(box.x+box.width/2,box.y+box.height/2);
            await page.mouse.down(); await waitPosition('pointer right',154,0);
            __SCREENSHOTS__
            await page.mouse.up(); await waitPosition('pointer release',0,0);
            await right.focus();
            await page.keyboard.down('ArrowLeft'); await waitPosition('keyboard left',-154,0);
            await page.keyboard.up('ArrowLeft'); await waitPosition('left release',0,0);
            await page.keyboard.down('ArrowUp'); await waitPosition('keyboard up',0,154);
            await page.keyboard.up('ArrowUp'); await waitPosition('up release',0,0);
            await page.keyboard.down('ArrowDown'); await waitPosition('keyboard down',0,-154);
            await page.keyboard.up('ArrowDown'); await waitPosition('down release',0,0);
            await page.keyboard.down('ArrowUp'); await page.keyboard.down('ArrowRight');
            await waitPosition('upper right diagonal',108,108);
            await page.keyboard.up('ArrowUp'); await waitPosition('upper partial release',154,0);
            await page.keyboard.up('ArrowRight'); await waitPosition('upper release',0,0);
            await page.keyboard.down('ArrowDown'); await page.keyboard.down('ArrowLeft');
            await waitPosition('lower left diagonal',-108,-108);
            await page.keyboard.up('ArrowDown'); await waitPosition('lower partial release',-154,0);
            await page.keyboard.up('ArrowLeft'); await waitPosition('lower release',0,0);
            await page.keyboard.down('ArrowRight'); await waitPosition('focus-loss hold',154,0);
            await page.getByRole('button',{name:'Stop preview',exact:true}).focus();
            await waitPosition('focus-loss neutral',0,0);
            await page.keyboard.up('ArrowRight');
            return {observations,final:await page.evaluate(() => ({...document.body.dataset})),
                pressed:await page.locator('#circle-pad button[aria-pressed="true"]').count()};
        }""".replace('__THRESHOLD__',str(args.sampled_frame))
        screenshots = ''
        if args.screenshots:
            screenshots = ('await page.screenshot({path:' + json.dumps(str(output / 'desktop_running.png')) + ',fullPage:true}); '
                'await page.mouse.up(); await waitPosition("resize pointer release",0,0); '
                'await page.setViewportSize({width:390,height:1200}); await page.evaluate(async()=>{await document.fonts.ready; '
                'await new Promise(resolve=>requestAnimationFrame(()=>requestAnimationFrame(resolve)));}); '
                'await right.focus(); await page.keyboard.down("ArrowRight"); await waitPosition("mobile keyboard right",154,0); '
                'await page.waitForTimeout(400); const mobileControl = await right.evaluate(element=>{const style=getComputedStyle(element); '
                'const rectangle=element.getBoundingClientRect(); return {text:element.textContent,color:style.color,background:style.backgroundColor,'
                'pressed:element.getAttribute("aria-pressed"),disabled:element.disabled,x:rectangle.x,y:rectangle.y,width:rectangle.width,'
                'height:rectangle.height,dataset:{...document.body.dataset},fonts_loaded:document.fonts.check(\'600 14px "IBM Plex Sans"\')};}); '
                'observations.push({name:"settled mobile control",...mobileControl}); await page.screenshot({path:' +
                json.dumps(str(output / 'mobile_running.png')) + ',fullPage:true}); '
                'await page.keyboard.up("ArrowRight"); await waitPosition("mobile keyboard release",0,0); '
                'await page.setViewportSize({width:1440,height:1080}); ')
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
        if any(event['buttons'] & 1 or event['touch_valid'] for event in polls):
            raise RuntimeError('Circle-only capture delivered A or touch')
        controls = manifest['circle_pad_controls']
        if controls['before_install'] != {'neutral':2,'out_of_range':1,'outside_disk':1,'active':0}:
            raise RuntimeError('Inactive native refusal controls differ')
        if controls['active'] != {'out_of_range':1,'outside_disk':1,'button_input_active':0,'button_setter_refused':2}:
            raise RuntimeError('Active native refusal controls differ')
        if manifest['circle_pad_closed_polls'] != len(polls) or any(counter['fallbacks'] != '0' for counter in manifest['counters']):
            raise RuntimeError('Circle cleanup or static execution closure differs')
        requests = manifest['circle_pad_requests']
        if not requests or any(item['sequence'] != index or item['status'] != 0 for index,item in enumerate(requests)):
            raise RuntimeError('Circle request sequence or acceptance differs')
        positions = [(item['circle_pad_x'],item['circle_pad_y']) for item in polls]
        # HID's public three-sample integer average rounds toward zero. Require
        # contiguous delivered ramps, independent of worker diagnostic positions.
        patterns = {
            'right': [(0,0),(51,0),(102,0),(154,0)],
            'right release': [(154,0),(102,0),(51,0),(0,0)],
            'left': [(0,0),(-51,0),(-102,0),(-154,0)],
            'left release': [(-154,0),(-102,0),(-51,0),(0,0)],
            'up': [(0,0),(0,51),(0,102),(0,154)],
            'up release': [(0,154),(0,102),(0,51),(0,0)],
            'down': [(0,0),(0,-51),(0,-102),(0,-154)],
            'down release': [(0,-154),(0,-102),(0,-51),(0,0)],
            'upper partial release': [(108,108),(123,72),(138,36),(154,0)],
            'lower partial release': [(-108,-108),(-123,-72),(-138,-36),(-154,0)],
        }
        ramps = {}
        for name,pattern in patterns.items():
            indices=[index for index in range(len(positions)-len(pattern)+1) if positions[index:index+len(pattern)]==pattern]
            if not indices: raise RuntimeError('Delivered circle ramp absent: '+name)
            ramps[name]={'first_poll':indices[0],'positions':pattern,'ticks':[polls[i]['ticks'] for i in range(indices[0],indices[0]+len(pattern))]}
        for pair in [(108,108),(-108,-108)]:
            if positions.count(pair)<3: raise RuntimeError('Delivered diagonal plateau absent')
        if positions[-1]!=(0,0) or delivery['pressed'] != 0:
            raise RuntimeError('Circle release remained held')
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
            "input_polls":len(polls),"delivered_ramps":ramps,"diagonal_polls":{"upper_right":positions.count((108,108)),"lower_left":positions.count((-108,-108))},
            "circle_pad_requests":requests,"circle_pad_controls":controls,"circle_pad_closed_polls":manifest['circle_pad_closed_polls'],
            "movie_sha256":digest(capture/'input_movie.ctm'),"movie_pad_count":len(movie['pads']),"movie_record_counts":movie['record_counts'],
            "snapshot_guest_files_exact":True,"snapshot_directories_exact":True,
            "raw_host_log_sha256":{"original":expected_files[host_log],"recorded":actual_files[host_log]},
            "manifest_sha256":digest(manifest_path),"receipt_sha256":digest(receipt_path),"source_sha256":source_sha256,
            "scope":"Actual finite browser circle-only recording, signed cardinal and diagonal HID delivery with normal averaging and releases, complete export and displayed pixels. Stock replay and semantic gameplay remain separate checks."}
        cleanup = browser.close()
        report["browser_session_policy"] = {"passed": cleanup["passed"],
                                             "cleanup_sha256": digest(output / "cleanup.json"),
                                             "registration_sha256": digest(output / "registration.json")}
    (output/'result.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({"passed":True,"capture":str(capture),"input_polls":len(polls),"delivered_ramps":list(ramps)}),flush=True)


if __name__ == "__main__":
    main()
