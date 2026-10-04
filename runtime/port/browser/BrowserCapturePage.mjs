// SPDX-License-Identifier: GPL-2.0-or-later
import {CapturedAudioPlayback} from './BrowserCapturedAudio.mjs';
import {StreamedAudioPlayback} from './BrowserStreamedAudio.mjs';
const form = document.querySelector('#preview-form');
const input = document.querySelector('#game-file');
const button = document.querySelector('#run-preview');
const status = document.querySelector('#capture-status');
const error = document.querySelector('#capture-error');
const soundButton = document.querySelector('#play-recorded-sound');
const soundStatus = document.querySelector('#sound-status');
const liveButton = document.querySelector('#hold-a');
const liveHelp = document.querySelector('#live-button-help');
const touchScreen = document.querySelector('#bottom-screen');
const touchHelp = document.querySelector('#touch-help');
const touchPosition = document.querySelector('#touch-position');
const modeSelection = document.querySelector('#session-mode');
const gameplaySelection = document.querySelector('#gameplay-selection');
const gameplayInputMode = document.querySelector('#gameplay-input-mode');
const gameplayPresentations = document.querySelector('#gameplay-presentations');
const gameplayWallSeconds = document.querySelector('#gameplay-wall-seconds');
const gameplayButtons = document.querySelector('#gameplay-buttons');
const gameplayButtonControls = [...gameplayButtons.querySelectorAll('button')];
const gameplayButtonSources = new Map();
const gameplayKeys = {KeyZ:0, KeyX:1, ShiftLeft:2, ShiftRight:2, Enter:3,
    KeyL:4, KeyJ:5, KeyI:6, KeyK:7, KeyW:8, KeyQ:9, KeyS:10, KeyA:11};
let touchPointer;
let touchPoint = {x:160, y:120};
const touchKeys = new Set();
let touchAnimation;
const circlePad = document.querySelector('#circle-pad');
const circleButtons = [...circlePad.querySelectorAll('button')];
const circleSources = new Map();
const arrowDirections = {ArrowUp:'up', ArrowDown:'down', ArrowLeft:'left', ArrowRight:'right'};
let configuration;
let session;
let completedAudio;
let streamedAudioPlayer;
let latestPreviewFrame;
let latestInitializationObservation;
let captureNote;
const heldSources = new Set();

function runOptions() { return session?.options ?? configuration?.options; }
function gameplay() { return runOptions()?.gameplay_session_presentations !== undefined; }
function maximumAudioBytes() { return gameplay() ? 256 * 1024 * 1024 : 64 * 1024 * 1024; }
function maximumRequests() { return gameplay() ? 65536 : 4096; }

function recording() {
    const options = runOptions();
    return gameplay() ? options.gameplay_input_mode === 'record' :
        options?.live_button_capture || options?.live_circle_pad_capture || options?.live_touch_capture;
}

function selectedInteger(control, maximum, description) {
    requireCondition(/^[0-9]+$/.test(control.value), `${description} must be a positive whole number.`);
    const value = Number(control.value);
    requireCondition(Number.isSafeInteger(value) && value >= 1 && value <= maximum,
        `${description} must be between 1 and ${maximum}.`);
    return value;
}

function selectedOptions() {
    if (modeSelection.value === 'capture') {
        requireCondition(runOptions().gameplay_session_presentations === undefined,
            'This server selected gameplay sessions. Restart it with the short capture profile to run that check.');
        return {...configuration.options};
    }
    const record = gameplayInputMode.value === 'record';
    requireCondition(record || gameplayInputMode.value === 'replay', 'Choose record or replay input.');
    return {presentation_limit:null, gameplay_session_presentations:selectedInteger(gameplayPresentations, 60000, 'Presentations'),
        gameplay_input_mode:gameplayInputMode.value, wall_time_seconds:selectedInteger(gameplayWallSeconds, 3600, 'Time limit'),
        input_capture:true, audio_capture:true, frame_output:true, stream_audio_output:true,
        live_button_capture:record, live_circle_pad_capture:record, live_touch_capture:record,
        ...(record && runOptions().record_base_ticks !== undefined ?
            {record_base_ticks:runOptions().record_base_ticks} : {})};
}

function setupAvailability(enabled) {
    for (const control of [modeSelection, gameplayInputMode, gameplayPresentations, gameplayWallSeconds]) control.disabled = !enabled;
    gameplaySelection.disabled = !enabled || modeSelection.value !== 'gameplay';
}

function selectionChanged() {
    gameplaySelection.hidden = modeSelection.value !== 'gameplay';
    gameplaySelection.disabled = !!session || modeSelection.value !== 'gameplay';
    if (session) return;
    button.textContent = modeSelection.value === 'gameplay' ? 'Run session' : 'Run preview';
    if (modeSelection.value === 'gameplay') document.querySelector('#preview-note').textContent =
        'Live screens and sound come from this browser run. Stop saves the final screens at the next game frame. Reaching the session limit does not verify the level goal.';
    else if (captureNote) document.querySelector('#preview-note').textContent = captureNote;
}
modeSelection.addEventListener('change', selectionChanged);

function publishGameplayButtons() {
    let mask = 0;
    for (const bit of gameplayButtonSources.values()) mask |= 1 << bit;
    if (session?.gameplayReady && session.gameplayButtonSequence >= 65535) {
        mask = 0; gameplayButtonSources.clear(); session.gameplayInputExhausted = true;
    }
    for (const control of gameplayButtonControls) control.setAttribute('aria-pressed', String((mask & (1 << Number(control.dataset.hidBit))) !== 0));
    if (!session?.gameplayReady) return;
    if (session.gameplayButtonMask !== mask) {
        session.gameplayButtonMask = mask;
        session.worker.postMessage({schema_version:1, type:'set_gameplay_button_mask', capture_identifier:session.identifier,
            sequence:session.gameplayButtonSequence++, buttons:mask});
    }
    if (session.gameplayInputExhausted) {
        session.gameplayReady = false; gameplayButtons.disabled = true;
        document.querySelector('#gameplay-buttons-help').textContent = 'Controller requests reached this session’s limit. Stop the session or let it finish.';
    }
}

function releaseGameplayButtons() { gameplayButtonSources.clear(); publishGameplayButtons(); }
function hideGameplayButtons() {
    releaseGameplayButtons();
    if (session) session.gameplayReady = false;
    gameplayButtons.hidden = gameplayButtons.disabled = true;
    document.querySelector('#top-screen').tabIndex = -1;
}

function requestGameplayStop(active) {
    if (session !== active || active.stopRequested) return;
    releaseGameplayButtons(); releaseCirclePad(); releaseTouch();
    active.stopRequested = true;
    active.gameplayReady = active.circleReady = active.touchReady = false;
    gameplayButtons.disabled = circlePad.disabled = true;
    button.textContent = 'Stopping session…'; button.disabled = true;
    status.textContent = 'Stop requested. Waiting for a natural presentation, then saving the final screens.';
    if (active.worker) active.worker.postMessage({schema_version:1, type:'request_gameplay_stop', capture_identifier:active.identifier});
}

for (const control of gameplayButtonControls) {
    control.addEventListener('pointerdown', event => {
        if (event.button !== 0 || !session?.gameplayReady) return;
        control.setPointerCapture(event.pointerId);
        gameplayButtonSources.set(`pointer:${event.pointerId}`, Number(control.dataset.hidBit)); publishGameplayButtons();
    });
    for (const name of ['pointerup', 'pointercancel', 'lostpointercapture']) control.addEventListener(name, event => {
        gameplayButtonSources.delete(`pointer:${event.pointerId}`); publishGameplayButtons();
    });
    control.addEventListener('keydown', event => {
        if (!['Space','Enter'].includes(event.code) || !session?.gameplayReady) return;
        event.preventDefault(); gameplayButtonSources.set(`key:${event.code}`, Number(control.dataset.hidBit)); publishGameplayButtons();
    });
    control.addEventListener('keyup', event => {
        if (!['Space','Enter'].includes(event.code)) return;
        event.preventDefault(); gameplayButtonSources.delete(`key:${event.code}`); publishGameplayButtons();
    });
}

function gameplayKeyboardTarget(target) {
    return target instanceof Element && (target.matches('canvas') || gameplayButtons.contains(target) || circlePad.contains(target));
}
window.addEventListener('keydown', event => {
    if (event.defaultPrevented || event.ctrlKey || event.metaKey || event.altKey ||
        !gameplayKeyboardTarget(event.target) || !session?.gameplayReady) return;
    if (Object.hasOwn(gameplayKeys, event.code)) {
        event.preventDefault(); gameplayButtonSources.set(`key:${event.code}`, gameplayKeys[event.code]); publishGameplayButtons();
    } else if (Object.hasOwn(arrowDirections, event.code) && session.circleReady) {
        event.preventDefault(); circleSources.set(`gameplay-key:${event.code}`, arrowDirections[event.code]); publishCirclePosition();
    }
});
window.addEventListener('keyup', event => {
    if (!session || !gameplay()) return;
    if (gameplayButtonSources.delete(`key:${event.code}`)) { event.preventDefault(); publishGameplayButtons(); }
    if (circleSources.delete(`gameplay-key:${event.code}`)) { event.preventDefault(); publishCirclePosition(); }
});
document.addEventListener('focusout', event => {
    if (gameplayKeyboardTarget(event.target) && !gameplayKeyboardTarget(event.relatedTarget)) {
        releaseGameplayButtons(); if (gameplay()) releaseCirclePad();
    }
});
window.addEventListener('blur', releaseGameplayButtons);
window.addEventListener('pagehide', releaseGameplayButtons);
document.addEventListener('visibilitychange', () => { if (document.hidden) releaseGameplayButtons(); });

function publishTouchState() {
    if (!session?.touchReady) return;
    let pressed = touchPointer !== undefined || touchKeys.size ? 1 : 0;
    let x = pressed ? touchPoint.x : 0, y = pressed ? touchPoint.y : 0;
    // Reserve the final bounded request for release, then finish this capture.
    if (session.touchSequence >= maximumRequests() - 1) {
        session.touchExhausted = true;
        pressed = x = y = 0;
        touchPointer = undefined;
        touchKeys.clear();
        if (touchAnimation !== undefined) cancelAnimationFrame(touchAnimation);
        touchAnimation = undefined;
    }
    touchScreen.setAttribute('aria-pressed', String(pressed !== 0));
    if (session.touchX === x && session.touchY === y && session.touchPressed === pressed) {
        if (session.touchExhausted) touchAvailability();
        return;
    }
    session.touchX = x; session.touchY = y; session.touchPressed = pressed;
    session.worker.postMessage({schema_version:1, type:'set_touch_state',capture_identifier:session.identifier,
        sequence:session.touchSequence++, x, y, pressed});
    if (session.touchExhausted) touchAvailability();
}

function positionTouchCursor() {
    touchPosition.style.left = `${(touchPoint.x + 0.5) / 320 * 100}%`;
    touchPosition.style.top = `${(touchPoint.y + 0.5) / 240 * 100}%`;
    touchPosition.toggleAttribute('hidden', !session?.touchReady || document.activeElement !== touchScreen);
}

function releaseTouch() {
    if (touchAnimation !== undefined) cancelAnimationFrame(touchAnimation);
    touchAnimation = undefined;
    touchPointer = undefined;
    touchKeys.clear();
    publishTouchState();
    positionTouchCursor();
}

function releaseTouchPointer() {
    if (touchAnimation !== undefined) cancelAnimationFrame(touchAnimation);
    touchAnimation = undefined;
    touchPointer = undefined;
    publishTouchState();
    positionTouchCursor();
}

function touchAvailability() {
    if (!runOptions()?.live_touch_capture || !session) return;
    const completeTarget = !touchScreen.hidden && touchScreen.width === 320 && touchScreen.height === 240;
    if (!completeTarget && session.touchReady && !session.touchExhausted) releaseTouch();
    session.touchReady = session.touchPolled && completeTarget && !session.touchExhausted && !session.stopRequested;
    touchHelp.hidden = false;
    touchHelp.textContent = session.touchExhausted ?
        'Touch input reached this preview’s limit. Let the capture finish, then run it again.' : !completeTarget ?
        'Touch will be available when the full bottom screen appears.' :
        'Touch the bottom screen. With it focused, use arrow keys to choose a point, then hold Space or Enter.';
    touchScreen.classList.toggle('touch-enabled', session.touchReady);
    touchScreen.tabIndex = session.touchReady ? 0 : -1;
    touchScreen.setAttribute('role', 'button');
    touchScreen.setAttribute('aria-label','Touch bottom game screen');
    touchScreen.setAttribute('aria-describedby','touch-help');
    touchScreen.setAttribute('aria-disabled',String(!session.touchReady));
    touchScreen.setAttribute('aria-pressed',String(session.touchPressed !== 0));
    positionTouchCursor();
}

function hideTouch() {
    releaseTouch();
    if (session) session.touchReady = session.touchPolled = false;
    touchPosition.setAttribute('hidden', '');
    touchHelp.hidden = true;
    touchScreen.classList.remove('touch-enabled');
    touchScreen.tabIndex = -1;
    touchScreen.setAttribute('aria-label','Bottom game screen');
    for (const name of ['role','aria-describedby','aria-disabled','aria-pressed']) touchScreen.removeAttribute(name);
}

function pointerTouchPoint(event, captured = false) {
    const rectangle = touchScreen.getBoundingClientRect();
    if (rectangle.width <= 0 || rectangle.height <= 0 || touchScreen.hidden ||
        !Number.isFinite(event.clientX) || !Number.isFinite(event.clientY) ||
        (!captured && (event.clientX < rectangle.left || event.clientX >= rectangle.right ||
        event.clientY < rectangle.top || event.clientY >= rectangle.bottom))) return undefined;
    return {x:Math.max(0, Math.min(319, Math.floor((event.clientX - rectangle.left) * 320 / rectangle.width))),
            y:Math.max(0, Math.min(239, Math.floor((event.clientY - rectangle.top) * 240 / rectangle.height)))};
}

touchScreen.addEventListener('pointerdown',event => {
    if (event.button !== 0 || !session?.touchReady || touchPointer !== undefined) return;
    const point = pointerTouchPoint(event);
    if (!point) return;
    touchPoint = point;
    touchPointer = event.pointerId;
    touchScreen.setPointerCapture(event.pointerId);
    touchScreen.focus({preventScroll:true});
    positionTouchCursor();
    publishTouchState();
});
touchScreen.addEventListener('pointermove',event => {
    if (event.pointerId !== touchPointer || !session?.touchReady) return;
    const point = pointerTouchPoint(event, true);
    if (!point) return releaseTouchPointer();
    touchPoint = point;
    positionTouchCursor();
    if (touchAnimation === undefined) touchAnimation = requestAnimationFrame(() => {
        touchAnimation = undefined;
        publishTouchState();
    });
});
for (const name of ['pointerup','pointercancel','lostpointercapture'])
    touchScreen.addEventListener(name,event => { if (event.pointerId === touchPointer) releaseTouchPointer(); });
touchScreen.addEventListener('keydown',event => {
    if (!session?.touchReady) return;
    if (['Space','Enter'].includes(event.code)) {
        event.preventDefault(); touchKeys.add(event.code); publishTouchState();
    } else if (Object.hasOwn(arrowDirections,event.code)) {
        event.preventDefault();
        touchPoint.x = Math.max(0,Math.min(319,touchPoint.x + (event.code==='ArrowRight' ? 8 : event.code==='ArrowLeft' ? -8 : 0)));
        touchPoint.y = Math.max(0,Math.min(239,touchPoint.y + (event.code==='ArrowDown' ? 8 : event.code==='ArrowUp' ? -8 : 0)));
        positionTouchCursor(); publishTouchState();
    }
});
touchScreen.addEventListener('keyup',event => {
    if (!['Space','Enter'].includes(event.code)) return;
    event.preventDefault(); touchKeys.delete(event.code); publishTouchState();
});
touchScreen.addEventListener('focus',positionTouchCursor);
touchScreen.addEventListener('blur',releaseTouch);
window.addEventListener('blur',releaseTouch);
window.addEventListener('pagehide',releaseTouch);
document.addEventListener('visibilitychange',()=>{if(document.hidden) releaseTouch();});

function publishCirclePosition() {
    const directions = new Set(circleSources.values());
    for (const control of circleButtons)
        control.setAttribute('aria-pressed', String(directions.has(control.dataset.direction)));
    if (!session?.circleReady) return;
    const horizontal = Number(directions.has('right')) - Number(directions.has('left'));
    const vertical = Number(directions.has('up')) - Number(directions.has('down'));
    const extent = horizontal && vertical ? 108 : 154;
    let x = horizontal * extent, y = vertical * extent;
    if (gameplay() && session.circleSequence >= 65535) {
        x = y = 0; circleSources.clear(); session.circleExhausted = true;
    }
    if (session.circleExhausted) {
        for (const control of circleButtons) control.setAttribute('aria-pressed', 'false');
        circlePad.disabled = true;
        document.querySelector('#circle-pad-help').textContent = 'Circle-pad requests reached this session’s limit. Stop the session or let it finish.';
    }
    if (session.circleX === x && session.circleY === y) return;
    session.circleX = x;
    session.circleY = y;
    session.worker.postMessage({schema_version:1, type:'set_circle_pad_position',
        capture_identifier:session.identifier, sequence:session.circleSequence++, x, y});
    if (session.circleExhausted) session.circleReady = false;
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
export function streamedAudio() { return streamedAudioPlayer; }
export function previewFrameObservation() { return latestPreviewFrame && structuredClone(latestPreviewFrame); }
export function runtimeInitializationObservation() {
    return latestInitializationObservation && structuredClone(latestInitializationObservation);
}

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
    const stream = streamedAudioPlayer;
    streamedAudioPlayer = undefined;
    if (stream) void stream.dispose().catch(() => {});
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

function stopStreamedAudio(active) {
    if (session !== active || active.audioStreamStopped) return;
    active.audioStreamStopped = true;
    active.worker.postMessage({schema_version: 1, type: 'stop_audio_stream', capture_identifier: active.identifier});
}

function streamChanged(active, player) {
    if (session !== active || streamedAudioPlayer !== player) return;
    const statistics = player.statistics();
    const running = ['running', 'finishing'].includes(statistics.state);
    soundButton.textContent = running ? 'Stop sound' : 'Play preview sound';
    soundButton.disabled = !['idle', 'running', 'finishing'].includes(statistics.state);
    soundStatus.textContent = statistics.state === 'idle' ? 'Sound is available during this run.' :
        statistics.state === 'starting' ? 'Starting preview sound.' :
        running ? 'Playing preview sound.' : statistics.state === 'ended' ? 'Preview sound finished.' :
        statistics.state === 'failed' ? (player.error ?? 'Preview sound stopped.') : 'Preview sound stopped.';
    document.body.dataset.audioState = statistics.state;
    if (statistics.state === 'failed') stopStreamedAudio(active);
    if (active.audioStreamEnabled && !active.audioStreamStopped &&
        statistics.consumed_source_frames > (active.audioConsumedFrames ?? 0)) {
        active.audioConsumedFrames = statistics.consumed_source_frames;
        active.worker.postMessage({schema_version: 1, type: 'audio_stream_consumption',
            capture_identifier: active.identifier, sample_frames: statistics.consumed_source_frames});
    }
}

function requireCondition(condition, message) {
    if (!condition) throw new Error(message);
}

function reportFailure(message) {
    const selected = gameplay();
    hideGameplayButtons();
    hideButton();
    hideCirclePad();
    hideTouch();
    discardAudio();
    if (session) {
        clearTimeout(session.watchdog);
        session.abort.abort();
        session.worker?.terminate();
        session = undefined;
    }
    error.textContent = message;
    error.hidden = false;
    status.textContent = selected ? 'Session stopped before completion.' : 'Preview stopped.';
    button.textContent = modeSelection.value === 'gameplay' ? 'Run session' : 'Run preview';
    input.disabled = false;
    setupAvailability(true);
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
        latestInitializationObservation = message.runtime_initialization;
        status.textContent = gameplay() && active.stopRequested ? 'Stop requested. Waiting for a natural presentation.' :
            gameplay() ? recording() ? 'Running the gameplay session. Controls are starting…' :
            'Running the gameplay movie. Live controls are off.' : recording() ?
            'Running the live input capture…' : 'Running the recorded startup…';
        document.body.dataset.captureState = 'running';
        if (runOptions().stream_audio_output) {
            requireCondition(!streamedAudioPlayer, 'Preview sound was initialized twice.');
            const metadata = {capture_identifier: active.identifier, sample_rate: 32728, channels: 2};
            const changed = () => streamChanged(active, player);
            const player = gameplay() ? new StreamedAudioPlayback(metadata, changed, maximumAudioBytes() / 4) :
                new StreamedAudioPlayback(metadata, changed);
            streamedAudioPlayer = player;
            active.audioPlayer = player;
            soundButton.hidden = soundStatus.hidden = false;
            streamChanged(active, player);
        }
    }
    else if (message.type === 'audio_stream_source_ended') {
        requireCondition(runOptions().stream_audio_output, 'Unexpected sound source closure.');
        active.audioSourceEnded = true;
        if (!active.audioStreamEnabled) {
            soundButton.disabled = true;
            soundStatus.textContent = gameplay() ? 'Preview sound was not started.' :
                'Preview sound was not started. Recorded sound will be ready shortly.';
        }
    }
    else if (message.type === 'audio_stream_unavailable') {
        requireCondition(runOptions().stream_audio_output, 'Unexpected sound availability response.');
        active.audioStreamStopped = true;
        await active.audioPlayer.stop();
    }
    else if (message.type === 'audio_stream_packet') {
        requireCondition(runOptions().stream_audio_output && active.audioStreamEnabled &&
            active.audioPlayer === streamedAudioPlayer && message.pcm instanceof ArrayBuffer && !active.manifest,
            'The preview sound stream is unavailable.');
        if (active.audioStreamStopped) return;
        const player = active.audioPlayer;
        try {
            await player.append({capture_identifier: message.capture_identifier, sequence: message.sequence,
                first_sample_frame: message.first_sample_frame, sample_frames: message.sample_frames,
                sample_rate: message.sample_rate, channels: message.channels, pcm: new Uint8Array(message.pcm)});
        } catch (problem) { if (!active.audioStreamStopped) throw problem; else return; }
        if (session !== active || active.audioStreamStopped) return;
        active.worker.postMessage({schema_version: 1, type: 'acknowledge_audio_packet',
            capture_identifier: active.identifier, sequence: message.sequence});
    }
    else if (message.type === 'audio_stream_end') {
        requireCondition(runOptions().stream_audio_output && active.audioStreamEnabled &&
            active.audioPlayer === streamedAudioPlayer && !active.manifest, 'The sound drain is unavailable.');
        if (active.audioStreamStopped) return;
        let receipt;
        try { receipt = await active.audioPlayer.finish(message.sample_frames); }
        catch (problem) { if (!active.audioStreamStopped) throw problem; else return; }
        if (session !== active || active.audioStreamStopped) return;
        active.worker.postMessage({schema_version: 1, type: 'acknowledge_audio_end',
            capture_identifier: active.identifier, receipt});
    }
    else if (message.type === 'gameplay_session_progress') {
        requireCondition(gameplay() && [0,1].includes(message.active) && [0,1].includes(message.input_active) &&
            [0,1].includes(message.has_gameplay_controls) && Number.isSafeInteger(message.requested_button_mask) &&
            message.requested_button_mask >= 0 && message.requested_button_mask <= 0x0fff &&
            Number.isSafeInteger(message.poll_count) && message.poll_count >= 0 &&
            Number.isSafeInteger(message.sampled_renderer_frame) && message.sampled_renderer_frame >= 0 &&
            typeof message.stop_requested === 'boolean' && typeof message.stop_accepted === 'boolean',
            'The gameplay controller progress is invalid.');
        requireCondition(recording() || (message.input_active === 0 && message.has_gameplay_controls === 0),
            'Replay unexpectedly enabled live controls.');
        active.gameplayReady = recording() && message.active === 1 && message.input_active === 1 &&
            message.has_gameplay_controls === 1 && message.poll_count > 0 && !active.stopRequested && !active.gameplayInputExhausted;
        gameplayButtons.hidden = !recording(); gameplayButtons.disabled = !active.gameplayReady;
        document.querySelector('#top-screen').tabIndex = active.gameplayReady ? 0 : -1;
        document.body.dataset.gameplayActive = String(message.active);
        document.body.dataset.gameplayRequestedMask = String(message.requested_button_mask);
        document.body.dataset.buttonPollCount = String(message.poll_count);
        document.body.dataset.sampledRendererFrame = String(message.sampled_renderer_frame);
    }
    else if (message.type === 'gameplay_button_status') {
        requireCondition(gameplay() && recording() && Number.isSafeInteger(message.sequence) &&
            message.sequence >= 0 && message.sequence < 65536 && [0,1,2,3].includes(message.status) &&
            message.accepted === (message.status === 0), 'The gameplay controller response is invalid.');
        document.body.dataset.buttonRequestSequence = String(message.sequence);
        document.body.dataset.buttonRequestAccepted = String(message.accepted);
    }
    else if (message.type === 'gameplay_stop_status') {
        requireCondition(gameplay() && active.stopRequested && [null,0,2].includes(message.status) &&
            message.accepted === (message.status === 0) && typeof message.pending_activation === 'boolean',
            'The gameplay Stop response is invalid.');
        active.stopAccepted = message.accepted;
        document.body.dataset.gameplayStopAccepted = String(message.accepted);
        status.textContent = message.accepted ? 'Stop accepted. Waiting for a natural presentation, then saving the final screens.' :
            message.pending_activation ? 'Stop requested. Waiting for the session to reach a natural presentation.' :
            'The session reached its endpoint before Stop was accepted. Saving its actual result.';
    }
    else if (message.type === 'gameplay_session_ended') hideGameplayButtons();
    else if (message.type === 'button_capture_progress') {
        requireCondition(runOptions().live_button_capture &&
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
        requireCondition(runOptions().live_button_capture && Number.isSafeInteger(message.sequence) &&
            typeof message.accepted === 'boolean', 'The live button response is invalid.');
        document.body.dataset.buttonRequestSequence = String(message.sequence);
        document.body.dataset.buttonRequestAccepted = String(message.accepted);
    }
    else if (message.type === 'button_capture_ended') hideButton();
    else if (message.type === 'circle_pad_capture_progress') {
        requireCondition(runOptions().live_circle_pad_capture && [0,1].includes(message.active) &&
            Number.isSafeInteger(message.poll_count) && message.poll_count >= 0 &&
            Number.isSafeInteger(message.sampled_renderer_frame) && message.sampled_renderer_frame >= 0 &&
            Number.isSafeInteger(message.sampled_requested_x) && Number.isSafeInteger(message.sampled_requested_y) &&
            message.sampled_requested_x ** 2 + message.sampled_requested_y ** 2 <= 154 ** 2,
            'The circle pad progress is invalid.');
        active.circleReady = message.active === 1 && message.poll_count > 0 && !active.stopRequested && !active.circleExhausted;
        circlePad.hidden = false;
        circlePad.disabled = !active.circleReady;
        document.body.dataset.circlePollCount = String(message.poll_count);
        document.body.dataset.circleRequestedX = String(message.sampled_requested_x);
        document.body.dataset.circleRequestedY = String(message.sampled_requested_y);
        document.body.dataset.sampledRendererFrame = String(message.sampled_renderer_frame);
    }
    else if (message.type === 'circle_pad_request_status') {
        requireCondition(runOptions().live_circle_pad_capture && Number.isSafeInteger(message.sequence) &&
            [0,1,2].includes(message.status), 'The circle pad response is invalid.');
        document.body.dataset.circleRequestSequence = String(message.sequence);
        document.body.dataset.circleRequestAccepted = String(message.status === 0);
    }
    else if (message.type === 'circle_pad_capture_ended') hideCirclePad();
    else if (message.type === 'touch_capture_progress') {
        requireCondition(runOptions().live_touch_capture && [0,1].includes(message.active) &&
            Number.isSafeInteger(message.poll_count) && message.poll_count >= 0 &&
            Number.isSafeInteger(message.sampled_requested_x) && message.sampled_requested_x >= 0 && message.sampled_requested_x <= 319 &&
            Number.isSafeInteger(message.sampled_requested_y) && message.sampled_requested_y >= 0 && message.sampled_requested_y <= 239 &&
            [0,1].includes(message.sampled_requested_pressed) && Number.isSafeInteger(message.sampled_renderer_frame) &&
            message.sampled_renderer_frame >= 0, 'The touch progress is invalid.');
        active.touchPolled = message.active === 1 && message.poll_count > 0;
        document.body.dataset.touchPollCount = String(message.poll_count);
        document.body.dataset.touchRequestedX = String(message.sampled_requested_x);
        document.body.dataset.touchRequestedY = String(message.sampled_requested_y);
        document.body.dataset.touchRequestedPressed = String(message.sampled_requested_pressed);
        document.body.dataset.sampledRendererFrame = String(message.sampled_renderer_frame);
        touchAvailability();
    }
    else if (message.type === 'touch_request_status') {
        requireCondition(runOptions().live_touch_capture && Number.isSafeInteger(message.sequence) &&
            [0,1,2].includes(message.status), 'The touch response is invalid.');
        document.body.dataset.touchRequestSequence = String(message.sequence);
        document.body.dataset.touchRequestAccepted = String(message.status === 0);
    }
    else if (message.type === 'touch_capture_ended') hideTouch();
    else if (message.type === 'preview_screens') {
        requireCondition(runOptions().frame_output && !active.manifest &&
            Number.isSafeInteger(message.sequence) && message.sequence === (active.previewCount ?? 0) + 1 &&
            typeof message.renderer_frame === 'string' && /^(0|[1-9][0-9]*)$/.test(message.renderer_frame) &&
            typeof message.sampled_ticks === 'string' && /^(0|[1-9][0-9]*)$/.test(message.sampled_ticks) &&
            message.screens?.length === 2 && message.screens[0].screen_identifier === 0 &&
            message.screens[1].screen_identifier === 2 && message.screens.every(screen =>
                typeof screen.sha256 === 'string' && /^[0-9a-f]{64}$/.test(screen.sha256)) &&
            (!latestPreviewFrame || BigInt(message.renderer_frame) > BigInt(latestPreviewFrame.renderer_frame)),
            'The preview frame order changed.');
        drawScreens(message.screens, true);
        touchAvailability();
        active.previewCount = message.sequence;
        if (message.runtime_initialization) latestInitializationObservation = message.runtime_initialization;
        latestPreviewFrame = {sequence: message.sequence, renderer_frame: message.renderer_frame,
            sampled_ticks: message.sampled_ticks, screens: message.screens.map(({rgba, ...screen}) =>
                ({...screen, bytes: rgba.byteLength}))};
        document.body.dataset.previewFrameCount = String(active.previewCount);
        document.body.dataset.previewRendererFrame = message.renderer_frame;
        status.textContent = gameplay() ? active.stopRequested ?
            'Stop requested. Waiting for a natural presentation, then saving the final screens.' : recording() ?
            'Showing live browser frames. Focus a screen or controller to play.' :
            'Showing live browser frames from the selected movie. Controls are read-only.' : runOptions().live_touch_capture ?
            active.touchExhausted ? 'Showing sampled frames. Touch input has reached this preview’s limit.' :
            active.touchReady ? 'Showing sampled frames. Touch the bottom screen to send input.' :
            'Showing sampled frames. Touch will be available when the full bottom screen appears.' : runOptions().live_circle_pad_capture ?
            'Showing sampled frames. Hold a direction to send input.' : runOptions().live_button_capture ?
            'Showing sampled frames. Hold A to send input.' : 'Showing sampled frames from the recorded startup…';
        active.worker.postMessage({schema_version:1, type:'acknowledge_preview',
            capture_identifier:active.identifier, sequence:message.sequence});
    }
    else if (message.type === 'capture_failed') {
        latestInitializationObservation = message.runtime_initialization;
        throw new Error(message.message);
    }
    else if (message.type === 'capture_manifest') {
        hideGameplayButtons();
        hideButton();
        hideCirclePad();
        hideTouch();
        requireCondition(!active.manifest, 'The capture manifest was repeated.');
        active.manifest = message;
        latestInitializationObservation = message.observations?.runtime_initialization;
        if (runOptions().audio_capture) {
            const metadata = message.observations?.audio;
            requireCondition(metadata?.relative_path === 'audio_pcm_s16le.bin' && metadata.channels === 2 &&
                metadata.sample_rate === 32728 && Number.isSafeInteger(metadata.sample_frames) && metadata.sample_frames > 0 &&
                Number.isSafeInteger(metadata.payload_bytes) && metadata.payload_bytes === metadata.sample_frames * 4 &&
                metadata.payload_bytes <= maximumAudioBytes(), 'The recorded sound metadata is incomplete.');
            const fileIndex = message.files.findIndex(file => file.relative_path === metadata.relative_path);
            requireCondition(fileIndex !== -1 && message.files[fileIndex].size === metadata.payload_bytes,
                             'The recorded sound file extent changed.');
            if (!gameplay()) active.audio = {metadata, fileIndex, offset: 0, pcm: new Uint8Array(metadata.payload_bytes)};
            else active.recordedAudio = metadata;
        }
        if (gameplay()) requireCondition(message.outcome?.complete === true &&
            ['presentation_limit','requested_stop'].includes(message.outcome.outcome) &&
            message.observations?.gameplay_session?.gpu_trace_absent === true &&
            message.observations.gameplay_session.exact_gpu_replay_accepted === false &&
            message.observations.gameplay_session.goal_accepted === false &&
            message.observations.gameplay_session.section_7_accepted === false,
            'The normal session outcome is incomplete.');
        await preserve(active, 'manifest', JSON.stringify(message));
        if (session !== active) return;
        status.textContent = gameplay() ? 'Saving the final session screens and recorded input/sound…' : 'Saving the captured frame…';
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
        requireCondition(active.manifest && (!(gameplay() || runOptions().presentation_limit) || active.screens),
                         'The capture did not include its screens.');
        await preserve(active, 'complete', JSON.stringify(message));
        if (session !== active) return;
        active.completed = true;
        acknowledge(active, message);
    } else if (message.type === 'shutdown_complete') {
        requireCondition(active.completed, 'The worker closed before the capture was saved.');
        if (runOptions().audio_capture && !gameplay()) {
            requireCondition(active.audio && active.audio.offset === active.audio.pcm.byteLength,
                             'The recorded sound did not finish transferring.');
            completedAudio = new CapturedAudioPlayback(active.audio.metadata, active.audio.pcm, audioChanged);
            soundButton.hidden = false;
            soundButton.disabled = false;
            soundStatus.hidden = false;
            audioChanged();
        }
        clearTimeout(active.watchdog);
        active.worker.terminate();
        const normalSession = gameplay();
        session = undefined;
        status.textContent = normalSession ? active.manifest.outcome.outcome === 'requested_stop' ?
            'Session stopped at a natural presentation. Final screens and recorded input/sound saved.' :
            'Session reached its presentation limit. Final screens and recorded input/sound saved.' : recording() ?
            'Captured the live input frame. Preview complete.' :
            active.screens ? 'Captured startup frame. Preview complete.' : 'Startup capture complete.';
        if (normalSession && active.recordedAudio && !active.audioStreamEnabled) {
            soundButton.hidden = true; soundStatus.hidden = false;
            soundStatus.textContent = 'Recorded sound was saved. Live sound was not started.';
        }
        button.textContent = modeSelection.value === 'gameplay' ? 'Run session' : 'Run preview';
        input.disabled = false;
        setupAvailability(true);
        button.disabled = false;
        document.body.dataset.captureState = 'complete';
        document.body.dataset.captureIdentifier = active.identifier;
    } else throw new Error('The preview worker returned an unknown event.');
}

input.addEventListener('change', () => {
    hideGameplayButtons();
    hideButton();
    hideCirclePad();
    hideTouch();
    discardAudio();
    error.hidden = true;
    button.disabled = !configuration || input.files.length !== 1;
    if (configuration) status.textContent = input.files.length ? 'Ready to run.' : 'Choose your game file to begin.';
});

form.addEventListener('submit', async event => {
    event.preventDefault();
    if (session) return gameplay() ? requestGameplayStop(session) :
        reportFailure('The preview was stopped. Run it again to start a new capture.');
    let active;
    try {
        discardAudio();
        hideGameplayButtons();
        hideButton();
        hideCirclePad();
        hideTouch();
        touchPoint = {x:160, y:120};
        latestPreviewFrame = undefined;
        latestInitializationObservation = undefined;
        const file = input.files[0];
        requireCondition(configuration && file, 'Choose your approved EU game file.');
        const options = selectedOptions();
        requireCondition(file.size === configuration.inputs.dump.expected_bytes,
                         'This file has a different size. Choose the approved EU .3ds dump.');
        error.hidden = true;
        input.disabled = true;
        setupAvailability(false);
        button.textContent = options.gameplay_session_presentations !== undefined ? 'Stop session' : 'Stop preview';
        button.disabled = false;
        status.textContent = 'Loading the local replay…';
        document.body.dataset.captureState = 'loading';
        delete document.body.dataset.captureIdentifier;
        for (const key of ['sampledRendererFrame', 'buttonPollCount', 'buttonRequestSequence', 'buttonRequestAccepted',
                           'circlePollCount', 'circleRequestedX', 'circleRequestedY', 'circleRequestSequence', 'circleRequestAccepted',
                           'touchPollCount','touchRequestedX','touchRequestedY','touchRequestedPressed','touchRequestSequence','touchRequestAccepted',
                           'previewFrameCount', 'previewRendererFrame', 'gameplayActive', 'gameplayRequestedMask', 'gameplayStopAccepted'])
            delete document.body.dataset[key];
        for (const canvas of document.querySelectorAll('canvas')) canvas.hidden = true;
        document.querySelector('.screen-placeholder').hidden = false;
        document.querySelector('.preview').classList.remove('has-frame');
        const identifier = `capture_${crypto.randomUUID().replaceAll('-', '')}`;
        active = {identifier, options, abort: new AbortController(), buttonSequence: 0, buttonHeld: 0, buttonReady: false,
                  gameplayButtonSequence:0, gameplayButtonMask:0, gameplayReady:false, gameplayInputExhausted:false,
                  circleSequence:0, circleX:0, circleY:0, circleReady:false,
                  touchSequence:0, touchX:0, touchY:0, touchPressed:0, touchReady:false, touchPolled:false, touchExhausted:false};
        session = active;
        active.watchdog = setTimeout(() => { if (session === active)
            reportFailure('The preview timed out. Run it again or check the local capture logs.'); },
            (options.wall_time_seconds + 180) * 1000);
        const inputs = configuration.inputs;
        const [blockSchedule, movie, initial] = await Promise.all([
            sidecar(inputs.block_schedule, active.abort.signal), sidecar(inputs.movie, active.abort.signal),
            Promise.all(inputs.initial_user_files.map(async item =>
                ({relative_path: item.relative_path, ...await sidecar(item, active.abort.signal)})))
        ]);
        if (session !== active) return;
        if (gameplay() && recording() && options.record_base_ticks === undefined) {
            const bytes = new Uint8Array(await movie.file.slice(0, 100).arrayBuffer());
            requireCondition(bytes.length === 100 && bytes[0] === 0x43 && bytes[1] === 0x54 && bytes[2] === 0x4d && bytes[3] === 0x1b,
                'The selected movie does not contain a recording clock.');
            const ticks = new DataView(bytes.buffer).getBigInt64(92, true);
            requireCondition(ticks >= 0n && ticks <= BigInt(Number.MAX_SAFE_INTEGER), 'The selected movie clock exceeds this browser’s bound.');
            options.record_base_ticks = ticks.toString();
        }
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
        button.textContent = active.stopRequested ? 'Stopping session…' : gameplay() ? 'Stop session' : 'Stop preview';
        button.disabled = !!active.stopRequested;
        worker.postMessage({schema_version: 1, type: 'start_capture', capture_identifier: identifier,
            inputs: {dump: {file, ...inputs.dump}, block_schedule: blockSchedule, movie,
                     initial_user_files: initial, initial_user_directories: inputs.initial_user_directories},
            options});
        if (active.stopRequested) worker.postMessage({schema_version:1, type:'request_gameplay_stop', capture_identifier:identifier});
    } catch (problem) { if (!active || session === active) reportFailure(problem.message); }
});

soundButton.addEventListener('click', async () => {
    if (session?.audioPlayer) {
        const active = session;
        const player = active.audioPlayer;
        if (['running', 'finishing'].includes(player.state)) {
            stopStreamedAudio(active);
            await player.stop();
            return;
        }
        if (player.state !== 'idle' || active.audioSourceEnded || active.manifest) return;
        soundButton.disabled = true;
        try {
            await player.start();
            if (session !== active || active.audioStreamStopped || active.audioSourceEnded) {
                await player.stop();
                return;
            }
            active.audioStreamEnabled = true;
            active.worker.postMessage({schema_version: 1, type: 'enable_audio_stream', capture_identifier: active.identifier});
            streamChanged(active, player);
        } catch (problem) {
            if (session === active) {
                stopStreamedAudio(active);
                soundStatus.textContent = problem.message;
            }
        }
        return;
    }
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
    if (runOptions().stream_audio_output)
        document.querySelector('#preview-note').textContent = 'Preview sound is available during this finite run. Start it with Play preview sound. The complete recorded sound is also available when the run ends.';
    else if (runOptions().frame_output)
        document.querySelector('#preview-note').textContent = runOptions().live_touch_capture ?
            'Sampled game frames appear during this finite run. Touch the bottom screen to send input. Recorded sound is ready when the run ends.' : runOptions().live_circle_pad_capture ?
            'Sampled game frames appear during this finite run. Hold a direction to send input. Recorded sound is ready when the run ends.' : runOptions().live_button_capture ?
            'Sampled game frames appear during this finite run. Hold A to send input. Recorded sound is ready when the run ends.' :
            'Sampled game frames appear during the recorded startup. The final frame and recorded sound arrive when the run ends.';
    else if (runOptions().live_circle_pad_capture)
        document.querySelector('#preview-note').textContent = 'Hold a direction during this finite input capture. The screens and recorded sound arrive when it ends. Continuous gameplay is still in progress.';
    else if (runOptions().live_button_capture)
        document.querySelector('#preview-note').textContent = 'Hold A during this finite input capture. The screens and recorded sound arrive when it ends. Continuous gameplay is still in progress.';
    else if (runOptions().presentation_limit === null)
        document.querySelector('#preview-note').textContent = 'This startup check captures the first GPU submission. Use the frame preview to see the game screens.';
    else if (runOptions().audio_capture)
        document.querySelector('#preview-note').textContent = 'This preview replays recorded menu input and stops at a captured frame. You can then play its recorded sound. Live controls and continuous sound are still in progress.';
    else if (runOptions().input_capture)
        document.querySelector('#preview-note').textContent = 'This preview replays recorded menu input and stops at a captured frame. Live controls and sound are still in progress.';
    captureNote = document.querySelector('#preview-note').textContent;
    if (configuration.options.gameplay_session_presentations !== undefined) {
        modeSelection.value = 'gameplay';
        gameplayPresentations.value = String(configuration.options.gameplay_session_presentations);
        gameplayWallSeconds.value = String(configuration.options.wall_time_seconds);
        gameplayInputMode.value = configuration.options.gameplay_input_mode;
    }
    selectionChanged();
    status.textContent = 'Choose your game file to begin.';
    document.body.dataset.captureState = 'ready';
    button.disabled = input.files.length !== 1;
} catch (problem) { reportFailure(problem.message); }
