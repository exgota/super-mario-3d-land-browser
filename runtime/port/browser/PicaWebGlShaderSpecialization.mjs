// Independently specialize the register-driven shader at draw time.
// Dynamic colors, coordinates, lookup contents and alpha reference remain uniforms.
globalThis.specializePicaWebGlShader = function specializePicaWebGlShader(registers) {
    if (!(registers instanceof Uint32Array) || registers.length !== 512)
        throw new Error('Invalid PICA shader specialization state');
    const bases = [0xc0, 0xc8, 0xd0, 0xd8, 0xf0, 0xf8];
    const reject = reason => { throw new Error('Unsupported PICA shader state: ' + reason); };
    if ((registers[0x100] & 3) !== 0) reject('fragment operation');
    if (![0, 1, 3].includes(registers[0x65] & 3)) reject('scissor mode');
    if (![0, 5].includes(registers[0xe0] & 7)) reject('fog mode');
    if ((registers[0x100] & 256) === 0 && ![0, 3, 6].includes(registers[0x102] & 15))
        reject('logic operation');
    if ((registers[0x80] & 1) !== 0 && ![0, 3, 5].includes((registers[0x83] >>> 28) & 7))
        reject('texture zero type');
    const operandCounts = [1, 2, 2, 2, 3, 2, 2, 2, 3, 3];
    const validSources = [0, 1, 2, 3, 4, 5, 13, 14, 15];
    const validColorModifiers = [0, 1, 2, 3, 4, 5, 8, 9, 12, 13];
    for (const base of bases) {
        const operation = registers[base + 2], modifiers = registers[base + 1];
        const colorOperation = operation & 15, alphaOperation = (operation >>> 16) & 15;
        if (colorOperation > 9) reject('combiner color operation');
        if (colorOperation !== 7 && ![0, 1, 2, 3, 4, 5, 8, 9].includes(alphaOperation))
            reject('combiner alpha operation');
        if ((registers[base + 4] & 3) === 3 || ((registers[base + 4] >>> 16) & 3) === 3)
            reject('reserved combiner scale');
        for (let operand = 0; operand < operandCounts[colorOperation]; ++operand) {
            if (!validSources.includes((registers[base] >>> (operand * 4)) & 15))
                reject('combiner color source');
            if (!validColorModifiers.includes((modifiers >>> (operand * 4)) & 15))
                reject('combiner color modifier');
        }
        if (colorOperation !== 7)
            for (let operand = 0; operand < operandCounts[alphaOperation]; ++operand)
                if (!validSources.includes((registers[base] >>> (16 + operand * 4)) & 15))
                    reject('combiner alpha source');
    }
    const staticFields = [[0x65, 3], [0x6d, 1], [0x80, 0x2007],
        [0x83, 0x70000000], [0xe0, 0x1ff07], [0x100, 0x100],
        [0x102, 15], [0x104, 0x71], [0x1c6, 1]];
    const values = staticFields.map(([index, mask]) => (registers[index] & mask) >>> 0);
    for (const base of bases)
        for (const offset of [0, 1, 2, 4]) values.push(registers[base + offset]);
    const key = values.map(value => value.toString(16)).join(':');
    const sources = globalThis.picaWebGlShaderSources;
    if (!sources?.vertex || !sources?.fragment) throw new Error('Independent PICA shader sources are missing');
    let fragment = sources.fragment;
    const replaceOnce = (before, after) => {
        if (fragment.split(before).length !== 2)
            throw new Error('PICA shader specialization anchor is ambiguous');
        fragment = fragment.replace(before, after);
    };
    if ((registers[0x1c6] & 1) !== 0)
        replaceOnce('    lightingColors(textures,lightingPrimary,lightingSecondary);',
            '    lightingPrimary=ivec4(0); lightingSecondary=ivec4(0);');
    const begin = '    for(int stage=0;stage<6;++stage) {';
    const end = '    uint alphaTest=registerValue(0x104);';
    const first = fragment.indexOf(begin), last = fragment.indexOf(end);
    if (first < 0 || last <= first || fragment.indexOf(begin, first + 1) !== -1)
        throw new Error('PICA combiner specialization anchors are ambiguous');
    const stages = bases.map((base, stage) => {
        const source = registers[base], modifier = registers[base + 1];
        const operation = registers[base + 2], scale = registers[base + 4];
        const colorOperation = operation & 15, alphaOperation = (operation >>> 16) & 15;
        const colors = [], alphas = [];
        for (let operand = 0; operand < 3; ++operand) {
            const arguments_ = 'primary,lightingPrimary,lightingSecondary,textures,buffer,constantColor,previous';
            colors.push(`colorModifier(combinerSource(${(source >>> (operand * 4)) & 15},${arguments_}),${(modifier >>> (operand * 4)) & 15})`);
            alphas.push(`alphaModifier(combinerSource(${(source >>> (16 + operand * 4)) & 15},${arguments_}),${(modifier >>> (12 + operand * 4)) & 7})`);
        }
        const colorMultiplier = 1 << Math.min(scale & 3, 2);
        const alphaMultiplier = 1 << Math.min((scale >>> 16) & 3, 2);
        const updateColor = stage < 4 && (registers[0xe0] & (1 << (8 + stage))) !== 0;
        const updateAlpha = stage < 4 && (registers[0xe0] & (1 << (12 + stage))) !== 0;
        return `    {
        ivec4 constantColor=colorBytes(registerValue(${base + 3}));
        ivec3 rgb=combineColor(${colorOperation},${colors.join(',')});
        int alpha=${colorOperation === 7 ? 'rgb.r' : `combineAlpha(${alphaOperation},${alphas.join(',')})`};
        previous=clamp(ivec4(rgb*${colorMultiplier},alpha*${alphaMultiplier}),ivec4(0),ivec4(255));
        buffer=pendingBuffer;
        ${updateColor ? 'pendingBuffer.rgb=previous.rgb;' : ''}
        ${updateAlpha ? 'pendingBuffer.a=previous.a;' : ''}
    }
`;
    }).join('');
    fragment = fragment.slice(0, first) + stages + fragment.slice(last);
    replaceOnce('    if(field(alphaTest,0,1)!=0&&!compareValue(field(alphaTest,4,3),previous.a,field(alphaTest,8,8))) discard;',
        (registers[0x104] & 1) !== 0 ?
            `    if(!compareValue(${(registers[0x104] >>> 4) & 7},previous.a,field(alphaTest,8,8))) discard;` : '');
    replaceOnce('    if(field(registerValue(0x83),28,3)==3) coordinateZero/=vTextureZero.z;',
        ((registers[0x83] >>> 28) & 7) === 3 ? '    coordinateZero/=vTextureZero.z;' : '');
    replaceOnce('    vec2 coordinateTwo=field(registerValue(0x80),13,1)!=0 ? coordinateOne : vTextureOneTwo.zw;',
        `    vec2 coordinateTwo=${(registers[0x80] & 0x2000) !== 0 ? 'coordinateOne' : 'vTextureOneTwo.zw'};`);
    replaceOnce('    int enabled=field(registerValue(0x80),0,3);', `    int enabled=${registers[0x80] & 7};`);
    replaceOnce('    if(field(registerValue(0x83),28,3)==5) textures[0]=ivec4(0);',
        ((registers[0x83] >>> 28) & 7) === 5 ? '    textures[0]=ivec4(0);' : '');
    replaceOnce('    int scissorMode=field(registerValue(0x65),0,2);', `    int scissorMode=${registers[0x65] & 3};`);
    replaceOnce('    if(field(registerValue(0x6d),0,1)==0) depth=clamp(depth/gl_FragCoord.w,0.0,1.0);',
        (registers[0x6d] & 1) === 0 ? '    depth=clamp(depth/gl_FragCoord.w,0.0,1.0);' : '');
    replaceOnce('    if(field(registerValue(0xe0),0,3)==5) {', `    if(${(registers[0xe0] & 7) === 5 ? 'true' : 'false'}) {`);
    replaceOnce('    if(field(registerValue(0x100),8,1)==0&&field(registerValue(0x102),0,4)==0) previous=ivec4(0);',
        (registers[0x100] & 256) === 0 && (registers[0x102] & 15) === 0 ? '    previous=ivec4(0);' : '');
    return {key, sources:{vertex:sources.vertex, fragment}};
};
