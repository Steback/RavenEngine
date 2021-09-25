#version 450


#define MANUAL_SRGB 1


layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 inWorlPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV0;
layout(location = 3) in vec2 inUV1;

layout (set = 1, binding = 0) uniform sampler2D colorMap;

layout (set = 0, binding = 2) uniform UboLight {
    vec3 position;
    vec3 color;
    float ambient;
    vec3 viewPosition;
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

    // TODO: Fix bug in light direction
    vec3 ambient = uboLight.ambient * uboLight.color;

    vec3 norm = normalize(inNormal);
    vec3 lightDir = normalize(uboLight.position - inWorlPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * uboLight.color;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(uboLight.viewPosition - inWorlPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    vec3 specular = specularStrength * spec * uboLight.color;

    outColor = baseColor * vec4(ambient + diffuse + specular, 1.0);
}
