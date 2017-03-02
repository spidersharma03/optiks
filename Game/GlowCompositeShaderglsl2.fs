varying vec2 texCoordVar;               // vertex texture coordinate varying
varying vec4 colorVar;                 // vertex color varying

uniform sampler2D TextureSampler;
uniform sampler2D GlowSampler;
uniform sampler2D SmokeSampler;
uniform sampler2D TexCoordSampler;

uniform float AvgLighting;
uniform float time;

uniform vec2 waveCentre;

void main(void)
{
    vec2 texCoordSample = texture2D( TexCoordSampler, texCoordVar ).st;
    
    vec2 offset = vec2(0.005 ,0.005);
    
    float scale = 2.0;
	vec4 texture1 = texture2D( TextureSampler, texCoordSample ) * 0.5 * scale;
	
    vec4 glow = texture2D( GlowSampler, texCoordSample);
    
    vec4 reflection = texture2D( SmokeSampler, texCoordSample + offset * 3.0) * 0.15;
    
    float avgGlow = (glow.x + glow.y + glow.z)/3.0;
    
	gl_FragColor = vec4( texture1 *  0.2 + texture1 * avgGlow * 3.0  + texture1 * glow * 4.0 + reflection);
}
