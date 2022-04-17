#version 300 es

in vec2 vertPosition;

out vec4 worldPosition;

uniform mat4 uniProjectionMatrix;
uniform mat4 uniModelMatrix;
uniform mat4 uniViewMatrix;

void main()
{
    worldPosition = uniModelMatrix * vec4(vertPosition, 0.0, 1.0);
    gl_Position = uniProjectionMatrix * uniViewMatrix * worldPosition;
}
