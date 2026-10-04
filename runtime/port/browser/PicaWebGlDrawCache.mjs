// Independently authored, byte-coherent resources for the raw PICA vertex path.
// The owning renderer supplies the unchanged fragment shader and fixed-function state.
(() => {
    'use strict';
    const sameValues = (left, right) => left?.length === right?.length && left.every((value, index) => value === right[index]);
    const equalBytes = (left, right) => {
        if (left.byteLength !== right.byteLength) return false;
        for (let index = 0; index < left.byteLength; ++index) if (left[index] !== right[index]) return false;
        return true;
    };
    const overlap = (address, bytes, otherAddress, otherBytes) =>
        address < otherAddress + otherBytes && otherAddress < address + bytes;
    class PicaWebGlDrawCache {
        constructor(context, options = {}) {
            this.gl = context;
            this.maximumPrograms = options.maximumPrograms ?? 128;
            this.maximumResources = options.maximumResources ?? 256;
            this.maximumResourceBytes = options.maximumResourceBytes ?? 256 * 1024 * 1024;
            this.textureUnitBase = options.textureUnitBase ?? 8;
            this.uniformBlockBinding = options.uniformBlockBinding ?? 1;
            this.maximumTextureSize = context.getParameter(context.MAX_TEXTURE_SIZE);
            this.maximumVertexSamplers = context.getParameter(context.MAX_VERTEX_TEXTURE_IMAGE_UNITS);
            this.maximumCombinedSamplers = context.getParameter(context.MAX_COMBINED_TEXTURE_IMAGE_UNITS);
            this.maximumUniformBlockBytes = context.getParameter(context.MAX_UNIFORM_BLOCK_SIZE);
            this.programs = new Map();
            this.translations = new Map();
            this.translationEntries = 0;
            this.resources = new Map();
            this.resourceBytes = 0;
            this.uniformBuffer = context.createBuffer();
            this.vertexArray = context.createVertexArray();
            this.uniformBytes = null;
            this.shaderDiagnostics = [];
            this.diagnosticOffset = 0;
            this.nextProgramIdentifier = 1;
            this.maximumShaderDiagnostics = options.maximumShaderDiagnostics ?? 12288;
            this.unsupportedStates = Object.create(null);
            this.statistics = {draws: 0, vertices: 0, translations: 0, programHits: 0,
                resourceHits: 0, resourceUploads: 0, resourceInvalidations: 0,
                uniformHits: 0, uniformUploads: 0, uploadedBytes: 0, fallbackDraws: 0};
        }
        unsupported(reason) {
            this.unsupportedStates[reason] = (this.unsupportedStates[reason] ?? 0) + 1;
            ++this.statistics.fallbackDraws;
            return {supported: false, diagnostics: [reason]};
        }
        translation(descriptor) {
            // Native hashes select a bucket only. Complete words are compared before a hit.
            const identity = descriptor.programIdentity ?? 'unidentified';
            const layout = JSON.stringify(globalThis.describePicaVertexProgram(descriptor, false));
            const bucketKey = identity + ':' + layout;
            const bucket = this.translations.get(bucketKey) ?? [];
            const found = bucket.find(entry => sameValues(entry.programWords, descriptor.programWords) &&
                sameValues(entry.swizzleWords, descriptor.swizzleWords));
            if (found) {
                this.translations.delete(bucketKey);
                this.translations.set(bucketKey, bucket);
                return found.translation;
            }
            const translation = globalThis.createPicaVertexProgram(descriptor);
            ++this.statistics.translations;
            bucket.push({programWords: Uint32Array.from(descriptor.programWords),
                swizzleWords: Uint32Array.from(descriptor.swizzleWords), translation});
            ++this.translationEntries;
            this.translations.set(bucketKey, bucket);
            while (this.translationEntries > this.maximumPrograms) {
                const expiredKey = this.translations.keys().next().value;
                this.translationEntries -= this.translations.get(expiredKey).length;
                this.translations.delete(expiredKey);
            }
            return translation;
        }
        compile(translation, fragmentSource) {
            // Exact source strings are keys. Uniforms, resource addresses and contents are dynamic.
            const key = translation.key + '\n' + fragmentSource;
            let entry = this.programs.get(key);
            if (entry) {
                this.programs.delete(key); this.programs.set(key, entry);
                ++this.statistics.programHits;
                return entry;
            }
            if (this.shaderDiagnostics.length + 3 > this.maximumShaderDiagnostics)
                throw new Error('PICA vertex shader diagnostic history bound reached');
            const gl = this.gl, program = gl.createProgram(), shaders = [];
            const identifier = 'vertex-program-' + this.nextProgramIdentifier++;
            try {
                for (const [stage, type, source] of [['vertex', gl.VERTEX_SHADER, translation.vertexSource],
                    ['fragment', gl.FRAGMENT_SHADER, fragmentSource]]) {
                    const shader = gl.createShader(type);
                    shaders.push(shader); gl.shaderSource(shader, source); gl.compileShader(shader);
                    const passed = Boolean(gl.getShaderParameter(shader, gl.COMPILE_STATUS));
                    const log = gl.getShaderInfoLog(shader) ?? '';
                    this.shaderDiagnostics.push({key: identifier, stage, passed, log});
                    if (!passed) throw new Error('PICA ' + stage + ' compilation failed: ' + log);
                    gl.attachShader(program, shader);
                }
                gl.linkProgram(program);
                const passed = Boolean(gl.getProgramParameter(program, gl.LINK_STATUS));
                const log = gl.getProgramInfoLog(program) ?? '';
                this.shaderDiagnostics.push({key: identifier, stage: 'link', passed, log});
                if (!passed) throw new Error('PICA vertex program link failed: ' + log);
                for (const [name, binding] of [['PicaRegisterBlock', 0], ['PicaVertexBlock', this.uniformBlockBinding]]) {
                    const block = gl.getUniformBlockIndex(program, name);
                    if (block !== gl.INVALID_INDEX) gl.uniformBlockBinding(program, block, binding);
                }
                entry = {supported: true, program, uniforms: new Map(), translation,
                    samplerLocations: translation.uniforms.samplers.map(name => gl.getUniformLocation(program, name)),
                    vertexOffsetLocation: gl.getUniformLocation(program, translation.uniforms.vertexOffset)};
            } catch (error) {
                gl.deleteProgram(program);
                entry = {supported: false, diagnostics: [String(error.message ?? error)]};
            } finally {
                for (const shader of shaders) gl.deleteShader(shader);
            }
            this.programs.set(key, entry);
            while (this.programs.size > this.maximumPrograms) {
                const expiredKey = this.programs.keys().next().value, expired = this.programs.get(expiredKey);
                if (expired.program) gl.deleteProgram(expired.program);
                this.programs.delete(expiredKey);
            }
            return entry;
        }
        removeResource(key) {
            const resource = this.resources.get(key);
            if (!resource) return;
            resource.destroy(resource.value);
            this.resourceBytes -= resource.storageBytes;
            this.resources.delete(key);
            ++this.statistics.resourceInvalidations;
        }
        reserveResource(bytes, protectedKeys) {
            if (bytes > this.maximumResourceBytes) throw new Error('PICA resource exceeds renderer byte budget');
            while (this.resources.size >= this.maximumResources || this.resourceBytes + bytes > this.maximumResourceBytes) {
                const expiredKey = [...this.resources.keys()].find(key => !protectedKeys.has(key));
                if (expiredKey === undefined) throw new Error('PICA draw resources exceed renderer cache budget');
                this.removeResource(expiredKey);
            }
        }
        resolveResource(key, descriptor, create, destroy, storageBytes, protectedKeys = new Set()) {
            const data = descriptor.data;
            if (!(data instanceof Uint8Array)) throw new Error('PICA resource bytes are missing');
            let entry = this.resources.get(key);
            if (entry && equalBytes(entry.bytes, data)) {
                this.resources.delete(key); this.resources.set(key, entry);
                ++this.statistics.resourceHits;
                return entry;
            }
            if (entry) this.removeResource(key);
            this.reserveResource(storageBytes, protectedKeys);
            const bytes = data.slice(); // Publication owns bytes, never a mutable Wasm heap view.
            const value = create(bytes);
            entry = {value, bytes, physicalAddress: descriptor.physicalAddress,
                byteLength: bytes.byteLength, storageBytes, destroy};
            this.resources.set(key, entry); this.resourceBytes += storageBytes;
            ++this.statistics.resourceUploads;
            return entry;
        }
        rawResourceKey(descriptor) {
            return 'raw-r32ui:' + descriptor.physicalAddress + ':' + descriptor.data.byteLength;
        }
        resolveRawBytes(descriptor, protectedKeys) {
            const gl = this.gl;
            const wordCount = Math.max(1, Math.ceil(descriptor.data.byteLength / 4));
            const width = Math.min(this.maximumTextureSize, wordCount), height = Math.ceil(wordCount / width);
            if (height > this.maximumTextureSize) throw new Error('PICA raw byte texture exceeds WebGL2 extent');
            const storageBytes = width * height * 4;
            return this.resolveResource(this.rawResourceKey(descriptor), descriptor, bytes => {
                const packed = new Uint32Array(width * height);
                const view = new DataView(bytes.buffer, bytes.byteOffset, bytes.byteLength);
                // Explicit little-endian packing includes partial final words and unaligned inputs.
                for (let index = 0; index < packed.length; ++index) {
                    const offset = index * 4;
                    if (offset + 4 <= bytes.length) packed[index] = view.getUint32(offset, true);
                    else for (let part = 0; part < 4 && offset + part < bytes.length; ++part)
                        packed[index] |= bytes[offset + part] << (part * 8);
                }
                const texture = gl.createTexture();
                gl.activeTexture(gl.TEXTURE0 + this.textureUnitBase);
                gl.bindTexture(gl.TEXTURE_2D, texture);
                gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
                gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
                gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
                gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
                gl.pixelStorei(gl.UNPACK_ALIGNMENT, 4);
                gl.pixelStorei(gl.UNPACK_ROW_LENGTH, 0);
                gl.pixelStorei(gl.UNPACK_SKIP_PIXELS, 0);
                gl.pixelStorei(gl.UNPACK_SKIP_ROWS, 0);
                gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, false);
                gl.pixelStorei(gl.UNPACK_PREMULTIPLY_ALPHA_WEBGL, false);
                gl.texImage2D(gl.TEXTURE_2D, 0, gl.R32UI, width, height, 0, gl.RED_INTEGER, gl.UNSIGNED_INT, packed);
                const error = gl.getError();
                if (error !== gl.NO_ERROR) {
                    gl.deleteTexture(texture);
                    throw new Error('PICA raw texture upload failed: 0x' + error.toString(16));
                }
                this.statistics.uploadedBytes += storageBytes;
                return texture;
            }, texture => gl.deleteTexture(texture), storageBytes, protectedKeys);
        }
        resolveConvertedTexture(descriptor, operations) {
            // The existing renderer retains its PICA decoder and texture upload implementation.
            // A revision/hash alone never proves that guest bytes remain unchanged.
            const key = JSON.stringify(['pica-texture', descriptor.physicalAddress, descriptor.width,
                descriptor.height, descriptor.format, descriptor.data.byteLength, operations.conversionIdentity]);
            if (!operations.conversionIdentity) throw new Error('PICA texture conversion identity is required');
            return this.resolveResource(key, descriptor, bytes => operations.upload(operations.convert({...descriptor, data: bytes})),
                operations.destroy, operations.storageBytes).value;
        }
        uniformPacket(descriptor) {
            if (descriptor.floatUniforms?.length !== 384 || descriptor.integerUniforms?.length !== 16 ||
                descriptor.defaultAttributes?.length !== 64) throw new Error('Invalid PICA vertex uniform packet');
            const bytes = new Uint8Array(1872), floats = new Float32Array(bytes.buffer), integers = new Uint32Array(bytes.buffer);
            floats.set(descriptor.floatUniforms, 0);
            integers.set(descriptor.integerUniforms, 384);
            integers[400] = descriptor.booleanUniforms >>> 0;
            floats.set(descriptor.defaultAttributes, 404);
            return bytes;
        }
        prepare(descriptor, fragmentSource) {
            try {
                if (!Number.isInteger(descriptor.vertexCount) || descriptor.vertexCount <= 0 || descriptor.vertexCount % 3 ||
                    ![0, 3].includes(descriptor.primitiveType) || descriptor.geometryShaderEnabled)
                    return this.unsupported('PICA draw is not a complete GS-disabled triangle list');
                if (typeof fragmentSource !== 'string' || !fragmentSource) return this.unsupported('Current fragment source is missing');
                const translation = this.translation({...descriptor,
                    consumedOutputSemantics:globalThis.consumedPicaVertexSemantics(fragmentSource)});
                if (!translation.supported) return this.unsupported(translation.diagnostics.join('; '));
                const samplerCount = translation.uniforms.samplers.length;
                if (samplerCount > this.maximumVertexSamplers || this.textureUnitBase + samplerCount > this.maximumCombinedSamplers ||
                    translation.uniforms.byteLength > this.maximumUniformBlockBytes)
                    return this.unsupported('PICA vertex resources exceed WebGL2 limits');
                const program = this.compile(translation, fragmentSource);
                if (!program.supported) return this.unsupported(program.diagnostics.join('; '));
                const descriptors = descriptor.buffers.concat(descriptor.indexFormat ? [descriptor.indexBuffer] : []);
                const protectedKeys = new Set(descriptors.map(buffer => this.rawResourceKey(buffer)));
                const resources = descriptors.map(buffer => this.resolveRawBytes(buffer, protectedKeys));
                return {supported: true, ...program, resources, uniformBytes: this.uniformPacket(descriptor),
                    vertexOffset: descriptor.indexFormat ? 0 : descriptor.vertexOffset, vertexCount: descriptor.vertexCount,
                    diagnostics: []};
            } catch (error) {
                return this.unsupported(String(error?.message ?? error));
            }
        }
        takeShaderDiagnostics() {
            const records = this.shaderDiagnostics.slice(this.diagnosticOffset);
            this.diagnosticOffset = this.shaderDiagnostics.length;
            return records;
        }
        bind(prepared) {
            if (!prepared.supported) throw new Error('Cannot bind an unsupported PICA vertex draw');
            const gl = this.gl;
            gl.useProgram(prepared.program); gl.bindVertexArray(this.vertexArray);
            gl.bindBuffer(gl.UNIFORM_BUFFER, this.uniformBuffer);
            if (!this.uniformBytes || !equalBytes(this.uniformBytes, prepared.uniformBytes)) {
                // Orphan changed storage. Previously submitted draws retain their original uniforms.
                gl.bufferData(gl.UNIFORM_BUFFER, prepared.uniformBytes, gl.STREAM_DRAW);
                this.uniformBytes = prepared.uniformBytes;
                ++this.statistics.uniformUploads;
                this.statistics.uploadedBytes += prepared.uniformBytes.byteLength;
            } else ++this.statistics.uniformHits;
            gl.bindBufferBase(gl.UNIFORM_BUFFER, this.uniformBlockBinding, this.uniformBuffer);
            for (let index = 0; index < prepared.resources.length; ++index) {
                const unit = this.textureUnitBase + index;
                gl.activeTexture(gl.TEXTURE0 + unit); gl.bindSampler(unit, null);
                gl.bindTexture(gl.TEXTURE_2D, prepared.resources[index].value);
                gl.uniform1i(prepared.samplerLocations[index], unit);
            }
            gl.uniform1ui(prepared.vertexOffsetLocation, prepared.vertexOffset);
        }
        draw(prepared) {
            this.bind(prepared);
            this.gl.drawArrays(this.gl.TRIANGLES, 0, prepared.vertexCount);
            ++this.statistics.draws; this.statistics.vertices += prepared.vertexCount;
        }
        invalidatePhysicalRange(address, bytes) {
            for (const [key, resource] of this.resources)
                if (overlap(address, bytes, resource.physicalAddress, resource.byteLength)) this.removeResource(key);
        }
        dispose() {
            for (const key of this.resources.keys()) this.removeResource(key);
            for (const entry of this.programs.values()) if (entry.program) this.gl.deleteProgram(entry.program);
            this.programs.clear(); this.translations.clear();
            this.translationEntries = 0;
            this.gl.deleteBuffer(this.uniformBuffer); this.gl.deleteVertexArray(this.vertexArray);
            this.uniformBytes = null;
        }
    }
    globalThis.createPicaWebGlDrawCache = function createPicaWebGlDrawCache(context, options) {
        return new PicaWebGlDrawCache(context, options);
    };
})();
