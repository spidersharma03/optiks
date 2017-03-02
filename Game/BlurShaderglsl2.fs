varying vec2 texCoordVar;               // vertex texture coordinate varying
//varying vec4 colorVar;                 // vertex color varying

uniform sampler2D TextureSampler;
uniform float nx;
uniform float ny;

void main(void)
{
	vec4 density = texture2D( TextureSampler, texCoordVar );
	float ds = 1.0/nx;
	float dt = 1.0/ny;
		
	vec4 densityR = texture2D( TextureSampler, vec2(texCoordVar.x + ds, texCoordVar.y) );
	vec4 densityL = texture2D( TextureSampler, vec2(texCoordVar.x - ds, texCoordVar.y) );
	vec4 densityT = texture2D( TextureSampler, vec2(texCoordVar.x, texCoordVar.y + dt) );
	vec4 densityB = texture2D( TextureSampler, vec2(texCoordVar.x, texCoordVar.y - dt) );
	//vec4 densityRT = texture2D( TextureSampler, vec2(texCoordVar.x + ds, texCoordVar.y + dt) );
	//vec4 densityRB = texture2D( TextureSampler, vec2(texCoordVar.x + ds, texCoordVar.y - dt) );
	//vec4 densityLT = texture2D( TextureSampler, vec2(texCoordVar.x - ds, texCoordVar.y + dt) );
	//vec4 densityLB = texture2D( TextureSampler, vec2(texCoordVar.x - ds, texCoordVar.y - dt) );

	vec4 outValue = ( density  + densityR + densityL + densityT + densityB ) *0.2;
     //               + densityRT + densityLT + densityRB + densityLB ) * 0.11111;
    //vec4 outValue = ( density * 0.5 + ( densityR + densityL + densityT + densityB ) * 0.125 );

	gl_FragColor = vec4( outValue );	
}