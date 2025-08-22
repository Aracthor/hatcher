#version 300 es

precision mediump float;

uniform float uniElapsedTime;

in float altitude;

out vec4 frag_colour;

float lerp(float a, float b, float t)
{
  return (1.0 - t) * a + t * b;
}

void main()
{
    float alphaFromTime = lerp(0.5, 1.0, (cos(uniElapsedTime * 3.0) + 1.0) / 2.0);
    float alphaFromAltitude = 1.0 - altitude * 2.0;
    float alpha = alphaFromTime * alphaFromAltitude;
    frag_colour = vec4(1.0, 1.0, 1.0, alpha);
}
