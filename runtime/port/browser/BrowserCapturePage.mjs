// SPDX-License-Identifier: GPL-2.0-or-later
const form = document.querySelector('#preview-form');
const input = document.querySelector('#game-file');
const button = document.querySelector('#run-preview');
const status = document.querySelector('#capture-status');
const error = document.querySelector('#capture-error');
let configuration;
let session;

function requireCondition(condition, message) {
    if (!condition) throw new Error(message);
}

function reportFailure(message) {
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
        status.textContent = 'Running the recorded startup…';
        document.body.dataset.captureState = 'running';
    }
    else if (message.type === 'capture_failed') throw new Error(message.message);
    else if (message.type === 'capture_manifest') {
        requireCondition(!active.manifest, 'The capture manifest was repeated.');
        active.manifest = message;
        await preserve(active, 'manifest', JSON.stringify(message));
        if (session !== active) return;
        status.textContent = 'Saving the captured frame…';
        acknowledge(active, message);
    } else if (message.type === 'software_screens') {
        requireCondition(active.manifest && !active.screens && message.screens.length === 2,
                         'The captured screens are incomplete.');
        for (const [index, screen] of message.screens.entries()) {
            const {width, height} = screen.metadata;
            requireCondition(screen.rgba instanceof ArrayBuffer && width * height * 4 === screen.rgba.byteLength,
                             'The captured screen extent changed.');
            const canvas = document.querySelector(index === 0 ? '#top-screen' : '#bottom-screen');
            canvas.width = width;
            canvas.height = height;
            canvas.getContext('2d').putImageData(new ImageData(new Uint8ClampedArray(screen.rgba), width, height), 0, 0);
            canvas.hidden = false;
        }
        active.screens = true;
        document.querySelector('.screen-placeholder').hidden = true;
        document.querySelector('.preview').classList.add('has-frame');
        acknowledge(active, message);
    } else if (message.type === 'capture_file_chunk') {
        requireCondition(active.manifest && message.bytes instanceof ArrayBuffer && message.bytes.byteLength <= 65536,
                         'The capture transfer exceeds its bound.');
        await preserve(active, 'chunk', message.bytes,
                       {'X-Capture-File': String(message.file_index), 'X-Capture-Offset': String(message.offset)});
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
        clearTimeout(active.watchdog);
        active.worker.terminate();
        session = undefined;
        status.textContent = active.screens ? 'Captured startup frame. Preview complete.' : 'Startup capture complete.';
        button.textContent = 'Run preview';
        input.disabled = false;
        button.disabled = false;
        document.body.dataset.captureState = 'complete';
        document.body.dataset.captureIdentifier = active.identifier;
    } else throw new Error('The preview worker returned an unknown event.');
}

input.addEventListener('change', () => {
    error.hidden = true;
    button.disabled = !configuration || input.files.length !== 1;
    if (configuration) status.textContent = input.files.length ? 'Ready to run.' : 'Choose your game file to begin.';
});

form.addEventListener('submit', async event => {
    event.preventDefault();
    if (session) return reportFailure('The preview was stopped. Run it again to start a new capture.');
    let active;
    try {
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
        for (const canvas of document.querySelectorAll('canvas')) canvas.hidden = true;
        document.querySelector('.screen-placeholder').hidden = false;
        document.querySelector('.preview').classList.remove('has-frame');
        const identifier = `capture_${crypto.randomUUID().replaceAll('-', '')}`;
        active = {identifier, abort: new AbortController()};
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

try {
    requireCondition(crossOriginIsolated && typeof SharedArrayBuffer === 'function',
                     'Open this page through the local preview server to enable the runtime.');
    const response = await fetch('/configuration.json');
    requireCondition(response.ok, 'The local preview configuration could not be loaded.');
    configuration = await response.json();
    requireCondition(configuration.schema_version === 1, 'The local preview configuration is incompatible.');
    if (configuration.options.presentation_limit === null)
        document.querySelector('#preview-note').textContent = 'This startup check captures the first GPU submission. Use the frame preview to see the game screens.';
    status.textContent = 'Choose your game file to begin.';
    document.body.dataset.captureState = 'ready';
    button.disabled = input.files.length !== 1;
} catch (problem) { reportFailure(problem.message); }
