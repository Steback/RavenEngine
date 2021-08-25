#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord0;
layout(location = 3) in vec2 texCoord1;
layout(location = 4) in vec4 jointIndices;
layout(location = 5) in vec4 jointWeights;

layout(push_constant) uniform MvpPushConstant {
    mat4 mvp; // projection * view * model
} pushMvp;

void main() {
    gl_Position = pushMvp.mvp * vec4(position, 1.0);
}