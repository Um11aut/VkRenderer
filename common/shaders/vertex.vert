#version 450

#extension GL_KHR_vulkan_glsl : enable

layout(location = 0) out vec3 fragColor;

layout(location = 0) in vec2 inPosition;

vec3 colors[3] = vec3[](
    vec3(1.0, 0.0, 1.0),
    vec3(1.0, 1.0, 1.0),
    vec3(1.0, 1.0, 1.0)
);

void main() {
    gl_Position = vec4(inPosition, 0.0, 1.0);
    fragColor = colors[gl_VertexIndex];
}