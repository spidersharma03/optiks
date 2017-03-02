varying mediump vec2 texCoordVar;               // vertex texture coordinate varying
varying mediump vec4 colorVar;                 // vertex color varying

uniform sampler2D TextureSampler;

uniform mediump float w;
uniform mediump float h;

void main(void)
{
    mediump float scale = 1.9;
    
    mediump vec2 ds = vec2(1.0/w, 1.0/h);

    
    mediump vec4 texture = texture2D( TextureSampler, texCoordVar );
            	
    //mediump float alpha = texture.w;
	
    gl_FragColor = texture;
}