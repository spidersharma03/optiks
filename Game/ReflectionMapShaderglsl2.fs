varying vec2 texCoordVar;               // vertex texture coordinate varying
//varying vec4 colorVar;                 // vertex color varying

uniform sampler2D TextureSampler;

uniform float w;
uniform float h;

void main(void)
{
    vec2 fragCoord = gl_FragCoord.xy;
    fragCoord.x /= w;
    fragCoord.y /= h;
    vec2 ds = vec2(1.0/w, 1.0/h);

    
    vec4 texture = texture2D( TextureSampler, texCoordVar );
            	
    float alpha = texture.w;
	
    //gl_FragColor = texture1 * ( 1.0 - alpha ) + superEllipseTexture * alpha + reflectionColor * 0.0 ;
    
    //gl_FragColor = texture1 * ( 1.0 - alpha ) + superEllipseTexture * alpha + reflectionColor * 10.0 * alpha;
    
    gl_FragColor = texture;
}