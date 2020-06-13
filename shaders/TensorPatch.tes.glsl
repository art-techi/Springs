#version 410 core

layout (quads, equal_spacing, cw) in;

#define UNIFORM_BINDING 1
#include "UniformMatrices.glsl"

out TessOut
{
    vec3 normal;
} tessOut;


vec4 quadraticBezier(vec4 A, vec4 B, vec4 C, float t)
{
    vec4 D = mix(A, B, t);
    vec4 E = mix(B, C, t);

    return mix(D, E, t);
}

vec4 cubicBezier(vec4 A, vec4 B, vec4 C, vec4 D, float t)
{
    vec4 E = mix(A, B, t);
    vec4 F = mix(B, C, t);
    vec4 G = mix(C, D, t);

    return quadraticBezier(E, F, G, t);
}

vec4 evaluatePatch(vec2 at)
{
    vec4 P[4];
    int i;

    for (i = 0; i < 4; i++)
    {
        P[i] = cubicBezier(
                gl_in[i + 0].gl_Position,
                gl_in[i + 4].gl_Position,
                gl_in[i + 8].gl_Position,
                gl_in[i + 12].gl_Position,
                at.y);
    }

    return cubicBezier(P[0], P[1], P[2], P[3], at.x);
}

const float epsilon = 0.001;

void main()
{
    vec4 p1 = evaluatePatch(gl_TessCoord.xy);
    vec4 p2 = evaluatePatch(gl_TessCoord.xy + vec2(0.0, epsilon));
    vec4 p3 = evaluatePatch(gl_TessCoord.xy + vec2(epsilon, 0.0));

    vec3 v1 = normalize(p2.xyz - p1.xyz);
    vec3 v2 = normalize(p3.xyz - p1.xyz);

    tessOut.normal = cross(v1, v2);

    gl_Position = projection * p1;
}
