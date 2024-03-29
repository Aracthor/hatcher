#version 300 es

precision mediump float;

in vec2 textureCoord;

uniform sampler2D uniTexture;

out vec4 fragColor;

void main()
{
    fragColor = texture(uniTexture, textureCoord);
}
