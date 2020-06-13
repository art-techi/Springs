#version 410 core

out vec4 fragColour;

in TessOut
{
    vec3 normal;
} tessIn;

void main()
{
    vec3 n = normalize(tessIn.normal);
    vec4 c = vec4(1.0, -1.0, 0.0, 0.0) * n.z + 
        vec4(0.0, 0.0, 0.0, 1.0);

    fragColour = clamp(c, vec4(0.0), vec4(1.0));
}
