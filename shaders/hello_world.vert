#version 300 es

in vec3 vertPosition;

uniform mat4 uniProjectionMatrix;
uniform mat4 uniModelMatrix;

void main()
{
    gl_Position = uniProjectionMatrix * uniModelMatrix * vec4(vertPosition, 1.0);
}
