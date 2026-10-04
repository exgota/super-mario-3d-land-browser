// Independent PICA play shaders. Register layouts come from interface descriptions.
// No Azahar renderer or shader generator implementation is included.
globalThis.picaWebGlShaderSources = {
    vertex: `#version 300 es
precision highp float;
layout(location=0) in vec4 aPosition;
layout(location=1) in vec4 aColor;
layout(location=2) in vec3 aTextureZero;
layout(location=3) in vec4 aTextureOneTwo;
layout(location=4) in vec4 aQuaternion;
layout(location=5) in vec3 aView;
out vec4 vColor;
out vec3 vTextureZero;
out vec4 vTextureOneTwo;
out vec4 vQuaternion;
out vec3 vView;
void main() {
    gl_Position = vec4(aPosition.xy, 2.0*aPosition.z+aPosition.w, aPosition.w);
    vColor=aColor; vTextureZero=aTextureZero; vTextureOneTwo=aTextureOneTwo;
    vQuaternion=aQuaternion; vView=aView;
}`,
    fragment: `#version 300 es
precision highp float;
precision highp int;
precision highp sampler2D;
uniform uvec4 uRegisters[128];
uniform vec2 uDepthConfiguration;
uniform sampler2D uTextureZero;
uniform sampler2D uTextureOne;
uniform sampler2D uTextureTwo;
uniform sampler2D uAlphaZero;
uniform sampler2D uAlphaOne;
uniform sampler2D uAlphaTwo;
uniform sampler2D uLightingLookup;
uniform sampler2D uFogLookup;
uniform ivec3 uSeparateAlpha;
in vec4 vColor;
in vec3 vTextureZero;
in vec4 vTextureOneTwo;
in vec4 vQuaternion;
in vec3 vView;
out vec4 fragmentColor;
uint registerValue(int index) { return uRegisters[index/4][index%4]; }
int field(uint value, int shift, int count) {
    return int((value >> uint(shift)) & ((1u << uint(count))-1u));
}
ivec4 colorBytes(uint value) {
    return ivec4(field(value,0,8),field(value,8,8),field(value,16,8),field(value,24,8));
}
vec3 lightColor(uint value) {
    return vec3(field(value,20,10),field(value,10,10),field(value,0,10))/255.0;
}
ivec4 quantizeColor(vec4 value) { return ivec4(floor(clamp(value,0.0,1.0)*255.0+0.5)); }
vec3 rotateQuaternion(vec4 quaternion, vec3 value) {
    return value+2.0*cross(quaternion.xyz,cross(quaternion.xyz,value)+quaternion.w*value);
}
vec3 normalizedVector(vec3 value) {
    float magnitude=dot(value,value);
    return magnitude>0.0 ? value*inversesqrt(magnitude) : vec3(0.0);
}
bool compareValue(int function, int left, int right) {
    if(function==0) return false; if(function==1) return true;
    if(function==2) return left==right; if(function==3) return left!=right;
    if(function==4) return left<right; if(function==5) return left<=right;
    if(function==6) return left>right; return left>=right;
}
float lookupInput(int selection, vec3 normal, vec3 tangent, vec3 view, vec3 light, vec3 halfVector) {
    if(selection==0) return dot(normal,halfVector);
    if(selection==1) return dot(view,halfVector);
    if(selection==2) return dot(normal,view);
    if(selection==3) return dot(light,normal);
    if(selection==4) return 0.0;
    return dot(tangent,normalizedVector(halfVector-normal*dot(normal,halfVector)));
}
float lightingLookup(int samplerIndex, int inputShift, vec3 normal, vec3 tangent,
                     vec3 view, vec3 light, vec3 halfVector) {
    int selection=field(registerValue(0x1d1),inputShift,3);
    float argument=lookupInput(selection,normal,tangent,view,light,halfVector);
    bool absoluteInput=field(registerValue(0x1d0),inputShift+1,1)==0;
    float coordinate=absoluteInput ? clamp(abs(argument),0.0,1.0)*256.0
                                   : clamp(argument,-1.0,1.0)*128.0;
    int integral=int(floor(coordinate));
    int index=absoluteInput ? clamp(integral,0,255) : (clamp(integral,-128,127)&255);
    vec2 entry=texelFetch(uLightingLookup,ivec2(index,samplerIndex),0).rg;
    int scale=field(registerValue(0x1d2),inputShift,3);
    float multiplier=scale<4 ? float(1<<scale) : (scale==6 ? 0.25 : (scale==7 ? 0.5 : 1.0));
    return max(0.0,entry.x+entry.y*fract(coordinate))*multiplier;
}
void lightingColors(ivec4 textureColors[3], out ivec4 primary, out ivec4 secondary) {
    primary=ivec4(0); secondary=ivec4(0);
    if(field(registerValue(0x1c6),0,1)!=0) return;
    uint configuration=registerValue(0x1c3);
    uint disabled=registerValue(0x1c4);
    vec4 quaternion=normalize(vQuaternion);
    vec3 mappedNormal=vec3(0.0,0.0,1.0);
    vec3 mappedTangent=vec3(1.0,0.0,0.0);
    int bumpMode=field(configuration,28,2);
    if(bumpMode!=0) {
        int selector=min(field(configuration,22,2),2);
        vec3 bump=vec3(textureColors[selector].rgb)/255.0*2.0-1.0;
        if(bumpMode==1) {
            mappedNormal=bump;
            if(field(configuration,30,1)==0)
                mappedNormal.z=sqrt(max(0.0,1.0-dot(bump.xy,bump.xy)));
        } else mappedTangent=bump;
    }
    vec3 normal=rotateQuaternion(quaternion,mappedNormal);
    vec3 tangent=rotateQuaternion(quaternion,mappedTangent);
    vec3 view=normalizedVector(vView);
    vec3 diffuseTotal=lightColor(registerValue(0x1c0));
    vec3 specularTotal=vec3(0.0);
    float primaryAlpha=1.0, secondaryAlpha=1.0;
    int lightCount=field(registerValue(0x1c2),0,3)+1;
    for(int slot=0;slot<8;++slot) {
        if(slot>=lightCount) break;
        int index=field(registerValue(0x1d9),slot*4,3);
        int base=0x140+index*16;
        vec3 position=vec3(unpackHalf2x16(registerValue(base+4)),unpackHalf2x16(registerValue(base+5)).x);
        uint lightConfiguration=registerValue(base+9);
        vec3 light=normalizedVector(field(lightConfiguration,0,1)!=0 ? position : position+vView);
        vec3 halfVector=normalizedVector(light+view);
        float diffuse=dot(normal,light);
        diffuse=field(lightConfiguration,1,1)!=0 ? abs(diffuse) : max(diffuse,0.0);
        diffuseTotal+=lightColor(registerValue(base+3))+lightColor(registerValue(base+2))*diffuse;
        float distributionZero=field(disabled,16,1)!=0 ? 1.0 : lightingLookup(0,0,normal,tangent,view,light,halfVector);
        float distributionOne=field(disabled,17,1)!=0 ? 1.0 : lightingLookup(1,4,normal,tangent,view,light,halfVector);
        vec3 reflectance=vec3(1.0);
        if(field(disabled,20,1)==0) reflectance.r=lightingLookup(6,24,normal,tangent,view,light,halfVector);
        if(field(disabled,21,1)==0) reflectance.g=lightingLookup(5,20,normal,tangent,view,light,halfVector);
        if(field(disabled,22,1)==0) reflectance.b=lightingLookup(4,16,normal,tangent,view,light,halfVector);
        float highlight=field(configuration,27,1)!=0 && diffuse<=0.0 ? 0.0 : 1.0;
        specularTotal+=highlight*(lightColor(registerValue(base))*distributionZero+
                                  lightColor(registerValue(base+1))*distributionOne*reflectance);
        if(slot==lightCount-1 && field(disabled,19,1)==0) {
            float fresnel=lightingLookup(3,12,normal,tangent,view,light,halfVector);
            if(field(configuration,2,1)!=0) primaryAlpha=fresnel;
            if(field(configuration,3,1)!=0) secondaryAlpha=fresnel;
        }
    }
    primary=quantizeColor(vec4(diffuseTotal,primaryAlpha));
    secondary=quantizeColor(vec4(specularTotal,secondaryAlpha));
}
ivec4 combinerSource(int selection, ivec4 primary, ivec4 lightingPrimary,
                     ivec4 lightingSecondary, ivec4 textures[3], ivec4 buffer,
                     ivec4 constantColor, ivec4 previous) {
    if(selection==0) return primary; if(selection==1) return lightingPrimary;
    if(selection==2) return lightingSecondary;
    if(selection>=3 && selection<=5) return textures[selection-3];
    if(selection==13) return buffer; if(selection==14) return constantColor;
    if(selection==15) return previous; return ivec4(0);
}
ivec3 colorModifier(ivec4 value, int modifier) {
    if(modifier==0) return value.rgb; if(modifier==1) return 255-value.rgb;
    int component=modifier<4 ? value.a : (modifier<8 ? value.r : (modifier<12 ? value.g : value.b));
    return ivec3((modifier&1)==0 ? component : 255-component);
}
int alphaModifier(ivec4 value, int modifier) {
    int component=modifier<2 ? value.a : (modifier<4 ? value.r : (modifier<6 ? value.g : value.b));
    return (modifier&1)==0 ? component : 255-component;
}
ivec3 combineColor(int operation, ivec3 first, ivec3 second, ivec3 third) {
    if(operation==0) return first;
    if(operation==1) return first*second/255;
    if(operation==2) return min(first+second,ivec3(255));
    if(operation==3) return clamp(first+second-128,ivec3(0),ivec3(255));
    if(operation==4) return (first*third+second*(255-third))/255;
    if(operation==5) return max(first-second,ivec3(0));
    if(operation==6 || operation==7) {
        ivec3 products=(first-128)*(second-128);
        return ivec3(clamp(4*(products.r+products.g+products.b)/255,0,255));
    }
    if(operation==8) return min(first*second/255+third,ivec3(255));
    return min(first+second,ivec3(255))*third/255;
}
int combineAlpha(int operation, int first, int second, int third) {
    if(operation==0) return first; if(operation==1) return first*second/255;
    if(operation==2) return min(first+second,255); if(operation==3) return clamp(first+second-128,0,255);
    if(operation==4) return (first*third+second*(255-third))/255;
    if(operation==5) return max(first-second,0); if(operation==8) return min(first*second/255+third,255);
    if(operation==9) return min(first+second,255)*third/255; return 0;
}
void main() {
    int scissorMode=field(registerValue(0x65),0,2);
    ivec2 scissorMinimum=ivec2(field(registerValue(0x66),0,10),field(registerValue(0x66),16,10));
    ivec2 scissorMaximum=ivec2(field(registerValue(0x67),0,10),field(registerValue(0x67),16,10));
    ivec2 pixel=ivec2(gl_FragCoord.xy);
    bool inScissor=all(greaterThanEqual(pixel,scissorMinimum))&&all(lessThanEqual(pixel,scissorMaximum));
    if((scissorMode==1&&inScissor)||(scissorMode==3&&!inScissor)) discard;
    int enabled=field(registerValue(0x80),0,3);
    vec2 coordinateZero=vTextureZero.xy;
    if(field(registerValue(0x83),28,3)==3) coordinateZero/=vTextureZero.z;
    vec2 coordinateOne=vTextureOneTwo.xy;
    vec2 coordinateTwo=field(registerValue(0x80),13,1)!=0 ? coordinateOne : vTextureOneTwo.zw;
    vec2 coordinates[3]=vec2[3](vec2(coordinateZero.x,1.0-coordinateZero.y),
                              vec2(coordinateOne.x,1.0-coordinateOne.y),vec2(coordinateTwo.x,1.0-coordinateTwo.y));
    vec4 sampled[3]=vec4[3](texture(uTextureZero,coordinates[0]),texture(uTextureOne,coordinates[1]),texture(uTextureTwo,coordinates[2]));
    if(uSeparateAlpha.x!=0) sampled[0].a=texture(uAlphaZero,coordinates[0]).r;
    if(uSeparateAlpha.y!=0) sampled[1].a=texture(uAlphaOne,coordinates[1]).r;
    if(uSeparateAlpha.z!=0) sampled[2].a=texture(uAlphaTwo,coordinates[2]).r;
    ivec4 textures[3];
    for(int index=0;index<3;++index) textures[index]=(enabled&(1<<index))!=0 ? quantizeColor(sampled[index]) : ivec4(0);
    if(field(registerValue(0x83),28,3)==5) textures[0]=ivec4(0);
    ivec4 primary=quantizeColor(vColor), lightingPrimary, lightingSecondary;
    lightingColors(textures,lightingPrimary,lightingSecondary);
    ivec4 buffer=colorBytes(registerValue(0xfd)), pendingBuffer=buffer;
    ivec4 previous=ivec4(0);
    int bases[6]=int[6](0xc0,0xc8,0xd0,0xd8,0xf0,0xf8);
    for(int stage=0;stage<6;++stage) {
        int base=bases[stage]; uint sources=registerValue(base),modifiers=registerValue(base+1),operations=registerValue(base+2);
        ivec4 constantColor=colorBytes(registerValue(base+3));
        ivec3 colors[3]; int alphas[3];
        for(int operand=0;operand<3;++operand) {
            colors[operand]=colorModifier(combinerSource(field(sources,operand*4,4),primary,lightingPrimary,lightingSecondary,textures,buffer,constantColor,previous),field(modifiers,operand*4,4));
            alphas[operand]=alphaModifier(combinerSource(field(sources,16+operand*4,4),primary,lightingPrimary,lightingSecondary,textures,buffer,constantColor,previous),field(modifiers,12+operand*4,3));
        }
        ivec3 rgb=combineColor(field(operations,0,4),colors[0],colors[1],colors[2]);
        int alpha=field(operations,0,4)==7 ? rgb.r : combineAlpha(field(operations,16,4),alphas[0],alphas[1],alphas[2]);
        uint scales=registerValue(base+4);
        previous=clamp(ivec4(rgb*(1<<min(field(scales,0,2),2)),alpha*(1<<min(field(scales,16,2),2))),ivec4(0),ivec4(255));
        buffer=pendingBuffer;
        if(stage<4&&field(registerValue(0xe0),8+stage,1)!=0) pendingBuffer.rgb=previous.rgb;
        if(stage<4&&field(registerValue(0xe0),12+stage,1)!=0) pendingBuffer.a=previous.a;
    }
    uint alphaTest=registerValue(0x104);
    if(field(alphaTest,0,1)!=0&&!compareValue(field(alphaTest,4,3),previous.a,field(alphaTest,8,8))) discard;
    float depth=clamp((gl_FragCoord.z-1.0)*uDepthConfiguration.x+uDepthConfiguration.y,0.0,1.0);
    if(field(registerValue(0x6d),0,1)==0) depth=clamp(depth/gl_FragCoord.w,0.0,1.0);
    if(field(registerValue(0xe0),0,3)==5) {
        float coordinate=(field(registerValue(0xe0),16,1)!=0 ? 1.0-depth : depth)*128.0;
        int index=clamp(int(floor(coordinate)),0,127);
        vec2 entry=texelFetch(uFogLookup,ivec2(index,0),0).rg;
        float factor=clamp(entry.x+entry.y*fract(coordinate),0.0,1.0);
        vec3 fogColor=vec3(colorBytes(registerValue(0xe1)).rgb);
        previous.rgb=ivec3(clamp(vec3(previous.rgb)*factor+fogColor*(1.0-factor),0.0,255.0));
    }
    if(field(registerValue(0x100),8,1)==0&&field(registerValue(0x102),0,4)==0) previous=ivec4(0);
    gl_FragDepth=depth;
    fragmentColor=vec4(previous)/255.0;
}`,
};
