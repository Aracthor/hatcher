#version 300 es

layout(location = 0) in vec3 vp;

uniform mat4 uniViewMatrix;

void main()
{
    gl_Position = uniViewMatrix * vec4(vp, 1.0);
}
