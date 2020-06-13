#version 410 core

layout (location = 0) in vec3 position;

#define UNIFORM_BINDING 1
#include "UniformMatrices.glsl"

void main()
{
    gl_Position = view * model * vec4(position, 1.0);
}
