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

// Called only by the native Execute hook, after native packet validation. All
// renderer calls run in this pthread. Borrowed result storage lasts this call;
// configuration/descriptor arrays are owned copies. Immutable registered
// resources can be reused after packet retirement, never borrowed from guest RAM.
globalThis.executeBrowserGpuRenderCommand = function({memoryBuffer, metadataByteOffset,
    payloadByteOffset, payloadBytes, resultByteOffset, resultBytes}) {
    globalThis.requireBrowserRuntimeWorkerIsolation(memoryBuffer);
    if (!globalThis.browserGpuWorkerAdmission || !globalThis.name?.startsWith('em-pthread-'))
        throw new Error('GPU render commands require the admitted renderer owner');
    const renderer = globalThis.browserWebGlRenderer;
    if (!renderer || renderer.gl.isContextLost())
        throw new Error('GPU renderer context is unavailable');
    function requireExtent(offset, bytes, alignment) {
        if (!Number.isSafeInteger(offset) || !Number.isSafeInteger(bytes) ||
            offset < 0 || bytes < 0 || offset % alignment !== 0 ||
            offset > memoryBuffer.byteLength || bytes > memoryBuffer.byteLength - offset)
            throw new Error('GPU command extent lies outside shared Wasm memory');
    }
    requireExtent(metadataByteOffset, 28, 4);
    requireExtent(payloadByteOffset, payloadBytes, 8);
    requireExtent(resultByteOffset, resultBytes, 1);
    const words = new Uint32Array(memoryBuffer, metadataByteOffset, 7);
    const wideWord = index => BigInt(words[index]) | (BigInt(words[index + 1]) << 32n);
    const payload = new DataView(memoryBuffer, payloadByteOffset, payloadBytes);
    if (payloadBytes < 40 || payload.getUint32(0, true) !== 0x50525047 ||
        payload.getUint32(4, true) !== 1 || payload.getUint32(8, true) !== payloadBytes ||
        payload.getUint32(16, true) !== words[0] || payload.getUint32(20, true) !== 16 ||
        payload.getBigUint64(24, true) !== wideWord(3) ||
        payload.getBigUint64(32, true) !== wideWord(5))
        throw new Error('GPU packet differs from its admitted queue record');
    const count = payload.getUint32(12, true);
    if (count > Math.floor((payloadBytes - 40) / 16))
        throw new Error('GPU packet directory exceeds its extent');
    const sections = new Map();
    for (let index = 0; index < count; ++index) {
        const entry = 40 + index * 16;
        const tag = payload.getUint32(entry, true), offset = payload.getUint32(entry + 4, true);
        const bytes = payload.getUint32(entry + 8, true);
        // Native validation already rejects overlapping sections and reserved words.
        if (sections.has(tag) || offset < 40 + count * 16 || offset % 8 !== 0 ||
            offset > payloadBytes || bytes > payloadBytes - offset)
            throw new Error('GPU packet section exceeds its validated extent');
        sections.set(tag, new Uint8Array(memoryBuffer, payloadByteOffset + offset, bytes));
    }
    const command = Object.freeze({schemaVersion: 1, kind: words[0],
        sequence: wideWord(1), frameIdentifier: wideWord(3), submissionTicks: wideWord(5),
        section(tag) {
            const bytes = sections.get(tag);
            if (!bytes) throw new Error('Required GPU render packet section is missing: ' + tag);
            return bytes;
        }, result: new Uint8Array(memoryBuffer, resultByteOffset, resultBytes)});
    const decodeJsonSection = tag => JSON.parse(new TextDecoder('utf-8', {fatal: true})
        .decode(new Uint8Array(command.section(tag))));
    const retained = globalThis.browserGpuRetainedResources ??= {entries: new Map(), bytes: 0,
        lastResourceIdentifier: 0, statistics: {publishedResources: 0, releasedResources: 0,
            resolvedReferences: 0, copiedResourceBytes: 0, retainedBytes: 0, retainedEntries: 0, peakRetainedBytes: 0}};
    function binaryArray(sectionId, elementType, elementCount) {
        const types = {uint8: Uint8Array, uint32: Uint32Array, float32: Float32Array};
        const ArrayType = Object.hasOwn(types, elementType) ? types[elementType] : null;
        if (!Number.isInteger(sectionId) || sectionId < 256 || sectionId > 0xffffffff ||
            !Number.isInteger(elementCount) || elementCount < 0 || elementCount > 0xffffffff || !ArrayType)
            throw new Error('Invalid GPU binary section reference');
        const bytes = command.section(sectionId);
        if (bytes.byteLength !== elementCount * ArrayType.BYTES_PER_ELEMENT ||
            bytes.byteOffset % ArrayType.BYTES_PER_ELEMENT !== 0)
            throw new Error('GPU binary section element count or alignment differs');
        return new ArrayType(new ArrayType(bytes.buffer, bytes.byteOffset, elementCount));
    }
    if (sections.has(4)) {
        if (![2, 3, 11, 12].includes(command.kind)) throw new Error('GPU resources require prepared draw submission');
        const directory = decodeJsonSection(4);
        if (directory?.schemaVersion !== 1 || Object.keys(directory).length !== 3 ||
            !Array.isArray(directory.resourceDefinitions) || !Array.isArray(directory.resourceReleases) ||
            directory.resourceDefinitions.length > 128 || directory.resourceReleases.length > 128)
            throw new Error('GPU retained resource directory is invalid');
        const pending = new Map(retained.entries), released = new Set();
        let bytes = retained.bytes, lastIdentifier = retained.lastResourceIdentifier, copiedBytes = 0;
        for (const identifier of directory.resourceReleases) {
            if (!Number.isInteger(identifier) || identifier <= 0 || identifier > 0xffffffff ||
                released.has(identifier) || !pending.has(identifier))
                throw new Error('GPU resource release references an unknown or repeated identifier');
            bytes -= pending.get(identifier).array.byteLength; pending.delete(identifier); released.add(identifier);
        }
        for (const definition of directory.resourceDefinitions) {
            const {resourceIdentifier, sectionId, elementType, elementCount} = definition;
            if (Object.keys(definition).length !== 4 || !Number.isInteger(resourceIdentifier) ||
                resourceIdentifier <= lastIdentifier || resourceIdentifier > 0xffffffff ||
                !['uint8', 'uint32'].includes(elementType) || !Number.isInteger(elementCount) || elementCount <= 0 ||
                elementCount > 0xffffffff)
                throw new Error('GPU resource definition has an invalid identity or element type');
            if (pending.size >= 128 || bytes + elementCount * (elementType === 'uint8' ? 1 : 4) > 32 * 1024 * 1024)
                throw new Error('GPU retained resources exceed the admitted queue limits');
            const array = binaryArray(sectionId, elementType, elementCount);
            bytes += array.byteLength; copiedBytes += array.byteLength;
            pending.set(resourceIdentifier, {elementType, elementCount, array}); lastIdentifier = resourceIdentifier;
            if (pending.size > 128 || bytes > 32 * 1024 * 1024)
                throw new Error('GPU retained resources exceed the admitted queue limits');
        }
        retained.entries = pending; retained.bytes = bytes; retained.lastResourceIdentifier = lastIdentifier;
        retained.statistics.publishedResources += directory.resourceDefinitions.length;
        retained.statistics.releasedResources += directory.resourceReleases.length;
        retained.statistics.copiedResourceBytes += copiedBytes;
        retained.statistics.retainedBytes = bytes; retained.statistics.retainedEntries = pending.size;
        retained.statistics.peakRetainedBytes = Math.max(retained.statistics.peakRetainedBytes, bytes);
    }
    function decodeValue(value, tag, path, root) {
        if (value === null || typeof value !== 'object') return value;
        if (Object.hasOwn(value, 'retainedResourceIdentifier')) {
            const {retainedResourceIdentifier, elementType, elementCount} = value;
            if (!Number.isInteger(retainedResourceIdentifier) || retainedResourceIdentifier <= 0 ||
                retainedResourceIdentifier > 0xffffffff || Object.keys(value).length !== 3 ||
                ![2, 3, 11, 12].includes(command.kind) ||
                !globalThis.isBrowserGpuRetainedResourceField(tag, path, root) ||
                elementType !== (tag === 1 ? 'uint8' : 'uint32'))
                throw new Error('GPU retained reference is outside the immutable resource fields');
            const resource = retained.entries.get(retainedResourceIdentifier);
            if (!resource || resource.elementType !== elementType || resource.elementCount !== elementCount)
                throw new Error('GPU retained resource is missing or differs from its declared type/count');
            ++retained.statistics.resolvedReferences;
            return resource.array;
        }
        if (Object.hasOwn(value, 'sectionId')) {
            const {sectionId, elementType, elementCount} = value;
            if (Object.keys(value).length !== 3) throw new Error('Invalid GPU binary section reference');
            return binaryArray(sectionId, elementType, elementCount);
        }
        if (Array.isArray(value)) return value.map((field, index) => decodeValue(field, tag, [...path, String(index)], root));
        return Object.fromEntries(Object.entries(value).map(([key, field]) =>
            [key, decodeValue(field, tag, [...path, key], root)]));
    }
    const decodeSection = tag => { const value = decodeJsonSection(tag); return decodeValue(value, tag, [], value); };
    function configuration() {
        const value = decodeSection(1);
        if (!(value.registers instanceof Uint32Array) || value.registers.length !== 512 ||
            value.viewport?.length !== 4 || value.depthConfiguration?.length !== 2 ||
            value.textures?.length !== 3)
            throw new Error('GPU draw configuration differs from the selected renderer schema');
        return value;
    }
    switch (command.kind) {
    case 2: // DrawState.
        renderer.beginDraw(configuration());
        return 0;
    case 3: // Accepted asynchronous raw VertexBatch. False must never skip a draw.
        renderer.beginDraw(configuration());
        if (renderer.tryDrawVertexBatch(decodeSection(2)) !== true)
            throw new Error('Previously accepted GPU vertex draw was rejected after publication');
        return 0;
    case 5: { // MemoryFill, on an already fenced surface.
        const fill = decodeSection(1);
        renderer.clearSurface(String(fill.surfaceId), fill.color ?? null, fill.depthStencil ?? null);
        return 0;
    }
    case 7: { // FramebufferReadback. Zero bytes means no dirty surface.
        const request = decodeSection(1);
        const pixels = renderer.readSurface(String(request.surfaceId));
        if (!pixels) return 0;
        if (!(pixels instanceof Uint8Array) || pixels.byteLength > resultBytes)
            throw new Error('GPU color readback exceeds its reserved result extent');
        command.result.set(pixels);
        return pixels.byteLength;
    }
    case 8: { // CacheInvalidation.
        const range = decodeSection(1);
        if (!Number.isInteger(range.physicalAddress) || range.physicalAddress < 0 ||
            !Number.isInteger(range.bytes) || range.bytes < 0 ||
            range.physicalAddress + range.bytes > 0x100000000)
            throw new Error('GPU vertex-cache invalidation has an invalid physical extent');
        renderer.endDraw();
        renderer.vertexDrawCache?.invalidatePhysicalRange(range.physicalAddress, range.bytes);
        return 0;
    }
    case 9: { // Ordered presentation, after all earlier GPU draws.
        if (!count && !resultBytes) {
            renderer.endDraw(); renderer.gl.finish(); return 0;
        }
        if (count !== 1 || !sections.has(1) || resultBytes !== 1)
            throw new Error('GPU presentation needs one owned descriptor and one admission byte');
        const descriptor = decodeSection(1);
        if (descriptor.rendererFrame !== command.frameIdentifier.toString() ||
            descriptor.sampledTicks !== command.submissionTicks.toString())
            throw new Error('GPU presentation metadata differs from its queue record');
        if (typeof globalThis.createPicaWebGlPresentation !== 'function') {
            command.result[0] = 0; return 1;
        }
        const publisher = globalThis.browserGpuPresentationPublisher ??=
            globalThis.initializeBrowserGpuPresentationPublisher(memoryBuffer);
        if (!publisher.ready()) { command.result[0] = 0; return 1; }
        if (!publisher.hasCapacity()) {
            publisher.noteBackpressure(); command.result[0] = 2; return 1;
        }
        const presentation = globalThis.browserGpuPresentation ??=
            globalThis.createPicaWebGlPresentation(renderer, {publishFrame: publisher.publishFrame});
        const status = presentation.present(descriptor);
        if (![0, 1, 2].includes(status)) throw new Error('GPU presentation returned an invalid admission status');
        command.result[0] = status;
        return 1;
    }
    case 10: // Explicit device barrier.
        if (count || resultBytes) throw new Error('GPU device barrier must have no sections or result');
        renderer.endDraw();
        renderer.gl.finish();
        return 0;
    case 11: { // CPU-shaded TriangleBatch, through the same GPU owner.
        const bytes = command.section(3);
        if (!bytes.byteLength || bytes.byteLength % (66 * 4) !== 0)
            throw new Error('GPU output triangle section must contain complete float22 triangles');
        const vertices = new Float32Array(bytes.buffer, bytes.byteOffset, bytes.byteLength / 4);
        // The CPU proxy may publish DrawState separately before triangle bytes.
        // That configuration already owns copies beyond queue retirement.
        const drawConfiguration = sections.has(1) ? configuration() : renderer.currentDrawConfiguration;
        if (!drawConfiguration) throw new Error('GPU output triangles require prepared draw state');
        renderer.beginDraw({...drawConfiguration, preparedVertexProgram: null});
        for (let offset = 0; offset < vertices.length; offset += 66)
            renderer.appendTriangle(vertices.subarray(offset, offset + 66));
        renderer.endDraw();
        return 0;
    }
    case 12: { // VertexProgramPreflight. Root prepares/compiles without drawing.
        if (resultBytes < 1) throw new Error('GPU vertex preflight needs one result byte');
        renderer.beginDraw(configuration());
        const supported = renderer.preflightVertexBatch(decodeSection(2));
        if (typeof supported !== 'boolean') throw new Error('GPU vertex preflight must return boolean');
        command.result[0] = supported ? 1 : 0;
        return 1;
    }
    case 13: // SurfaceDeletion, after CPU color synchronization and its fence.
        renderer.endDraw();
        renderer.deleteSurface(String(decodeSection(1).surfaceId));
        return 0;
    case 14: { // RendererDiagnostics. CPU owns file output after retirement.
        renderer.collectVertexDiagnostics?.();
        const presentation = globalThis.browserGpuPresentation;
        if (presentation) renderer.shaderDiagnostics.push(...presentation.takeShaderDiagnostics());
        const bytes = new TextEncoder().encode(JSON.stringify({schemaVersion: 1,
            workerAdmission: globalThis.browserGpuWorkerAdmission,
            rendererStatistics: renderer.statistics,
            stateCacheStatistics: renderer.gl.stateCacheStatistics ?? {},
            vertexStatistics: renderer.vertexDrawCache?.statistics ?? {},
            vertexShaderPrograms: renderer.vertexDrawCache?.programs?.size ?? 0,
            presentationStatistics: presentation?.statistics ?? {},
            presentationUnsupportedStates: presentation?.unsupportedStates ?? {},
            presentationTransportStatistics: globalThis.browserGpuPresentationPublisher?.statistics ?? {},
            resourceRetentionStatistics: retained.statistics,
            shaderDiagnostics: renderer.shaderDiagnostics ?? [],
            unsupportedStates: renderer.unsupportedStates ?? {},
            shaderPrograms: renderer.shaderPrograms?.size ?? 0}) + '\n');
        if (bytes.byteLength > resultBytes) throw new Error('GPU diagnostics exceed reserved result extent');
        command.result.set(bytes);
        return bytes.byteLength;
    }
    case 15: // RendererShutdown.
        if (count || resultBytes) throw new Error('GPU renderer shutdown has unexpected payload');
        renderer.endDraw();
        renderer.gl.finish();
        globalThis.browserGpuPresentation?.dispose();
        globalThis.browserGpuPresentationPublisher?.close();
        renderer.close();
        delete globalThis.browserGpuRetainedResources;
        return 0;
    case 16: { // Ordered GPU-only DisplayTransfer into an owner-mapped alias.
        if (count !== 1 || !sections.has(1) || resultBytes !== 1)
            throw new Error('GPU display transfer needs one descriptor and one supported-status byte');
        const request = decodeSection(1);
        if (!['sourceSurfaceIdentifier', 'destinationSurfaceIdentifier', 'width', 'height'].every(field =>
            Number.isInteger(request[field]) && request[field] > 0 && request[field] <= 0xffffffff) ||
            !Number.isInteger(request.colorFormat) || request.colorFormat < 0 || request.colorFormat > 4 ||
            typeof request.flipVertically !== 'boolean')
            throw new Error('GPU display transfer descriptor is invalid');
        if (typeof globalThis.createPicaWebGlPresentation !== 'function') {
            command.result[0] = 0; return 1;
        }
        const presentation = globalThis.browserGpuPresentation ??=
            globalThis.createPicaWebGlPresentation(renderer, {publishFrame(packet, transfer) {
                const publisher = globalThis.browserGpuPresentationPublisher ??=
                    globalThis.initializeBrowserGpuPresentationPublisher(memoryBuffer);
                return publisher.publishFrame(packet, transfer);
            }});
        renderer.endDraw();
        const supported = presentation.copyDisplaySurface?.(request) ?? 0;
        if (![0, 1].includes(supported)) throw new Error('GPU display transfer must return supported status zero or one');
        command.result[0] = supported;
        return 1;
    }
    default:
        throw new Error('GPU render command is outside the prepared-draw stage: ' + command.kind);
    }
};

globalThis.releaseBrowserGpuWorker = function() {
    const renderer = globalThis.browserWebGlRenderer;
    if (!globalThis.browserGpuWorkerAdmission) return;
    globalThis.browserGpuPresentation?.dispose();
    globalThis.browserGpuPresentationPublisher?.close();
    if (renderer && !renderer.gl.isContextLost()) {
        renderer.endDraw();
        renderer.gl.finish();
        renderer.close();
    }
    delete globalThis.browserWebGlRenderer;
    delete globalThis.browserGpuWorkerAdmission;
    delete globalThis.browserGpuPresentation;
    delete globalThis.browserGpuPresentationPublisher;
    delete globalThis.browserGpuRetainedResources;
};
