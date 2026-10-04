// SPDX-License-Identifier: GPL-2.0-or-later
import {AudioFileStream} from './BrowserAudioFileStream.mjs';
import {beginRuntimeInitialization} from './BrowserRuntimeInitialization.mjs';
import {createWorkerFileCache} from './BrowserWorkerFileCache.mjs';
const ChunkBytes = 64 * 1024;
const MaximumFiles = 16384;
const MaximumDirectoryEntries = 32768;
const MaximumOutputBytes = 2 ** 31;
const MaximumEventBytes = 256 * 1024 * 1024;
const MaximumEvents = 2000000;
const MaximumLogBytes = 16 * 1024 * 1024;
const MaximumLineBytes = 64 * 1024;
const MaximumAudioBytes = 64 * 1024 * 1024;
const MaximumGameplayAudioBytes = 256 * 1024 * 1024;
const MaximumGameplayRequests = 65536;
const MaximumGameplayPreviewFrames = 65536;
const MaximumGameplayPreviewReceipts = 64;
const TransferTimeoutMilliseconds = 30000;
const UInt32Maximum = 0xffffffff;
const encoder = new TextEncoder();
const stdout = [];
const stderr = [];
let logBytes = 0;
let phase = 'created';
let started = false;
let descriptor;
let module;
let initialization;
let fileCache;
let admissionObservation;
let firstPreviewObservation;
let nextTransferIdentifier = 0;
let pendingTransfer;
let audioStream;
let audioTimer;
let pendingAudio;
let buttonTimer;
let buttonSequence = 0;
let buttonProgress;
const buttonRequests = [];
let circleTimer;
let circleSequence = 0;
let circleProgress;
let circleControls;
const circleRequests = [];
let touchTimer;
let touchSequence = 0;
let touchProgress;
let touchControls;
const touchRequests = [];
let frameTimer;
let frameTask;
let pendingFrame;
let frameControls;
let webGlPresentationReceiver;
const frameReceipts = [];
const MaximumPreviewFrames = 8192;
let gameplayTimer;
let gameplayProgress;
let gameplayButtonSequence = 0;
let gameplayPreviewCount = 0;
let gameplayStopRequested = false;
let gameplayStopAccepted = false;
let gameplayControlsObserved = false;
const gameplayButtonRequests = [];
let gameplayStopRelease;

function gameplay() { return descriptor?.options?.gameplay_session_presentations !== undefined; }
function maximumAudioBytes() { return gameplay() ? MaximumGameplayAudioBytes : MaximumAudioBytes; }
function maximumRequests() { return gameplay() ? MaximumGameplayRequests : 4096; }
function previewCount() { return gameplay() ? gameplayPreviewCount : frameReceipts.length; }
function retainRequest(destination, response) {
    destination.push(response);
    if (gameplay() && destination.length > 128) destination.shift();
}

function requireCondition(condition, message) {
    if (!condition) throw new Error(message);
}

function recording() {
    if (gameplay()) return descriptor.options.gameplay_input_mode === 'record';
    return descriptor.options.live_button_capture || descriptor.options.live_circle_pad_capture || descriptor.options.live_touch_capture;
}

function circlePosition(x, y) {
    requireCondition(Number.isSafeInteger(x) && Number.isSafeInteger(y) &&
        x >= -154 && x <= 154 && y >= -154 && y <= 154 && x * x + y * y <= 154 * 154,
        'Invalid circle-pad position');
}

function touchState(x, y, pressed) {
    integer(x, 319); integer(y, 239); integer(pressed, 1);
    requireCondition(pressed === 1 || (x === 0 && y === 0), 'Invalid released touch position');
}

function integer(value, maximum = Number.MAX_SAFE_INTEGER, minimum = 0) {
    requireCondition(Number.isSafeInteger(value) && value >= minimum && value <= maximum,
                     'Invalid integer extent');
    return value;
}

function record(value, keys) {
    requireCondition(value !== null && typeof value === 'object' && !Array.isArray(value),
                     'Invalid record');
    requireCondition(Object.keys(value).length === keys.length &&
                     keys.every(key => Object.hasOwn(value, key)), 'Unexpected record fields');
    return value;
}

function relativePath(value) {
    requireCondition(typeof value === 'string' && value.length > 0 && value.length <= 2048 &&
        value.split('/').every(part => /^[A-Za-z0-9_.-](?:[A-Za-z0-9_. -]*[A-Za-z0-9_.-])?$/.test(part) &&
                                      part !== '.' && part !== '..' && part !== '__proto__'), 'Invalid relative path');
    return value;
}

function inputRecord(value, path) {
    record(value, ['file', 'expected_bytes', 'expected_sha256']);
    requireCondition(value.file instanceof File, 'Input must be an owner-selected File');
    integer(value.expected_bytes, UInt32Maximum);
    requireCondition(value.file.size === value.expected_bytes &&
                     typeof value.expected_sha256 === 'string' &&
                     /^[0-9a-f]{64}$/.test(value.expected_sha256), 'Input identity is malformed');
    return {path, ...value};
}

function validateStart(value) {
    record(value, ['schema_version', 'type', 'capture_identifier', 'inputs', 'options']);
    requireCondition(value.schema_version === 1 && value.type === 'start_capture' &&
        typeof value.capture_identifier === 'string' &&
        /^[A-Za-z0-9_-]{1,64}$/.test(value.capture_identifier), 'Invalid start descriptor');
    record(value.inputs, ['dump', 'block_schedule', 'movie', 'initial_user_files', 'initial_user_directories']);
    const observationKeys = ['input_capture', 'audio_capture', 'live_button_capture', 'live_circle_pad_capture', 'live_touch_capture',
                             'record_base_ticks', 'frame_output', 'stream_audio_output']
        .filter(key => Object.hasOwn(value.options, key));
    const selected = Object.hasOwn(value.options, 'gameplay_session_presentations');
    record(value.options, ['presentation_limit', 'wall_time_seconds', ...(selected ?
        ['gameplay_session_presentations', 'gameplay_input_mode'] : ['pica_payload_limit_bytes']), ...observationKeys]);
    if (selected) {
        integer(value.options.gameplay_session_presentations, 60000, 1);
        requireCondition(value.options.presentation_limit === null &&
            ['record', 'replay'].includes(value.options.gameplay_input_mode), 'Invalid gameplay session selection');
    }
    const inputCapture = Object.hasOwn(value.options, 'input_capture') ? value.options.input_capture : false;
    const audioCapture = Object.hasOwn(value.options, 'audio_capture') ? value.options.audio_capture : false;
    const liveButtonCapture = Object.hasOwn(value.options, 'live_button_capture') ? value.options.live_button_capture : false;
    const liveCirclePadCapture = Object.hasOwn(value.options, 'live_circle_pad_capture') ? value.options.live_circle_pad_capture : false;
    const liveTouchCapture = Object.hasOwn(value.options, 'live_touch_capture') ? value.options.live_touch_capture : false;
    const frameOutput = Object.hasOwn(value.options, 'frame_output') ? value.options.frame_output : false;
    const streamAudio = Object.hasOwn(value.options, 'stream_audio_output') ? value.options.stream_audio_output : false;
    requireCondition(typeof inputCapture === 'boolean' && typeof audioCapture === 'boolean' &&
        typeof liveButtonCapture === 'boolean' && typeof liveCirclePadCapture === 'boolean' && typeof liveTouchCapture === 'boolean' && typeof frameOutput === 'boolean' &&
        typeof streamAudio === 'boolean' && (!streamAudio || audioCapture) &&
        (!frameOutput || selected || value.options.presentation_limit !== null) &&
        (!liveButtonCapture || (inputCapture && audioCapture)) &&
        (!liveCirclePadCapture || (inputCapture && audioCapture)) &&
        (!liveTouchCapture || (inputCapture && audioCapture && frameOutput)) &&
        (!audioCapture || inputCapture) &&
        (!(inputCapture || audioCapture) || selected || value.options.presentation_limit !== null), 'Invalid observation profile');
    if (selected) requireCondition(inputCapture && audioCapture && frameOutput &&
        (value.options.gameplay_input_mode === 'record' ?
            liveButtonCapture && liveCirclePadCapture && liveTouchCapture :
            !liveButtonCapture && !liveCirclePadCapture && !liveTouchCapture), 'Invalid gameplay input profile');
    requireCondition((liveButtonCapture || liveCirclePadCapture || liveTouchCapture) ? typeof value.options.record_base_ticks === 'string' &&
        /^(0|[1-9][0-9]{0,18})$/.test(value.options.record_base_ticks) &&
        BigInt(value.options.record_base_ticks) <= BigInt(Number.MAX_SAFE_INTEGER) :
        !Object.hasOwn(value.options, 'record_base_ticks'), 'Invalid recording clock');
    if (value.options.presentation_limit !== null) integer(value.options.presentation_limit, 3600, 1);
    integer(value.options.wall_time_seconds, 3600, 1);
    if (!selected) integer(value.options.pica_payload_limit_bytes, 1024 ** 3, 1);
    const inputs = [inputRecord(value.inputs.dump, 'dump.3ds'),
                    inputRecord(value.inputs.block_schedule, 'block_schedule.bin'),
                    inputRecord(value.inputs.movie, 'input_movie.ctm')];
    requireCondition(Array.isArray(value.inputs.initial_user_files) &&
        value.inputs.initial_user_files.length > 0 &&
        value.inputs.initial_user_files.length <= 4096, 'Invalid initial-user manifest');
    const names = new Set();
    for (const item of value.inputs.initial_user_files) {
        record(item, ['relative_path', 'file', 'expected_bytes', 'expected_sha256']);
        const name = relativePath(item.relative_path);
        requireCondition(!names.has(name), 'Duplicate initial-user file');
        names.add(name);
        inputs.push(inputRecord({file: item.file, expected_bytes: item.expected_bytes,
            expected_sha256: item.expected_sha256}, `initial_user_state/${name}`));
    }
    for (const name of names) {
        const parts = name.split('/');
        for (let index = 1; index < parts.length; ++index)
            requireCondition(!names.has(parts.slice(0, index).join('/')), 'File/directory collision');
    }
    requireCondition(Array.isArray(value.inputs.initial_user_directories) &&
        value.inputs.initial_user_directories.length <= MaximumDirectoryEntries, 'Invalid initial-user directories');
    const directories = new Set();
    for (const directory of value.inputs.initial_user_directories) {
        const name = relativePath(directory);
        requireCondition(!names.has(name) && !directories.has(name) && name.split('/').length <= 32,
                         'Duplicate or colliding initial-user directory');
        directories.add(name);
    }
    for (const name of [...names, ...directories]) {
        const parts = name.split('/');
        for (let index = 1; index < parts.length; ++index)
            requireCondition(directories.has(parts.slice(0, index).join('/')),
                             'Initial-user directory parent is absent');
    }
    requireCondition(inputs.reduce((sum, input) => sum + input.expected_bytes, 0) <= MaximumOutputBytes,
                     'Input extent exceeds the finite worker bound');
    return {...value, options: {...value.options, input_capture: inputCapture, audio_capture: audioCapture,
                               live_button_capture: liveButtonCapture, live_circle_pad_capture: liveCirclePadCapture, live_touch_capture: liveTouchCapture,
                               frame_output: frameOutput, stream_audio_output: streamAudio},
            validated_inputs: inputs};
}

function send(message, transfer = []) {
    self.postMessage({schema_version: 1, capture_identifier: descriptor?.capture_identifier ?? null,
                      ...message}, transfer);
}

function fail(error) {
    if (phase === 'failed' || phase === 'closed') return;
    const failedPhase = phase;
    phase = 'failed';
    initialization?.cancel(error);
    clearInterval(buttonTimer);
    clearInterval(gameplayTimer);
    clearInterval(circleTimer);
    clearInterval(touchTimer);
    clearInterval(frameTimer);
    webGlPresentationReceiver?.close();
    stopAudio(error);
    if (pendingFrame) {
        clearTimeout(pendingFrame.timer);
        pendingFrame.reject(error);
        pendingFrame = undefined;
    }
    if (pendingTransfer) {
        clearTimeout(pendingTransfer.timer);
        pendingTransfer.reject(error);
        pendingTransfer = undefined;
    }
    send({type: 'capture_failed', phase: failedPhase, message: String(error?.message ?? error),
          stdout, stderr, runtime_initialization: initializationObservation()});
    // The page owns the independent watchdog and abnormal worker disposal.
    // Never label abrupt termination as a successful C++ shutdown.
}

function initializationObservation() {
    return {initialization: initialization?.observation() ?? null,
        file_cache: fileCache?.observation() ?? null,
        admission: admissionObservation ?? null, first_preview: firstPreviewObservation ?? null};
}

function stopAudio(error = new Error('Streamed sound was stopped')) {
    clearInterval(audioTimer);
    audioStream?.stop();
    if (pendingAudio) {
        clearTimeout(pendingAudio.timer);
        pendingAudio.reject(error);
        pendingAudio = undefined;
    }
}

function audioAcknowledged(type, detail, transfer = []) {
    requireCondition(!pendingAudio && ['running', 'validating'].includes(phase), 'Invalid audio transfer state');
    return new Promise((resolve, reject) => {
        const timer = setTimeout(() => {
            pendingAudio = undefined;
            reject(new Error('Streamed sound acknowledgment timed out'));
        }, TransferTimeoutMilliseconds);
        pendingAudio = {type, sequence: detail.sequence, resolve, reject, timer};
        send({type, ...detail}, transfer);
    });
}

async function completeAudio() {
    if (!audioStream) return undefined;
    try {
        const observation = await audioStream.finish();
        if (observation.enabled && observation.state === 'completed') {
            observation.consumer = await audioAcknowledged('audio_stream_end', {sample_frames: observation.sample_frames});
            requireCondition(!audioStream.closed, 'Audio stream stopped during final drain');
        }
        return observation;
    } catch (error) {
        if (audioStream.closed) return audioStream.receipt('stopped');
        throw error;
    }
}

function captureLine(destination, value) {
    requireCondition(typeof value === 'string', 'Unexpected runtime log argument');
    logBytes += encoder.encode(value).length + 1;
    requireCondition(logBytes <= MaximumLogBytes, 'Runtime log exceeds the finite bound');
    destination.push(value);
}

function acknowledged(message, transfer = []) {
    requireCondition(!pendingTransfer && phase === 'exporting', 'Invalid transfer state');
    const identifier = nextTransferIdentifier++;
    return new Promise((resolve, reject) => {
        const timer = setTimeout(() => {
            pendingTransfer = undefined;
            reject(new Error('Capture transfer acknowledgment timed out'));
        }, TransferTimeoutMilliseconds);
        pendingTransfer = {identifier, resolve, reject, timer};
        send({...message, transfer_identifier: identifier}, transfer);
    });
}

function ownedHeapCopy(pointer, length) {
    const heap = module.HEAPU8;
    pointer >>>= 0;
    requireCondition(heap instanceof Uint8Array && pointer > 0 && Number.isSafeInteger(length) &&
        length > 0 && pointer + length <= heap.byteLength, 'Preview slot extent is invalid');
    const copy = new Uint8Array(length);
    copy.set(heap.subarray(pointer, pointer + length));
    requireCondition(copy.buffer instanceof ArrayBuffer, 'Preview copy still uses shared storage');
    return copy;
}

function copyPreviewFrame() {
    // This whole acquire/copy/release section is synchronous. No native export
    // is called after an await or from onExit. Only owned slots are read here.
    const lease = module._BrowserFrameOutputAcquire();
    requireCondition(lease >= 0 && lease <= 2, 'Native preview bridge refused');
    if (!lease) return;
    let sequence;
    try {
        const words = ownedHeapCopy(module._BrowserFrameOutputMetadata(lease), 48);
        const view = new DataView(words.buffer);
        const fields = Array.from({length:12}, (_, index) => view.getUint32(index * 4, true));
        sequence = fields[1];
        requireCondition(fields[0] === 1 && sequence === previewCount() + 1 &&
            sequence <= (gameplay() ? MaximumGameplayPreviewFrames : MaximumPreviewFrames), 'Preview publication order changed');
        const wide = (low, high) => ((BigInt(high) << 32n) | BigInt(low)).toString();
        const rendererFrame = wide(fields[2], fields[3]);
        const sampledTicks = wide(fields[4], fields[5]);
        requireCondition(BigInt(rendererFrame) > 0n && (!frameReceipts.length ||
            BigInt(rendererFrame) > BigInt(frameReceipts.at(-1).renderer_frame)), 'Preview frame order changed');
        if (!frameControls) {
            const results = {
                invalid_lease_metadata: module._BrowserFrameOutputMetadata(3),
                invalid_lease_release: module._BrowserFrameOutputRelease(3, sequence),
                stale_sequence_pixels: module._BrowserFrameOutputPixels(lease, sequence - 1, 0),
                stale_sequence_release: module._BrowserFrameOutputRelease(lease, sequence - 1),
                invalid_screen_pixels: module._BrowserFrameOutputPixels(lease, sequence, 1),
                lease_preserved: module._BrowserFrameOutputMetadata(lease) > 0
            };
            requireCondition(results.invalid_lease_metadata === 0 && results.invalid_lease_release === 1 &&
                results.stale_sequence_pixels === 0 && results.stale_sequence_release === 1 &&
                results.invalid_screen_pixels === 0 && results.lease_preserved, 'Preview refusal control failed');
            frameControls = results;
        }
        const screens = [0, 2].map((screenIdentifier, index) => {
            const [width, height, bytes] = fields.slice(6 + index * 3, 9 + index * 3);
            requireCondition(width > 0 && height > 0 && width <= 4096 && height <= 4096 &&
                width * height * 4 === bytes && bytes <= 1024 * 1024, 'Preview screen dimensions changed');
            const rgba = ownedHeapCopy(module._BrowserFrameOutputPixels(lease, sequence, screenIdentifier), bytes);
            return {screen_identifier: screenIdentifier, width, height, rgba};
        });
        if (sequence === 1) {
            initialization.checkpoint('first_preview_copied');
            firstPreviewObservation = {initialization: initialization.observation(),
                file_cache: fileCache?.observation() ?? null};
        }
        return {sequence, renderer_frame: rendererFrame, sampled_ticks: sampledTicks, screens,
            ...(sequence === 1 ? {runtime_initialization: initializationObservation()} : {})};
    } finally {
        if (sequence !== undefined)
            requireCondition(module._BrowserFrameOutputRelease(lease, sequence) === 0, 'Preview lease release failed');
    }
}

async function transferPreviewFrame(frame) {
    const screens = await Promise.all(frame.screens.map(async screen => ({...screen,
        sha256: Array.from(new Uint8Array(await crypto.subtle.digest('SHA-256', screen.rgba)))
            .map(value => value.toString(16).padStart(2, '0')).join(''), rgba: screen.rgba.buffer})));
    requireCondition(['running','validating'].includes(phase), 'Preview publication outlived the session');
    const receipt = {sequence: frame.sequence, renderer_frame: frame.renderer_frame,
        sampled_ticks: frame.sampled_ticks, screens: screens.map(({rgba, ...screen}) =>
            ({...screen, bytes: rgba.byteLength}))};
    frameReceipts.push(receipt);
    if (gameplay()) {
        ++gameplayPreviewCount;
        if (frameReceipts.length > MaximumGameplayPreviewReceipts) frameReceipts.shift();
    }
    await new Promise((resolve, reject) => {
        const timer = setTimeout(() => { pendingFrame = undefined;
            reject(new Error('Preview acknowledgment timed out')); }, TransferTimeoutMilliseconds);
        pendingFrame = {sequence: frame.sequence, resolve, reject, timer};
        send({type:'preview_screens', ...frame, screens}, screens.map(screen => screen.rgba));
    });
}

function previewObservations() {
    const lines = stderr.filter(line => line.startsWith('browser frame output '));
    requireCondition(lines.length === 1 && (gameplay() || (frameControls && frameReceipts.length > 0)),
                     'Preview producer or real lease controls are absent');
    const totals = parseRecord(lines[0].slice('browser frame output '.length));
    record(totals, ['unique_frames','published','full_drops','unavailable','duplicate_polls','frame_gaps','geometry_errors']);
    for (const value of Object.values(totals)) requireCondition(typeof value === 'string' &&
        /^(0|[1-9][0-9]*)$/.test(value) && BigInt(value) <= BigInt(MaximumEvents), 'Invalid preview producer total');
    requireCondition(totals.geometry_errors === '0' &&
        BigInt(totals.unique_frames) === BigInt(totals.published) + BigInt(totals.full_drops) + BigInt(totals.unavailable) &&
        BigInt(totals.published) >= BigInt(previewCount()), 'Preview producer totals do not reconcile');
    return {producer: totals, copied_and_acknowledged: previewCount(),
        unconsumed_publications: Number(BigInt(totals.published) - BigInt(previewCount())),
        lease_controls: frameControls, frames: frameReceipts, ...(gameplay() ? {
            frame_receipt_scope: 'last_64_acknowledged_samples',
            evicted_frame_receipts: previewCount() - frameReceipts.length,
            maximum_acknowledged_samples: MaximumGameplayPreviewFrames,
            lease_control_scope: frameControls ? 'observed_on_first_copied_frame' : 'unavailable_no_frame_copied_before_shutdown',
            exact_gpu_replay_accepted: false} : {})};
}

function enumerateCapture() {
    const files = [];
    const directories = [];
    let totalBytes = 0;
    let directoryEntries = 0;
    function walk(relative, depth = 0) {
        requireCondition(depth <= 32, 'Capture directory nesting exceeds the finite bound');
        const path = relative ? `/capture/${relative}` : '/capture';
        requireCondition(module.FS.isDir(module.FS.lstat(path).mode), 'Invalid capture directory');
        for (const name of module.FS.readdir(path).filter(name => name !== '.' && name !== '..').sort()) {
            requireCondition(++directoryEntries <= MaximumDirectoryEntries,
                             'Capture directory entry count exceeds the finite bound');
            const next = relativePath(relative ? `${relative}/${name}` : name);
            const stat = module.FS.lstat(`/capture/${next}`);
            if (module.FS.isDir(stat.mode)) { directories.push(next); walk(next, depth + 1); }
            else {
                requireCondition(module.FS.isFile(stat.mode), 'Capture contains a nonregular file');
                const size = integer(stat.size, MaximumOutputBytes);
                totalBytes += size;
                requireCondition(totalBytes <= MaximumOutputBytes && files.length < MaximumFiles,
                                 'Capture exceeds the finite output bound');
                files.push({relative_path: next, size});
            }
        }
    }
    walk('');
    return {files, directories};
}

function readChunks(path, expectedBytes, accept) {
    const stream = module.FS.open(path, 'r');
    try {
        let position = 0;
        while (position < expectedBytes) {
            const buffer = new Uint8Array(Math.min(ChunkBytes, expectedBytes - position));
            const count = module.FS.read(stream, buffer, 0, buffer.length, position);
            requireCondition(count === buffer.length, 'Finished capture file has a short read');
            accept(buffer, position);
            position += count;
        }
        requireCondition(module.FS.read(stream, new Uint8Array(1), 0, 1, position) === 0,
                         'Finished capture file has a trailing byte');
    } finally {
        module.FS.close(stream);
    }
}

function readText(path, bytes, maximum) {
    integer(bytes, maximum);
    const decoder = new TextDecoder('utf-8', {fatal: true});
    let text = '';
    readChunks(path, bytes, buffer => { text += decoder.decode(buffer, {stream: true}); });
    return text + decoder.decode();
}

// JSON.parse validates syntax; this second lexical walk rejects duplicate keys,
// including differently escaped spellings. No original event bytes are rewritten.
function parseRecord(text) {
    requireCondition(encoder.encode(text).length <= MaximumLineBytes, 'Metadata line exceeds bound');
    const value = JSON.parse(text);
    let position = 0;
    function whitespace() { while (/\s/.test(text[position] ?? '') && position < text.length) ++position; }
    function quoted() {
        const start = position++;
        while (position < text.length) {
            if (text[position++] === '"') return JSON.parse(text.slice(start, position));
            if (text[position - 1] === '\\') ++position;
        }
        throw new Error('Unterminated metadata string');
    }
    function visit(depth) {
        requireCondition(depth <= 32, 'Metadata nesting exceeds bound');
        whitespace();
        const token = text[position++];
        if (token === '{') {
            const keys = new Set();
            whitespace();
            if (text[position] === '}') { ++position; return; }
            while (true) {
                const key = quoted();
                requireCondition(!keys.has(key), 'Duplicate metadata key');
                keys.add(key);
                whitespace(); ++position; visit(depth + 1); whitespace();
                if (text[position++] === '}') return;
                whitespace();
            }
        } else if (token === '[') {
            whitespace();
            if (text[position] === ']') { ++position; return; }
            while (true) {
                visit(depth + 1); whitespace();
                if (text[position++] === ']') return;
            }
        } else if (token === '"') { --position; quoted(); }
        else { while (position < text.length && !/[\s,}\]]/.test(text[position])) ++position; }
    }
    visit(0); whitespace();
    requireCondition(position === text.length && value && typeof value === 'object' &&
                     !Array.isArray(value), 'Invalid metadata record');
    return value;
}

function validateEvents(files) {
    const entries = new Map(files.map(file => [file.relative_path, file]));
    const eventsFile = entries.get('gpu_events.jsonl');
    requireCondition(eventsFile, 'GPU event stream is absent');
    integer(eventsFile.size, MaximumEventBytes, 1);
    const counts = {gsp_command: 0, pica_command_list: 0, vblank: 0};
    const payloadNames = new Set();
    let sequence = 0, payloadBytes = 0, previousTicks = 0;
    let outcome, presentation, submissionTicks, lastVblank;
    const decoder = new TextDecoder('utf-8', {fatal: true});
    let pending = '';
    function line(text) {
        const event = parseRecord(text);
        requireCondition(!outcome && sequence < MaximumEvents && integer(event.sequence) === sequence++ &&
                         typeof event.kind === 'string', 'GPU stream is incomplete');
        integer(event.ticks);
        requireCondition(event.ticks >= previousTicks && event.frame === 0, 'Invalid event timing');
        previousTicks = event.ticks;
        if (Object.hasOwn(counts, event.kind)) {
            const index = counts[event.kind]++;
            if (event.kind === 'gsp_command') requireCondition(event.command_index === index, 'GSP index gap');
            if (event.kind === 'vblank') {
                requireCondition(event.vblank_index === index, 'VBlank index gap');
                lastVblank = event;
            }
            if (event.kind === 'pica_command_list') {
                requireCondition(event.list_index === index && event.payload ===
                    `pica_command_list_${String(index).padStart(6, '0')}.bin`, 'PICA index/path gap');
                integer(event.size, descriptor.options.pica_payload_limit_bytes);
                requireCondition(event.size % 4 === 0 && entries.get(event.payload)?.size === event.size,
                                 'PICA extent disagrees');
                payloadNames.add(event.payload);
                payloadBytes += event.size;
            }
        } else if (event.kind === 'buffer_swap') {
            if (event.screen_id === 0 && submissionTicks === undefined) submissionTicks = event.ticks;
        } else if (event.kind === 'frame_presentation') {
            requireCondition(!presentation, 'Duplicate software presentation');
            presentation = event;
        } else if (event.kind === 'capture_outcome') outcome = event;
        else requireCondition(['hardware_register_write', 'color_fill'].includes(event.kind),
                              'Unknown GPU event kind');
    }
    readChunks('/capture/gpu_events.jsonl', eventsFile.size, buffer => {
        pending += decoder.decode(buffer, {stream: true});
        let end;
        while ((end = pending.indexOf('\n')) >= 0) {
            line(pending.slice(0, end)); pending = pending.slice(end + 1);
        }
        requireCondition(encoder.encode(pending).length <= MaximumLineBytes, 'GPU line exceeds bound');
    });
    pending += decoder.decode();
    requireCondition(pending === '' && outcome, 'GPU stream has no complete final outcome');
    record(outcome, ['sequence', 'kind', 'ticks', 'frame', 'outcome', 'complete', 'gsp_commands',
                     'pica_lists', 'payload_bytes', 'vblanks']);
    const expected = descriptor.options.presentation_limit === null ? 'first_top_screen_swap' : 'software_presentation';
    requireCondition(outcome.complete === true && outcome.outcome === expected &&
        outcome.gsp_commands === counts.gsp_command && outcome.pica_lists === counts.pica_command_list &&
        outcome.vblanks === counts.vblank && outcome.payload_bytes === payloadBytes &&
        payloadBytes <= descriptor.options.pica_payload_limit_bytes && submissionTicks !== undefined,
        'Capture outcome/counts disagree');
    requireCondition(files.filter(file => /^pica_command_list_.*\.bin$/.test(file.relative_path))
        .every(file => payloadNames.has(file.relative_path)), 'Unreferenced PICA payload');
    if (descriptor.options.presentation_limit === null) requireCondition(!presentation, 'Unexpected presentation');
    else requireCondition(presentation && presentation.sequence === outcome.sequence - 1 && lastVblank &&
        presentation.presentation_index === descriptor.options.presentation_limit - 1 &&
        presentation.submission_ticks === submissionTicks && presentation.vblank_index === lastVblank.vblank_index &&
        presentation.ticks === lastVblank.ticks, 'Software presentation boundary disagrees');
    if (presentation) integer(presentation.renderer_frame, Number.MAX_SAFE_INTEGER, 1);
    return {outcome, presentation, entries};
}

function validateGameplaySession(files) {
    const entries = new Map(files.map(file => [file.relative_path, file]));
    requireCondition(!entries.has('gpu_events.jsonl') && !entries.has('gpu_window_events.jsonl') &&
        !files.some(file => /^pica_command_list_.*\.bin$/.test(file.relative_path)),
        'A normal gameplay session must not contain GPU trace or PICA capture files');
    const configurationFile = entries.get('gameplay_session_configuration.json');
    const outcomeFile = entries.get('gameplay_session_outcome.json');
    requireCondition(configurationFile && outcomeFile, 'Gameplay session metadata is absent');
    const configuration = parseRecord(readText('/capture/gameplay_session_configuration.json', configurationFile.size, 4096));
    record(configuration, ['type', 'version', 'mode', 'presentation_limit', 'maximum_presentations', 'presentation_scope',
        'gpu_trace_absent', 'pica_payload_files_absent', 'final_frame_only', 'maximum_rgba_bytes_per_screen',
        'maximum_framebuffer_bytes_per_screen', 'maximum_final_payload_bytes', 'maximum_configuration_bytes',
        'maximum_outcome_bytes', 'wall_deadline_owner', 'input_audio_scope', 'exact_replay_accepted',
        'section_7_accepted', 'goal_accepted', 'host_timing_scope']);
    requireCondition(configuration.type === 'gameplay_session_configuration' && configuration.version === 1 &&
        configuration.mode === 'normal_gameplay_session' && configuration.presentation_limit === descriptor.options.gameplay_session_presentations &&
        configuration.maximum_presentations === 60000 && configuration.presentation_scope === 'natural_presentations_after_first_top_screen_submission' &&
        configuration.gpu_trace_absent === true && configuration.pica_payload_files_absent === true &&
        configuration.final_frame_only === true && configuration.maximum_rgba_bytes_per_screen === 67108864 &&
        configuration.maximum_framebuffer_bytes_per_screen === 16777216 && configuration.maximum_final_payload_bytes === 167772160 &&
        configuration.maximum_configuration_bytes === 4096 && configuration.maximum_outcome_bytes === 16384 &&
        configuration.wall_deadline_owner === 'frontend_explicit_at_most_3600_seconds' &&
        configuration.input_audio_scope === 'frontend_owned' && configuration.exact_replay_accepted === false &&
        configuration.section_7_accepted === false && configuration.goal_accepted === false &&
        configuration.host_timing_scope === 'aggregate_presentation_intervals_and_final_export_including_io',
        'Gameplay configuration disagrees with the selected bounded session');
    const outcome = parseRecord(readText('/capture/gameplay_session_outcome.json', outcomeFile.size, 16384));
    record(outcome, ['type', 'version', 'outcome', 'endpoint', 'complete', 'failure_reason', 'presentation_limit',
        'presentations', 'presentation_callbacks', 'top_screen_submitted', 'submission_ticks', 'last_eligible_presentation_ticks',
        'finish_ticks', 'stop_pending_at_finish', 'stop_observed_at_final_presentation', 'limit_reached_at_final_presentation',
        'final_frame_exported', 'payload_files_written', 'payload_bytes_written', 'final_presentation', 'screens', 'counts',
        'host_diagnostics', 'gpu_trace_absent', 'pica_payload_files_absent', 'exact_replay_accepted', 'section_7_accepted', 'goal_accepted']);
    requireCondition(outcome.type === 'gameplay_session_outcome' && outcome.version === 1 && outcome.complete === true &&
        outcome.failure_reason === null && ['presentation_limit', 'requested_stop'].includes(outcome.outcome) &&
        outcome.endpoint === outcome.outcome && outcome.presentation_limit === configuration.presentation_limit &&
        outcome.top_screen_submitted === true && outcome.final_frame_exported === true && outcome.payload_files_written === 4 &&
        outcome.gpu_trace_absent === true && outcome.pica_payload_files_absent === true &&
        outcome.exact_replay_accepted === false && outcome.section_7_accepted === false && outcome.goal_accepted === false,
        'Gameplay session did not finish its natural endpoint and final export');
    integer(outcome.presentations, configuration.presentation_limit, 1);
    integer(outcome.presentation_callbacks, Number.MAX_SAFE_INTEGER, outcome.presentations);
    requireCondition(typeof outcome.stop_pending_at_finish === 'boolean' &&
        outcome.stop_observed_at_final_presentation === (outcome.outcome === 'requested_stop') &&
        outcome.limit_reached_at_final_presentation === (outcome.presentations === configuration.presentation_limit) &&
        (outcome.outcome !== 'presentation_limit' || outcome.limit_reached_at_final_presentation) &&
        (outcome.outcome !== 'requested_stop' || gameplayStopAccepted), 'Gameplay Stop or limit outcome disagrees');
    record(outcome.final_presentation, ['presentation_index', 'renderer_frame', 'ticks', 'vblank_index']);
    const final = outcome.final_presentation;
    integer(final.renderer_frame, Number.MAX_SAFE_INTEGER, 1);
    integer(final.ticks); integer(outcome.submission_ticks); integer(outcome.last_eligible_presentation_ticks); integer(outcome.finish_ticks);
    requireCondition(final.presentation_index === outcome.presentations - 1 && outcome.submission_ticks <= final.ticks &&
        outcome.last_eligible_presentation_ticks === final.ticks && final.ticks <= outcome.finish_ticks, 'Gameplay final boundary disagrees');
    record(outcome.counts, ['gsp_commands', 'pica_lists', 'pica_bytes_observed_without_export', 'buffer_swaps',
        'vblanks', 'hardware_register_writes', 'color_fills']);
    for (const value of Object.values(outcome.counts)) integer(value);
    requireCondition(outcome.counts.buffer_swaps > 0 && outcome.counts.vblanks > 0 &&
        final.vblank_index === outcome.counts.vblanks - 1, 'Gameplay aggregate counters disagree');
    record(outcome.host_diagnostics, ['elapsed_ns', 'first_eligible_presentation_elapsed_ns', 'presentation_interval_samples',
        'presentation_interval_total_ns', 'presentation_interval_min_ns', 'presentation_interval_max_ns', 'final_export_elapsed_ns']);
    const timing = outcome.host_diagnostics;
    for (const key of ['elapsed_ns', 'first_eligible_presentation_elapsed_ns', 'presentation_interval_samples',
                       'presentation_interval_total_ns', 'final_export_elapsed_ns']) integer(timing[key]);
    requireCondition(timing.presentation_interval_samples === outcome.presentations - 1 &&
        timing.first_eligible_presentation_elapsed_ns <= timing.elapsed_ns && timing.final_export_elapsed_ns <= timing.elapsed_ns,
        'Gameplay host timing diagnostics disagree');
    if (timing.presentation_interval_samples) {
        integer(timing.presentation_interval_min_ns); integer(timing.presentation_interval_max_ns);
        requireCondition(timing.presentation_interval_min_ns <= timing.presentation_interval_max_ns,
            'Gameplay host interval range disagrees');
    } else requireCondition(timing.presentation_interval_min_ns === null && timing.presentation_interval_max_ns === null &&
        timing.presentation_interval_total_ns === 0, 'Unexpected single-presentation timing interval');
    const presentation = {...final, submission_ticks: outcome.submission_ticks, screens: outcome.screens};
    const screens = softwareScreens(presentation, entries);
    requireCondition(outcome.payload_bytes_written === screens.reduce((sum, screen) =>
        sum + screen.metadata.rgba_bytes + screen.metadata.framebuffer_bytes, 0) &&
        outcome.payload_bytes_written <= configuration.maximum_final_payload_bytes, 'Gameplay final payload count disagrees');
    return {outcome, presentation, entries, screens, configuration};
}

function softwareScreens(presentation, entries) {
    if (!presentation) return [];
    requireCondition(Array.isArray(presentation.screens) && presentation.screens.length === 2,
                     'Software capture needs two screens');
    return presentation.screens.map((screen, index) => {
        record(screen, ['screen_id', 'storage_width', 'storage_height', 'width', 'height',
            'rgba_payload', 'rgba_bytes', 'framebuffer_payload', 'framebuffer_bytes',
            'framebuffer_width', 'framebuffer_height', 'stride', 'format', 'color_format',
            'active_fb', 'physical_address', 'right_physical_address', 'color_fill']);
        const identifier = index === 0 ? 0 : 2;
        requireCondition(screen.screen_id === identifier, 'Software screen identifier disagrees');
        for (const key of ['width', 'height', 'storage_width', 'storage_height']) integer(screen[key], 4096, 1);
        requireCondition(screen.width === screen.storage_height && screen.height === screen.storage_width &&
            screen.rgba_bytes === screen.width * screen.height * 4 &&
            screen.rgba_payload === `rendered_screen_${identifier}.rgba` &&
            entries.get(screen.rgba_payload)?.size === screen.rgba_bytes, 'Software RGBA extent disagrees');
        integer(screen.framebuffer_width, 65535, 1);
        integer(screen.framebuffer_height, 65535, 1);
        integer(screen.stride, UInt32Maximum, 1);
        requireCondition(screen.framebuffer_bytes === screen.stride * screen.framebuffer_height &&
            screen.framebuffer_bytes <= 16 * 1024 * 1024 &&
            screen.framebuffer_payload === `framebuffer_screen_${identifier}.bin` &&
            entries.get(screen.framebuffer_payload)?.size === screen.framebuffer_bytes,
            'Raw framebuffer extent disagrees');
        integer(screen.color_format, 4);
        integer(screen.format, UInt32Maximum);
        integer(screen.active_fb, UInt32Maximum);
        integer(screen.physical_address, UInt32Maximum, 1);
        integer(screen.right_physical_address, UInt32Maximum);
        integer(screen.color_fill, UInt32Maximum);
        const bytesPerPixel = [4, 3, 2, 2, 2][screen.color_format];
        requireCondition(screen.color_format === (screen.format & 7) &&
            screen.stride % bytesPerPixel === 0 &&
            screen.stride >= screen.framebuffer_width * bytesPerPixel &&
            screen.physical_address + screen.framebuffer_bytes <= UInt32Maximum,
            'Raw framebuffer format/address disagrees');
        const rgba = new Uint8Array(screen.rgba_bytes);
        readChunks(`/capture/${screen.rgba_payload}`, screen.rgba_bytes,
                   (buffer, offset) => rgba.set(buffer, offset));
        return {metadata: screen, rgba: rgba.buffer};
    });
}

function observationRecords(entry, accept) {
    requireCondition(entry && entry.size > 0 && entry.size <= maximumAudioBytes(), 'Observation extent is invalid');
    const decoder = new TextDecoder('utf-8', {fatal: true});
    let pending = '';
    let count = 0;
    function consume(text) {
        pending += text;
        let newline;
        while ((newline = pending.indexOf('\n')) !== -1) {
            requireCondition(++count <= MaximumEvents, 'Observation event count exceeds bound');
            accept(parseRecord(pending.slice(0, newline)));
            pending = pending.slice(newline + 1);
        }
        requireCondition(encoder.encode(pending).length <= MaximumLineBytes, 'Observation line exceeds bound');
    }
    readChunks(`/capture/${entry.relative_path}`, entry.size, buffer => consume(decoder.decode(buffer, {stream: true})));
    consume(decoder.decode());
    requireCondition(pending === '', 'Observation has an incomplete final line');
}

function validateObservations(presentation, entries) {
    if (!descriptor.options.input_capture && !descriptor.options.audio_capture) return {};
    requireCondition(presentation, 'Observations need a software presentation');
    const configurationEntry = entries.get('capture_configuration.json');
    requireCondition(configurationEntry, 'Capture configuration is absent');
    const configuration = parseRecord(readText('/capture/capture_configuration.json', configurationEntry.size, MaximumLineBytes));
    const baseTicks = integer(configuration.base_ticks);
    let inputOutcome;
    let polls = 0;
    let previousTicks = baseTicks;
    let previousFrame = 0;
    observationRecords(entries.get('input_events.jsonl'), event => {
        requireCondition(!inputOutcome, 'Input observation continues after its outcome');
        if (event.kind === 'input_outcome') {
            record(event, ['sequence', 'kind', 'polls', 'complete']);
            requireCondition(integer(event.sequence) === polls && integer(event.polls, MaximumEvents, 1) === polls &&
                event.complete === true, 'Input observation is incomplete');
            inputOutcome = event;
            return;
        }
        record(event, ['sequence', 'kind', 'ticks', 'renderer_frame', 'pad_index', 'touch_index', 'buttons',
            'delta_additions', 'delta_removals', 'circle_pad_x', 'circle_pad_y', 'touch_x', 'touch_y', 'touch_valid']);
        requireCondition(event.kind === 'hid_input' && integer(event.sequence) === polls++, 'Input sequence disagrees');
        const ticks = integer(event.ticks);
        const frame = integer(event.renderer_frame);
        requireCondition(ticks >= previousTicks && frame >= previousFrame && ticks <= presentation.ticks &&
            frame <= presentation.renderer_frame, 'Input timing exceeds its presentation boundary');
        previousTicks = ticks; previousFrame = frame;
        integer(event.pad_index, 7); integer(event.touch_index, 7);
        for (const key of ['buttons', 'delta_additions', 'delta_removals']) integer(event[key], UInt32Maximum);
        integer(event.circle_pad_x, 154, -154); integer(event.circle_pad_y, 154, -154);
        integer(event.touch_x, 319); integer(event.touch_y, 239); integer(event.touch_valid, 1);
    });
    requireCondition(inputOutcome, 'Input observation has no final outcome');
    if (!descriptor.options.audio_capture) return {input: inputOutcome};
    requireCondition(configuration.audio_sink === 'null', 'Audio observation must preserve the null sink');
    let audioOutcome;
    let blocks = 0;
    previousTicks = baseTicks; previousFrame = 0;
    observationRecords(entries.get('audio_events.jsonl'), event => {
        requireCondition(!audioOutcome, 'Audio observation continues after its outcome');
        if (event.kind === 'audio_outcome') {
            record(event, ['sequence', 'kind', 'blocks', 'sample_frames', 'payload_bytes', 'complete']);
            requireCondition(integer(event.sequence) === blocks && integer(event.blocks, MaximumEvents, 1) === blocks &&
                integer(event.sample_frames) === blocks * 160 && integer(event.payload_bytes, maximumAudioBytes(), 1) === blocks * 640 &&
                event.complete === true, 'Audio observation is incomplete');
            audioOutcome = event;
            return;
        }
        record(event, ['sequence', 'kind', 'ticks', 'renderer_frame', 'sample_rate', 'channels',
            'sample_frames', 'first_sample_frame', 'payload_offset', 'payload_bytes']);
        requireCondition(event.kind === 'audio_samples' && integer(event.sequence) === blocks &&
            event.sample_rate === 32728 && event.channels === 2 && event.sample_frames === 160 &&
            integer(event.first_sample_frame) === blocks * 160 && integer(event.payload_offset) === blocks * 640 &&
            event.payload_bytes === 640, 'Audio sample extent disagrees');
        const ticks = integer(event.ticks);
        const frame = integer(event.renderer_frame);
        requireCondition(ticks >= previousTicks && frame >= previousFrame && ticks <= presentation.ticks &&
            frame <= presentation.renderer_frame, 'Audio timing exceeds its presentation boundary');
        previousTicks = ticks; previousFrame = frame;
        requireCondition(++blocks * 640 <= maximumAudioBytes(), 'Audio payload exceeds bound');
    });
    const pcm = entries.get('audio_pcm_s16le.bin');
    requireCondition(audioOutcome && pcm && pcm.size === audioOutcome.payload_bytes, 'Audio PCM extent disagrees');
    return {input: inputOutcome, audio: {relative_path: pcm.relative_path, payload_bytes: pcm.size,
        sample_rate: 32728, channels: 2, sample_frames: audioOutcome.sample_frames, blocks}};
}

function validateRecordedMovie(entries, observations) {
    const entry = entries.get('input_movie.ctm');
    requireCondition(entry && entry.size > 256 && entry.size <= 16 * 1024 * 1024 &&
                     (entry.size - 256) % 7 === 0, 'Recorded movie extent is invalid');
    const bytes = new Uint8Array(entry.size);
    readChunks('/capture/input_movie.ctm', entry.size, (buffer, offset) => bytes.set(buffer, offset));
    const view = new DataView(bytes.buffer);
    const configuration = parseRecord(readText('/capture/capture_configuration.json',
        entries.get('capture_configuration.json').size, MaximumLineBytes));
    const program = parseRecord(readText('/capture/program_identity.json',
        entries.get('program_identity.json')?.size, MaximumLineBytes));
    requireCondition(bytes[0] === 0x43 && bytes[1] === 0x54 && bytes[2] === 0x4d && bytes[3] === 0x1b &&
        view.getBigUint64(4, true) === BigInt(integer(program.program_id, Number.MAX_SAFE_INTEGER, 1)) &&
        [...bytes.slice(12, 32)].map(value => value.toString(16).padStart(2, '0')).join('') === configuration.source_commit &&
        view.getBigUint64(32, true) === BigInt(integer(configuration.init_time)) &&
        view.getBigInt64(92, true) === BigInt(descriptor.options.record_base_ticks) &&
        configuration.base_ticks === Number(view.getBigInt64(92, true)) &&
        bytes.slice(100, 256).every(value => value === 0), 'Recorded movie metadata disagrees');
    let padOffset = -1;
    let awaitingTouch = false;
    const delivered = [];
    const counts = Array(6).fill(0);
    for (let offset = 256; offset < bytes.length; offset += 7) {
        const kind = bytes[offset];
        requireCondition(kind < counts.length && (!awaitingTouch || kind === 1) &&
                         (kind !== 1 || awaitingTouch), 'Recorded movie call order disagrees');
        ++counts[kind];
        if (kind === 0) {
            padOffset = offset;
            awaitingTouch = true;
            const buttons = view.getUint16(offset + 1, true);
            const x = view.getInt16(offset + 3, true), y = view.getInt16(offset + 5, true);
            requireCondition((buttons & ~0x3fff) === 0 && Math.abs(x) <= 154 && Math.abs(y) <= 154,
                             'Recorded pad exceeds its bounds');
        } else if (kind === 1) {
            const x = view.getUint16(offset + 1, true), y = view.getUint16(offset + 3, true);
            requireCondition(x < 320 && y < 240 && bytes[offset + 5] <= 1 && bytes[offset + 6] === 0,
                             'Recorded touch exceeds its bounds');
            delivered.push({buttons: view.getUint16(padOffset + 1, true),
                circle_pad_x: view.getInt16(padOffset + 3, true), circle_pad_y: view.getInt16(padOffset + 5, true),
                touch_x: x, touch_y: y, touch_valid: bytes[offset + 5]});
            awaitingTouch = false;
        } else if (kind === 4) requireCondition(bytes[offset + 5] <= 1 && bytes[offset + 6] <= 1,
                                               'Recorded infrared flags are invalid');
        else if (kind === 5) requireCondition(bytes[offset + 5] === 0 && bytes[offset + 6] === 0,
                                              'Recorded extra-HID padding is invalid');
    }
    requireCondition(!awaitingTouch && delivered.length === observations.input.polls &&
        counts[0] === delivered.length && counts[1] === delivered.length &&
        view.getBigUint64(84, true) === BigInt(delivered.length), 'Recorded pad/poll counts disagree');
    let index = 0;
    observationRecords(entries.get('input_events.jsonl'), event => {
        if (event.kind !== 'hid_input') return;
        const pad = delivered[index++];
        requireCondition(pad && pad.buttons === (event.buttons & 0x3fff) &&
            ['circle_pad_x', 'circle_pad_y', 'touch_x', 'touch_y', 'touch_valid'].every(key => pad[key] === event[key]),
            'Recorded movie differs from delivered HID');
    });
    requireCondition(index === delivered.length, 'Recorded movie has an unmatched poll');
    return {pad_count: delivered.length, record_counts: counts, base_ticks: descriptor.options.record_base_ticks};
}

async function exportFile(file, fileIndex) {
    const stream = module.FS.open(`/capture/${file.relative_path}`, 'r');
    try {
        let offset = 0;
        while (offset < file.size) {
            const bytes = new Uint8Array(Math.min(ChunkBytes, file.size - offset));
            requireCondition(module.FS.read(stream, bytes, 0, bytes.length, offset) === bytes.length,
                             'Capture file changed during export');
            const length = bytes.length;
            await acknowledged({type: 'capture_file_chunk', file_index: fileIndex, offset,
                                total_bytes: file.size, bytes: bytes.buffer}, [bytes.buffer]);
            offset += length;
        }
        requireCondition(module.FS.read(stream, new Uint8Array(1), 0, 1, offset) === 0,
                         'Capture file grew during export');
    } finally { module.FS.close(stream); }
}

async function finish(status) {
    clearInterval(frameTimer);
    clearInterval(audioTimer);
    webGlPresentationReceiver?.close();
    requireCondition(phase === 'running' && status === 0, `Capture exited with status ${status}`);
    phase = 'validating';
    if (audioStream) send({type: 'audio_stream_source_ended'});
    clearInterval(buttonTimer);
    clearInterval(circleTimer);
    clearInterval(touchTimer);
    clearInterval(gameplayTimer);
    // Any already copied ordinary frame can finish its page acknowledgment.
    // Native exports are forbidden here because SDK exitRuntime has run.
    await frameTask;
    const audioObservation = await completeAudio();
    requireCondition(phase === 'validating', 'Preview failure prevents capture completion');
    if (gameplay()) send({type: 'gameplay_session_ended'});
    else if (descriptor.options.live_button_capture) send({type: 'button_capture_ended'});
    if (descriptor.options.live_circle_pad_capture) send({type: 'circle_pad_capture_ended'});
    if (descriptor.options.live_touch_capture) send({type: 'touch_capture_ended'});
    const {files, directories} = enumerateCapture();
    const validation = gameplay() ? validateGameplaySession(files) : validateEvents(files);
    const {outcome, presentation, entries} = validation;
    const observations = validateObservations(presentation, entries);
    observations.runtime_initialization = initializationObservation();
    if (audioObservation) {
        requireCondition(!audioObservation.enabled || audioObservation.state === 'stopped' ||
            audioObservation.sample_frames === observations.audio?.sample_frames,
            'Streamed sound did not cover the original audio observer');
        observations.audio_stream = audioObservation;
    }
    if (recording()) observations.recorded_movie = validateRecordedMovie(entries, observations);
    if (descriptor.options.frame_output) observations.preview_frames = previewObservations();
    if (webGlPresentationReceiver)
        observations.webgl_presentation = {...webGlPresentationReceiver.statistics};
    const log = entries.get('user/log/reference_capture.log');
    requireCondition(log && !/\b(?:Input|Movie|Audio|Service\.DSP)(?:\.[A-Za-z0-9_.]+)?\s+<(?:Error|Critical)>/.test(readText('/capture/user/log/reference_capture.log',
        log.size, MaximumLogBytes)), 'Capture log is absent or reports Movie/Audio errors');
    const counters = stderr.flatMap(line => {
        const match = /^static CPU ([0-9]+) executed ([0-9]+) guest instructions; interpreter\/JIT fallbacks ([0-9]+)$/.exec(line);
        return match ? [{cpu_identifier: match[1], instructions: match[2], fallbacks: match[3], original_line: line}] : [];
    });
    requireCondition(counters.length > 0 && counters.every(counter => counter.fallbacks === '0') &&
        new Set(counters.map(counter => counter.cpu_identifier)).size === counters.length, 'Invalid CPU counters');
    if (gameplay()) requireCondition(counters.length === 2 && counters.some(counter => counter.cpu_identifier === '0') &&
        counters.some(counter => counter.cpu_identifier === '1'), 'Both gameplay CPU zero-fallback reports are required');
    const screens = validation.screens ?? softwareScreens(presentation, entries);
    phase = 'exporting';
    const manifest = {type: 'capture_manifest', files, directories, outcome, stdout, stderr, counters, observations};
    if (gameplay()) {
        observations.gameplay_session = {mode: descriptor.options.gameplay_input_mode,
            configuration: validation.configuration, aggregate_gpu_counts_only: true,
            gpu_trace_absent: true, pica_capture_files_absent: true,
            exact_gpu_replay_accepted: false, section_7_accepted: false, goal_accepted: false,
            controller_request_scope: 'last_128_requests_per_device; delivered_input_movie_retains_all_polls',
            button_request_count: gameplayButtonSequence, circle_request_count: circleSequence,
            touch_request_count: touchSequence, stop_requested: gameplayStopRequested,
            stop_accepted_by_native_bridge: gameplayStopAccepted, stop_release: gameplayStopRelease ?? null,
            live_control_bridge_observed: gameplayControlsObserved,
            control_delivery_scope: 'only_actual_HID_movie_polls; setter_acceptance_is_not_delivery'};
        manifest.gameplay_progress = gameplayProgress ?? null;
        manifest.gameplay_button_requests = gameplayButtonRequests;
    } else if (descriptor.options.live_button_capture) {
        requireCondition(buttonProgress?.poll_count > 0 && entries.get('input_movie.ctm')?.size > 256,
                         'Live input device or recorded movie is absent');
        manifest.button_requests = buttonRequests;
        manifest.button_progress = buttonProgress;
    }
    if (descriptor.options.live_circle_pad_capture) {
        const closure = stderr.flatMap(line => {
            const match = /^browser circle pad closed after ([0-9]+) device polls; profile restored$/.exec(line);
            return match ? [Number(match[1])] : [];
        });
        requireCondition((gameplay() || (circleProgress?.poll_count > 0 && circleControls?.active)) &&
            closure.length === 1 && closure[0] === observations.input.polls &&
            entries.get('input_movie.ctm')?.size > 256, 'Circle-pad device, cleanup or recorded movie is absent');
        manifest.circle_pad_requests = circleRequests;
        manifest.circle_pad_progress = circleProgress ?? null;
        manifest.circle_pad_controls = circleControls;
        manifest.circle_pad_closed_polls = closure[0];
    }
    if (descriptor.options.live_touch_capture) {
        const closure = stderr.flatMap(line => {
            const match = /^browser touch closed after ([0-9]+) device polls; profile restored$/.exec(line);
            return match ? [Number(match[1])] : [];
        });
        requireCondition((gameplay() || (touchProgress?.poll_count > 0 && touchControls?.active)) &&
            closure.length === 1 && closure[0] === observations.input.polls &&
            entries.get('input_movie.ctm')?.size > 256, 'Touch device, cleanup or recorded movie is absent');
        manifest.touch_requests = touchRequests;
        manifest.touch_progress = touchProgress ?? null;
        manifest.touch_controls = touchControls;
        manifest.touch_closed_polls = closure[0];
    }
    requireCondition(encoder.encode(JSON.stringify({schema_version: 1,
        capture_identifier: descriptor.capture_identifier, transfer_identifier: nextTransferIdentifier,
        ...manifest})).length <= MaximumLogBytes, 'Serialized capture metadata exceeds the finite bound');
    await acknowledged(manifest);
    if (screens.length) await acknowledged({type: 'software_screens', presentation, screens},
                                         screens.map(screen => screen.rgba));
    for (let index = 0; index < files.length; ++index) await exportFile(files[index], index);
    await acknowledged({type: 'capture_completed', exit_status: status, outcome, files: files.length});
    phase = 'closed';
    fileCache?.dispose();
    send({type: 'shutdown_complete'});
    self.close();
}

async function start(value) {
    if (typeof value?.capture_identifier === 'string' && /^[A-Za-z0-9_-]{1,64}$/.test(value.capture_identifier))
        descriptor = {capture_identifier: value.capture_identifier};
    descriptor = validateStart(value);
    requireCondition(self.crossOriginIsolated && typeof SharedArrayBuffer === 'function',
                     'Browser isolation/shared memory is unavailable');
    requireCondition(!self.name?.startsWith('em-pthread'), 'Runtime worker uses a reserved pthread name');
    const configured = new URL(self.location.href).searchParams.get('module_url');
    requireCondition(configured, 'The worker needs a configured module_url');
    const url = new URL(configured, self.location.href);
    requireCondition(url.origin === self.location.origin && ['http:', 'https:'].includes(url.protocol) &&
                     !url.username && !url.password && !url.hash, 'Module URL must be same-origin');
    phase = 'initializing';
    initialization = beginRuntimeInitialization(url.href, {noInitialRun: true, thisProgram: 'root_port_browser_capture',
        locateFile: name => new URL(name, url).href,
        preRun: [instance => {
            instance.FS.mkdir('/owned');
            const mount = instance.FS.mount(instance.WORKERFS, {blobs: descriptor.validated_inputs.map(input =>
                ({name: input.path, data: input.file}))}, '/owned');
            for (const path of ['initial_user_state', ...descriptor.inputs.initial_user_directories
                    .map(name => `initial_user_state/${name}`)]) {
                let parent = mount;
                for (const component of path.split('/')) {
                    const child = Object.hasOwn(parent.contents, component) ? parent.contents[component] :
                        instance.WORKERFS.createNode(parent, component, instance.WORKERFS.DIR_MODE, 0);
                    requireCondition(instance.FS.isDir(child.mode) && child.parent === parent &&
                        child.name === component,
                        'Initial-user mount contains an invalid directory node');
                    parent = child;
                }
            }
            if (gameplay()) fileCache = createWorkerFileCache(instance,
                descriptor.validated_inputs.map(input => `/owned/${input.path}`));
            if (gameplay()) {
                for (const name of Object.keys(instance.ENV)) if (name.startsWith('ROOT_PORT_')) delete instance.ENV[name];
                instance.ENV.ROOT_PORT_GAMEPLAY_SESSION_PRESENTATIONS = String(descriptor.options.gameplay_session_presentations);
            }
            instance.ENV.ROOT_PORT_CAPTURE_WALL_SECONDS = String(descriptor.options.wall_time_seconds);
            if (!gameplay()) instance.ENV.ROOT_PORT_CAPTURE_PICA_BYTES = String(descriptor.options.pica_payload_limit_bytes);
            if (!gameplay() && descriptor.options.presentation_limit !== null)
                instance.ENV.ROOT_PORT_CAPTURE_PRESENTATION = String(descriptor.options.presentation_limit);
            else delete instance.ENV.ROOT_PORT_CAPTURE_PRESENTATION;
            for (const [option, name] of [['input_capture', 'ROOT_PORT_CAPTURE_INPUTS'], ['audio_capture', 'ROOT_PORT_CAPTURE_AUDIO']]) {
                if (descriptor.options[option]) instance.ENV[name] = '1';
                else delete instance.ENV[name];
            }
            for (const name of ['ROOT_PORT_BROWSER_BUTTON_CAPTURE', 'ROOT_PORT_RECORD_INITIAL_USER_STATE',
                                'ROOT_PORT_RECORD_BASE_TICKS', 'ROOT_PORT_BROWSER_FRAME_OUTPUT',
                                'ROOT_PORT_BROWSER_CIRCLE_PAD_CAPTURE', 'ROOT_PORT_BROWSER_TOUCH_CAPTURE']) delete instance.ENV[name];
            if (descriptor.options.frame_output) instance.ENV.ROOT_PORT_BROWSER_FRAME_OUTPUT = '1';
            if (descriptor.options.live_button_capture) {
                instance.ENV.ROOT_PORT_BROWSER_BUTTON_CAPTURE = '1';
            }
            if (descriptor.options.live_circle_pad_capture)
                instance.ENV.ROOT_PORT_BROWSER_CIRCLE_PAD_CAPTURE = '1';
            if (descriptor.options.live_touch_capture) instance.ENV.ROOT_PORT_BROWSER_TOUCH_CAPTURE = '1';
            if (recording()) {
                instance.ENV.ROOT_PORT_RECORD_INITIAL_USER_STATE = '/owned/initial_user_state';
                instance.ENV.ROOT_PORT_RECORD_BASE_TICKS = descriptor.options.record_base_ticks;
            }
        }],
        print: line => captureLine(stdout, line), printErr: line => captureLine(stderr, line),
        onAbort: reason => fail(new Error(`Runtime aborted: ${reason}`)),
        onExit: status => { void finish(status).catch(fail); }
    });
    module = await initialization.ready;
    requireCondition(phase !== 'failed', 'Module initialization failed');
    if (typeof globalThis.installBrowserWebGlPresentationReceiver === 'function')
        webGlPresentationReceiver = globalThis.installBrowserWebGlPresentationReceiver({
            publishFrame: (packet, transfer) => {
                if (phase !== 'running' || !descriptor.options.frame_output) return false;
                send(packet, transfer);
                return true;
            }
        });
    phase = 'identifying';
    initialization.checkpoint('input_identity_started');
    for (const input of descriptor.validated_inputs) {
        const status = module.ccall('BrowserInputIdentityValidateSha256', 'number',
            ['string', 'string', 'number'], [`/owned/${input.path}`, input.expected_sha256, input.expected_bytes]);
        requireCondition(status === 0, `Input identity failed for ${input.path}, status ${status}`);
    }
    initialization.checkpoint('input_identity_completed');
    if (descriptor.options.live_circle_pad_capture) {
        circleControls = {before_install: {
            neutral: module._BrowserCirclePadInputSetPosition(0, 0),
            out_of_range: module._BrowserCirclePadInputSetPosition(155, 0),
            outside_disk: module._BrowserCirclePadInputSetPosition(154, 154),
            active: module._BrowserCirclePadInputIsActive()
        }};
        requireCondition(circleControls.before_install.neutral === 2 && circleControls.before_install.out_of_range === 1 &&
            circleControls.before_install.outside_disk === 1 && circleControls.before_install.active === 0,
            'Inactive circle-pad controls failed');
    }
    if (descriptor.options.live_touch_capture) {
        touchControls = {before_install: {
            neutral: module._BrowserTouchInputSetState(0, 0, 0),
            out_of_range: module._BrowserTouchInputSetState(320, 0, 1),
            invalid_pressed: module._BrowserTouchInputSetState(0, 0, 2),
            noncanonical_release: module._BrowserTouchInputSetState(1, 0, 0),
            active: module._BrowserTouchInputIsActive()
        }};
        requireCondition(touchControls.before_install.neutral === 2 && touchControls.before_install.out_of_range === 1 &&
            touchControls.before_install.invalid_pressed === 1 && touchControls.before_install.noncanonical_release === 1 &&
            touchControls.before_install.active === 0,
            'Inactive touch controls failed');
    }
    if (gameplay()) for (const name of ['BrowserGameplaySessionRequestStop', 'BrowserGameplaySessionIsActive',
        'BrowserButtonInputSetButtonMask', 'BrowserButtonInputHeldButtonMask', 'BrowserButtonInputHasGameplayControls'])
        requireCondition(typeof module[`_${name}`] === 'function', `Gameplay runtime export is absent: ${name}`);
    if (descriptor.options.stream_audio_output) {
        const deliver = packet => audioAcknowledged('audio_stream_packet', {...packet, pcm: packet.pcm.buffer}, [packet.pcm.buffer]);
        audioStream = gameplay() ? new AudioFileStream(module.FS, descriptor.capture_identifier, deliver, () => phase, MaximumGameplayAudioBytes) :
            new AudioFileStream(module.FS, descriptor.capture_identifier, deliver, () => phase);
    }
    initialization.checkpoint('runtime_admitted');
    admissionObservation = {initialization: initialization.observation(),
        file_cache: fileCache?.observation() ?? null};
    send({type: 'capture_started', runtime_initialization: initializationObservation()});
    phase = 'running';
    // In the pinned SDK this launches the proxy pthread. It is not completion.
    const arguments_ = ['/owned/block_schedule.bin', '/owned/dump.3ds', '/capture'];
    if (!recording())
        arguments_.push('/owned/input_movie.ctm', '/owned/initial_user_state');
    const launchStatus = module.callMain(arguments_);
    requireCondition(launchStatus === 0, `CPU pthread launch refused with status ${launchStatus}`);
    if (audioStream) audioTimer = setInterval(() => {
        if (phase !== 'running') return;
        void audioStream.pump().catch(error => { if (!audioStream.closed) fail(error); });
    }, 25);
    if (descriptor.options.frame_output) frameTimer = setInterval(() => {
        if (phase !== 'running' || frameTask) return;
        try {
            const frame = copyPreviewFrame();
            if (frame) frameTask = transferPreviewFrame(frame).catch(fail).finally(() => { frameTask = undefined; });
        } catch (error) { fail(error); }
    }, 8);
    if (gameplay()) gameplayTimer = setInterval(() => {
        if (phase !== 'running') return;
        try {
            gameplayProgress = {active: module._BrowserGameplaySessionIsActive(),
                input_active: module._BrowserButtonInputIsActive(),
                has_gameplay_controls: module._BrowserButtonInputHasGameplayControls(),
                requested_button_mask: module._BrowserButtonInputHeldButtonMask(),
                poll_count: module._BrowserButtonInputPollCount(),
                sampled_renderer_frame: module._BrowserButtonInputRendererFrame()};
            for (const key of ['active', 'input_active', 'has_gameplay_controls']) integer(gameplayProgress[key], 1);
            integer(gameplayProgress.requested_button_mask, 0x0fff);
            integer(gameplayProgress.poll_count, UInt32Maximum); integer(gameplayProgress.sampled_renderer_frame, UInt32Maximum);
            if (gameplayProgress.input_active && gameplayProgress.has_gameplay_controls && gameplayProgress.poll_count)
                gameplayControlsObserved = true;
            requireCondition(recording() || (gameplayProgress.input_active === 0 && gameplayProgress.has_gameplay_controls === 0),
                'Read-only gameplay replay enabled live HID controls');
            if (gameplayStopRequested && !gameplayStopAccepted && gameplayProgress.active) applyGameplayStop();
            send({type: 'gameplay_session_progress', ...gameplayProgress, stop_requested: gameplayStopRequested,
                stop_accepted: gameplayStopAccepted});
        } catch (error) { fail(error); }
    }, 25);
    if (!gameplay() && descriptor.options.live_button_capture) buttonTimer = setInterval(() => {
        if (phase !== 'running') return;
        // The CPU samples its renderer during HID polling. These exports read
        // only our atomics, not Core or the renderer on this outer thread.
        buttonProgress = {active: module._BrowserButtonInputIsActive(),
            poll_count: module._BrowserButtonInputPollCount(),
            sampled_renderer_frame: module._BrowserButtonInputRendererFrame()};
        send({type: 'button_capture_progress', ...buttonProgress});
    }, 25);
    if (descriptor.options.live_circle_pad_capture) circleTimer = setInterval(() => {
        if (phase !== 'running') return;
        try {
            const active = module._BrowserCirclePadInputIsActive();
            const pollCount = module._BrowserCirclePadInputPollCount();
            const packed = module._BrowserCirclePadInputSampledPosition();
            const x = (packed & 0x1ff) - 154;
            const y = ((packed >>> 9) & 0x1ff) - 154;
            circlePosition(x, y);
            if (active && !circleControls.active) {
                circleControls.active = {
                    out_of_range: module._BrowserCirclePadInputSetPosition(-155, 0),
                    outside_disk: module._BrowserCirclePadInputSetPosition(-154, -154)
                };
                requireCondition(circleControls.active.out_of_range === 1 && circleControls.active.outside_disk === 1,
                    'Active circle-pad controls failed');
                if (!descriptor.options.live_button_capture) {
                    circleControls.active.button_input_active = module._BrowserButtonInputIsActive();
                    circleControls.active.button_setter_refused = module._BrowserButtonInputSetHeldState(1);
                    requireCondition(circleControls.active.button_input_active === 0 &&
                        circleControls.active.button_setter_refused === 2, 'Circle-only capture enabled A input');
                }
            }
            circleProgress = {active, poll_count: pollCount, sampled_requested_x: x, sampled_requested_y: y,
                sampled_renderer_frame: module._BrowserButtonInputRendererFrame()};
            send({type: 'circle_pad_capture_progress', ...circleProgress});
        } catch (error) { fail(error); }
    }, 25);
    if (descriptor.options.live_touch_capture) touchTimer = setInterval(() => {
        if (phase !== 'running') return;
        try {
            const active = module._BrowserTouchInputIsActive();
            const pollCount = module._BrowserTouchInputPollCount();
            const packed = module._BrowserTouchInputSampledState();
            const x = packed & 0x1ff;
            const y = (packed >>> 9) & 0xff;
            const pressed = (packed >>> 17) & 1;
            touchState(x, y, pressed);
            if (active && !touchControls.active) {
                touchControls.active = {
                    negative_coordinate: module._BrowserTouchInputSetState(-1, 0, 1),
                    outside_screen: module._BrowserTouchInputSetState(319, 240, 1),
                    invalid_pressed: module._BrowserTouchInputSetState(0, 0, 2),
                    noncanonical_release: module._BrowserTouchInputSetState(0, 1, 0)
                };
                requireCondition(Object.values(touchControls.active).every(status => status === 1),
                    'Active touch controls failed');
                if (!descriptor.options.live_button_capture) {
                    touchControls.active.button_input_active = module._BrowserButtonInputIsActive();
                    touchControls.active.button_setter_refused = module._BrowserButtonInputSetHeldState(1);
                    requireCondition(touchControls.active.button_input_active === 0 &&
                        touchControls.active.button_setter_refused === 2, 'Touch-only capture enabled A input');
                }
                if (!descriptor.options.live_circle_pad_capture) {
                    touchControls.active.circle_input_active = module._BrowserCirclePadInputIsActive();
                    touchControls.active.circle_setter_refused = module._BrowserCirclePadInputSetPosition(0, 0);
                    requireCondition(touchControls.active.circle_input_active === 0 &&
                        touchControls.active.circle_setter_refused === 2, 'Touch-only capture enabled circle input');
                }
            }
            touchProgress = {active, poll_count:pollCount, sampled_requested_x:x, sampled_requested_y:y,
                sampled_requested_pressed:pressed, sampled_renderer_frame:module._BrowserButtonInputRendererFrame()};
            send({type:'touch_capture_progress', ...touchProgress});
        } catch (error) { fail(error); }
    }, 25);

}

function setButton(value) {
    record(value, ['schema_version', 'type', 'capture_identifier', 'sequence', 'held']);
    requireCondition(value.schema_version === 1 && descriptor?.options.live_button_capture &&
        value.capture_identifier === descriptor.capture_identifier &&
        integer(value.sequence, 4095) === buttonSequence++ &&
        integer(value.held, 1) === value.held, 'Invalid button request');
    // A correctly scoped release can arrive after normal guest shutdown. It
    // earns no delivery claim and must not invalidate the completed capture.
    const status = phase === 'running' ? module._BrowserButtonInputSetHeldState(value.held) : 2;
    const response = {sequence: value.sequence, held: value.held, status,
        accepted: status === 0, phase, ...(buttonProgress ?? {})};
    buttonRequests.push(response);
    send({type: 'button_request_status', ...response});
}

function setCirclePad(value) {
    record(value, ['schema_version', 'type', 'capture_identifier', 'sequence', 'x', 'y']);
    requireCondition(value.schema_version === 1 && descriptor?.options.live_circle_pad_capture &&
        value.capture_identifier === descriptor.capture_identifier && integer(value.sequence, maximumRequests() - 1) === circleSequence,
        'Invalid circle-pad request');
    circlePosition(value.x, value.y);
    ++circleSequence;
    const status = phase === 'running' && (!gameplay() || !gameplayStopRequested || (value.x === 0 && value.y === 0)) ?
        module._BrowserCirclePadInputSetPosition(value.x, value.y) : 2;
    const response = {sequence: value.sequence, x: value.x, y: value.y, status,
        accepted: status === 0, phase, ...(circleProgress ?? {})};
    retainRequest(circleRequests, response);
    send({type: 'circle_pad_request_status', ...response});
}

function setTouch(value) {
    record(value, ['schema_version', 'type', 'capture_identifier', 'sequence', 'x', 'y', 'pressed']);
    requireCondition(value.schema_version === 1 && descriptor?.options.live_touch_capture &&
        value.capture_identifier === descriptor.capture_identifier && integer(value.sequence, maximumRequests() - 1) === touchSequence,
        'Invalid touch request');
    touchState(value.x, value.y, value.pressed);
    ++touchSequence;
    const status = phase === 'running' && (!gameplay() || !gameplayStopRequested || value.pressed === 0) ?
        module._BrowserTouchInputSetState(value.x, value.y, value.pressed) : 2;
    const response = {sequence:value.sequence, x:value.x, y:value.y, pressed:value.pressed, status,
        accepted:status === 0, phase, ...(touchProgress ?? {})};
    retainRequest(touchRequests, response);
    send({type:'touch_request_status', ...response});
}

function applyGameplayStop() {
    // Atomics only. A Stop request does not terminate the proxy CPU or invent a frame.
    if (recording()) gameplayStopRelease = {
        buttons: module._BrowserButtonInputSetButtonMask(0),
        circle: module._BrowserCirclePadInputSetPosition(0, 0),
        touch: module._BrowserTouchInputSetState(0, 0, 0)};
    const status = module._BrowserGameplaySessionRequestStop();
    requireCondition(status === 0 || status === 2, 'Gameplay Stop bridge returned an invalid status');
    gameplayStopAccepted = status === 0;
    send({type: 'gameplay_stop_status', status, pending_activation: status === 2 && phase === 'running',
        accepted: gameplayStopAccepted});
}

function requestGameplayStop(value) {
    record(value, ['schema_version', 'type', 'capture_identifier']);
    requireCondition(value.schema_version === 1 && gameplay() && value.capture_identifier === descriptor.capture_identifier,
        'Invalid gameplay Stop request');
    gameplayStopRequested = true;
    if (phase === 'running' && module._BrowserGameplaySessionIsActive()) applyGameplayStop();
    else send({type: 'gameplay_stop_status', status: null, accepted: false,
        pending_activation: ['initializing', 'identifying', 'running'].includes(phase)});
}

function setGameplayButtons(value) {
    record(value, ['schema_version', 'type', 'capture_identifier', 'sequence', 'buttons']);
    requireCondition(value.schema_version === 1 && gameplay() && recording() &&
        value.capture_identifier === descriptor.capture_identifier &&
        integer(value.sequence, MaximumGameplayRequests - 1) === gameplayButtonSequence++, 'Invalid gameplay button request');
    integer(value.buttons, 0x0fff);
    const status = phase === 'running' && (!gameplayStopRequested || value.buttons === 0) ?
        module._BrowserButtonInputSetButtonMask(value.buttons) : 2;
    requireCondition([0, 1, 2, 3].includes(status), 'Gameplay button bridge returned an invalid status');
    const response = {sequence: value.sequence, buttons: value.buttons, status, accepted: status === 0,
        phase, ...(gameplayProgress ?? {})};
    retainRequest(gameplayButtonRequests, response);
    send({type: 'gameplay_button_status', ...response});
}

self.onmessage = event => {
    try {
        if (['enable_audio_stream', 'stop_audio_stream'].includes(event.data?.type)) {
            record(event.data, ['schema_version', 'type', 'capture_identifier']);
            requireCondition(event.data.schema_version === 1 && audioStream &&
                event.data.capture_identifier === descriptor.capture_identifier &&
                ['running', 'validating'].includes(phase), 'Unexpected audio stream action');
            if (event.data.type === 'stop_audio_stream') stopAudio();
            else {
                if (phase === 'running') audioStream.enable();
                else send({type: 'audio_stream_unavailable'});
            }
        } else if (event.data?.type === 'audio_stream_consumption') {
            record(event.data, ['schema_version', 'type', 'capture_identifier', 'sample_frames']);
            requireCondition(event.data.schema_version === 1 && audioStream?.enabled &&
                event.data.capture_identifier === descriptor.capture_identifier &&
                ['running', 'validating'].includes(phase), 'Unexpected audio consumption observation');
            if (!audioStream.closed) audioStream.reportConsumption(event.data.sample_frames);
        } else if (event.data?.type === 'acknowledge_audio_packet') {
            record(event.data, ['schema_version', 'type', 'capture_identifier', 'sequence']);
            requireCondition(event.data.schema_version === 1 && !audioStream?.closed &&
                event.data.capture_identifier === descriptor?.capture_identifier &&
                pendingAudio?.type === 'audio_stream_packet' && event.data.sequence === pendingAudio.sequence,
                'Unexpected audio packet acknowledgment');
            const pending = pendingAudio; pendingAudio = undefined;
            clearTimeout(pending.timer); pending.resolve();
        } else if (event.data?.type === 'acknowledge_audio_end') {
            record(event.data, ['schema_version', 'type', 'capture_identifier', 'receipt']);
            const receipt = event.data.receipt;
            requireCondition(event.data.schema_version === 1 && !audioStream?.closed && phase === 'validating' &&
                event.data.capture_identifier === descriptor?.capture_identifier && pendingAudio?.type === 'audio_stream_end' &&
                receipt?.capture_identifier === descriptor.capture_identifier && receipt.state === 'ended' &&
                receipt.accepted_source_frames === audioStream.position / 4 &&
                receipt.consumed_source_frames === receipt.accepted_source_frames &&
                receipt.accepted_packet_count === audioStream.records.length && receipt.buffered_source_frames === 0 &&
                receipt.context_state === 'closed' && receipt.context_rate === 32728 &&
                encoder.encode(JSON.stringify(receipt)).length <= MaximumLineBytes, 'Unexpected audio drain acknowledgment');
            const pending = pendingAudio; pendingAudio = undefined;
            clearTimeout(pending.timer); pending.resolve(receipt);
        } else if (event.data?.type === 'acknowledge_browser_webgl_presentation') {
            record(event.data, ['schema_version','type','capture_identifier','sequence']);
            requireCondition(event.data.schema_version === 1 && webGlPresentationReceiver &&
                event.data.capture_identifier === descriptor?.capture_identifier,
                'Unexpected GPU bitmap acknowledgment');
            integer(event.data.sequence, 0x7fffffff, 1);
            // False is a stale/repeated acknowledgment, including Stop teardown.
            // This path only returns transport credit. It calls no native export.
            webGlPresentationReceiver.acknowledge(event.data.sequence);
        } else if (event.data?.type === 'acknowledge_preview') {
            record(event.data, ['schema_version','type','capture_identifier','sequence']);
            requireCondition(event.data.schema_version === 1 && descriptor?.options.frame_output &&
                ['running','validating'].includes(phase) && pendingFrame &&
                event.data.capture_identifier === descriptor.capture_identifier &&
                event.data.sequence === pendingFrame.sequence, 'Unexpected preview acknowledgment');
            const pending = pendingFrame;
            pendingFrame = undefined;
            clearTimeout(pending.timer);
            pending.resolve();
        } else if (event.data?.type === 'acknowledge_transfer') {
            record(event.data, ['schema_version', 'type', 'capture_identifier', 'transfer_identifier']);
            requireCondition(event.data.schema_version === 1 && phase === 'exporting' && pendingTransfer &&
                event.data.capture_identifier === descriptor.capture_identifier &&
                event.data.transfer_identifier === pendingTransfer.identifier, 'Unexpected transfer acknowledgment');
            const pending = pendingTransfer;
            pendingTransfer = undefined;
            clearTimeout(pending.timer); pending.resolve();
        } else if (event.data?.type === 'request_gameplay_stop') {
            requestGameplayStop(event.data);
        } else if (event.data?.type === 'set_gameplay_button_mask') {
            setGameplayButtons(event.data);
        } else if (event.data?.type === 'set_button_held_state') {
            setButton(event.data);
        } else if (event.data?.type === 'set_touch_state') {
            setTouch(event.data);
        } else if (event.data?.type === 'set_circle_pad_position') {
            setCirclePad(event.data);
        } else {
            requireCondition(!started, 'This worker accepts one capture session only');
            started = true;
            void start(event.data).catch(fail);
        }
    } catch (error) { fail(error); }
};
self.onunhandledrejection = event => { event.preventDefault(); fail(event.reason); };
self.addEventListener('error', event => { fail(event.error ?? new Error(event.message)); });
self.addEventListener('messageerror', () => { fail(new Error('Worker message could not be decoded')); });
