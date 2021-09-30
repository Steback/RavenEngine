#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

layout(location = 0) out vec3 fragWorlPos;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec2 fragUV;

layout(set = 0, binding = 0) uniform UboTransform {
    mat4 mvp;
    mat4 invTransform;
} uboTransform;

layout(set = 0, binding = 1) uniform UboNode {
    mat4 matrix;
} uboNode;


void main() {
    vec4 locPos = uboTransform.mvp * uboNode.matrix * vec4(position, 1.0);
    gl_Position = locPos;
    fragWorlPos = locPos.xyz / locPos.w;
    fragNormal = mat3(transpose(uboTransform.invTransform)) * normal;
    fragUV = uv;
}
