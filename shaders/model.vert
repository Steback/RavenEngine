#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv0;
layout(location = 3) in vec2 uv1;

layout(location = 0) out vec2 fragUV0;
layout(location = 1) out vec2 fragUV1;

layout(push_constant) uniform PushConstant {
    mat4 mvp; // projection * view * model
    mat4 node; // Node local transform
} push;

const float M_PI = 3.141592653589793;
const float c_MinRoughness = 0.04;

const float PBR_WORKFLOW_METALLIC_ROUGHNESS = 0.0;
const float PBR_WORKFLOW_SPECULAR_GLOSINESS = 1.0f;

void main() {
    gl_Position = push.mvp * push.node * vec4(position, 1.0);
    fragUV0 = uv0;
    fragUV1 = uv0;
}
