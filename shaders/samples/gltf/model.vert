#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texCoord0;
layout(location = 4) in vec2 texCoord1;
layout(location = 5) in vec4 jointIndices;
layout(location = 6) in vec4 jointWeights;

layout(location = 0) out vec3 fragColor;

layout(push_constant) uniform MvpPushConstant {
    mat4 mvp; // projection * view * model
} pushMvp;

void main() {
    gl_Position = pushMvp.mvp * vec4(position, 1.0);
    fragColor = color;
}
