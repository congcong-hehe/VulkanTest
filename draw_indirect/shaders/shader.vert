#version 450

// vertex attribute
layout(location = 0) in vec2 inPosition;

// instance attribute
layout(location = 1) in vec2 instancePos;
layout(location = 2) in float instanceScale;
layout(location = 3) in vec3 instanceColor;

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = vec4(inPosition * instanceScale + instancePos, 0.0, 1.0);
    fragColor = instanceColor;
}