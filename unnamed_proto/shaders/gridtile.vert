#version 300 es

in vec2 vertPosition;

uniform mat4 uniProjectionMatrix;
uniform mat4 uniModelMatrix;
uniform mat4 uniViewMatrix;

out vec2 textureCoord;
out vec3 normal;

void main()
{
    vec4 worldPosition = uniModelMatrix * vec4(vertPosition, 0.0, 1.0);
    gl_Position = uniProjectionMatrix * uniViewMatrix * worldPosition;
    textureCoord = worldPosition.xy;
    normal = vec3(0.0, 0.0, 1.0);
}
