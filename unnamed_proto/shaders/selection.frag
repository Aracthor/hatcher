#version 300 es

precision mediump float;

out vec4 frag_colour;

void main()
{
    const float dash_length = 10.0;
    const float dash_spacing = 10.0;
    bool matchOnX = mod(gl_FragCoord.x, dash_length + dash_spacing) < dash_spacing;
    bool matchOnY = mod(gl_FragCoord.y, dash_length + dash_spacing) < dash_spacing;
    float alpha = (matchOnX == matchOnY) ? 0.0 : 1.0;
    frag_colour = vec4(1.0, 1.0, 1.0, alpha);
}
