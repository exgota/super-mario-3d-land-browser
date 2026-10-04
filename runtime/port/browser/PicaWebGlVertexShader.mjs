// Independently authored PICA vertex translator. Instruction encodings are documented at
// https://3dbrew.org/wiki/Shader_Instruction_Set and in nihstro's shader_bytecode.h.
// This file does not contain an existing-platform shader generator implementation.
(() => {
    'use strict';
    const componentNames = 'xyzw';
    const programSafetyCache = new Map();
    class UnsupportedVertexProgram extends Error {}
    const requireState = (condition, reason) => {
        if (!condition) throw new UnsupportedVertexProgram(reason);
    };
    const words = value => Array.from(value ?? [], word => word >>> 0);
    function staticDescription(descriptor, includeWords = true) {
        return {
            schemaVersion: descriptor.schemaVersion,
            programWords: includeWords ? words(descriptor.programWords) : undefined,
            swizzleWords: includeWords ? words(descriptor.swizzleWords) : undefined,
            entryPoint: descriptor.entryPoint,
            inputRegisterMapping: words(descriptor.inputRegisterMapping),
            inputAttributeCount: descriptor.inputAttributeCount,
            outputMask: descriptor.outputMask,
            outputCount: descriptor.outputCount,
            outputMapping: words(descriptor.outputMapping),
            attributes: descriptor.attributes.map(attribute => ({
                index: attribute.index, default: Boolean(attribute.default),
                format: attribute.format, components: attribute.components,
                bufferIndex: attribute.bufferIndex, byteOffset: attribute.byteOffset,
                byteStride: attribute.byteStride,
            })),
            indexFormat: descriptor.indexFormat,
            bufferCount: descriptor.buffers.length,
            clipEnabled: Boolean(descriptor.clipEnabled),
            inputToUniform: descriptor.inputToUniform ?? 0,
            consumedOutputSemantics: descriptor.consumedOutputSemantics ? words(descriptor.consumedOutputSemantics) : null,
        };
    }
    function validateDescriptor(descriptor) {
        requireState(descriptor?.schemaVersion === 1, 'Unsupported vertex descriptor schema');
        requireState(descriptor.programWords?.length > 0 && descriptor.programWords.length <= 4096,
            'Invalid PICA program extent');
        requireState(descriptor.swizzleWords?.length > 0 && descriptor.swizzleWords.length <= 4096,
            'Invalid PICA operand descriptor extent');
        requireState(Number.isInteger(descriptor.entryPoint) && descriptor.entryPoint >= 0 &&
            descriptor.entryPoint < descriptor.programWords.length, 'Invalid PICA entry point');
        requireState(!descriptor.clipEnabled, 'PICA user clip plane requires the output-vertex fallback');
        requireState(!(descriptor.inputToUniform ?? 0), 'Unsupported PICA input-to-uniform mode');
        requireState(Number.isInteger(descriptor.inputAttributeCount) && descriptor.inputAttributeCount >= 1 &&
            descriptor.inputAttributeCount <= 16, 'Invalid PICA input count');
        requireState(descriptor.inputRegisterMapping?.length === 16 &&
            Array.from(descriptor.inputRegisterMapping).every(value => Number.isInteger(value) && value < 16 && value >= 0),
            'Invalid PICA input register mapping');
        requireState(descriptor.attributes?.length >= descriptor.inputAttributeCount, 'Missing PICA input attributes');
        requireState(descriptor.buffers?.length <= 12, 'Invalid PICA attribute buffer count');
        requireState([0, 1, 2].includes(descriptor.indexFormat), 'Unsupported PICA index format');
        for (let index = 0; index < descriptor.inputAttributeCount; ++index) {
            const attribute = descriptor.attributes[index];
            requireState(attribute.index === index, 'PICA input attributes must be in loading order');
            if (attribute.default) continue;
            requireState([0, 1, 2, 3].includes(attribute.format) && attribute.components >= 1 && attribute.components <= 4,
                'Unsupported PICA input attribute format');
            requireState(Number.isInteger(attribute.bufferIndex) && attribute.bufferIndex >= 0 &&
                attribute.bufferIndex < descriptor.buffers.length, 'Missing PICA input buffer');
            requireState(Number.isInteger(attribute.byteOffset) && attribute.byteOffset >= 0 &&
                Number.isInteger(attribute.byteStride) && attribute.byteStride >= 0 && attribute.byteStride <= 255,
                'Invalid PICA input attribute layout');
        }
        const enabledOutputs = Array.from({length: 16}, (_, index) => index)
            .filter(index => ((descriptor.outputMask >>> index) & 1) !== 0);
        requireState(Number.isInteger(descriptor.outputCount) && descriptor.outputCount >= 1 &&
            descriptor.outputCount <= 7 && descriptor.outputCount <= enabledOutputs.length &&
            descriptor.outputMapping?.length >= descriptor.outputCount, 'Invalid PICA output register mapping');
        return enabledOutputs;
    }
    class VertexTranslator {
        constructor(descriptor) {
            this.descriptor = descriptor;
            this.functions = new Map();
            this.activeFunctions = new Set();
            this.instructionOffsets = new Set();
            this.loopRanges = new Set();
            this.arithmeticAccess = new Map();
            this.jumps = [];
            this.conditionals = [];
        }
        instruction(offset) {
            requireState(offset >= 0 && offset < this.descriptor.programWords.length,
                'PICA control flow leaves the uploaded program at ' + offset);
            this.instructionOffsets.add(offset);
            return this.descriptor.programWords[offset] >>> 0;
        }
        source(register, address = 0) {
            if (register < 16) return 'picaInput[' + register + ']';
            if (register < 32) return 'picaTemporary[' + (register - 16) + ']';
            return 'picaFloatUniform(' + (register - 32) + ', ' +
                (address ? 'picaAddress.' + 'xyz'[address - 1] : '0') + ')';
        }
        swizzle(source, operand, position) {
            const selectorStart = [5, 14, 23][position];
            const negateBit = [4, 13, 22][position];
            const selectors = [3, 2, 1, 0].map(index => componentNames[(operand >>> (selectorStart + index * 2)) & 3]).join('');
            return ((operand >>> negateBit) & 1 ? '-' : '') + '(' + source + ').' + selectors;
        }
        condition(word) {
            const left = 'picaComparison.x == ' + ((word >>> 25) & 1 ? 'true' : 'false');
            const right = 'picaComparison.y == ' + ((word >>> 24) & 1 ? 'true' : 'false');
            return ['(' + left + ') || (' + right + ')', '(' + left + ') && (' + right + ')', left, right][(word >>> 22) & 3];
        }
        boolean(word, invert = false) {
            return (invert ? '!' : '') + 'picaBoolean(' + ((word >>> 22) & 15) + ')';
        }
        arithmetic(word, offset) {
            let opcode = word >>> 26;
            const multiplyAdd = opcode >= 0x30;
            const inverted = multiplyAdd ? opcode < 0x38 : [0x18, 0x19, 0x1a, 0x1b].includes(opcode);
            const operandIndex = word & (multiplyAdd ? 31 : 127);
            requireState(operandIndex < this.descriptor.swizzleWords.length, 'Missing PICA operand descriptor at ' + offset);
            const operand = this.descriptor.swizzleWords[operandIndex] >>> 0;
            let registerOne, registerTwo, registerThree, destination, address;
            if (multiplyAdd) {
                opcode = inverted ? 0x30 : 0x38;
                registerOne = (word >>> 17) & 31;
                registerTwo = inverted ? (word >>> 12) & 31 : (word >>> 10) & 127;
                registerThree = inverted ? (word >>> 5) & 127 : (word >>> 5) & 31;
                destination = (word >>> 24) & 31;
                address = (word >>> 22) & 3;
            } else {
                if ((opcode & 0x3e) === 0x2e) opcode = 0x2e;
                registerOne = inverted ? (word >>> 14) & 31 : (word >>> 12) & 127;
                registerTwo = inverted ? (word >>> 7) & 127 : (word >>> 7) & 31;
                destination = (word >>> 21) & 31;
                address = (word >>> 19) & 3;
            }
            const one = this.swizzle(this.source(registerOne, !multiplyAdd && !inverted ? address : 0), operand, 0);
            const two = this.swizzle(this.source(registerTwo, multiplyAdd ? (!inverted ? address : 0) : (inverted ? address : 0)), operand, 1);
            const lines = ['{', 'vec4 sourceOne = ' + one + ';'];
            const unary = [0x05, 0x06, 0x0b, 0x0e, 0x0f, 0x12, 0x13].includes(opcode);
            const destinations = [0, 1, 2, 3].filter(component => operand & (8 >>> component));
            let written = 0n;
            const read = (register, position, components) => {
                if (!components.length || register < 16) return 0n;
                if (register >= 32) {
                    const relative = position === 0 ? (!multiplyAdd && !inverted ? address : 0) :
                        position === 1 ? (multiplyAdd ? (!inverted ? address : 0) : (inverted ? address : 0)) : (inverted ? address : 0);
                    return relative ? 1n << BigInt(127 + relative) : 0n;
                }
                let mask = 0n;
                for (const component of components) {
                    const selected = (operand >>> ([5, 14, 23][position] + (3 - component) * 2)) & 3;
                    mask |= 1n << BigInt((register - 16) * 4 + selected);
                }
                return mask;
            };
            const dependencies = new Map();
            for (const component of opcode === 0x2e ? [0, 1] : destinations) {
                if (opcode === 0x12 && component > 1) continue;
                const bit = 1n << BigInt(opcode === 0x2e ? 131 + component : opcode === 0x12 ? 128 + component :
                    destination < 16 ? 64 + destination * 4 + component : (destination - 16) * 4 + component);
                written |= bit;
                let selectedOne = [component], selectedTwo = unary ? [] : [component];
                if ([0x01, 0x03, 0x18].includes(opcode)) selectedOne = [0, 1, 2];
                if (opcode === 0x01) selectedTwo = [0, 1, 2];
                if (opcode === 0x02) selectedOne = selectedTwo = [0, 1, 2, 3];
                if ([0x03, 0x18].includes(opcode)) selectedTwo = [0, 1, 2, 3];
                if ([0x05, 0x06, 0x0e, 0x0f].includes(opcode)) selectedOne = [0];
                if ([0x04, 0x19].includes(opcode)) {
                    selectedOne = component === 1 || component === 2 ? [component] : [];
                    selectedTwo = component === 1 || component === 3 ? [component] : [];
                }
                let mask = read(registerOne, 0, selectedOne) | read(registerTwo, 1, selectedTwo);
                if (multiplyAdd) mask |= read(registerThree, 2, [component]);
                dependencies.set(bit, mask);
            }
            this.arithmeticAccess.set(offset, {written, dependencies});
            if (!unary) lines.push('vec4 sourceTwo = ' + two + ';');
            if (opcode === 0x2e) {
                const comparisons = ['==', '!=', '<', '<=', '>', '>='];
                const x = comparisons[(word >>> 24) & 7], y = comparisons[(word >>> 21) & 7];
                requireState(x && y, 'Unsupported PICA comparison operation at ' + offset);
                lines.push('picaComparison = bvec2(sourceOne.x ' + x + ' sourceTwo.x, sourceOne.y ' + y + ' sourceTwo.y);', '}');
                return lines;
            }
            const mask = [0, 1, 2, 3].filter(component => operand & (8 >>> component))
                .map(component => componentNames[component]).join('');
            if (opcode === 0x12) {
                for (const component of ['x', 'y']) {
                    if (mask.includes(component)) lines.push('picaAddress.' + component + ' = int(sourceOne.' + component + ');');
                }
                lines.push('}');
                return lines;
            }
            let expression;
            switch (opcode) {
            case 0x00: expression = 'sourceOne + sourceTwo'; break;
            case 0x01: expression = 'vec4(picaDot3(sourceOne, sourceTwo))'; break;
            case 0x02: expression = 'vec4(picaDot4(sourceOne, sourceTwo))'; break;
            case 0x03: case 0x18: expression = 'vec4(picaDot3(sourceOne, sourceTwo) + sourceTwo.w)'; break;
            case 0x04: case 0x19: expression = 'vec4(1.0, picaProduct(sourceOne.y, sourceTwo.y), sourceOne.z, sourceTwo.w)'; break;
            case 0x05: expression = 'vec4(exp2(sourceOne.x))'; break;
            case 0x06: expression = 'vec4(log2(sourceOne.x))'; break;
            case 0x08: expression = 'picaProduct4(sourceOne, sourceTwo)'; break;
            case 0x09: case 0x1a: expression = 'vec4(greaterThanEqual(sourceOne, sourceTwo))'; break;
            case 0x0a: case 0x1b: expression = 'vec4(lessThan(sourceOne, sourceTwo))'; break;
            case 0x0b: expression = 'floor(sourceOne)'; break;
            case 0x0c: expression = 'picaMaximum(sourceOne, sourceTwo)'; break;
            case 0x0d: expression = 'picaMinimum(sourceOne, sourceTwo)'; break;
            case 0x0e: expression = 'vec4(1.0 / sourceOne.x)'; break;
            case 0x0f: expression = 'vec4(inversesqrt(sourceOne.x))'; break;
            case 0x13: expression = 'sourceOne'; break;
            case 0x30: case 0x38:
                lines.push('vec4 sourceThree = ' + this.swizzle(this.source(registerThree, inverted ? address : 0), operand, 2) + ';');
                lines.push('vec4 product = picaProduct4(sourceOne, sourceTwo);');
                expression = 'product + sourceThree';
                break;
            default: throw new UnsupportedVertexProgram('Unsupported PICA opcode 0x' + opcode.toString(16) + ' at ' + offset);
            }
            lines.push('vec4 result = ' + expression + ';');
            const target = destination < 16 ? 'picaOutput[' + destination + ']' : 'picaTemporary[' + (destination - 16) + ']';
            if (mask) lines.push(target + '.' + mask + ' = result.' + mask + ';');
            lines.push('}');
            return lines;
        }
        section(start, finish, loopDepth = 0, boundary = finish) {
            requireState(start <= finish && start >= 0 && finish <= this.descriptor.programWords.length,
                'Invalid PICA control-flow section ' + start + ':' + finish);
            const name = 'picaSection_' + start + '_' + finish + '_' + loopDepth + '_' + boundary;
            if (this.functions.has(name)) return name;
            requireState(!this.activeFunctions.has(name), 'Recursive PICA call is unsupported');
            this.activeFunctions.add(name);
            const lines = ['int ' + name + '() {', 'int continuation = ' + start + ';'];
            const propagate = (functionName, indentation = '') => {
                lines.push(indentation + '{ int completion = ' + functionName + '();',
                    'if (completion == 1 || completion == 2) return completion;',
                    'if (completion >= 4) {', 'int destination = completion - 4;',
                    'if (destination >= ' + finish + ') return completion;',
                    'continuation = destination;', '}', '}');
            };
            for (let offset = start; offset < finish; ++offset) {
                const word = this.instruction(offset), opcode = word >>> 26;
                const destination = (word >>> 10) & 4095, count = word & 255;
                if (opcode === 0x21) continue;
                lines.push('if (continuation <= ' + offset + ') {');
                if (opcode === 0x22) {
                    requireState(!this.jumps.some(jump => jump.boundary === boundary && jump.destination > offset),
                        'PICA jump bypasses an END instruction at ' + offset);
                    lines.push('return 1;', '}'); break;
                }
                if (opcode === 0x20 || opcode === 0x23) {
                    requireState(loopDepth > 0, 'PICA break outside a loop at ' + offset);
                    lines.push((opcode === 0x23 ? 'if (' + this.condition(word) + ') ' : '') + 'return 2;');
                    lines.push('}');
                    if (opcode === 0x20) break;
                    continue;
                }
                if (opcode >= 0x24 && opcode <= 0x26) {
                    const called = this.section(destination, destination + count, loopDepth);
                    if (opcode !== 0x24) lines.push('if (' + (opcode === 0x25 ? this.condition(word) : this.boolean(word)) + ') {');
                    propagate(called);
                    if (opcode !== 0x24) lines.push('}');
                    lines.push('}');
                    continue;
                }
                if (opcode === 0x27 || opcode === 0x28) {
                    requireState(destination > offset && destination + count <= finish,
                        'PICA conditional crosses a section boundary at ' + offset);
                    const thenSection = this.section(offset + 1, destination, loopDepth, boundary);
                    const elseSection = this.section(destination, destination + count, loopDepth, boundary);
                    this.conditionals.push({start:offset + 1, elseStart:destination, finish:destination + count, boundary});
                    lines.push('if (' + (opcode === 0x28 ? this.condition(word) : this.boolean(word)) + ') {');
                    propagate(thenSection);
                    lines.push('} else {');
                    propagate(elseSection);
                    lines.push('}', '}');
                    offset = destination + count - 1;
                    continue;
                }
                if (opcode === 0x29) {
                    requireState(loopDepth === 0 && destination > offset && destination < finish,
                        'Nested or crossing PICA loop at ' + offset);
                    this.loopRanges.add(offset + ':' + destination);
                    const body = this.section(offset + 1, destination + 1, loopDepth + 1, destination + 1);
                    const uniform = (word >>> 22) & 3;
                    lines.push('{', 'picaAddress.z = uPicaInteger[' + uniform + '].y;',
                        'for (int repetition = 0; repetition <= uPicaInteger[' + uniform + '].x; ++repetition) {',
                        'int completion = ' + body + '();', 'if (completion == 1) return 1;',
                        'if (completion >= 4) return completion;',
                        'picaAddress.z += uPicaInteger[' + uniform + '].z;',
                        'if (completion == 2) break;', '}', '}', '}');
                    offset = destination;
                    continue;
                }
                if (opcode === 0x2c || opcode === 0x2d) {
                    requireState(destination > offset && destination < boundary,
                        'Backward or crossing PICA jump at ' + offset);
                    this.jumps.push({offset, destination, boundary});
                    const condition = opcode === 0x2c ? this.condition(word) : this.boolean(word, Boolean(count & 1));
                    lines.push('if (' + condition + ') {', destination >= finish ?
                        'return ' + (destination + 4) + ';' : 'continuation = ' + destination + ';', '}', '}');
                    continue;
                }
                lines.push(...this.arithmetic(word, offset));
                lines.push('}');
            }
            lines.push('return 0;', '}');
            this.activeFunctions.delete(name);
            this.functions.set(name, lines.join('\n'));
            return name;
        }
        validateAssignment(enabledOutputs) {
            for (const jump of this.jumps) {
                for (const region of this.conditionals) {
                    if (jump.boundary !== region.boundary || jump.destination < region.start || jump.destination >= region.finish) continue;
                    requireState(jump.offset >= region.start && jump.offset < region.finish &&
                        !(jump.offset < region.elseStart && jump.destination >= region.elseStart),
                        'PICA jump enters a conditional branch at ' + jump.offset);
                }
            }
            let requiredOutputs = 0n;
            for (let output = 0; output < this.descriptor.outputCount; ++output) {
                const mapping = this.descriptor.outputMapping[output] >>> 0;
                for (let component = 0; component < 4; ++component) {
                    const semantic = (mapping >>> (component * 8)) & 31;
                    requireState(semantic < 24 || semantic === 31, 'Unknown PICA output semantic');
                    if (semantic !== 31 && semantic !== 17 && semantic !== 21 &&
                        (!this.descriptor.consumedOutputSemantics || this.descriptor.consumedOutputSemantics.includes(semantic)))
                        requiredOutputs |= 1n << BigInt(64 + enabledOutputs[output] * 4 + component);
                }
            }
            const key = JSON.stringify([words(this.descriptor.programWords), words(this.descriptor.swizzleWords),
                this.descriptor.entryPoint, requiredOutputs.toString(16)]);
            if (programSafetyCache.has(key)) {
                const result = programSafetyCache.get(key);
                requireState(result.supported, result.reason);
                programSafetyCache.delete(key); programSafetyCache.set(key, result);
                return;
            }
            try {
                this.analyzeProgramAssignment(requiredOutputs);
                programSafetyCache.set(key, {supported:true});
            } catch (error) {
                if (!(error instanceof UnsupportedVertexProgram)) throw error;
                programSafetyCache.set(key, {supported:false, reason:error.message});
                throw error;
            } finally {
                while (programSafetyCache.size > 128) programSafetyCache.delete(programSafetyCache.keys().next().value);
            }
        }
        analyzeProgramAssignment(requiredOutputs) {
            // Correlate the two condition-code bits and repeated uniform tests. A plain
            // branch intersection incorrectly rejects complementary IFC/CALLC sequences.
            // This is compilation analysis, never a per-vertex CPU interpreter.
            const states = new Map(), pending = [];
            const successors = new Map(), terminals = new Set();
            let activeKey = null;
            let required = 0n, missingOutputs = 0n, ended = false;
            const unassignedReads = new Map();
            const publish = state => {
                const key = JSON.stringify([state.offset, state.comparison, state.knownBooleans, state.booleanValues, state.scopes]);
                if (activeKey !== null) {
                    if (!successors.has(activeKey)) successors.set(activeKey, new Set());
                    successors.get(activeKey).add(key);
                }
                const previous = states.get(key);
                if (previous) {
                    const assigned = previous.assigned & state.assigned;
                    if (assigned === previous.assigned) return;
                    previous.assigned = assigned;
                    pending.push(previous);
                } else {
                    requireState(states.size < 262144, 'PICA assignment analysis exceeds the compilation state budget');
                    state.key = key;
                    states.set(key, state); pending.push(state);
                }
            };
            const advance = (state, offset, scopes = state.scopes) => {
                scopes = scopes.slice();
                const loop = scopes.findLast(scope => scope[0] === 'loop');
                if (loop && offset === loop[2]) {
                    const loopIndex = scopes.lastIndexOf(loop);
                    publish({...state, offset:loop[1], scopes:scopes.slice(0, loopIndex + 1)});
                    scopes = scopes.slice(0, loopIndex);
                }
                while (scopes.length) {
                    const top = scopes.at(-1);
                    if (offset !== top[2]) break;
                    scopes.pop();
                    if (top[0] === 'call') offset = top[3];
                    if (top[0] === 'conditional') offset = top[3];
                }
                publish({...state, offset, scopes});
            };
            const conditional = (word, comparison) => {
                const x = Boolean(comparison & 1) === Boolean((word >>> 25) & 1);
                const y = Boolean(comparison & 2) === Boolean((word >>> 24) & 1);
                return [x || y, x && y, x, y][(word >>> 22) & 3];
            };
            const booleanPaths = (state, word, invert, operation) => {
                const bit = 1 << ((word >>> 22) & 15);
                const values = state.knownBooleans & bit ? [Boolean(state.booleanValues & bit)] : [false, true];
                for (const value of values) {
                    const branch = {...state, knownBooleans:state.knownBooleans | bit,
                        booleanValues:value ? state.booleanValues | bit : state.booleanValues & ~bit};
                    operation(branch, invert ? !value : value);
                }
            };
            let allWritten = 0n;
            for (const access of this.arithmeticAccess.values()) allWritten |= access.written;
            const initiallyAssigned = ((1n << 133n) - 1n) & ~allWritten;
            publish({offset:this.descriptor.entryPoint, comparison:0, knownBooleans:0, booleanValues:0, scopes:[], assigned:initiallyAssigned});
            while (pending.length) {
                const state = {...pending.pop()}, offset = state.offset;
                activeKey = state.key;
                requireState(offset >= 0 && offset < this.descriptor.programWords.length,
                    'PICA assignment flow leaves the uploaded program');
                const word = this.descriptor.programWords[offset] >>> 0, opcode = word >>> 26;
                const destination = (word >>> 10) & 4095, count = word & 255;
                if (opcode === 0x22) {
                    ended = true; terminals.add(state.key); continue;
                }
                if (opcode === 0x21) { advance(state, offset + 1); continue; }
                const branchCall = (branch, selected) => {
                    if (!selected || !count) { advance(branch, offset + 1); return; }
                    requireState(branch.scopes.filter(scope => scope[0] === 'call').length < 4,
                        'PICA call depth exceeds the shader hardware stack');
                    publish({...branch, offset:destination, scopes:[...branch.scopes, ['call', destination, destination + count, offset + 1]]});
                };
                if (opcode >= 0x24 && opcode <= 0x26) {
                    if (opcode === 0x26) booleanPaths(state, word, false, branchCall);
                    else branchCall(state, opcode === 0x24 || conditional(word, state.comparison));
                    continue;
                }
                const branchConditional = (branch, selected) => {
                    if (!selected) { advance(branch, destination); return; }
                    publish({...branch, offset:offset + 1,
                        scopes:[...branch.scopes, ['conditional', offset + 1, destination, destination + count]]});
                };
                if (opcode === 0x27 || opcode === 0x28) {
                    if (opcode === 0x27) booleanPaths(state, word, false, branchConditional);
                    else branchConditional(state, conditional(word, state.comparison));
                    continue;
                }
                if (opcode === 0x29) {
                    publish({...state, offset:offset + 1, assigned:state.assigned | (1n << 130n),
                        scopes:[...state.scopes, ['loop', offset + 1, destination + 1]]});
                    continue;
                }
                if (opcode === 0x20 || opcode === 0x23) {
                    if (opcode === 0x23 && !conditional(word, state.comparison)) advance(state, offset + 1);
                    else {
                        const loopIndex = state.scopes.findLastIndex(scope => scope[0] === 'loop');
                        requireState(loopIndex >= 0, 'PICA assignment break has no loop');
                        advance(state, state.scopes[loopIndex][2], state.scopes.slice(0, loopIndex));
                    }
                    continue;
                }
                if (opcode === 0x2c || opcode === 0x2d) {
                    const jump = (branch, selected) => advance(branch, selected ? destination : offset + 1);
                    if (opcode === 0x2d) booleanPaths(state, word, Boolean(count & 1), jump);
                    else jump(state, conditional(word, state.comparison));
                    continue;
                }
                const access = this.arithmeticAccess.get(offset);
                requireState(access, 'PICA assignment flow reaches untranslated instruction ' + offset);
                state.assigned |= access.written;
                if ((opcode & 0x3e) === 0x2e) {
                    for (let comparison = 0; comparison < 4; ++comparison) advance({...state, comparison}, offset + 1);
                } else advance(state, offset + 1);
            }
            const incomingLive = new Map([...terminals].map(key => [key, requiredOutputs]));
            const controlReads = word => {
                const opcode = word >>> 26;
                if (![0x23, 0x25, 0x28, 0x2c].includes(opcode)) return 0n;
                return ((word >>> 22) & 3) === 2 ? 1n << 131n :
                    ((word >>> 22) & 3) === 3 ? 1n << 132n : (1n << 131n) | (1n << 132n);
            };
            let changed = true;
            while (changed) {
                changed = false;
                for (const [key, state] of states) {
                    if (terminals.has(key)) continue;
                    let outgoing = 0n;
                    for (const next of successors.get(key) ?? []) outgoing |= incomingLive.get(next) ?? 0n;
                    const word = this.descriptor.programWords[state.offset] >>> 0;
                    const access = this.arithmeticAccess.get(state.offset);
                    let incoming = outgoing;
                    if (access) {
                        incoming &= ~access.written;
                        for (const [destination, reads] of access.dependencies)
                            if (outgoing & destination) incoming |= reads;
                    } else if ((word >>> 26) === 0x29) incoming &= ~(1n << 130n);
                    incoming |= controlReads(word);
                    if (incoming !== (incomingLive.get(key) ?? 0n)) {
                        incomingLive.set(key, incoming); changed = true;
                    }
                }
            }
            for (const [key, state] of states) {
                if (terminals.has(key)) {
                    missingOutputs |= requiredOutputs & ~state.assigned;
                    continue;
                }
                const word = this.descriptor.programWords[state.offset] >>> 0;
                const access = this.arithmeticAccess.get(state.offset);
                let needed = controlReads(word);
                if (access) {
                    let outgoing = 0n;
                    for (const next of successors.get(key) ?? []) outgoing |= incomingLive.get(next) ?? 0n;
                    for (const [destination, reads] of access.dependencies)
                        if (outgoing & destination) needed |= reads;
                }
                const unassigned = needed & ~state.assigned;
                required |= unassigned;
                if (unassigned) unassignedReads.set(state.offset, (unassignedReads.get(state.offset) ?? 0n) | unassigned);
            }
            requireState(ended, 'PICA vertex program has no reachable END');
            requireState(required === 0n, 'PICA program reads a temporary before definite assignment: ' +
                [...unassignedReads].map(([offset, mask]) => offset + '/0x' + mask.toString(16)).join(','));
            requireState(missingOutputs === 0n, 'PICA program leaves a mapped output unassigned: 0x' + missingOutputs.toString(16));
        }
    }
    function readerDeclaration(name) {
        return `uniform highp usampler2D ${name};
uint ${name}Word(uint wordOffset) {
    uint width = uint(textureSize(${name}, 0).x);
    return texelFetch(${name}, ivec2(int(wordOffset % width), int(wordOffset / width)), 0).r;
}
uint ${name}Read(uint byteOffset, uint byteCount) {
    uint shift = (byteOffset & 3u) * 8u;
    uint value = ${name}Word(byteOffset >> 2u) >> shift;
    if (shift + byteCount * 8u > 32u)
        value |= ${name}Word((byteOffset >> 2u) + 1u) << (32u - shift);
    return byteCount == 4u ? value : value & ((1u << (byteCount * 8u)) - 1u);
}`;
    }
    const arithmeticDeclarations = `
layout(std140) uniform PicaVertexBlock {
    vec4 uPicaFloat[96];
    ivec4 uPicaInteger[4];
    uvec4 uPicaBoolean;
    vec4 uPicaDefault[16];
};
uniform highp uint uPicaVertexOffset;
vec4 picaInput[16];
vec4 picaTemporary[16];
vec4 picaOutput[16];
ivec3 picaAddress;
bvec2 picaComparison;
bool picaBoolean(int index) { return (uPicaBoolean.x & (1u << uint(index))) != 0u; }
vec4 picaFloatUniform(int index, int offset) {
    if (offset < -128 || offset > 127) offset = 0;
    int address = (index + offset) & 127;
    return address < 96 ? uPicaFloat[address] : vec4(1.0);
}
float picaProduct(float left, float right) {
    float product = left * right;
    return isnan(product) && !isnan(left) && !isnan(right) ? 0.0 : product;
}
vec4 picaProduct4(vec4 left, vec4 right) {
    return vec4(picaProduct(left.x,right.x), picaProduct(left.y,right.y),
                picaProduct(left.z,right.z), picaProduct(left.w,right.w));
}
float picaDot3(vec4 left, vec4 right) {
    vec4 product = picaProduct4(left, right);
    return (product.x + product.y) + product.z;
}
float picaDot4(vec4 left, vec4 right) {
    vec4 product = picaProduct4(left, right);
    return ((product.x + product.y) + product.z) + product.w;
}
vec4 picaMaximum(vec4 left, vec4 right) {
    return vec4(left.x > right.x ? left.x : right.x, left.y > right.y ? left.y : right.y,
                left.z > right.z ? left.z : right.z, left.w > right.w ? left.w : right.w);
}
vec4 picaMinimum(vec4 left, vec4 right) {
    return vec4(left.x < right.x ? left.x : right.x, left.y < right.y ? left.y : right.y,
                left.z < right.z ? left.z : right.z, left.w < right.w ? left.w : right.w);
}
float picaSignedByte(uint value) { return float(int(value << 24u) >> 24); }
float picaSignedShort(uint value) { return float(int(value << 16u) >> 16); }
`;
    globalThis.createPicaVertexProgram = function createPicaVertexProgram(descriptor) {
        try {
            const enabledOutputs = validateDescriptor(descriptor);
            const translator = new VertexTranslator(descriptor);
            const main = translator.section(descriptor.entryPoint, descriptor.programWords.length);
            translator.validateAssignment(enabledOutputs);
            const readers = descriptor.buffers.map((_, index) => readerDeclaration('uPicaInputBytes' + index));
            if (descriptor.indexFormat) readers.push(readerDeclaration('uPicaIndexBytes'));
            const indexSource = descriptor.indexFormat ? 'uPicaIndexBytesRead(sequence * ' + descriptor.indexFormat + 'u, ' + descriptor.indexFormat + 'u)' : 'sequence + uPicaVertexOffset';
            const load = ['void picaLoadInput(uint vertex) {'];
            for (let index = 0; index < 16; ++index) load.push('picaInput[' + index + '] = vec4(0.0);');
            for (let index = 0; index < descriptor.inputAttributeCount; ++index) {
                const attribute = descriptor.attributes[index], register = descriptor.inputRegisterMapping[index];
                if (attribute.default) {
                    load.push('picaInput[' + register + '] = uPicaDefault[' + index + '];');
                    continue;
                }
                const size = attribute.format === 3 ? 4 : attribute.format === 2 ? 2 : 1;
                const components = [];
                for (let component = 0; component < 4; ++component) {
                    if (component >= attribute.components) { components.push(component === 3 ? '1.0' : '0.0'); continue; }
                    const bytes = 'uPicaInputBytes' + attribute.bufferIndex + 'Read(' +
                        (attribute.byteOffset + size * component) + 'u + vertex * ' + attribute.byteStride + 'u, ' + size + 'u)';
                    components.push(['picaSignedByte(', 'float(', 'picaSignedShort(', 'uintBitsToFloat('][attribute.format] + bytes + ')');
                }
                load.push('picaInput[' + register + '] = vec4(' + components.join(', ') + ');');
            }
            load.push('}');
            const semanticSources = Array(32).fill('1.0');
            for (let output = 0; output < descriptor.outputCount; ++output) {
                const mapping = descriptor.outputMapping[output] >>> 0;
                for (let component = 0; component < 4; ++component) {
                    const semantic = (mapping >>> (component * 8)) & 31;
                    semanticSources[semantic] = 'picaOutput[' + enabledOutputs[output] + '].' + componentNames[component];
                }
            }
            const vector = (start, count) => 'vec' + count + '(' + semanticSources.slice(start, start + count).join(', ') + ')';
            const evaluate = ['void picaEvaluate(uint sequence, out vec4 position, out vec4 quaternion,',
                '                  out vec4 color, out vec3 textureZero, out vec4 textureOneTwo, out vec3 view) {',
                'picaAddress = ivec3(0); picaComparison = bvec2(false);'];
            for (let index = 0; index < 16; ++index) evaluate.push('picaTemporary[' + index + '] = vec4(0.0); picaOutput[' + index + '] = vec4(0.0);');
            evaluate.push('picaLoadInput(' + indexSource + ');', main + '();',
                'position = ' + vector(0, 4) + ';', 'quaternion = ' + vector(4, 4) + ';',
                'color = ' + vector(8, 4) + ';');
            for (const component of componentNames) evaluate.push('color.' + component + ' = abs(color.' + component + ') < 1.0 ? abs(color.' + component + ') : 1.0;');
            evaluate.push('textureZero = vec3(' + semanticSources[12] + ', ' + semanticSources[13] + ', ' + semanticSources[16] + ');',
                'textureOneTwo = vec4(' + [14, 15, 22, 23].map(index => semanticSources[index]).join(', ') + ');',
                'view = ' + vector(18, 3) + ';', '}');
            const source = ['#version 300 es', 'precision highp float;', 'precision highp int;',
                'out vec4 vColor;', 'out vec3 vTextureZero;', 'out vec4 vTextureOneTwo;',
                'out vec4 vQuaternion;', 'out vec3 vView;', arithmeticDeclarations,
                ...readers, ...translator.functions.values(), load.join('\n'), evaluate.join('\n'),
                'void main() {', 'vec4 position;',
                'picaEvaluate(uint(gl_VertexID), position, vQuaternion, vColor, vTextureZero, vTextureOneTwo, vView);',
                'if ((gl_VertexID % 3) != 0) {',
                'vec4 firstPosition, firstQuaternion, firstColor, firstTextureOneTwo;',
                'vec3 firstTextureZero, firstView;',
                'picaEvaluate(uint(gl_VertexID - gl_VertexID % 3), firstPosition, firstQuaternion, firstColor, firstTextureZero, firstTextureOneTwo, firstView);',
                'if (picaDot4(vQuaternion, firstQuaternion) < 0.0) vQuaternion = -vQuaternion;', '}',
                'gl_Position = vec4(position.xy, 2.0 * position.z + position.w, position.w);', '}'].join('\n');
            return {supported: true, schemaVersion: 1, vertexSource: source,
                key: JSON.stringify(staticDescription(descriptor)), diagnostics: [],
                attributes: [], uniforms: {block: 'PicaVertexBlock', byteLength: 1872, binding: 1,
                    floatOffset: 0, integerOffset: 1536, booleanOffset: 1600, defaultOffset: 1616,
                    samplers: descriptor.buffers.map((_, index) => 'uPicaInputBytes' + index)
                        .concat(descriptor.indexFormat ? ['uPicaIndexBytes'] : []), vertexOffset: 'uPicaVertexOffset'},
                outputMapping: semanticSources,
                instructionOffsets: [...translator.instructionOffsets].sort((left, right) => left - right)};
        } catch (error) {
            if (!(error instanceof UnsupportedVertexProgram)) throw error;
            return {supported: false, schemaVersion: 1, vertexSource: null, diagnostics: [error.message]};
        }
    };
    globalThis.describePicaVertexProgram = staticDescription;
    globalThis.consumedPicaVertexSemantics = function consumedPicaVertexSemantics(fragmentSource) {
        const semantics = new Set([0, 1, 2, 3]);
        for (const [name, components] of [['vColor', [8, 9, 10, 11]], ['vTextureZero', [12, 13, 16]],
            ['vTextureOneTwo', [14, 15, 22, 23]], ['vQuaternion', [4, 5, 6, 7]], ['vView', [18, 19, 20]]]) {
            const expression = new RegExp('\\b' + name + '\\b(?:\\s*\\.\\s*([xyzwrgba]+))?', 'g');
            for (const match of fragmentSource.matchAll(expression)) {
                const prefix = fragmentSource.slice(0, match.index);
                if (/\bin\s+(?:(?:highp|mediump|lowp)\s+)?vec[234]\s*$/.test(prefix)) continue;
                const selected = match[1] ? [...match[1]].map(component => 'xyzw'.indexOf(component) >= 0 ?
                    'xyzw'.indexOf(component) : 'rgba'.indexOf(component)) : components.map((_, index) => index);
                for (const index of selected) if (index < components.length) semantics.add(components[index]);
            }
        }
        return [...semantics].sort((left, right) => left - right);
    };
})();
