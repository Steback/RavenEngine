#version 450


#define MANUAL_SRGB 1


layout(location = 0) out vec4 outColor;

layout(location = 0) in vec2 inUV0;
layout(location = 1) in vec2 inUV1;

layout (set = 0, binding = 0) uniform sampler2D colorMap;
layout (set = 0, binding = 1) uniform sampler2D physicalDescriptorMap;
layout (set = 0, binding = 2) uniform sampler2D normalMap;
layout (set = 0, binding = 3) uniform sampler2D aoMap;
layout (set = 0, binding = 4) uniform sampler2D emissiveMap;

layout (push_constant) uniform Material {
    vec4 baseColorFactor;
    vec4 emissiveFactor;
    vec4 diffuseFactor;
    vec4 specularFactor;
    float workflow;
    int baseColorTextureSet;
    int physicalDescriptorTextureSet;
    int normalTextureSet;
    int occlusionTextureSet;
    int emissiveTextureSet;
    float metallicFactor;
    float roughnessFactor;
    float alphaMask;
    float alphaMaskCutoff;
} material;


vec4 SRGBtoLINEAR(vec4 srgbIn) {
    #ifdef MANUAL_SRGB
    #ifdef SRGB_FAST_APPROXIMATION
    vec3 linOut = pow(srgbIn.xyz,vec3(2.2));
    #else //SRGB_FAST_APPROXIMATION
    vec3 bLess = step(vec3(0.04045),srgbIn.xyz);
    vec3 linOut = mix( srgbIn.xyz/vec3(12.92), pow((srgbIn.xyz+vec3(0.055))/vec3(1.055),vec3(2.4)), bLess );
    #endif //SRGB_FAST_APPROXIMATION
    return vec4(linOut,srgbIn.w);;
#else //MANUAL_SRGB
    return srgbIn;
    #endif //MANUAL_SRGB
}

void main() {
    float perceptualRoughness;
    float metallic;
    vec3 diffuseColor;
    vec4 baseColor;
    vec3 f0 = vec3(0.04);

    if (material.alphaMask == 1.0f) {
        if (material.baseColorTextureSet > -1) {
            baseColor = SRGBtoLINEAR(texture(colorMap, material.baseColorTextureSet == 0 ? inUV0 : inUV1)) * material.baseColorFactor;
        } else {
            baseColor = material.baseColorFactor;
        }
        if (baseColor.a < material.alphaMaskCutoff) {
            discard;
        }
    }

    outColor = vec4(baseColor);
}
