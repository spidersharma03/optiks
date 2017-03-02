precision mediump float;

varying vec2 texCoordVar;               // vertex texture coordinate varying
varying vec4 colorVar;                 // vertex color varying

uniform sampler2D TextureSampler;
uniform mediump float nx;
uniform mediump float ny;

void main(void)
{
    vec4 density = texture2D( TextureSampler, texCoordVar );
    float ds = 1.0/nx;
    float dt = 1.0/ny;
    
    vec2 texCoordOffsetPosX = vec2(texCoordVar.x + ds, texCoordVar.y);
    vec2 texCoordOffsetNegX = vec2(texCoordVar.x - ds, texCoordVar.y);
    vec2 texCoordOffsetPosY = vec2(texCoordVar.x, texCoordVar.y + dt);
    vec2 texCoordOffsetNegY = vec2(texCoordVar.x, texCoordVar.y - dt);
    
    lowp vec4 densityR = texture2D( TextureSampler, texCoordOffsetPosX );
    lowp vec4 densityL = texture2D( TextureSampler, texCoordOffsetNegX );
    lowp vec4 densityT = texture2D( TextureSampler, texCoordOffsetPosY );
    lowp vec4 densityB = texture2D( TextureSampler, texCoordOffsetNegY );
	//mediump vec4 densityRT = texture2D( TextureSampler, mediump vec2(texCoordVar.x + ds, texCoordVar.y + dt) );
	//mediump vec4 densityRB = texture2D( TextureSampler, mediump vec2(texCoordVar.x + ds, texCoordVar.y - dt) );
	//mediump vec4 densityLT = texture2D( TextureSampler, mediump vec2(texCoordVar.x - ds, texCoordVar.y + dt) );
	//mediump vec4 densityLB = texture2D( TextureSampler, mediump vec2(texCoordVar.x - ds, texCoordVar.y - dt) );

    lowp vec4 outValue = ( density  + densityR + densityL + densityT + densityB )*0.2;
     //               + densityRT + densityLT + densityRB + densityLB ) * 0.11111;
    //vec4 outValue = ( density * 0.5 + ( densityR + densityL + densityT + densityB ) * 0.125 );

	                  

	gl_FragColor = outValue;	
}