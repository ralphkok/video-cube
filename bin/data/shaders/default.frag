#version 150

uniform sampler2DRect uTexture;
uniform vec2 uTextureSize;
uniform vec2 uOutputResolution;
uniform float uThreshold = 0.0;

in vec2 vUV;

out vec4 outputColor;

void main()
{
    vec2 uv = gl_FragCoord.xy / uOutputResolution;
    vec4 color = texture(uTexture, uv * uTextureSize);
    float luminance = 0.299 * color.r + 0.587 * color.g + 0.114 * color.b;
    outputColor = vec4(color.rgb, color.a * smoothstep(uThreshold, 1.0, luminance));
}
