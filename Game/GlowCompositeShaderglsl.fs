
varying vec2 texCoordVar;

uniform sampler2D TextureSampler;
uniform sampler2D GlowSampler;
uniform sampler2D SmokeSampler;

uniform float AvgLighting;
uniform float time;

uniform vec2 waveCentre;



//vec2 K = vec2();
float frequency = 30.0;
float Amplitude = 0.003;
vec2 Centre1 = vec2(0.5,0.5);
vec2 Centre2 = vec2(0.2,0.2);
float kmag = 200.0;


void main(void)
{
    vec2 d1 = vec2(0.0,0.0);
    //vec2 d2 = vec2(0.0,0.0);

    d1 = texCoordVar - waveCentre;
    float distance = length(d1);
    //d2 = uv - Centre2;

    vec2 k1 = normalize(d1);
    //vec2 k2 = normalize(d2);
    //Amplitude *= exp(-3.0*time);
    
    float y1 = Amplitude * sin( dot(k1,d1) * kmag - frequency * time );// * exp(-5.0*length(d1));
    //float y2 = Amplitude * sin( dot(k2,d2) * kmag - frequency * time ) * exp(-1.0*length(d2));

    vec2 uvNew = texCoordVar;
    float waveSpeed = frequency/kmag;
    
    if( time > 0.0 &&  distance < waveSpeed * time )
        uvNew += k1 * y1;
        
    //uvNew += k2 * y2;
    
    vec2 offset = vec2(0.005 ,0.005);
    
    float scale = 2.0;
	vec4 texture1 = texture2D( TextureSampler, uvNew ) * 0.5 * scale;
	
    vec4 glow = texture2D( GlowSampler, uvNew);
    
    vec4 reflection = texture2D( SmokeSampler, uvNew + offset * 3.0) * 0.15;
    
    float avgGlow = (glow.x + glow.y + glow.z)/3.0;
    
    
	gl_FragColor = vec4( texture1 * vec4(1.0,1.0,1.0,0.0) *  0.2 + texture1 * avgGlow * 3.0  + texture1 * glow * 4.0 + reflection);
}



/*
varying vec2 uv;
varying vec3 position;

uniform sampler2D TextureSampler;
uniform sampler2D GlowSampler;
uniform sampler2D SmokeSampler;

uniform float AvgLighting;
uniform float time;

//uniform sampler2D TextureSampler2;
//uniform sampler2D TextureSampler3;
//uniform sampler2D TextureSampler4;
//uniform sampler2D TextureSampler5;
//uniform sampler2D TextureSampler6;
//uniform sampler2D TextureSampler7;
//uniform sampler2D TextureSampler8;

//vec2 K = vec2();
float frequency = 10.0;
float Amplitude = 1.0;
vec2 Centre1 = vec2(0.5,0.5);
vec2 Centre2 = vec2(0.2,0.2);
float kmag = 100.0;

void main(void)
{
    vec2 d1 = vec2(0.0,0.0);
    //vec2 d2 = vec2(0.0,0.0);

    d1 = uv - Centre1;
    float distance = length(d1);
    //d2 = uv - Centre2;

    vec2 k1 = normalize(d1);
    //vec2 k2 = normalize(d2);
    Amplitude *= exp(-2.0*time);
    
    float y1 = Amplitude * sin( dot(k1,d1) * kmag - frequency * time ) * exp(-5.0*length(d1));
    //float y2 = Amplitude * sin( dot(k2,d2) * kmag - frequency * time ) * exp(-1.0*length(d2));

    vec2 uvNew = uv;
    float waveSpeed = frequency/kmag;
    
    if( time > 0.30 &&  distance < waveSpeed * time )
        uvNew += k1 * y1;
        
    //uvNew += k2 * y2;
    
    vec2 offset = vec2(0.005 ,0.005);
    
    float scale = 2.0;
	vec4 texture1 = texture2D( TextureSampler, uvNew ) * 0.5 * scale;
	//vec4 texture2 = texture2D( TextureSampler, uvNew ) * 0.5 * scale;

	//vec4 texture2 = texture2D( TextureSampler2, uv ) * 0.25 * scale;
	//vec4 texture3 = texture2D( TextureSampler3, uv ) * 0.125 * scale;
	//vec4 texture4 = texture2D( TextureSampler4, uv ) * 0.0625 * scale;
    vec4 glow = texture2D( GlowSampler, uvNew);
    
    vec4 reflection = texture2D( SmokeSampler, uvNew + offset * 2.0) * 0.25;
    
    float avgGlow = (glow.x + glow.y + glow.z)/3.0;
	//vec4 texture5 = texture2D( TextureSampler5, uv ) * 0.0625 * scale;
	//vec4 texture6 = texture2D( TextureSampler6, uv ) * 0.03125 * scale;
	//vec4 texture7 = texture2D( TextureSampler7, uv ) * 0.15625 * scale;
	//vec4 texture8 = texture2D( TextureSampler8, uv ) * 0.078125 * scale;
    //vec4 color = dFdx(texture1);
    
    
    
	gl_FragColor = vec4( texture1 * vec4(1.0,0.7,0.0,0.0) *  0.2 + texture1 * avgGlow * 3.0  + texture1 * glow * 4.0 + reflection);
    // + texture2 + texture3 + texture4 );
    // + texture2 + texture3 + texture4 + 
                         //texture5 + texture6 + texture7 + texture8 );	
}

*/
