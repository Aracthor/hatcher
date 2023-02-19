#version 300 es

in vec2 vertPosition;

uniform float uniHeight;
uniform mat4 uniProjectionMatrix;
uniform mat4 uniModelMatrix;
uniform mat4 uniViewMatrix;

void main()
{
    gl_Position = uniProjectionMatrix * uniViewMatrix * uniModelMatrix * vec4(vertPosition, uniHeight, 1.0);
}
