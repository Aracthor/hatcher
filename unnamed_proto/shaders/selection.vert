#version 300 es

in vec3 vertPosition;

uniform mat4 uniProjectionMatrix;
uniform mat4 uniModelMatrix;
uniform mat4 uniViewMatrix;

out float altitude;

void main()
{
    altitude = vertPosition.z;
    gl_Position = uniProjectionMatrix * uniViewMatrix * uniModelMatrix * vec4(vertPosition, 1.0);
}
