#version 300 es

in vec3 vertPosition;
in vec2 vertTextureCoord;
in vec3 vertNormal;

uniform mat4 uniProjectionViewMatrix;
uniform mat4 uniModelMatrix;

out vec2 textureCoord;
out vec3 normal;

void main()
{
    gl_Position = uniProjectionViewMatrix * uniModelMatrix * vec4(vertPosition, 1.0);
    textureCoord = vertTextureCoord;

    mat4 rotationMatrix = uniModelMatrix;
    rotationMatrix[0] = normalize(rotationMatrix[0]);
    rotationMatrix[1] = normalize(rotationMatrix[1]);
    rotationMatrix[2] = normalize(rotationMatrix[2]);
    rotationMatrix[3] = vec4(0.0, 0.0, 0.0, 1.0);
    normal = (rotationMatrix * vec4(vertNormal, 1.0)).xyz;
}
