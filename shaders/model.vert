#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv0;
layout(location = 3) in vec2 uv1;

layout(location = 0) out vec3 fragWorldPos;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec2 fragUV0;
layout(location = 3) out vec2 fragUV1;

layout(set = 0, binding = 0) uniform UboModel {
    mat4 mvp; // projection * view * model
    mat4 node; // Node local transform
} uboModel;


void main() {
    vec4 locPos = uboModel.mvp * uboModel.node * vec4(position, 1.0);
    gl_Position = locPos;
    fragUV0 = uv0;
    fragUV1 = uv0;
    fragWorldPos = locPos.xyz / locPos.w;
}
