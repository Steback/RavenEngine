#version 450

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec2 inUV;


layout (set = 0, binding = 0) uniform sampler2D colorMap;
layout (set = 0, binding = 1) uniform sampler2D physicalDescriptorMap;
layout (set = 0, binding = 2) uniform sampler2D normalMap;
layout (set = 0, binding = 3) uniform sampler2D aoMap;
layout (set = 0, binding = 4) uniform sampler2D emissiveMap;

void main() {
    outColor = texture(colorMap, inUV);
}
