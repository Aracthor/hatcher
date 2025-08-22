#version 300 es

precision mediump float;

in float altitude;

out vec4 frag_colour;

void main()
{
    float alpha = 1.0 - altitude * 2.0;
    frag_colour = vec4(1.0, 1.0, 1.0, alpha);
}
