// SPDX-License-Identifier: GPL-2.0-or-later
import {CapturedAudioPlayback} from './BrowserCapturedAudio.mjs';
const form = document.querySelector('#preview-form');
const input = document.querySelector('#game-file');
const button = document.querySelector('#run-preview');
const status = document.querySelector('#capture-status');
const error = document.querySelector('#capture-error');
const soundButton = document.querySelector('#play-recorded-sound');
const soundStatus = document.querySelector('#sound-status');
const liveButton = document.querySelector('#hold-a');
const liveHelp = document.querySelector('#live-button-help');
const circlePad = document.querySelector('#circle-pad');
const circleButtons = [...circlePad.querySelectorAll('button')];
const circleSources = new Map();
const arrowDirections = {ArrowUp:'up', ArrowDown:'down', ArrowLeft:'left', ArrowRight:'right'};
let configuration;
let session;
let completedAudio;
let latestPreviewFrame;
const heldSources = new Set();

function recording() {
    return configuration?.options.live_button_capture || configuration?.options.live_circle_pad_capture;
}

function publishCirclePosition() {
    const directions = new Set(circleSources.values());
    for (const control of circleButtons)
        control.setAttribute('aria-pressed', String(directions.has(control.dataset.direction)));
    if (!session?.circleReady) return;
    const horizontal = Number(directions.has('right')) - Number(directions.has('left'));
    const vertical = Number(directions.has('up')) - Number(directions.has('down'));
    const extent = horizontal && vertical ? 108 : 154;
    const x = horizontal * extent, y = vertical * extent;
    if (session.circleX === x && session.circleY === y) return;
    session.circleX = x;
    session.circleY = y;
    session.worker.postMessage({schema_version:1, type:'set_circle_pad_position',
        capture_identifier:session.identifier, sequence:session.circleSequence++, x, y});
}

function releaseCirclePad() {
    circleSources.clear();
    publishCirclePosition();
}

function hideCirclePad() {
    releaseCirclePad();
    if (session) session.circleReady = false;
    circlePad.hidden = circlePad.disabled = true;
}

for (const control of circleButtons) {
    control.addEventListener('pointerdown', event => {
        if (event.button !== 0 || !session?.circleReady) return;
        control.setPointerCapture(event.pointerId);
        circleSources.set(`pointer:${event.pointerId}`, control.dataset.direction);
        publishCirclePosition();
    });
    for (const name of ['pointerup', 'pointercancel', 'lostpointercapture'])
        control.addEventListener(name, event => {
            circleSources.delete(`pointer:${event.pointerId}`);
            publishCirclePosition();
        });
    control.addEventListener('keydown', event => {
        const direction = arrowDirections[event.code] ??
            (['Space','Enter'].includes(event.code) ? control.dataset.direction : undefined);
        if (!direction || !session?.circleReady) return;
        event.preventDefault();
        circleSources.set(`key:${event.code}`, direction);
        publishCirclePosition();
    });
    control.addEventListener('keyup', event => {
        if (!arrowDirections[event.code] && !['Space','Enter'].includes(event.code)) return;
        event.preventDefault();
        circleSources.delete(`key:${event.code}`);
        publishCirclePosition();
    });
}
circlePad.addEventListener('focusout', event => {
    if (!circlePad.contains(event.relatedTarget)) releaseCirclePad();
});
window.addEventListener('blur', releaseCirclePad);
window.addEventListener('pagehide', releaseCirclePad);
document.addEventListener('visibilitychange', () => { if (document.hidden) releaseCirclePad(); });

function publishButtonState() {
    if (!session?.buttonReady) return;
    const held = heldSources.size ? 1 : 0;
    if (session.buttonHeld === held) return;
    session.buttonHeld = held;
    liveButton.setAttribute('aria-pressed', String(held !== 0));
    session.worker.postMessage({schema_version: 1, type: 'set_button_held_state',
        capture_identifier: session.identifier, sequence: session.buttonSequence++, held});
}

function releaseButton() {
    heldSources.clear();
    publishButtonState();
}

function hideButton() {
    releaseButton();
    if (session) session.buttonReady = false;
    liveButton.hidden = liveHelp.hidden = true;
    liveButton.disabled = true;
    liveButton.setAttribute('aria-pressed', 'false');
}

liveButton.addEventListener('pointerdown', event => {
    if (event.button !== 0 || !session?.buttonReady) return;
    liveButton.setPointerCapture(event.pointerId);
    heldSources.add(`pointer:${event.pointerId}`);
    publishButtonState();
});
for (const name of ['pointerup', 'pointercancel', 'lostpointercapture'])
    liveButton.addEventListener(name, event => {
        heldSources.delete(`pointer:${event.pointerId}`);
        publishButtonState();
    });
liveButton.addEventListener('keydown', event => {
    if (!['Space', 'Enter'].includes(event.code) || !session?.buttonReady) return;
    event.preventDefault();
    heldSources.add(`key:${event.code}`);
    publishButtonState();
});
liveButton.addEventListener('keyup', event => {
    if (!['Space', 'Enter'].includes(event.code)) return;
    event.preventDefault();
    heldSources.delete(`key:${event.code}`);
    publishButtonState();
});
liveButton.addEventListener('blur', releaseButton);
window.addEventListener('blur', releaseButton);
window.addEventListener('pagehide', releaseButton);
document.addEventListener('visibilitychange', () => { if (document.hidden) releaseButton(); });

export function capturedAudio() { return completedAudio; }
export function previewFrameObservation() { return latestPreviewFrame && structuredClone(latestPreviewFrame); }

function drawScreens(screens, preview = false) {
    for (const [index, screen] of screens.entries()) {
        const {width, height} = preview ? screen : screen.metadata;
        requireCondition(screen.rgba instanceof ArrayBuffer && Number.isSafeInteger(width) &&
            Number.isSafeInteger(height) && width > 0 && height > 0 && width <= 4096 && height <= 4096 &&
            width * height * 4 === screen.rgba.byteLength && screen.rgba.byteLength <= 1024 * 1024,
            'The screen extent changed.');
        const canvas = document.querySelector(index === 0 ? '#top-screen' : '#bottom-screen');
        canvas.width = width;
        canvas.height = height;
        canvas.getContext('2d').putImageData(new ImageData(new Uint8ClampedArray(screen.rgba), width, height), 0, 0);
        canvas.hidden = false;
    }
    document.querySelector('.screen-placeholder').hidden = true;
    document.querySelector('.preview').classList.add('has-frame');
}

function discardAudio() {
    const previous = completedAudio;
    completedAudio = undefined;
    previous?.dispose();
    soundButton.hidden = true;
    soundButton.disabled = false;
    soundButton.textContent = 'Play recorded sound';
    soundStatus.hidden = true;
    soundStatus.textContent = '';
    delete document.body.dataset.audioState;
}

window.addEventListener('pagehide', discardAudio);

function audioChanged() {
    if (!completedAudio) return;
    soundButton.textContent = completedAudio.source ? 'Stop sound' : 'Play recorded sound';
    soundStatus.textContent = completedAudio.source ? 'Playing the recorded sound.' : 'Recorded sound is ready.';
    document.body.dataset.audioState = completedAudio.source ? 'playing' : 'ready';
}

function requireCondition(condition, message) {
    if (!condition) throw new Error(message);
}

function reportFailure(message) {
    hideButton();
    hideCirclePad();
    discardAudio();
    if (session) {
        clearTimeout(session.watchdog);
        session.abort.abort();
        session.worker?.terminate();
        session = undefined;
    }
    error.textContent = message;
    error.hidden = false;
    status.textContent = 'Preview stopped.';
    button.textContent = 'Run preview';
    input.disabled = false;
    button.disabled = !configuration || input.files.length !== 1;
    document.body.dataset.captureState = 'failed';
}

async function sidecar(record, signal) {
    const response = await fetch(record.url, {signal});
    requireCondition(response.ok, 'The local replay files could not be loaded. Restart the preview server.');
    const blob = await response.blob();
    requireCondition(blob.size === record.expected_bytes, 'A local replay file changed. Restart the preview server.');
    return {file: new File([blob], 'replay-input'), expected_bytes: record.expected_bytes,
            expected_sha256: record.expected_sha256};
}

async function preserve(active, action, body, headers = {}) {
    const response = await fetch(`/capture/${active.identifier}/${action}`,
                                 {method: 'POST', body, headers, signal: active.abort.signal});
    requireCondition(response.ok, 'The captured output could not be saved. Restart the preview server.');
}

function acknowledge(active, message) {
    if (session !== active) return;
    active.worker.postMessage({schema_version: 1, type: 'acknowledge_transfer',
        capture_identifier: active.identifier, transfer_identifier: message.transfer_identifier});
}

async function receive(active, message) {
    if (session !== active) return;
    requireCondition(message.schema_version === 1 && message.capture_identifier === active.identifier,
                     'The preview worker returned an invalid session.');
    if (message.type === 'capture_started') {
        status.textContent = recording() ?
            'Running the live input capture…' : 'Running the recorded startup…';
        document.body.dataset.captureState = 'running';
    }
    else if (message.type === 'button_capture_progress') {
        requireCondition(configuration.options.live_button_capture &&
            [0, 1].includes(message.active) && Number.isSafeInteger(message.poll_count) &&
            message.poll_count >= 0 && Number.isSafeInteger(message.sampled_renderer_frame) &&
            message.sampled_renderer_frame >= 0, 'The live button progress is invalid.');
        document.body.dataset.sampledRendererFrame = String(message.sampled_renderer_frame);
        document.body.dataset.buttonPollCount = String(message.poll_count);
        active.buttonReady = message.active === 1 && message.poll_count > 0;
        liveButton.disabled = !active.buttonReady;
        liveButton.hidden = liveHelp.hidden = false;
    }
    else if (message.type === 'button_request_status') {
        requireCondition(configuration.options.live_button_capture && Number.isSafeInteger(message.sequence) &&
            typeof message.accepted === 'boolean', 'The live button response is invalid.');
        document.body.dataset.buttonRequestSequence = String(message.sequence);
        document.body.dataset.buttonRequestAccepted = String(message.accepted);
    }
    else if (message.type === 'button_capture_ended') hideButton();
    else if (message.type === 'circle_pad_capture_progress') {
        requireCondition(configuration.options.live_circle_pad_capture && [0,1].includes(message.active) &&
            Number.isSafeInteger(message.poll_count) && message.poll_count >= 0 &&
            Number.isSafeInteger(message.sampled_renderer_frame) && message.sampled_renderer_frame >= 0 &&
            Number.isSafeInteger(message.sampled_requested_x) && Number.isSafeInteger(message.sampled_requested_y) &&
            message.sampled_requested_x ** 2 + message.sampled_requested_y ** 2 <= 154 ** 2,
            'The circle pad progress is invalid.');
        active.circleReady = message.active === 1 && message.poll_count > 0;
        circlePad.hidden = false;
        circlePad.disabled = !active.circleReady;
        document.body.dataset.circlePollCount = String(message.poll_count);
        document.body.dataset.circleRequestedX = String(message.sampled_requested_x);
        document.body.dataset.circleRequestedY = String(message.sampled_requested_y);
        document.body.dataset.sampledRendererFrame = String(message.sampled_renderer_frame);
    }
    else if (message.type === 'circle_pad_request_status') {
        requireCondition(configuration.options.live_circle_pad_capture && Number.isSafeInteger(message.sequence) &&
            [0,1,2].includes(message.status), 'The circle pad response is invalid.');
        document.body.dataset.circleRequestSequence = String(message.sequence);
        document.body.dataset.circleRequestAccepted = String(message.status === 0);
    }
    else if (message.type === 'circle_pad_capture_ended') hideCirclePad();
    else if (message.type === 'preview_screens') {
        requireCondition(configuration.options.frame_output && !active.manifest &&
            Number.isSafeInteger(message.sequence) && message.sequence === (active.previewCount ?? 0) + 1 &&
            typeof message.renderer_frame === 'string' && /^(0|[1-9][0-9]*)$/.test(message.renderer_frame) &&
            typeof message.sampled_ticks === 'string' && /^(0|[1-9][0-9]*)$/.test(message.sampled_ticks) &&
            message.screens?.length === 2 && message.screens[0].screen_identifier === 0 &&
            message.screens[1].screen_identifier === 2 && message.screens.every(screen =>
                typeof screen.sha256 === 'string' && /^[0-9a-f]{64}$/.test(screen.sha256)) &&
            (!latestPreviewFrame || BigInt(message.renderer_frame) > BigInt(latestPreviewFrame.renderer_frame)),
            'The preview frame order changed.');
        drawScreens(message.screens, true);
        active.previewCount = message.sequence;
        latestPreviewFrame = {sequence: message.sequence, renderer_frame: message.renderer_frame,
            sampled_ticks: message.sampled_ticks, screens: message.screens.map(({rgba, ...screen}) =>
                ({...screen, bytes: rgba.byteLength}))};
        document.body.dataset.previewFrameCount = String(active.previewCount);
        document.body.dataset.previewRendererFrame = message.renderer_frame;
        status.textContent = configuration.options.live_circle_pad_capture ?
            'Showing sampled frames. Hold a direction to send input.' : configuration.options.live_button_capture ?
            'Showing sampled frames. Hold A to send input.' : 'Showing sampled frames from the recorded startup…';
        active.worker.postMessage({schema_version:1, type:'acknowledge_preview',
            capture_identifier:active.identifier, sequence:message.sequence});
    }
    else if (message.type === 'capture_failed') throw new Error(message.message);
    else if (message.type === 'capture_manifest') {
        hideButton();
        hideCirclePad();
        requireCondition(!active.manifest, 'The capture manifest was repeated.');
        active.manifest = message;
        if (configuration.options.audio_capture) {
            const metadata = message.observations?.audio;
            requireCondition(metadata?.relative_path === 'audio_pcm_s16le.bin' && metadata.channels === 2 &&
                metadata.sample_rate === 32728 && Number.isSafeInteger(metadata.sample_frames) && metadata.sample_frames > 0 &&
                Number.isSafeInteger(metadata.payload_bytes) && metadata.payload_bytes === metadata.sample_frames * 4 &&
                metadata.payload_bytes <= 64 * 1024 * 1024, 'The recorded sound metadata is incomplete.');
            const fileIndex = message.files.findIndex(file => file.relative_path === metadata.relative_path);
            requireCondition(fileIndex !== -1 && message.files[fileIndex].size === metadata.payload_bytes,
                             'The recorded sound file extent changed.');
            active.audio = {metadata, fileIndex, offset: 0, pcm: new Uint8Array(metadata.payload_bytes)};
        }
        await preserve(active, 'manifest', JSON.stringify(message));
        if (session !== active) return;
        status.textContent = 'Saving the captured frame…';
        acknowledge(active, message);
    } else if (message.type === 'software_screens') {
        requireCondition(active.manifest && !active.screens && message.screens.length === 2,
                         'The captured screens are incomplete.');
        drawScreens(message.screens);
        active.screens = true;
        acknowledge(active, message);
    } else if (message.type === 'capture_file_chunk') {
        requireCondition(active.manifest && message.bytes instanceof ArrayBuffer && message.bytes.byteLength <= 65536,
                         'The capture transfer exceeds its bound.');
        await preserve(active, 'chunk', message.bytes,
                       {'X-Capture-File': String(message.file_index), 'X-Capture-Offset': String(message.offset)});
        if (session !== active) return;
        if (active.audio?.fileIndex === message.file_index) {
            const audio = active.audio;
            requireCondition(message.offset === audio.offset && message.total_bytes === audio.pcm.byteLength &&
                message.bytes.byteLength > 0 && audio.offset + message.bytes.byteLength <= audio.pcm.byteLength,
                'The recorded sound transfer is incomplete.');
            audio.pcm.set(new Uint8Array(message.bytes), audio.offset);
            audio.offset += message.bytes.byteLength;
        }
        acknowledge(active, message);
    } else if (message.type === 'capture_completed') {
        requireCondition(active.manifest && (!configuration.options.presentation_limit || active.screens),
                         'The capture did not include its screens.');
        await preserve(active, 'complete', JSON.stringify(message));
        if (session !== active) return;
        active.completed = true;
        acknowledge(active, message);
    } else if (message.type === 'shutdown_complete') {
        requireCondition(active.completed, 'The worker closed before the capture was saved.');
        if (configuration.options.audio_capture) {
            requireCondition(active.audio && active.audio.offset === active.audio.pcm.byteLength,
                             'The recorded sound did not finish transferring.');
            completedAudio = new CapturedAudioPlayback(active.audio.metadata, active.audio.pcm, audioChanged);
            soundButton.hidden = false;
            soundStatus.hidden = false;
            audioChanged();
        }
        clearTimeout(active.watchdog);
        active.worker.terminate();
        session = undefined;
        status.textContent = recording() ?
            'Captured the live input frame. Preview complete.' :
            active.screens ? 'Captured startup frame. Preview complete.' : 'Startup capture complete.';
        button.textContent = 'Run preview';
        input.disabled = false;
        button.disabled = false;
        document.body.dataset.captureState = 'complete';
        document.body.dataset.captureIdentifier = active.identifier;
    } else throw new Error('The preview worker returned an unknown event.');
}

input.addEventListener('change', () => {
    hideButton();
    hideCirclePad();
    discardAudio();
    error.hidden = true;
    button.disabled = !configuration || input.files.length !== 1;
    if (configuration) status.textContent = input.files.length ? 'Ready to run.' : 'Choose your game file to begin.';
});

form.addEventListener('submit', async event => {
    event.preventDefault();
    if (session) return reportFailure('The preview was stopped. Run it again to start a new capture.');
    let active;
    try {
        discardAudio();
        hideButton();
        hideCirclePad();
        latestPreviewFrame = undefined;
        const file = input.files[0];
        requireCondition(configuration && file, 'Choose your approved EU game file.');
        requireCondition(file.size === configuration.inputs.dump.expected_bytes,
                         'This file has a different size. Choose the approved EU .3ds dump.');
        error.hidden = true;
        input.disabled = true;
        button.textContent = 'Stop preview';
        button.disabled = false;
        status.textContent = 'Loading the local replay…';
        document.body.dataset.captureState = 'loading';
        delete document.body.dataset.captureIdentifier;
        for (const key of ['sampledRendererFrame', 'buttonPollCount', 'buttonRequestSequence', 'buttonRequestAccepted',
                           'circlePollCount', 'circleRequestedX', 'circleRequestedY', 'circleRequestSequence', 'circleRequestAccepted',
                           'previewFrameCount', 'previewRendererFrame'])
            delete document.body.dataset[key];
        for (const canvas of document.querySelectorAll('canvas')) canvas.hidden = true;
        document.querySelector('.screen-placeholder').hidden = false;
        document.querySelector('.preview').classList.remove('has-frame');
        const identifier = `capture_${crypto.randomUUID().replaceAll('-', '')}`;
        active = {identifier, abort: new AbortController(), buttonSequence: 0, buttonHeld: 0, buttonReady: false,
                  circleSequence:0, circleX:0, circleY:0, circleReady:false};
        session = active;
        active.watchdog = setTimeout(() => { if (session === active)
            reportFailure('The preview timed out. Run it again or check the local capture logs.'); },
            (configuration.options.wall_time_seconds + 180) * 1000);
        const inputs = configuration.inputs;
        const [blockSchedule, movie, initial] = await Promise.all([
            sidecar(inputs.block_schedule, active.abort.signal), sidecar(inputs.movie, active.abort.signal),
            Promise.all(inputs.initial_user_files.map(async item =>
                ({relative_path: item.relative_path, ...await sidecar(item, active.abort.signal)})))
        ]);
        if (session !== active) return;
        const workerUrl = new URL('BrowserCaptureWorker.mjs', location.href);
        workerUrl.searchParams.set('module_url', configuration.module_url);
        const worker = new Worker(workerUrl, {type: 'module', name: 'static-port-capture'});
        active.worker = worker;
        worker.onmessage = event => { void receive(active, event.data).catch(problem => {
            if (session === active) reportFailure(problem.message);
        }); };
        worker.onerror = event => { event.preventDefault(); if (session === active)
            reportFailure(event.message || 'The preview worker could not start.'); };
        worker.onmessageerror = () => { if (session === active)
            reportFailure('The preview worker could not transfer its output.'); };
        status.textContent = 'Starting the runtime and checking your file…';
        button.textContent = 'Stop preview';
        button.disabled = false;
        worker.postMessage({schema_version: 1, type: 'start_capture', capture_identifier: identifier,
            inputs: {dump: {file, ...inputs.dump}, block_schedule: blockSchedule, movie,
                     initial_user_files: initial, initial_user_directories: inputs.initial_user_directories},
            options: configuration.options});
    } catch (problem) { if (!active || session === active) reportFailure(problem.message); }
});

soundButton.addEventListener('click', async () => {
    const audio = completedAudio;
    if (!audio) return;
    if (audio.source) return audio.stop();
    soundButton.disabled = true;
    try { await audio.play(); }
    catch (problem) { if (completedAudio === audio) {
        soundStatus.textContent = problem.message;
        document.body.dataset.audioState = 'failed';
    } }
    finally { if (completedAudio === audio) soundButton.disabled = false; }
});

try {
    requireCondition(crossOriginIsolated && typeof SharedArrayBuffer === 'function',
                     'Open this page through the local preview server to enable the runtime.');
    const response = await fetch('/configuration.json');
    requireCondition(response.ok, 'The local preview configuration could not be loaded.');
    configuration = await response.json();
    requireCondition(configuration.schema_version === 1, 'The local preview configuration is incompatible.');
    if (configuration.options.frame_output)
        document.querySelector('#preview-note').textContent = configuration.options.live_circle_pad_capture ?
            'Sampled game frames appear during this finite run. Hold a direction to send input. Recorded sound is ready when the run ends.' : configuration.options.live_button_capture ?
            'Sampled game frames appear during this finite run. Hold A to send input. Recorded sound is ready when the run ends.' :
            'Sampled game frames appear during the recorded startup. The final frame and recorded sound arrive when the run ends.';
    else if (configuration.options.live_circle_pad_capture)
        document.querySelector('#preview-note').textContent = 'Hold a direction during this finite input capture. The screens and recorded sound arrive when it ends. Continuous gameplay is still in progress.';
    else if (configuration.options.live_button_capture)
        document.querySelector('#preview-note').textContent = 'Hold A during this finite input capture. The screens and recorded sound arrive when it ends. Continuous gameplay is still in progress.';
    else if (configuration.options.presentation_limit === null)
        document.querySelector('#preview-note').textContent = 'This startup check captures the first GPU submission. Use the frame preview to see the game screens.';
    else if (configuration.options.audio_capture)
        document.querySelector('#preview-note').textContent = 'This preview replays recorded menu input and stops at a captured frame. You can then play its recorded sound. Live controls and continuous sound are still in progress.';
    else if (configuration.options.input_capture)
        document.querySelector('#preview-note').textContent = 'This preview replays recorded menu input and stops at a captured frame. Live controls and sound are still in progress.';
    status.textContent = 'Choose your game file to begin.';
    document.body.dataset.captureState = 'ready';
    button.disabled = input.files.length !== 1;
} catch (problem) { reportFailure(problem.message); }
