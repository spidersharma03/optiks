varying vec2 texCoordVar;

uniform sampler2D TextureSampler1;
uniform sampler2D TextureSampler2;
uniform sampler2D TextureSampler3;
uniform sampler2D TextureSampler4;
uniform sampler2D TextureSampler5;
uniform sampler2D TextureSampler6;
uniform sampler2D TextureSampler7;
uniform sampler2D TextureSampler8;

void main(void)
{
    float scale = 0.2;
	vec4 texture1 = texture2D( TextureSampler1, texCoordVar ) * 0.5 * scale;
	vec4 texture2 = texture2D( TextureSampler2, texCoordVar ) * 0.5 * scale;
	vec4 texture3 = texture2D( TextureSampler3, texCoordVar ) * 0.5 * scale;
	vec4 texture4 = texture2D( TextureSampler4, texCoordVar ) * 0.5 * scale;
    
	vec4 texture5 = texture2D( TextureSampler5, texCoordVar ) * 2.0 * scale;
	vec4 texture6 = texture2D( TextureSampler6, texCoordVar ) * 4.0 * scale;
	//vec4 texture7 = texture2D( TextureSampler7, uv ) * 0.15625 * scale;
	//vec4 texture8 = texture2D( TextureSampler8, uv ) * 0.078125 * scale;
    //vec4 color = dFdx(texture1);
	gl_FragColor = vec4( texture1  + texture2 + texture3 + texture4 + texture5 + texture6  );
}