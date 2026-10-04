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
