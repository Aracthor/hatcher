#version 300 es

in vec3 vertPosition;
in vec3 vertColor;
in vec3 vertNormal;

uniform mat4 uniProjectionMatrix;
uniform mat4 uniModelMatrix;
uniform mat4 uniViewMatrix;

out vec3 color;
out vec3 normal;

void main()
{
    gl_Position = uniProjectionMatrix * uniViewMatrix * uniModelMatrix * vec4(vertPosition, 1.0);
    color = vertColor;

    mat4 rotationMatrix = uniModelMatrix;
    rotationMatrix[0] = normalize(rotationMatrix[0]);
    rotationMatrix[1] = normalize(rotationMatrix[1]);
    rotationMatrix[2] = normalize(rotationMatrix[2]);
    rotationMatrix[3] = vec4(0.0, 0.0, 0.0, 1.0);
    normal = (rotationMatrix * vec4(vertNormal, 1.0)).xyz;
}
