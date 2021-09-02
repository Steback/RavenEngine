#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

layout(location = 0) out vec2 fragUV;

layout(push_constant) uniform PushConstant {
    mat4 mvp; // projection * view * model
    mat4 node; // Node local transform
} push;

void main() {
    gl_Position = pusb.mvp * push.node * vec4(position, 1.0);
    fragUV = uv;
}
