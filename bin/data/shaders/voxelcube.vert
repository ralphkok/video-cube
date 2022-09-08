#version 150

uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec2 texcoord;

uniform sampler2DArray uTextures;
uniform vec3 uResolution;
uniform vec3 uSize;
uniform float uVoxelSize;
uniform int uVariableVoxelSize;

out float vVoxelSize;
out vec4 vVoxelColor;

void main()
{
    vec2 uv = texcoord;
    vec2 xy = vec2(uv.x, mod(uv.y, uResolution.y));
    float z = floor(uv.y / uResolution.y);// * uResolution.z;
    
    vec4 color = texture(uTextures, vec3(xy / uResolution.xy, z));
    float luminance = 0.299 * color.r + 0.587 * color.g + 0.114 * color.b;
    
    vVoxelSize = uVoxelSize * (uVariableVoxelSize > 0 ? luminance : 1.0);
    vVoxelColor = vec4(color.rgb, luminance);
    gl_Position = vec4(vec3(xy, z) * uSize / uResolution, 1.0);
}
