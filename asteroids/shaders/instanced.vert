#version 300 es

in vec4 vertPosition;
in mat4 inModelMatrix;

uniform mat4 uniProjectionMatrix;
uniform mat4 uniViewMatrix;

void main()
{
    gl_Position = uniProjectionMatrix * uniViewMatrix * inModelMatrix * vertPosition;
}
