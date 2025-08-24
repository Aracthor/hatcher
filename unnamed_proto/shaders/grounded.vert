#version 300 es

in vec2 vertPosition;

uniform float uniHeight;
uniform mat4 uniProjectionViewMatrix;
uniform mat4 uniModelMatrix;

void main()
{
    gl_Position = uniProjectionViewMatrix * uniModelMatrix * vec4(vertPosition, uniHeight, 1.0);
}
