// Independently authored CPU-side proxy for the GPU-owned WebGL renderer.
(() => {
    globalThis.browserGpuPipelineEnabled = true;
    const rendererFactory = globalThis.createBrowserWebGlRenderer;
    const encoder = new TextEncoder();
    const equalWords = (left, right) => {
        if (left.length !== right.length) return false;
        for (let index = 0; index < left.length; ++index)
            if (left[index] !== right[index]) return false;
        return true;
    };
    class BrowserGpuRendererProxy {
        constructor() {
            _BrowserGpuRendererInitialize();
            this.surfaces = new Map();
            this.surfaceDescriptions = new Map();
            this.acceptedPrograms = new Map();
            this.acceptedProgramEntries = [];
            this.triangles = new Float32Array(22 * 65535);
            this.triangleComponents = 0;
            this.statistics = {};
            this.shaderDiagnostics = [];
            this.unsupportedStates = {};
            this.shaderPrograms = new Map();
            this.invalidateVertexRange = (physicalAddress, bytes) => {
                this.endDraw();
                this.submit(8, [[1, {physicalAddress, bytes}]], 0, true);
            };
            this.vertexDrawCache = {programs: new Map(), statistics: {},
                invalidatePhysicalRange: this.invalidateVertexRange};
            this.gl = {stateCacheStatistics: {}};
            this.closed = false;
        }
        submit(kind, sections = [], resultCapacity = 0, wait = false) {
            if (this.closed) throw new Error('GPU renderer proxy is closed');
            const binary = [];
            const encodeValue = value => {
                if (ArrayBuffer.isView(value)) {
                    const elementType = value instanceof Uint8Array ? 'uint8' :
                        value instanceof Uint32Array ? 'uint32' : value instanceof Float32Array ? 'float32' : null;
                    if (!elementType) throw new Error('Unsupported GPU packet element type');
                    const sectionId = 256 + binary.length;
                    binary.push([sectionId, new Uint8Array(value.buffer, value.byteOffset, value.byteLength)]);
                    return {sectionId, elementType, elementCount: value.length};
                }
                if (Array.isArray(value)) return value.map(encodeValue);
                if (value && typeof value === 'object')
                    return Object.fromEntries(Object.entries(value).map(([key, item]) => [key, encodeValue(item)]));
                return value;
            };
            const payload = sections.map(([tag, value]) => [tag,
                tag === 3 ? new Uint8Array(value.buffer, value.byteOffset, value.byteLength) :
                encoder.encode(JSON.stringify(encodeValue(value)))]).concat(binary);
            let bytes = 40 + payload.length * 16;
            const align = value => Math.ceil(value / 8) * 8;
            for (const [, data] of payload) bytes = align(bytes) + data.byteLength;
            if (bytes + resultCapacity > 32 * 1024 * 1024)
                throw new Error('GPU render packet exceeds queue capacity');
            const pointer = _malloc(bytes);
            if (!pointer) throw new Error('GPU packet allocation failed');
            try {
                const packet = new Uint8Array(HEAPU8.buffer, pointer, bytes);
                packet.fill(0);
                const header = new DataView(packet.buffer, packet.byteOffset, packet.byteLength);
                header.setUint32(0, 0x50525047, true);
                header.setUint32(4, 1, true);
                header.setUint32(8, bytes, true);
                header.setUint32(12, payload.length, true);
                header.setUint32(16, kind, true);
                header.setUint32(20, 16, true);
                header.setBigUint64(24, BigInt(_BrowserButtonInputRendererFrame()), true);
                header.setBigUint64(32, BigInt(globalThis.browserGpuSubmissionTicks ?? 0), true);
                let offset = 40 + payload.length * 16;
                payload.forEach(([tag, data], index) => {
                    offset = align(offset);
                    const directory = 40 + index * 16;
                    header.setUint32(directory, tag, true);
                    header.setUint32(directory + 4, offset, true);
                    header.setUint32(directory + 8, data.byteLength, true);
                    packet.set(data, offset);
                    offset += data.byteLength;
                });
                const resultBytes = _BrowserGpuRendererSubmit(pointer, bytes, resultCapacity, Number(wait));
                return resultCapacity ? HEAPU8.slice(_BrowserGpuRendererResultPointer(),
                    _BrowserGpuRendererResultPointer() + resultBytes) : null;
            } finally { _free(pointer); }
        }
        beginDraw(configuration) {
            this.endDraw();
            this.configuration = configuration;
            this.configurationPending = true;
            const {surface} = configuration;
            this.surfaceDescriptions.set(String(surface.key), {width: surface.width, height: surface.height, dirty: false});
        }
        publishedDrawState() {
            const key = String(this.configuration.surface.key);
            this.surfaces.set(key, this.surfaceDescriptions.get(key));
            this.lightingRevision = this.configuration.lightingLookup.revision;
            this.fogRevision = this.configuration.fogLookup.revision;
            this.configurationPending = false;
        }
        ensureDrawState() {
            if (!this.configuration) throw new Error('GPU draw state is unavailable');
            if (this.configurationPending) {
                this.submit(2, [[1, this.configuration]]);
                this.publishedDrawState();
            }
        }
        tryDrawVertexBatch(descriptor) {
            if (!this.configuration) throw new Error('GPU raw vertices require draw state');
            this.endDraw();
            const fragment = globalThis.specializePicaWebGlShader(this.configuration.registers).sources.fragment;
            const layout = JSON.stringify(globalThis.describePicaVertexProgram(descriptor, false));
            const resourceExtents = descriptor.buffers.map(buffer => buffer.data.byteLength)
                .concat(descriptor.indexBuffer?.data.byteLength ?? 0).join(',');
            const key = descriptor.programIdentity + ':' + layout + ':' + resourceExtents;
            const bucket = this.acceptedPrograms.get(key) ?? [];
            let entry = bucket.find(item => item.fragment === fragment &&
                equalWords(item.programWords, descriptor.programWords) &&
                equalWords(item.swizzleWords, descriptor.swizzleWords));
            if (!entry) {
                const result = this.submit(12, [[1, this.configuration], [2, descriptor]], 1, true);
                if (result.length !== 1) throw new Error('GPU preflight returned no supported status');
                entry = {key, fragment, accepted: result[0] === 1,
                    programWords: Uint32Array.from(descriptor.programWords),
                    swizzleWords: Uint32Array.from(descriptor.swizzleWords)};
                bucket.push(entry);
                this.acceptedPrograms.set(key, bucket);
                this.acceptedProgramEntries.push(entry);
                if (this.acceptedProgramEntries.length > 128) {
                    const expired = this.acceptedProgramEntries.shift();
                    const previous = this.acceptedPrograms.get(expired.key);
                    previous.splice(previous.indexOf(expired), 1);
                    if (!previous.length) this.acceptedPrograms.delete(expired.key);
                }
                this.publishedDrawState();
            }
            if (!entry.accepted) return false;
            this.submit(3, [[1, this.configuration], [2, descriptor]]);
            this.publishedDrawState();
            return true;
        }
        appendTriangle(vertices) {
            if (vertices.length !== 66) throw new Error('GPU triangle requires three float22 vertices');
            if (this.triangleComponents + 66 > this.triangles.length) this.endDraw();
            this.triangles.set(vertices, this.triangleComponents);
            this.triangleComponents += 66;
        }
        endDraw() {
            if (!this.triangleComponents) return;
            this.ensureDrawState();
            this.submit(11, [[3, this.triangles.subarray(0, this.triangleComponents)]]);
            this.triangleComponents = 0;
        }
        readSurface(key) {
            this.endDraw();
            this.ensureDrawState();
            const surface = this.surfaces.get(String(key));
            if (!surface) return null;
            const result = this.submit(7, [[1, {surfaceId: String(key)}]], surface.width * surface.height * 4, true);
            return result.length ? result : null;
        }
        clearSurface(key, color, depthStencil) {
            this.endDraw();
            this.ensureDrawState();
            this.submit(5, [[1, {surfaceId: String(key), color, depthStencil}]], 0, true);
        }
        deleteSurface(key) {
            this.endDraw();
            this.submit(13, [[1, {surfaceId: String(key)}]], 0, true);
            this.surfaces.delete(String(key));
            this.surfaceDescriptions.delete(String(key));
        }
        synchronizeDiagnostics() {
            this.endDraw();
            const bytes = this.submit(14, [], 4 * 1024 * 1024, true);
            const record = JSON.parse(new TextDecoder().decode(bytes));
            this.statistics = record.rendererStatistics;
            this.shaderDiagnostics = record.shaderDiagnostics;
            this.unsupportedStates = record.unsupportedStates;
            this.shaderPrograms = {size: record.shaderPrograms};
            this.vertexDrawCache = {statistics: record.vertexStatistics,
                programs: {size: record.vertexShaderPrograms},
                invalidatePhysicalRange: this.invalidateVertexRange};
            this.gl.stateCacheStatistics = record.stateCacheStatistics;
            this.workerAdmission = record.workerAdmission;
        }
        close() {
            if (this.closed) return;
            this.endDraw();
            this.submit(15, [], 0, true);
            _BrowserGpuRendererShutdown();
            this.closed = true;
        }
    }
    globalThis.createBrowserWebGlRenderer = options =>
        globalThis.browserGpuPipelineEnabled && options?.gpuWorkerOwner !== true ?
            new BrowserGpuRendererProxy() : rendererFactory(options);
})();
