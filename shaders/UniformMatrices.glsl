#ifndef SHADERS_UNIFORM_MATRICES_GLSL
#define SHADERS_UNIFORM_MATRICES_GLSL

layout (std140, binding = UNIFORM_BINDING) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

#endif
