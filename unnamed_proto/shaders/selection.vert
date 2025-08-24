#version 300 es

in vec3 vertPosition;

uniform mat4 uniProjectionViewMatrix;
uniform mat4 uniModelMatrix;

out float altitude;

void main()
{
    altitude = vertPosition.z;
    gl_Position = uniProjectionViewMatrix * uniModelMatrix * vec4(vertPosition, 1.0);
}
