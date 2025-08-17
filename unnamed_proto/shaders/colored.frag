#version 300 es

precision mediump float;

in vec3 color;
in vec3 normal;

out vec4 fragColor;

const vec3 lightDir = vec3(-0.3, 0.2, -0.6);

void main()
{
    float cosAngle = dot(normal, -normalize(lightDir));
    float lightPower = cosAngle > 0.f ? cosAngle : 0.f;
    fragColor = vec4(color + color * lightPower, 1.0);
}
