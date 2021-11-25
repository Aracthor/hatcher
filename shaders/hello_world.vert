#version 300 es

in vec3 vertPosition;

uniform mat4 uniViewMatrix;
uniform mat4 uniModelMatrix;

void main()
{
    gl_Position = uniViewMatrix * uniModelMatrix * vec4(vertPosition, 1.0);
}
