#version 300 es

in vec2 vertPosition;

uniform mat4 uniProjectionViewMatrix;
uniform mat4 uniModelMatrix;

void main()
{
    gl_Position = uniProjectionViewMatrix * uniModelMatrix * vec4(vertPosition, 0.0, 1.0);
}
