#version 300 es

in vec4 vertPosition;
in mat4 inModelMatrix;

uniform mat4 uniProjectionViewMatrix;

void main()
{
    gl_Position = uniProjectionViewMatrix * inModelMatrix * vertPosition;
}
