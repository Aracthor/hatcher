#version 300 es

in vec3 vertPosition;
in vec3 vertColor;

uniform mat4 uniProjectionMatrix;
uniform mat4 uniModelMatrix;
uniform mat4 uniViewMatrix;

out vec3 color;

void main()
{
    gl_Position = uniProjectionMatrix * uniViewMatrix * uniModelMatrix * vec4(vertPosition, 1.0);
    color = vertColor;
}
