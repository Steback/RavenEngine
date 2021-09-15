#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv0;
layout(location = 3) in vec2 uv1;


layout (set = 0, binding = 0) uniform UBO
{
	mat4 projection;
	mat4 model;
} ubo;

layout (location = 0) out vec3 outUVW;

void main() 
{
	outUVW = position;
	// Convert cubemap coordinates into Vulkan coordinate space
	outUVW.xy *= -1.0;
	gl_Position = ubo.projection * ubo.model * vec4(position.xyz, 1.0);
}
