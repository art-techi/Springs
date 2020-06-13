#version 410 core

out vec4 fragColour;

uniform vec4 colour;

void main()
{
    fragColour = colour;
}
