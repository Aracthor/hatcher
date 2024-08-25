#version 300 es

in vec3 vertPosition;
in vec2 vertTextureCoord;

uniform mat4 uniProjectionMatrix;
uniform mat4 uniModelMatrix;
uniform mat4 uniViewMatrix;

out vec2 textureCoord;

void main()
{
    gl_Position = uniProjectionMatrix * uniViewMatrix * uniModelMatrix * vec4(vertPosition, 1.0);
    textureCoord = vertTextureCoord;
}
