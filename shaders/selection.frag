#version 300 es

precision mediump float;

in vec4 worldPosition;

out vec4 frag_colour;

void main()
{
    const float dash_length = 0.1;
    const float dash_spacing = 0.1;
    bool matchOnX = mod(worldPosition.x, dash_length + dash_spacing) < dash_spacing;
    bool matchOnY = mod(worldPosition.y, dash_length + dash_spacing) < dash_spacing;
    float alpha = (matchOnX == matchOnY) ? 0.0 : 1.0;
    frag_colour = vec4(1.0, 1.0, 1.0, alpha);
}
