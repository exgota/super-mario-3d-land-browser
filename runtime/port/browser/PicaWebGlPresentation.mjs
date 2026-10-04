// Independently authored GPU surface presentation. Guest memory coherence is owner-managed.
(() => {
    'use strict';
    const Result = Object.freeze({Unsupported: 0, Submitted: 1, Dropped: 2});
    const sources = {
        vertex: `#version 300 es
precision highp float;
out vec2 vPresentationCoordinate;
void main() {
    vec2 coordinate = vec2(float((gl_VertexID << 1) & 2), float(gl_VertexID & 2));
    vPresentationCoordinate = coordinate;
    gl_Position = vec4(coordinate * 2.0 - 1.0, 0.0, 1.0);
}`,
        fragment: `#version 300 es
precision highp float;
precision highp int;
uniform sampler2D uPresentationSource;
uniform ivec4 uPresentationSourceRectangle;
uniform int uPresentationRotation;
uniform bool uPresentationVerticalFlip;
uniform int uPresentationColorFormat;
in vec2 vPresentationCoordinate;
out vec4 oPresentationColor;
vec4 convertColor(vec4 value) {
    ivec4 color = ivec4(floor(clamp(value, 0.0, 1.0) * 255.0 + 0.5));
    if (uPresentationColorFormat == 1) color.a = 255;
    else if (uPresentationColorFormat == 2) {
        ivec3 packed = color.rgb >> ivec3(3, 2, 3);
        color.rgb = (packed << ivec3(3, 2, 3)) | (packed >> ivec3(2, 4, 2));
        color.a = 255;
    } else if (uPresentationColorFormat == 3) {
        ivec3 packed = color.rgb >> 3;
        color.rgb = (packed << 3) | (packed >> 2);
        color.a = (color.a >> 7) * 255;
    } else if (uPresentationColorFormat == 4) {
        ivec4 packed = color >> 4;
        color = (packed << 4) | packed;
    }
    return vec4(color) / 255.0;
}
void main() {
    vec2 coordinate = vPresentationCoordinate;
    if (uPresentationRotation == 1) coordinate = vec2(1.0 - coordinate.y, coordinate.x);
    else if (uPresentationRotation == 2) coordinate = vec2(1.0) - coordinate;
    else if (uPresentationRotation == 3) coordinate = vec2(coordinate.y, 1.0 - coordinate.x);
    if (uPresentationVerticalFlip) coordinate.y = 1.0 - coordinate.y;
    ivec2 pixel = clamp(ivec2(floor(coordinate * vec2(uPresentationSourceRectangle.zw))),
        ivec2(0), uPresentationSourceRectangle.zw - ivec2(1));
    oPresentationColor = convertColor(texelFetch(uPresentationSource,
        uPresentationSourceRectangle.xy + pixel, 0));
}`,
    };
    class PicaWebGlPresentation {
        constructor(renderer, {publishFrame} = {}) {
            if (!renderer?.gl || !renderer.canvas || !(renderer.surfaces instanceof Map))
                throw new Error('GPU presentation requires the owning WebGL2 renderer');
            this.renderer = renderer;
            this.gl = renderer.gl;
            this.publishFrame = publishFrame;
            this.maximumExtent = this.gl.getParameter(this.gl.MAX_TEXTURE_SIZE);
            this.maximumViewport = this.gl.getParameter(this.gl.MAX_VIEWPORT_DIMS);
            this.program = null;
            this.vertexArray = null;
            this.sampler = null;
            this.sequence = 0;
            this.closed = false;
            this.shaderDiagnostics = [];
            this.diagnosticOffset = 0;
            this.unsupportedStates = Object.create(null);
            this.aliasSurfaces = new Map();
            this.statistics = {submittedFrames: 0, submittedScreens: 0, droppedFrames: 0,
                unsupportedFrames: 0, canvasResizes: 0, bitmapTransfers: 0,
                copiedSurfaces: 0, aliasAllocations: 0, unsupportedTransfers: 0};
        }
        unsupported(reason, transfer = false) {
            this.unsupportedStates[reason] = (this.unsupportedStates[reason] ?? 0) + 1;
            ++this.statistics[transfer ? 'unsupportedTransfers' : 'unsupportedFrames'];
            return Result.Unsupported;
        }
        initialize() {
            if (this.program) return;
            const gl = this.gl, program = gl.createProgram(), shaders = [];
            try {
                for (const [stage, type] of [['vertex', gl.VERTEX_SHADER], ['fragment', gl.FRAGMENT_SHADER]]) {
                    const shader = gl.createShader(type);
                    shaders.push(shader);
                    gl.shaderSource(shader, sources[stage]); gl.compileShader(shader);
                    const passed = Boolean(gl.getShaderParameter(shader, gl.COMPILE_STATUS));
                    const log = gl.getShaderInfoLog(shader) ?? '';
                    this.shaderDiagnostics.push({key: 'gpu-presentation', stage, passed, log});
                    if (!passed) throw new Error('GPU presentation ' + stage + ' compilation failed: ' + log);
                    gl.attachShader(program, shader);
                }
                gl.linkProgram(program);
                const passed = Boolean(gl.getProgramParameter(program, gl.LINK_STATUS));
                const log = gl.getProgramInfoLog(program) ?? '';
                this.shaderDiagnostics.push({key: 'gpu-presentation', stage: 'link', passed, log});
                if (!passed) throw new Error('GPU presentation link failed: ' + log);
                this.uniforms = Object.fromEntries(['Source', 'SourceRectangle', 'Rotation', 'VerticalFlip', 'ColorFormat']
                    .map(name => [name, gl.getUniformLocation(program, 'uPresentation' + name)]));
                this.vertexArray = gl.createVertexArray();
                this.sampler = gl.createSampler();
                gl.samplerParameteri(this.sampler, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
                gl.samplerParameteri(this.sampler, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
                gl.samplerParameteri(this.sampler, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
                gl.samplerParameteri(this.sampler, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
                this.program = program;
            } catch (error) {
                gl.deleteProgram(program);
                this.initializationFailure = String(error?.message ?? error);
                throw error;
            } finally {
                for (const shader of shaders) gl.deleteShader(shader);
            }
        }
        prepare(frame) {
            if (this.closed || this.initializationFailure) throw new Error(this.initializationFailure ?? 'GPU presentation is closed');
            if (frame?.schemaVersion !== 1 || !Array.isArray(frame.screens) ||
                frame.screens.length < 1 || frame.screens.length > 2 ||
                !/^\d+$/.test(frame.rendererFrame) || !/^\d+$/.test(frame.sampledTicks))
                throw new Error('Invalid GPU presentation frame');
            const identifiers = new Set(), screens = [];
            let width = 0, height = 0;
            for (const screen of frame.screens) {
                const {screenIdentifier, surfaceIdentifier, sourceX, sourceY, sourceWidth, sourceHeight,
                    rotationQuarterTurns, flags, colorFormat} = screen;
                if (![0, 2].includes(screenIdentifier) || identifiers.has(screenIdentifier) ||
                    !Number.isInteger(surfaceIdentifier) || surfaceIdentifier < 1 ||
                    ![sourceX, sourceY, sourceWidth, sourceHeight, screen.width, screen.height,
                        rotationQuarterTurns, flags, colorFormat].every(Number.isInteger) ||
                    sourceX < 0 || sourceY < 0 || sourceWidth < 1 || sourceHeight < 1 ||
                    screen.width < 1 || screen.height < 1 || rotationQuarterTurns < 0 || rotationQuarterTurns > 3 ||
                    flags < 0 || flags > 1 || colorFormat < 0 || colorFormat > 4)
                    throw new Error('Unsupported GPU presentation screen');
                const rotated = (rotationQuarterTurns & 1) !== 0;
                if (screen.width !== (rotated ? sourceHeight : sourceWidth) ||
                    screen.height !== (rotated ? sourceWidth : sourceHeight))
                    throw new Error('GPU presentation requires an owner-resolved unscaled screen');
                const surface = this.renderer.surfaces.get(String(surfaceIdentifier));
                if (!surface?.colorTexture || !Number.isInteger(surface.width) || !Number.isInteger(surface.height) ||
                    sourceX + sourceWidth > surface.width || sourceY + sourceHeight > surface.height)
                    throw new Error('GPU presentation source surface is unavailable or outside its extent');
                identifiers.add(screenIdentifier);
                screens.push({...screen, surface});
                width = Math.max(width, screen.width); height += screen.height;
            }
            if (width > this.maximumExtent || height > this.maximumExtent ||
                width > this.maximumViewport[0] || height > this.maximumViewport[1])
                throw new Error('GPU presentation exceeds the drawing-buffer extent');
            screens.sort((left, right) => left.screenIdentifier - right.screenIdentifier);
            let offset = 0;
            for (const screen of screens) {
                screen.x = Math.floor((width - screen.width) / 2);
                screen.y = offset; offset += screen.height;
            }
            return {width, height, screens};
        }
        configureCopy(framebuffer, width, height) {
            const gl = this.gl;
            gl.bindFramebuffer(gl.FRAMEBUFFER, framebuffer);
            gl.useProgram(this.program); gl.bindVertexArray(this.vertexArray);
            gl.disable(gl.DEPTH_TEST); gl.disable(gl.STENCIL_TEST); gl.disable(gl.BLEND);
            gl.disable(gl.CULL_FACE); gl.disable(gl.SCISSOR_TEST);
            gl.colorMask(true, true, true, true);
            gl.viewport(0, 0, width, height);
            gl.activeTexture(gl.TEXTURE0); gl.bindSampler(0, this.sampler);
            gl.uniform1i(this.uniforms.Source, 0);
        }
        copyDisplaySurface(request) {
            let created = null;
            try {
                if (this.closed || this.initializationFailure)
                    throw new Error(this.initializationFailure ?? 'GPU presentation is closed');
                const {sourceSurfaceIdentifier, destinationSurfaceIdentifier, width, height,
                    colorFormat, flipVertically} = request ?? {};
                if (![sourceSurfaceIdentifier, destinationSurfaceIdentifier, width, height, colorFormat].every(Number.isInteger) ||
                    sourceSurfaceIdentifier < 1 || destinationSurfaceIdentifier < 1 ||
                    sourceSurfaceIdentifier === destinationSurfaceIdentifier || width < 1 || height < 1 ||
                    colorFormat < 0 || colorFormat > 4 || typeof flipVertically !== 'boolean' ||
                    width > this.maximumExtent || height > this.maximumExtent ||
                    width > this.maximumViewport[0] || height > this.maximumViewport[1])
                    throw new Error('Unsupported GPU display-surface copy descriptor');
                const source = this.renderer.surfaces.get(String(sourceSurfaceIdentifier));
                if (!source?.colorTexture || source.width !== width || !Number.isInteger(source.height) ||
                    source.height < height || source.height > this.maximumExtent)
                    throw new Error('GPU display-surface copy requires an available unscaled source with sufficient rows');
                const key = String(destinationSurfaceIdentifier);
                let destination = this.renderer.surfaces.get(key);
                for (const [aliasKey, alias] of this.aliasSurfaces)
                    if (this.renderer.surfaces.get(aliasKey) !== alias) this.aliasSurfaces.delete(aliasKey);
                if (destination && (this.aliasSurfaces.get(key) !== destination ||
                    destination.width !== width || destination.height !== height ||
                    destination.colorTexture === source.colorTexture))
                    throw new Error('GPU display-surface destination collides with a different surface');
                this.initialize();
                this.renderer.endDraw();
                const gl = this.gl;
                if (!destination) {
                    if (!Number.isInteger(this.renderer.maximumSurfaces) || this.renderer.maximumSurfaces < 1 ||
                        this.renderer.surfaces.size >= this.renderer.maximumSurfaces)
                        throw new Error('GPU display-surface copy exceeds the owning surface budget');
                    created = {key, width, height, colorTexture: gl.createTexture(), framebuffer: gl.createFramebuffer(),
                        depthTexture: null, dirty: false, readback: null, presentationAlias: true};
                    gl.activeTexture(gl.TEXTURE0); gl.bindTexture(gl.TEXTURE_2D, created.colorTexture);
                    gl.texStorage2D(gl.TEXTURE_2D, 1, gl.RGBA8, width, height);
                    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
                    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
                    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
                    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
                    gl.bindFramebuffer(gl.FRAMEBUFFER, created.framebuffer);
                    gl.framebufferTexture2D(gl.FRAMEBUFFER, gl.COLOR_ATTACHMENT0, gl.TEXTURE_2D, created.colorTexture, 0);
                    if (gl.checkFramebufferStatus(gl.FRAMEBUFFER) !== gl.FRAMEBUFFER_COMPLETE)
                        throw new Error('GPU display-surface framebuffer is incomplete');
                    destination = created;
                }
                this.configureCopy(destination.framebuffer, width, height);
                gl.bindTexture(gl.TEXTURE_2D, source.colorTexture);
                // Render-surface texture rows run opposite the physical transfer rows.
                gl.uniform4i(this.uniforms.SourceRectangle, 0, source.height - height, width, height);
                gl.uniform1i(this.uniforms.Rotation, 0);
                gl.uniform1i(this.uniforms.VerticalFlip, flipVertically ? 1 : 0);
                gl.uniform1i(this.uniforms.ColorFormat, colorFormat);
                gl.drawArrays(gl.TRIANGLES, 0, 3);
                destination.dirty = true;
                if (created) {
                    // The renderer owns these GPU resources and their guest-read/close lifetime.
                    this.renderer.surfaces.set(key, created); this.aliasSurfaces.set(key, created);
                    created = null; ++this.statistics.aliasAllocations;
                }
                ++this.statistics.copiedSurfaces;
                return Result.Submitted;
            } catch (error) {
                if (created) {
                    this.gl.deleteFramebuffer(created.framebuffer);
                    this.gl.deleteTexture(created.colorTexture);
                }
                return this.unsupported(String(error?.message ?? error), true);
            }
        }
        present(frame) {
            let bitmap = null;
            try {
                if (typeof this.renderer.canvas.transferToImageBitmap !== 'function' || typeof this.publishFrame !== 'function')
                    throw new Error('GPU bitmap presentation and an explicit publisher are required');
                const prepared = this.prepare(frame);
                this.initialize();
                this.renderer.endDraw();
                const gl = this.gl, canvas = this.renderer.canvas;
                if (canvas.width !== prepared.width || canvas.height !== prepared.height) {
                    canvas.width = prepared.width; canvas.height = prepared.height;
                    ++this.statistics.canvasResizes;
                }
                this.configureCopy(null, prepared.width, prepared.height);
                gl.clearBufferfv(gl.COLOR, 0, new Float32Array([0, 0, 0, 1]));
                for (const screen of prepared.screens) {
                    gl.viewport(screen.x, prepared.height - screen.y - screen.height, screen.width, screen.height);
                    gl.bindTexture(gl.TEXTURE_2D, screen.surface.colorTexture);
                    gl.uniform4i(this.uniforms.SourceRectangle, screen.sourceX, screen.sourceY,
                        screen.sourceWidth, screen.sourceHeight);
                    gl.uniform1i(this.uniforms.Rotation, screen.rotationQuarterTurns);
                    gl.uniform1i(this.uniforms.VerticalFlip, screen.flags & 1);
                    gl.uniform1i(this.uniforms.ColorFormat, screen.colorFormat);
                    gl.drawArrays(gl.TRIANGLES, 0, 3);
                }
                bitmap = canvas.transferToImageBitmap();
                const packet = {schemaVersion: 1, type: 'browser_webgl_presentation', sequence: this.sequence + 1,
                    rendererFrame: frame.rendererFrame, sampledTicks: frame.sampledTicks,
                    width: prepared.width, height: prepared.height, bitmap,
                    screens: prepared.screens.map(({screenIdentifier, x, y, width, height}) =>
                        ({screenIdentifier, x, y, width, height}))};
                const admitted = this.publishFrame(packet, [bitmap]);
                if (admitted === false) {
                    bitmap.close(); bitmap = null;
                    ++this.statistics.droppedFrames;
                    return Result.Dropped;
                }
                if (admitted !== true) throw new Error('GPU presentation publisher did not report admission');
                bitmap = null; // The publisher owns the transferred bitmap, including drop/paint cleanup.
                ++this.sequence; ++this.statistics.submittedFrames;
                this.statistics.submittedScreens += prepared.screens.length;
                ++this.statistics.bitmapTransfers;
                return Result.Submitted;
            } catch (error) {
                bitmap?.close();
                return this.unsupported(String(error?.message ?? error));
            }
        }
        takeShaderDiagnostics() {
            const records = this.shaderDiagnostics.slice(this.diagnosticOffset);
            this.diagnosticOffset = this.shaderDiagnostics.length;
            return records;
        }
        dispose() {
            if (this.closed) return;
            this.closed = true;
            if (this.program) this.gl.deleteProgram(this.program);
            if (this.vertexArray) this.gl.deleteVertexArray(this.vertexArray);
            if (this.sampler) this.gl.deleteSampler(this.sampler);
            this.aliasSurfaces.clear(); // Alias texture/FBO ownership stays with the renderer.
        }
    }
    globalThis.browserWebGlPresentationResult = Result;
    globalThis.createPicaWebGlPresentation = function createPicaWebGlPresentation(renderer, options) {
        return new PicaWebGlPresentation(renderer, options);
    };
})();
