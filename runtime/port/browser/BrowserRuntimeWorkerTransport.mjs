// Independently authored browser runtime transport.
// Also usable as Emscripten pre-JS. Generated pthreads keep their existing URL.
globalThis.requireBrowserRuntimeWorkerIsolation = function(memoryBuffer,
    expectedOrigin = globalThis.location?.origin) {
    if (globalThis.isSecureContext !== true || globalThis.crossOriginIsolated !== true)
        throw new Error('Runtime worker requires a secure, cross-origin-isolated context');
    if (!expectedOrigin || expectedOrigin !== globalThis.location?.origin)
        throw new Error('Runtime worker origin differs from its admitted origin');
    if (typeof SharedArrayBuffer !== 'function' || !(memoryBuffer instanceof SharedArrayBuffer))
        throw new Error('Runtime worker requires shared Wasm memory');
    return Object.freeze({schemaVersion: 1, origin: expectedOrigin,
        secureContext: true, crossOriginIsolated: true, sharedMemoryBytes: memoryBuffer.byteLength});
};

// For an outer-worker admission packet. Shared buffers are never transferred.
// This does not instantiate a second Wasm module or replace Emscripten pthreads.
globalThis.postBrowserRuntimeSharedMemory = function(worker, workerUrl, packet, canvas = null) {
    const admission = globalThis.requireBrowserRuntimeWorkerIsolation(packet.memoryBuffer);
    const url = new URL(workerUrl, globalThis.location.href);
    if (url.origin !== admission.origin)
        throw new Error('Runtime worker URL must use the exact admitted origin');
    if (!worker || typeof worker.postMessage !== 'function' || packet.schemaVersion !== 1)
        throw new Error('Invalid runtime worker admission packet');
    if (packet.controlBuffer && !(packet.controlBuffer instanceof SharedArrayBuffer))
        throw new Error('Runtime worker control buffer must be shared');
    const envelope = {...packet, origin: admission.origin};
    if (canvas === null) worker.postMessage(envelope);
    else {
        if (typeof OffscreenCanvas !== 'function' || !(canvas instanceof OffscreenCanvas))
            throw new Error('Runtime worker canvas must be an OffscreenCanvas');
        envelope.canvas = canvas;
        worker.postMessage(envelope, [canvas]);
    }
    return admission;
};

// Presentation uses the selected Emscripten 6.0.10 CMD_CALL_HANDLER channel.
// Its existing parent handler invokes Module[handler](...args). Only the bitmap
// is transferred. The acknowledgement cells are structured-shared, never moved.
(() => {
    const maximumPendingBitmaps = 2;
    const maximumBitmapBytes = 32 * 1024 * 1024 / maximumPendingBitmaps;
    const handlerName = 'browserWebGlPresentation';
    const screenFields = ['screenIdentifier', 'surfaceIdentifier', 'sourceX', 'sourceY',
        'sourceWidth', 'sourceHeight', 'width', 'height', 'rotationQuarterTurns', 'flags', 'colorFormat'];
    const unsignedWord = value => Number.isInteger(value) && value >= 0 && value <= 0xffffffff;
    const unsignedWideWord = value => typeof value === 'string' && /^(0|[1-9][0-9]*)$/.test(value) &&
        BigInt(value) <= 0xffffffffffffffffn;
    function copyDescriptor(value) {
        if (value?.schemaVersion !== 1 || !unsignedWideWord(value.rendererFrame) ||
            !unsignedWideWord(value.sampledTicks) || !Array.isArray(value.screens) ||
            value.screens.length < 1 || value.screens.length > 2) return null;
        const identifiers = new Set(), screens = [];
        for (const source of value.screens) {
            if (!source || !screenFields.every(field => unsignedWord(source[field]))) return null;
            const screen = Object.fromEntries(screenFields.map(field => [field, source[field]]));
            const rotated = (screen.rotationQuarterTurns & 1) !== 0;
            if (![0, 2].includes(screen.screenIdentifier) || identifiers.has(screen.screenIdentifier) ||
                !screen.surfaceIdentifier || !screen.sourceWidth || !screen.sourceHeight ||
                screen.sourceX + screen.sourceWidth > 0x100000000 ||
                screen.sourceY + screen.sourceHeight > 0x100000000 ||
                screen.rotationQuarterTurns > 3 || screen.flags > 1 || screen.colorFormat > 4 ||
                screen.width !== (rotated ? screen.sourceHeight : screen.sourceWidth) ||
                screen.height !== (rotated ? screen.sourceWidth : screen.sourceHeight)) return null;
            identifiers.add(screen.screenIdentifier);
            screens.push(screen);
        }
        return {schemaVersion: 1, rendererFrame: value.rendererFrame,
            sampledTicks: value.sampledTicks, screens};
    }
    function controlView(buffer) {
        if (!(buffer instanceof SharedArrayBuffer) || buffer.byteLength !== (2 + maximumPendingBitmaps) * 4)
            throw new Error('GPU presentation acknowledgement cells are invalid');
        return new Int32Array(buffer);
    }
    function acknowledge(packet) {
        const control = controlView(packet.acknowledgmentBuffer);
        if (!Number.isInteger(packet.acknowledgmentIndex) || packet.acknowledgmentIndex < 2 ||
            packet.acknowledgmentIndex >= control.length || !Number.isInteger(packet.sequence) ||
            packet.sequence < 1 || packet.sequence > 0x7fffffff)
            throw new Error('GPU bitmap acknowledgement is invalid');
        return Atomics.compareExchange(control, packet.acknowledgmentIndex, packet.sequence, 0) === packet.sequence;
    }
    function discard(packet) {
        try { packet.bitmap?.close(); } finally { acknowledge(packet); }
    }
    function validateBitmap(packet) {
        if (packet?.schemaVersion !== 1 || packet.type !== 'browser_webgl_presentation' ||
            !Number.isInteger(packet.sequence) || packet.sequence < 1 || packet.sequence > 0x7fffffff ||
            !unsignedWideWord(packet.rendererFrame) || !unsignedWideWord(packet.sampledTicks) ||
            !(packet.bitmap instanceof ImageBitmap) || !unsignedWord(packet.width) || !packet.width ||
            !unsignedWord(packet.height) || !packet.height ||
            packet.width * packet.height * 4 > maximumBitmapBytes ||
            packet.bitmap.width !== packet.width || packet.bitmap.height !== packet.height ||
            !Array.isArray(packet.screens) || packet.screens.length < 1 || packet.screens.length > 2)
            throw new Error('GPU bitmap presentation extent is invalid');
        const identifiers = new Set();
        for (const screen of packet.screens) {
            if (!screen || ![0, 2].includes(screen.screenIdentifier) || identifiers.has(screen.screenIdentifier) ||
                !['x', 'y', 'width', 'height'].every(field => unsignedWord(screen[field])) ||
                !screen.width || !screen.height || screen.x + screen.width > packet.width ||
                screen.y + screen.height > packet.height)
                throw new Error('GPU bitmap screen rectangle is invalid');
            identifiers.add(screen.screenIdentifier);
        }
    }

    // Install in the existing outer worker before starting the guest. The caller
    // forwards the public packet to the page and acknowledges only after paint
    // or an explicit downstream drop. No native export is called on that path.
    globalThis.installBrowserWebGlPresentationReceiver = function({publishFrame}) {
        if (typeof publishFrame !== 'function' || globalThis.browserWebGlPresentationReceiver)
            throw new Error('GPU presentation requires one owning outer-worker receiver');
        const controls = new Map(), pending = new Map();
        const statistics = {receivedFrames: 0, acknowledgedFrames: 0, downstreamDrops: 0};
        let closed = false, lastSequence = 0, lastRendererFrame = -1n;
        const receiver = {
            statistics,
            receive(packet) {
                if (packet?.type === 'browser_webgl_presentation_admission') {
                    const control = controlView(packet.controlBuffer);
                    if (packet.schemaVersion !== 1 || packet.origin !== globalThis.location.origin ||
                        packet.maximumPendingBitmaps !== maximumPendingBitmaps ||
                        typeof packet.transportIdentifier !== 'string' || !/^[0-9a-f]{32}$/.test(packet.transportIdentifier))
                        throw new Error('GPU presentation admission differs from the owning origin');
                    for (const [identifier, previous] of controls)
                        if (Atomics.load(previous, 1) && previous.slice(2).every(value => value === 0))
                            controls.delete(identifier);
                    if (controls.size && !controls.has(packet.transportIdentifier))
                        throw new Error('GPU presentation already has an admitted owner');
                    controls.set(packet.transportIdentifier, control);
                    Atomics.store(control, 1, Number(closed));
                    Atomics.store(control, 0, closed ? -1 : 1);
                    Atomics.notify(control, 0);
                    return;
                }
                try {
                    validateBitmap(packet);
                    const control = controls.get(packet.transportIdentifier);
                    controlView(packet.acknowledgmentBuffer);
                    if (!control ||
                        !Number.isInteger(packet.acknowledgmentIndex) || packet.acknowledgmentIndex < 2 ||
                        packet.acknowledgmentIndex >= control.length ||
                        Atomics.load(control, packet.acknowledgmentIndex) !== packet.sequence)
                        throw new Error('GPU bitmap was not admitted by the owning transport');
                    // Structured-cloned SharedArrayBuffers have distinct wrapper
                    // identities. Use the cells retained at owner admission.
                    packet = {...packet, acknowledgmentBuffer: control.buffer};
                    if (closed || Atomics.load(control, 1)) {
                        discard(packet); ++statistics.downstreamDrops; return;
                    }
                    if (packet.sequence !== lastSequence + 1 || BigInt(packet.rendererFrame) <= lastRendererFrame ||
                        pending.size >= maximumPendingBitmaps)
                        throw new Error('GPU bitmap publication order or pending bound changed');
                    lastSequence = packet.sequence; lastRendererFrame = BigInt(packet.rendererFrame);
                    const {acknowledgmentBuffer, acknowledgmentIndex, transportIdentifier, ...publicPacket} = packet;
                    pending.set(packet.sequence, packet);
                    if (publishFrame(publicPacket, [packet.bitmap]) !== true) {
                        pending.delete(packet.sequence); discard(packet); ++statistics.downstreamDrops;
                    } else ++statistics.receivedFrames;
                } catch (error) {
                    if (pending.get(packet?.sequence) === packet) pending.delete(packet.sequence);
                    if (packet?.acknowledgmentBuffer) discard(packet);
                    else packet?.bitmap?.close();
                    throw error;
                }
            },
            acknowledge(sequence) {
                const packet = pending.get(sequence);
                if (!packet) return false;
                pending.delete(sequence);
                if (!acknowledge(packet)) throw new Error('GPU bitmap acknowledgement repeated or changed');
                ++statistics.acknowledgedFrames;
                return true;
            },
            close() {
                if (closed) return;
                closed = true;
                for (const control of controls.values()) {
                    Atomics.store(control, 1, 1); Atomics.store(control, 0, -1); Atomics.notify(control, 0);
                }
                for (const packet of pending.values()) { discard(packet); ++statistics.downstreamDrops; }
                pending.clear();
                if (globalThis.browserWebGlPresentationReceiver === receiver)
                    delete globalThis.browserWebGlPresentationReceiver;
            },
        };
        globalThis.browserWebGlPresentationReceiver = receiver;
        return receiver;
    };

    if (typeof Module === 'object' && Module &&
        !(typeof ENVIRONMENT_IS_PTHREAD === 'boolean' && ENVIRONMENT_IS_PTHREAD)) {
        Module[handlerName] = packet => {
            const receiver = globalThis.browserWebGlPresentationReceiver;
            if (receiver) return receiver.receive(packet);
            if (packet?.type === 'browser_webgl_presentation_admission') {
                const control = controlView(packet.controlBuffer);
                Atomics.store(control, 1, 1); Atomics.store(control, 0, -1); Atomics.notify(control, 0);
            } else discard(packet);
        };
    }

    globalThis.initializeBrowserGpuPresentationPublisher = function(memoryBuffer) {
        const admission = globalThis.requireBrowserRuntimeWorkerIsolation(memoryBuffer);
        if (!globalThis.browserGpuWorkerAdmission || !globalThis.name?.startsWith('em-pthread-'))
            throw new Error('GPU bitmap publication requires the admitted renderer owner');
        const control = new Int32Array(new SharedArrayBuffer((2 + maximumPendingBitmaps) * 4));
        const transportIdentifier = Array.from(crypto.getRandomValues(new Uint32Array(4)),
            word => word.toString(16).padStart(8, '0')).join('');
        const statistics = {transferredFrames: 0, backpressureDrops: 0};
        let lastRendererFrame = -1n;
        globalThis.postMessage({cmd: 9, handler: handlerName, args: [{schemaVersion: 1,
            type: 'browser_webgl_presentation_admission', origin: admission.origin,
            controlBuffer: control.buffer, maximumPendingBitmaps, transportIdentifier}]});
        // One bounded handshake lets the existing outer worker prove that its
        // paint receiver exists. A missing receiver preserves explicit fallback.
        Atomics.wait(control, 0, 0, 1000);
        return {
            statistics,
            ready: () => Atomics.load(control, 0) === 1 && Atomics.load(control, 1) === 0,
            hasCapacity: () => control.subarray(2).some((_, index) => Atomics.load(control, index + 2) === 0),
            noteBackpressure() { ++statistics.backpressureDrops; },
            publishFrame(packet, transfer) {
                validateBitmap(packet);
                if (transfer?.length !== 1 || transfer[0] !== packet.bitmap)
                    throw new Error('GPU presentation transfers only its owned bitmap');
                if (Atomics.load(control, 0) !== 1 || Atomics.load(control, 1)) return false;
                if (BigInt(packet.rendererFrame) <= lastRendererFrame)
                    throw new Error('GPU bitmap renderer-frame order changed');
                let slot = -1;
                for (let index = 2; index < control.length; ++index)
                    if (Atomics.compareExchange(control, index, 0, packet.sequence) === 0) { slot = index; break; }
                if (slot < 0) { ++statistics.backpressureDrops; return false; }
                try {
                    globalThis.postMessage({cmd: 9, handler: handlerName, args: [{...packet,
                        acknowledgmentBuffer: control.buffer, acknowledgmentIndex: slot, transportIdentifier}]}, transfer);
                } catch (error) {
                    Atomics.compareExchange(control, slot, packet.sequence, 0);
                    throw error;
                }
                lastRendererFrame = BigInt(packet.rendererFrame);
                ++statistics.transferredFrames;
                return true;
            },
            close() { Atomics.store(control, 1, 1); Atomics.store(control, 0, -1); },
        };
    };

    // Native R3 admission supplies only owned numbers/strings. This fixed tag1
    // packet copies those bytes before Root's immutable native queue publication.
    globalThis.submitBrowserWebGlPresentation = function(value) {
        const descriptor = copyDescriptor(value);
        if (!descriptor || globalThis.browserGpuPipelineEnabled !== true ||
            globalThis.browserGpuWorkerAdmission || !globalThis.browserWebGlRenderer ||
            typeof _BrowserGpuRendererSubmit !== 'function') return 0;
        globalThis.browserWebGlRenderer.endDraw();
        const json = new TextEncoder().encode(JSON.stringify(descriptor));
        const bytes = 56 + json.length, pointer = _malloc(bytes);
        if (!pointer) throw new Error('GPU presentation packet allocation failed');
        try {
            const packet = new Uint8Array(HEAPU8.buffer, pointer, bytes);
            packet.fill(0);
            const header = new DataView(packet.buffer, packet.byteOffset, packet.byteLength);
            [0x50525047, 1, bytes, 1, 9, 16].forEach((word, index) => header.setUint32(index * 4, word, true));
            header.setBigUint64(24, BigInt(descriptor.rendererFrame), true);
            header.setBigUint64(32, BigInt(descriptor.sampledTicks), true);
            header.setUint32(40, 1, true); header.setUint32(44, 56, true);
            header.setUint32(48, json.length, true); packet.set(json, 56);
            if (_BrowserGpuRendererSubmit(pointer, bytes, 1, 1) !== 1)
                throw new Error('GPU presentation returned no admission status');
            const status = HEAPU8[_BrowserGpuRendererResultPointer()];
            if (![0, 1, 2].includes(status)) throw new Error('GPU presentation admission status is invalid');
            return status;
        } finally { _free(pointer); }
    };
})();
