#version 300 es

precision mediump float;

in vec2 textureCoord;
in vec3 normal;

uniform sampler2D uniTexture;

out vec4 fragColor;

const vec3 lightDir = vec3(-0.3, 0.2, -0.6);

void main()
{
    float cosAngle = dot(normal, -normalize(lightDir));
    float lightPower = cosAngle > 0.0 ? cosAngle : 0.0;
    fragColor = texture(uniTexture, textureCoord);
    fragColor.rgb *= mix(0.5, 1.0, lightPower);
}
