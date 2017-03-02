precision highp float;
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
	
    float ds = 4.0/2048.0; float dt = 4.0/1536.0;
//    vec4 glow1 = texture2D( GlowSampler, texCoordSample + vec2(ds,0.0));
//    vec4 glow2 = texture2D( GlowSampler, texCoordSample + vec2(-ds,0.0));
//    vec4 glow3 = texture2D( GlowSampler, texCoordSample + vec2(0.0,dt));
//    vec4 glow4 = texture2D( GlowSampler, texCoordSample + vec2(0.0,-dt));
    vec4 glow5 = texture2D( GlowSampler, texCoordSample);
    
    vec4 glow = glow5;//( glow1 + glow2 + glow3 + glow4 + glow5 ) * 0.2;
    
    vec4 reflection = texture2D( SmokeSampler, texCoordSample + offset * 3.0) * 0.15;
    
    float avgGlow = (glow.x + glow.y + glow.z)/3.0;
    
	gl_FragColor = vec4( texture1 *  (0.2 + avgGlow * 0.0)  + texture1 * glow * 4.0 + reflection);
}

