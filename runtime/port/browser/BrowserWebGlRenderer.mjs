// Independently authored WebGL2 play renderer. Existing software rendering is separate.
class BrowserWebGlRenderer {
    constructor({convertTexture} = {}) {
        this.canvas = new OffscreenCanvas(8, 8);
        this.gl = this.canvas.getContext('webgl2', {
            alpha: false, antialias: false, depth: true, stencil: true,
            preserveDrawingBuffer: false, premultipliedAlpha: false,
        });
        if (!this.gl) throw new Error('WebGL2 is unavailable in the execution worker');
        const gl = this.gl;
        this.compressedTexture = gl.getExtension('WEBGL_compressed_texture_etc1');
        if (!this.compressedTexture) throw new Error('Native ETC1 texture support is unavailable');
        this.convertTexture = convertTexture;
        this.program = this.createProgram(globalThis.picaWebGlShaderSources);
        this.uniforms = new Map();
        this.surfaces = new Map();
        this.textures = new Map();
        this.maximumSurfaces = 16;
        this.maximumTextures = 256;
        this.maximumTextureBytes = 256 * 1024 * 1024;
        this.textureBytes = 0;
        this.vertices = new Float32Array(22 * 65536);
        this.vertexCount = 0;
        this.buffer = gl.createBuffer();
        this.vertexArray = gl.createVertexArray();
        gl.bindVertexArray(this.vertexArray);
        gl.bindBuffer(gl.ARRAY_BUFFER, this.buffer);
        gl.bufferData(gl.ARRAY_BUFFER, this.vertices.byteLength, gl.DYNAMIC_DRAW);
        let offset = 0;
        for (const [location, components] of [[0, 4], [1, 4], [2, 3], [3, 4], [4, 4], [5, 3]]) {
            gl.enableVertexAttribArray(location);
            gl.vertexAttribPointer(location, components, gl.FLOAT, false, 22 * 4, offset * 4);
            offset += components;
        }
        this.samplers = Array.from({length: 6}, () => gl.createSampler());
        this.placeholder = this.uploadTexture({width: 1, height: 1, storage: 'rgba8',
            color: new Uint8Array(4), alpha: null});
        this.lightingTexture = gl.createTexture();
        this.fogTexture = gl.createTexture();
        this.lightingRevision = null;
        this.fogRevision = null;
        this.statistics = {draws: 0, triangles: 0, readbacks: 0, textureUploads: 0,
            uploadedBytes: 0, drawMilliseconds: 0, readbackMilliseconds: 0};
        gl.disable(gl.DITHER);
        gl.disable(gl.SCISSOR_TEST);
        gl.disable(gl.SAMPLE_COVERAGE);
        this.requireNoError('renderer initialization');
    }

    createProgram(sources) {
        if (!sources?.vertex || !sources?.fragment) throw new Error('Independent shader sources are missing');
        const gl = this.gl;
        const shaders = [];
        const program = gl.createProgram();
        try {
            for (const [type, source] of [[gl.VERTEX_SHADER, sources.vertex], [gl.FRAGMENT_SHADER, sources.fragment]]) {
                const shader = gl.createShader(type);
                shaders.push(shader);
                gl.shaderSource(shader, source);
                gl.compileShader(shader);
                if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS))
                    throw new Error('Independent shader compilation failed: ' + gl.getShaderInfoLog(shader));
                gl.attachShader(program, shader);
            }
            gl.linkProgram(program);
            if (!gl.getProgramParameter(program, gl.LINK_STATUS))
                throw new Error('Independent shader link failed: ' + gl.getProgramInfoLog(program));
            return program;
        } catch (error) {
            gl.deleteProgram(program);
            throw error;
        } finally {
            for (const shader of shaders) gl.deleteShader(shader);
        }
    }

    uniform(name) {
        if (!this.uniforms.has(name)) this.uniforms.set(name, this.gl.getUniformLocation(this.program, name));
        return this.uniforms.get(name);
    }

    requireNoError(operation) {
        const error = this.gl.getError();
        if (error !== this.gl.NO_ERROR) throw new Error('WebGL2 ' + operation + ' failed: 0x' + error.toString(16));
    }

    createSurface({key, width, height, color, depthStencil}) {
        const gl = this.gl;
        if (!key || !Number.isInteger(width) || !Number.isInteger(height) ||
            width < 1 || height < 1 || width > 1024 || height > 1024 ||
            !(color instanceof Uint8Array) || color.length !== width * height * 4 ||
            !(depthStencil instanceof Uint32Array) || depthStencil.length !== width * height)
            throw new Error('Invalid GPU render-target extent');
        if (this.surfaces.size >= this.maximumSurfaces) throw new Error('GPU render-target bound reached');
        const colorTexture = gl.createTexture();
        gl.bindTexture(gl.TEXTURE_2D, colorTexture);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
        gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA8, width, height, 0, gl.RGBA, gl.UNSIGNED_BYTE, color);
        const depthTexture = gl.createTexture();
        gl.bindTexture(gl.TEXTURE_2D, depthTexture);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
        gl.texImage2D(gl.TEXTURE_2D, 0, gl.DEPTH24_STENCIL8, width, height, 0,
            gl.DEPTH_STENCIL, gl.UNSIGNED_INT_24_8, depthStencil);
        const framebuffer = gl.createFramebuffer();
        gl.bindFramebuffer(gl.FRAMEBUFFER, framebuffer);
        gl.framebufferTexture2D(gl.FRAMEBUFFER, gl.COLOR_ATTACHMENT0, gl.TEXTURE_2D, colorTexture, 0);
        gl.framebufferTexture2D(gl.FRAMEBUFFER, gl.DEPTH_STENCIL_ATTACHMENT, gl.TEXTURE_2D, depthTexture, 0);
        if (gl.checkFramebufferStatus(gl.FRAMEBUFFER) !== gl.FRAMEBUFFER_COMPLETE)
            throw new Error('GPU framebuffer is incomplete');
        this.requireNoError('render-target upload');
        const surface = {key, width, height, colorTexture, depthTexture, framebuffer,
            readback: new Uint8Array(width * height * 4), dirty: false};
        this.surfaces.set(key, surface);
        return surface;
    }

    uploadTexture(converted) {
        const gl = this.gl;
        const {width, height, storage, color, alpha} = converted;
        if (!Number.isInteger(width) || !Number.isInteger(height) || width < 1 || height < 1 ||
            width > 2048 || height > 2048 || !(color instanceof Uint8Array) ||
            color.buffer instanceof SharedArrayBuffer || (alpha !== null &&
                (!(alpha instanceof Uint8Array) || alpha.length !== width * height || alpha.buffer instanceof SharedArrayBuffer)))
            throw new Error('Invalid converted GPU texture');
        const colorTexture = gl.createTexture();
        gl.bindTexture(gl.TEXTURE_2D, colorTexture);
        gl.pixelStorei(gl.UNPACK_ALIGNMENT, 1);
        if (storage === 'etc1') {
            if (color.length !== Math.ceil(width / 4) * Math.ceil(height / 4) * 8)
                throw new Error('Invalid ETC1 block count');
            gl.compressedTexImage2D(gl.TEXTURE_2D, 0,
                this.compressedTexture.COMPRESSED_RGB_ETC1_WEBGL, width, height, 0, color);
        } else if (storage === 'rgba8' && color.length === width * height * 4) {
            gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA8, width, height, 0, gl.RGBA, gl.UNSIGNED_BYTE, color);
        } else throw new Error('Unsupported converted GPU texture storage');
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
        let alphaTexture = null;
        if (alpha) {
            alphaTexture = gl.createTexture();
            gl.bindTexture(gl.TEXTURE_2D, alphaTexture);
            gl.texImage2D(gl.TEXTURE_2D, 0, gl.R8, width, height, 0, gl.RED, gl.UNSIGNED_BYTE, alpha);
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
        }
        this.requireNoError('texture upload');
        return {colorTexture, alphaTexture, bytes: color.byteLength + (alpha?.byteLength ?? 0)};
    }

    resolveTexture(descriptor) {
        if (!descriptor?.enabled) return this.placeholder;
        if (!descriptor.key || typeof this.convertTexture !== 'function')
            throw new Error('Independent PICA texture conversion is unavailable');
        if (this.textures.has(descriptor.key)) {
            const texture = this.textures.get(descriptor.key);
            this.textures.delete(descriptor.key);
            this.textures.set(descriptor.key, texture);
            return texture;
        }
        const texture = this.uploadTexture(this.convertTexture(descriptor));
        if (texture.bytes > this.maximumTextureBytes) throw new Error('GPU texture exceeds cache bound');
        while (this.textures.size >= this.maximumTextures || this.textureBytes + texture.bytes > this.maximumTextureBytes) {
            const key = this.textures.keys().next().value;
            const expired = this.textures.get(key);
            this.gl.deleteTexture(expired.colorTexture);
            if (expired.alphaTexture) this.gl.deleteTexture(expired.alphaTexture);
            this.textureBytes -= expired.bytes;
            this.textures.delete(key);
        }
        this.textures.set(descriptor.key, texture);
        this.textureBytes += texture.bytes;
        ++this.statistics.textureUploads;
        this.statistics.uploadedBytes += texture.bytes;
        return texture;
    }

    uploadLookup(unit, texture, width, height, data, uniformName) {
        if (!(data instanceof Float32Array) || data.length !== width * height * 2 || data.buffer instanceof SharedArrayBuffer)
            throw new Error('Invalid GPU lookup table');
        const gl = this.gl;
        gl.activeTexture(gl.TEXTURE0 + unit);
        gl.bindTexture(gl.TEXTURE_2D, texture);
        gl.texImage2D(gl.TEXTURE_2D, 0, gl.RG32F, width, height, 0, gl.RG, gl.FLOAT, data);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
        gl.uniform1i(this.uniform(uniformName), unit);
    }

    beginDraw(configuration) {
        if (this.vertexCount) throw new Error('A GPU draw batch is already open');
        const gl = this.gl;
        const {registers, viewport, depthConfiguration, surface, textures, lightingLookup, fogLookup} = configuration;
        if (!(registers instanceof Uint32Array) || registers.length !== 512 || registers.buffer instanceof SharedArrayBuffer ||
            viewport.length !== 4 || depthConfiguration.length !== 2 || textures.length !== 3)
            throw new Error('Invalid GPU draw configuration');
        this.currentSurface = this.surfaces.get(surface.key) ?? this.createSurface(surface);
        gl.bindFramebuffer(gl.FRAMEBUFFER, this.currentSurface.framebuffer);
        gl.useProgram(this.program);
        gl.bindVertexArray(this.vertexArray);
        gl.viewport(...viewport);
        gl.uniform4uiv(this.uniform('uRegisters[0]'), registers);
        gl.uniform2fv(this.uniform('uDepthConfiguration'), depthConfiguration);
        const comparisons = [gl.NEVER, gl.ALWAYS, gl.EQUAL, gl.NOTEQUAL, gl.LESS, gl.LEQUAL, gl.GREATER, gl.GEQUAL];
        const depth = registers[0x107];
        gl.enable(gl.DEPTH_TEST);
        gl.depthFunc((depth & 1) !== 0 ? comparisons[(depth >>> 4) & 7] : gl.ALWAYS);
        gl.depthMask((depth & 0x1000) !== 0 && (registers[0x115] & 3) !== 0);
        const colorWrite = (registers[0x113] & 15) !== 0 && !((registers[0x100] & 256) === 0 && (registers[0x102] & 15) === 6);
        gl.colorMask(...[8, 9, 10, 11].map(bit => colorWrite && (depth & (1 << bit)) !== 0));
        const cull = registers[0x40] & 3;
        if (cull === 1 || cull === 2) {
            gl.enable(gl.CULL_FACE);
            gl.cullFace(gl.BACK);
            gl.frontFace(cull === 1 ? gl.CW : gl.CCW);
        } else gl.disable(gl.CULL_FACE);
        const stencil = registers[0x105];
        if ((stencil & 1) !== 0) {
            gl.enable(gl.STENCIL_TEST);
            gl.stencilFunc(comparisons[(stencil >>> 4) & 7], (stencil >>> 16) & 255, stencil >>> 24);
            gl.stencilMask((stencil >>> 8) & 255);
            const operations = [gl.KEEP, gl.ZERO, gl.REPLACE, gl.INCR, gl.DECR, gl.INVERT, gl.INCR_WRAP, gl.DECR_WRAP];
            const operation = registers[0x106];
            gl.stencilOp(...[0, 4, 8].map(shift => operations[(operation >>> shift) & 7]));
        } else gl.disable(gl.STENCIL_TEST);
        if ((registers[0x100] & 256) !== 0) {
            gl.enable(gl.BLEND);
            const equations = [gl.FUNC_ADD, gl.FUNC_SUBTRACT, gl.FUNC_REVERSE_SUBTRACT, gl.MIN, gl.MAX];
            const factors = [gl.ZERO, gl.ONE, gl.SRC_COLOR, gl.ONE_MINUS_SRC_COLOR, gl.DST_COLOR,
                gl.ONE_MINUS_DST_COLOR, gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA, gl.DST_ALPHA,
                gl.ONE_MINUS_DST_ALPHA, gl.CONSTANT_COLOR, gl.ONE_MINUS_CONSTANT_COLOR,
                gl.CONSTANT_ALPHA, gl.ONE_MINUS_CONSTANT_ALPHA, gl.SRC_ALPHA_SATURATE];
            const blend = registers[0x101];
            if (!equations[blend & 7] || !equations[(blend >>> 8) & 7]) throw new Error('Unsupported GPU blend equation');
            gl.blendEquationSeparate(equations[blend & 7], equations[(blend >>> 8) & 7]);
            gl.blendFuncSeparate(...[16, 20, 24, 28].map(shift => factors[(blend >>> shift) & 15]));
            gl.blendColor(...[0, 8, 16, 24].map(shift => ((registers[0x103] >>> shift) & 255) / 255));
        } else gl.disable(gl.BLEND);
        const alpha = [];
        for (let index = 0; index < 3; ++index) {
            const descriptor = textures[index];
            const texture = this.resolveTexture(descriptor);
            const wrapModes = new Map([[0, gl.CLAMP_TO_EDGE], [2, gl.REPEAT], [3, gl.MIRRORED_REPEAT]]);
            const wrapS = wrapModes.get(descriptor.wrapS ?? 0), wrapT = wrapModes.get(descriptor.wrapT ?? 0);
            if (wrapS === undefined || wrapT === undefined) throw new Error('Unsupported GPU texture wrap mode');
            for (const unit of [index, index + 3]) {
                gl.activeTexture(gl.TEXTURE0 + unit);
                gl.bindTexture(gl.TEXTURE_2D, unit < 3 ? texture.colorTexture : (texture.alphaTexture ?? this.placeholder.colorTexture));
                gl.samplerParameteri(this.samplers[unit], gl.TEXTURE_MIN_FILTER, gl.NEAREST);
                gl.samplerParameteri(this.samplers[unit], gl.TEXTURE_MAG_FILTER, gl.NEAREST);
                gl.samplerParameteri(this.samplers[unit], gl.TEXTURE_WRAP_S, wrapS);
                gl.samplerParameteri(this.samplers[unit], gl.TEXTURE_WRAP_T, wrapT);
                gl.bindSampler(unit, this.samplers[unit]);
            }
            gl.uniform1i(this.uniform(['uTextureZero', 'uTextureOne', 'uTextureTwo'][index]), index);
            gl.uniform1i(this.uniform(['uAlphaZero', 'uAlphaOne', 'uAlphaTwo'][index]), index + 3);
            alpha.push(texture.alphaTexture ? 1 : 0);
        }
        gl.uniform3iv(this.uniform('uSeparateAlpha'), alpha);
        // Rebind immutable lookup textures even when their revisions are unchanged.
        if (lightingLookup.revision !== this.lightingRevision) {
            this.uploadLookup(6, this.lightingTexture, 256, 24, lightingLookup.data, 'uLightingLookup');
            this.lightingRevision = lightingLookup.revision;
        } else { gl.activeTexture(gl.TEXTURE6); gl.bindTexture(gl.TEXTURE_2D, this.lightingTexture); }
        if (fogLookup.revision !== this.fogRevision) {
            this.uploadLookup(7, this.fogTexture, 128, 1, fogLookup.data, 'uFogLookup');
            this.fogRevision = fogLookup.revision;
        } else { gl.activeTexture(gl.TEXTURE7); gl.bindTexture(gl.TEXTURE_2D, this.fogTexture); }
        this.requireNoError('draw configuration');
    }

    appendTriangle(vertices) {
        if (!this.currentSurface || vertices.length !== 66) throw new Error('Invalid GPU triangle batch');
        if (this.vertexCount + 3 > 65536) {
            this.endDraw();
        }
        this.vertices.set(vertices, this.vertexCount * 22);
        this.vertexCount += 3;
    }

    endDraw() {
        if (!this.vertexCount) return;
        const gl = this.gl;
        const started = performance.now();
        gl.bindBuffer(gl.ARRAY_BUFFER, this.buffer);
        gl.bufferSubData(gl.ARRAY_BUFFER, 0, this.vertices.subarray(0, this.vertexCount * 22));
        gl.drawArrays(gl.TRIANGLES, 0, this.vertexCount);
        this.currentSurface.dirty = true;
        ++this.statistics.draws;
        this.statistics.triangles += this.vertexCount / 3;
        this.statistics.drawMilliseconds += performance.now() - started;
        this.vertexCount = 0;
    }

    readSurface(key) {
        this.endDraw();
        const surface = this.surfaces.get(key);
        if (!surface || !surface.dirty) return null;
        const gl = this.gl;
        const started = performance.now();
        gl.bindFramebuffer(gl.FRAMEBUFFER, surface.framebuffer);
        gl.readPixels(0, 0, surface.width, surface.height, gl.RGBA, gl.UNSIGNED_BYTE, surface.readback);
        this.requireNoError('color readback');
        surface.dirty = false;
        ++this.statistics.readbacks;
        this.statistics.readbackMilliseconds += performance.now() - started;
        return surface.readback;
    }

    invalidateSurfaces() {
        if (this.vertexCount) throw new Error('Cannot invalidate an open GPU draw batch');
        for (const surface of this.surfaces.values()) {
            if (surface.dirty) throw new Error('GPU surface requires memory synchronization before invalidation');
            this.gl.deleteFramebuffer(surface.framebuffer);
            this.gl.deleteTexture(surface.colorTexture);
            this.gl.deleteTexture(surface.depthTexture);
        }
        this.surfaces.clear();
        this.currentSurface = null;
    }

    close() {
        this.endDraw();
        this.gl.getExtension('WEBGL_lose_context')?.loseContext();
    }
}
globalThis.createBrowserWebGlRenderer = options => new BrowserWebGlRenderer(options);
