#version 450


#define MANUAL_SRGB 1


layout(location = 0) out vec4 outColor;

layout(location = 2) in vec2 inUV0;
layout(location = 3) in vec2 inUV1;

layout (set = 1, binding = 0) uniform sampler2D colorMap;

layout (set = 0, binding = 2) uniform UboLight {
    vec3 color;
    float ambient;
} uboLight;

layout (push_constant) uniform Material {
    vec4 baseColorFactor;
    int baseColorTextureSet;
} material;

void main() {
    vec4 baseColor;

    if (material.baseColorTextureSet > -1) {
        baseColor = texture(colorMap, material.baseColorTextureSet == 0 ? inUV0 : inUV1) * material.baseColorFactor;
    } else {
        baseColor = material.baseColorFactor;
    }

    outColor = baseColor * vec4(uboLight.color * uboLight.ambient, 1.0);
}
