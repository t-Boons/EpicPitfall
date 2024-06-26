#version 330
uniform sampler2D c;
uniform sampler2D c2;
in vec2 u;
out vec4 color;

uniform float time;
uniform float pixelationAmount;
uniform float brightness;

// Resources for CRT Shader
// https://www.shadertoy.com/view/WsVSzV
vec2 Distort(vec2 centeredUV, float strength)
{
   float distortionAmount = length(centeredUV) * strength;

   vec2 distortedUV = -centeredUV * (distortionAmount + 1.0f);

   return distortedUV * (1.0f - strength * 0.25f);
}

float Vignette(vec2 centeredUV, float strength, float intensity)
{
    
    return -clamp(pow(sin(length(centeredUV) * 3.16f * 0.5f), strength) * intensity, 0, 1);
}

float CRT(vec2 uv)
{
       // CRT Properties.
       float scanLineAmount = 750;
       float scanLineStrength = 0.1f;
       float scanLineBrightnessReduction = 0.1f;
       float scanlineTransposeSpeed = 0.025f;

       // CRT Calculation.
       return abs(sin((uv.y + mod(time, 1.0f) * scanlineTransposeSpeed) * scanLineAmount)) * -scanLineStrength + scanLineBrightnessReduction;
}


vec2 Pixelate(float percentage, vec2 uv)
{
    float multiplier = (1920 * 0.5f) * percentage;

    return vec2(int(uv.x * multiplier) / multiplier, int(uv.y * multiplier) / multiplier);
}

void main()
{
   vec2 cuv = vec2(0.5f - u);

   vec2 distortedUV = Distort(cuv, 0.33f);

    if (distortedUV.y > 0.5f || distortedUV.x < -0.5f || distortedUV.x > 0.5f || distortedUV.y < -0.5f)
    {
       color = vec4(0,0,0,0);
    }
    else
    {
       distortedUV = Pixelate(pixelationAmount, distortedUV);

       color = texture(c, distortedUV + 0.5f);
       color += Vignette(distortedUV, 2.5f, 0.25);
       color += CRT(u);
       color *= vec4(brightness, brightness, brightness, 1.0f);
    }
}