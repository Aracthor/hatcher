#version 300 es

in vec2 vertPosition;
in vec2 vertTextureCoord;

uniform mat4 uniProjectionViewMatrix;
uniform mat4 uniModelMatrix;

out vec2 textureCoord;

void main()
{
    gl_Position = uniProjectionViewMatrix * uniModelMatrix * vec4(vertPosition, 0.0, 1.0);
    textureCoord = vertTextureCoord;
}
