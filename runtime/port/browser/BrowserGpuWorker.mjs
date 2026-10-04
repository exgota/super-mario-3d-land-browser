// Independently authored browser GPU worker admission.
// Loaded as pre-JS in the existing shared-Wasm module, including its pthreads.
globalThis.initializeBrowserGpuWorker = function({memoryBuffer, controlByteOffset,
    createRenderer, rendererOptions}) {
    const isolation = globalThis.requireBrowserRuntimeWorkerIsolation(memoryBuffer);
    if (!globalThis.name?.startsWith('em-pthread-'))
        throw new Error('GPU execution requires a generated Emscripten pthread');
    if (!Number.isSafeInteger(controlByteOffset) || controlByteOffset < 0 ||
        controlByteOffset % 4 !== 0 || controlByteOffset > memoryBuffer.byteLength - 24)
        throw new Error('GPU admission cells lie outside shared Wasm memory');
    if (globalThis.browserGpuWorkerAdmission || globalThis.browserWebGlRenderer)
        throw new Error('GPU worker already owns a renderer');
    const control = new Uint32Array(memoryBuffer, controlByteOffset, 6);
    if (Atomics.load(control, 0) !== 1 || Atomics.load(control, 1) !== 1 ||
        Atomics.compareExchange(control, 2, 41, 42) !== 41)
        throw new Error('GPU worker cannot observe the CPU shared-memory admission cells');
    Atomics.store(control, 3, 1);
    if (typeof OffscreenCanvas !== 'function' || typeof createRenderer !== 'function')
        throw new Error('GPU worker requires OffscreenCanvas and the selected renderer factory');
    const renderer = createRenderer(rendererOptions);
    const canvas = renderer?.canvas, context = renderer?.gl;
    if (!(canvas instanceof OffscreenCanvas) || !context || context.canvas !== canvas ||
        context.isContextLost()) {
        context?.getExtension('WEBGL_lose_context')?.loseContext();
        throw new Error('GPU worker did not create its own live WebGL context');
    }
    const version = String(context.getParameter(context.VERSION));
    if (!version.startsWith('WebGL 2.0')) {
        context.getExtension('WEBGL_lose_context')?.loseContext();
        throw new Error('GPU worker requires WebGL2');
    }
    globalThis.browserWebGlRenderer = renderer;
    globalThis.browserGpuWorkerAdmission = Object.freeze({...isolation,
        threadName: globalThis.name, sharedAtomicWriteVisible: true,
        webGlVersion: version, ownsOffscreenCanvas: true});
    Atomics.store(control, 4, 1);
    Atomics.store(control, 5, 1);
    return globalThis.browserGpuWorkerAdmission;
};

globalThis.releaseBrowserGpuWorker = function() {
    const renderer = globalThis.browserWebGlRenderer;
    if (!globalThis.browserGpuWorkerAdmission) return;
    renderer?.gl?.getExtension('WEBGL_lose_context')?.loseContext();
    delete globalThis.browserWebGlRenderer;
    delete globalThis.browserGpuWorkerAdmission;
};
