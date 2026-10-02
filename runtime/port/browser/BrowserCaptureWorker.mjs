// SPDX-License-Identifier: GPL-2.0-or-later
const ChunkBytes = 64 * 1024;
const MaximumFiles = 16384;
const MaximumDirectoryEntries = 32768;
const MaximumOutputBytes = 2 ** 31;
const MaximumEventBytes = 256 * 1024 * 1024;
const MaximumEvents = 2000000;
const MaximumLogBytes = 16 * 1024 * 1024;
const MaximumLineBytes = 64 * 1024;
const MaximumAudioBytes = 64 * 1024 * 1024;
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
let nextTransferIdentifier = 0;
let pendingTransfer;
let buttonTimer;
let buttonSequence = 0;
let buttonProgress;
const buttonRequests = [];
let circleTimer;
let circleSequence = 0;
let circleProgress;
let circleControls;
const circleRequests = [];
let frameTimer;
let frameTask;
let pendingFrame;
let frameControls;
const frameReceipts = [];
const MaximumPreviewFrames = 8192;

function requireCondition(condition, message) {
    if (!condition) throw new Error(message);
}

function recording() {
    return descriptor.options.live_button_capture || descriptor.options.live_circle_pad_capture;
}

function circlePosition(x, y) {
    requireCondition(Number.isSafeInteger(x) && Number.isSafeInteger(y) &&
        x >= -154 && x <= 154 && y >= -154 && y <= 154 && x * x + y * y <= 154 * 154,
        'Invalid circle-pad position');
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
        value.split('/').every(part => /^[A-Za-z0-9_.-]+$/.test(part) &&
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
    const observationKeys = ['input_capture', 'audio_capture', 'live_button_capture', 'live_circle_pad_capture',
                             'record_base_ticks', 'frame_output']
        .filter(key => Object.hasOwn(value.options, key));
    record(value.options, ['presentation_limit', 'wall_time_seconds', 'pica_payload_limit_bytes', ...observationKeys]);
    const inputCapture = Object.hasOwn(value.options, 'input_capture') ? value.options.input_capture : false;
    const audioCapture = Object.hasOwn(value.options, 'audio_capture') ? value.options.audio_capture : false;
    const liveButtonCapture = Object.hasOwn(value.options, 'live_button_capture') ? value.options.live_button_capture : false;
    const liveCirclePadCapture = Object.hasOwn(value.options, 'live_circle_pad_capture') ? value.options.live_circle_pad_capture : false;
    const frameOutput = Object.hasOwn(value.options, 'frame_output') ? value.options.frame_output : false;
    requireCondition(typeof inputCapture === 'boolean' && typeof audioCapture === 'boolean' &&
        typeof liveButtonCapture === 'boolean' && typeof liveCirclePadCapture === 'boolean' && typeof frameOutput === 'boolean' &&
        (!frameOutput || value.options.presentation_limit !== null) &&
        (!liveButtonCapture || (inputCapture && audioCapture)) &&
        (!liveCirclePadCapture || (inputCapture && audioCapture)) &&
        (!audioCapture || inputCapture) &&
        (!(inputCapture || audioCapture) || value.options.presentation_limit !== null), 'Invalid observation profile');
    requireCondition((liveButtonCapture || liveCirclePadCapture) ? typeof value.options.record_base_ticks === 'string' &&
        /^(0|[1-9][0-9]{0,18})$/.test(value.options.record_base_ticks) &&
        BigInt(value.options.record_base_ticks) <= BigInt(Number.MAX_SAFE_INTEGER) :
        !Object.hasOwn(value.options, 'record_base_ticks'), 'Invalid recording clock');
    if (value.options.presentation_limit !== null) integer(value.options.presentation_limit, 3600, 1);
    integer(value.options.wall_time_seconds, 3600, 1);
    integer(value.options.pica_payload_limit_bytes, 1024 ** 3, 1);
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
                               live_button_capture: liveButtonCapture, live_circle_pad_capture: liveCirclePadCapture,
                               frame_output: frameOutput},
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
    clearInterval(buttonTimer);
    clearInterval(circleTimer);
    clearInterval(frameTimer);
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
          stdout, stderr});
    // The page owns the independent watchdog and abnormal worker disposal.
    // Never label abrupt termination as a successful C++ shutdown.
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
        requireCondition(fields[0] === 1 && sequence === frameReceipts.length + 1 &&
            sequence <= MaximumPreviewFrames, 'Preview publication order changed');
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
        return {sequence, renderer_frame: rendererFrame, sampled_ticks: sampledTicks, screens};
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
    await new Promise((resolve, reject) => {
        const timer = setTimeout(() => { pendingFrame = undefined;
            reject(new Error('Preview acknowledgment timed out')); }, TransferTimeoutMilliseconds);
        pendingFrame = {sequence: frame.sequence, resolve, reject, timer};
        send({type:'preview_screens', ...frame, screens}, screens.map(screen => screen.rgba));
    });
}

function previewObservations() {
    const lines = stderr.filter(line => line.startsWith('browser frame output '));
    requireCondition(lines.length === 1 && frameControls && frameReceipts.length > 0,
                     'Preview producer or real lease controls are absent');
    const totals = parseRecord(lines[0].slice('browser frame output '.length));
    record(totals, ['unique_frames','published','full_drops','unavailable','duplicate_polls','frame_gaps','geometry_errors']);
    for (const value of Object.values(totals)) requireCondition(typeof value === 'string' &&
        /^(0|[1-9][0-9]*)$/.test(value) && BigInt(value) <= BigInt(MaximumEvents), 'Invalid preview producer total');
    requireCondition(totals.geometry_errors === '0' &&
        BigInt(totals.unique_frames) === BigInt(totals.published) + BigInt(totals.full_drops) + BigInt(totals.unavailable) &&
        BigInt(totals.published) >= BigInt(frameReceipts.length), 'Preview producer totals do not reconcile');
    return {producer: totals, copied_and_acknowledged: frameReceipts.length,
        unconsumed_publications: Number(BigInt(totals.published) - BigInt(frameReceipts.length)),
        lease_controls: frameControls, frames: frameReceipts};
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
    requireCondition(entry && entry.size > 0 && entry.size <= MaximumAudioBytes, 'Observation extent is invalid');
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
                integer(event.sample_frames) === blocks * 160 && integer(event.payload_bytes, MaximumAudioBytes, 1) === blocks * 640 &&
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
        requireCondition(++blocks * 640 <= MaximumAudioBytes, 'Audio payload exceeds bound');
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
    requireCondition(phase === 'running' && status === 0, `Capture exited with status ${status}`);
    phase = 'validating';
    clearInterval(buttonTimer);
    clearInterval(circleTimer);
    // Any already copied ordinary frame can finish its page acknowledgment.
    // Native exports are forbidden here because SDK exitRuntime has run.
    await frameTask;
    requireCondition(phase === 'validating', 'Preview failure prevents capture completion');
    if (descriptor.options.live_button_capture) send({type: 'button_capture_ended'});
    if (descriptor.options.live_circle_pad_capture) send({type: 'circle_pad_capture_ended'});
    const {files, directories} = enumerateCapture();
    const {outcome, presentation, entries} = validateEvents(files);
    const observations = validateObservations(presentation, entries);
    if (recording()) observations.recorded_movie = validateRecordedMovie(entries, observations);
    if (descriptor.options.frame_output) observations.preview_frames = previewObservations();
    const log = entries.get('user/log/reference_capture.log');
    requireCondition(log && !/\b(?:Input|Movie|Audio|Service\.DSP)(?:\.[A-Za-z0-9_.]+)?\s+<(?:Error|Critical)>/.test(readText('/capture/user/log/reference_capture.log',
        log.size, MaximumLogBytes)), 'Capture log is absent or reports Movie/Audio errors');
    const counters = stderr.flatMap(line => {
        const match = /^static CPU ([0-9]+) executed ([0-9]+) guest instructions; interpreter\/JIT fallbacks ([0-9]+)$/.exec(line);
        return match ? [{cpu_identifier: match[1], instructions: match[2], fallbacks: match[3], original_line: line}] : [];
    });
    requireCondition(counters.length > 0 && counters.every(counter => counter.fallbacks === '0') &&
        new Set(counters.map(counter => counter.cpu_identifier)).size === counters.length, 'Invalid CPU counters');
    const screens = softwareScreens(presentation, entries);
    phase = 'exporting';
    const manifest = {type: 'capture_manifest', files, directories, outcome, stdout, stderr, counters, observations};
    if (descriptor.options.live_button_capture) {
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
        requireCondition(circleProgress?.poll_count > 0 && circleControls?.active &&
            closure.length === 1 && closure[0] === observations.input.polls &&
            entries.get('input_movie.ctm')?.size > 256, 'Circle-pad device, cleanup or recorded movie is absent');
        manifest.circle_pad_requests = circleRequests;
        manifest.circle_pad_progress = circleProgress;
        manifest.circle_pad_controls = circleControls;
        manifest.circle_pad_closed_polls = closure[0];
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
    const factory = (await import(url.href)).default;
    requireCondition(typeof factory === 'function', 'Generated ES module has no default factory');
    module = await factory({noInitialRun: true, thisProgram: 'root_port_browser_capture',
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
            instance.ENV.ROOT_PORT_CAPTURE_WALL_SECONDS = String(descriptor.options.wall_time_seconds);
            instance.ENV.ROOT_PORT_CAPTURE_PICA_BYTES = String(descriptor.options.pica_payload_limit_bytes);
            if (descriptor.options.presentation_limit !== null)
                instance.ENV.ROOT_PORT_CAPTURE_PRESENTATION = String(descriptor.options.presentation_limit);
            else delete instance.ENV.ROOT_PORT_CAPTURE_PRESENTATION;
            for (const [option, name] of [['input_capture', 'ROOT_PORT_CAPTURE_INPUTS'], ['audio_capture', 'ROOT_PORT_CAPTURE_AUDIO']]) {
                if (descriptor.options[option]) instance.ENV[name] = '1';
                else delete instance.ENV[name];
            }
            for (const name of ['ROOT_PORT_BROWSER_BUTTON_CAPTURE', 'ROOT_PORT_RECORD_INITIAL_USER_STATE',
                                'ROOT_PORT_RECORD_BASE_TICKS', 'ROOT_PORT_BROWSER_FRAME_OUTPUT',
                                'ROOT_PORT_BROWSER_CIRCLE_PAD_CAPTURE']) delete instance.ENV[name];
            if (descriptor.options.frame_output) instance.ENV.ROOT_PORT_BROWSER_FRAME_OUTPUT = '1';
            if (descriptor.options.live_button_capture) {
                instance.ENV.ROOT_PORT_BROWSER_BUTTON_CAPTURE = '1';
            }
            if (descriptor.options.live_circle_pad_capture)
                instance.ENV.ROOT_PORT_BROWSER_CIRCLE_PAD_CAPTURE = '1';
            if (recording()) {
                instance.ENV.ROOT_PORT_RECORD_INITIAL_USER_STATE = '/owned/initial_user_state';
                instance.ENV.ROOT_PORT_RECORD_BASE_TICKS = descriptor.options.record_base_ticks;
            }
        }],
        print: line => captureLine(stdout, line), printErr: line => captureLine(stderr, line),
        onAbort: reason => fail(new Error(`Runtime aborted: ${reason}`)),
        onExit: status => { void finish(status).catch(fail); }
    });
    requireCondition(phase !== 'failed', 'Module initialization failed');
    phase = 'identifying';
    for (const input of descriptor.validated_inputs) {
        const status = module.ccall('BrowserInputIdentityValidateSha256', 'number',
            ['string', 'string', 'number'], [`/owned/${input.path}`, input.expected_sha256, input.expected_bytes]);
        requireCondition(status === 0, `Input identity failed for ${input.path}, status ${status}`);
    }
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
    send({type: 'capture_started'});
    phase = 'running';
    // In the pinned SDK this launches the proxy pthread. It is not completion.
    const arguments_ = ['/owned/block_schedule.bin', '/owned/dump.3ds', '/capture'];
    if (!recording())
        arguments_.push('/owned/input_movie.ctm', '/owned/initial_user_state');
    const launchStatus = module.callMain(arguments_);
    requireCondition(launchStatus === 0, `CPU pthread launch refused with status ${launchStatus}`);
    if (descriptor.options.frame_output) frameTimer = setInterval(() => {
        if (phase !== 'running' || frameTask) return;
        try {
            const frame = copyPreviewFrame();
            if (frame) frameTask = transferPreviewFrame(frame).catch(fail).finally(() => { frameTask = undefined; });
        } catch (error) { fail(error); }
    }, 25);
    if (descriptor.options.live_button_capture) buttonTimer = setInterval(() => {
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
        value.capture_identifier === descriptor.capture_identifier && integer(value.sequence, 4095) === circleSequence,
        'Invalid circle-pad request');
    circlePosition(value.x, value.y);
    ++circleSequence;
    const status = phase === 'running' ? module._BrowserCirclePadInputSetPosition(value.x, value.y) : 2;
    const response = {sequence: value.sequence, x: value.x, y: value.y, status,
        accepted: status === 0, phase, ...(circleProgress ?? {})};
    circleRequests.push(response);
    send({type: 'circle_pad_request_status', ...response});
}

self.onmessage = event => {
    try {
        if (event.data?.type === 'acknowledge_preview') {
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
        } else if (event.data?.type === 'set_button_held_state') {
            setButton(event.data);
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
