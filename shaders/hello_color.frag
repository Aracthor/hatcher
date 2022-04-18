#version 300 es

precision mediump float;

uniform vec4 uniColor;

out vec4 frag_colour;

void main()
{
    frag_colour = uniColor;
}
