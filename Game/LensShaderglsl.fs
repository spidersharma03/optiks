varying vec2 texCoordVar;

uniform float CentreX;
uniform float CentreY;

uniform sampler2D TextureSampler;
uniform sampler2D NormalMap;
uniform sampler2D ReflectionMap;
uniform sampler2D SuperEllipseMap;
//uniform sampler2D TextureSampler2;
//uniform sampler2D TextureSampler3;
//uniform sampler2D TextureSampler4;
//uniform sampler2D TextureSampler5;
//uniform sampler2D TextureSampler6;
//uniform sampler2D TextureSampler7;
//uniform sampler2D TextureSampler8;
uniform float w;
uniform float h;

void main(void)
{
    float scale = 1.9;
    vec2 fragCoord = gl_FragCoord.xy;
    fragCoord.x /= w;
    fragCoord.y /= h;
    vec2 ds = vec2(1.0/w, 1.0/h);

    
    vec3 Normal = texture2D( NormalMap, texCoordVar ).rgb;
    vec4 superEllipseTexture = texture2D( SuperEllipseMap, texCoordVar );

    Normal.x = 2.0 * Normal.x - 1.0;
    Normal.y = 2.0 * Normal.y - 1.0;
    Normal.z = 2.0 * Normal.z - 1.0;
    
    Normal = normalize(Normal);
   
    //vec3 reflectedRay = reflect( vec3(fragCoord.x/10.0,fragCoord.y/10.0,-1.0), Normal);
    vec3 refractedRay = refract( vec3(0.0,0.0,-1.0), Normal, 1.9);
    
    //reflectedRay = normalize(reflectedRay);
    
    //float rz = reflectedRay.z + 1.0;
    //float m = sqrt( reflectedRay.x * reflectedRay.x + reflectedRay.y * reflectedRay.y  + rz*rz);
    //vec2 uvrefl = vec2(0.0,0.0);
    //uvrefl.xy = reflectedRay.xy/m + 0.5;
    
    vec4 reflectionColor = texture2D( ReflectionMap, fragCoord ).rgba;

    vec2 Centre = vec2(CentreX/w, CentreY/h);
    //vec2 d = abs(fragCoord - Centre)*0.2;
    vec2 d = (fragCoord)*0.2;
    d.xy = refractedRay.xy * 0.05;
    
    
    //d *= d;
    //d = pow( d, vec2(13.0/14.0, 12.0/13.0) );
	vec4 texture1 = texture2D( TextureSampler, vec2(fragCoord.x + ds.x, fragCoord.y) + d ) * 0.5 * scale;
    //vec4 texture2 = texture2D( TextureSampler, vec2(fragCoord.x - ds.x, fragCoord.y) + d ) * 0.5 * scale;
	//vec4 texture3 = texture2D( TextureSampler, vec2(fragCoord.x, fragCoord.y + ds.y) + d ) * 0.5 * scale;
	//vec4 texture4 = texture2D( TextureSampler, vec2(fragCoord.x, fragCoord.y - ds.y) + d ) * 0.5 * scale;
    
    float alpha = superEllipseTexture.a;
	//vec4 texture5 = texture2D( TextureSampler5, uv ) * 0.0625 * scale;
	//vec4 texture6 = texture2D( TextureSampler6, uv ) * 0.03125 * scale;
	//vec4 texture7 = texture2D( TextureSampler7, uv ) * 0.15625 * scale;
	//vec4 texture8 = texture2D( TextureSampler8, uv ) * 0.078125 * scale;
    //vec4 color = dFdx(texture1);
    vec4 color = vec4(0.0,0.0,0.0,1.0);
    //color += ( texture1 + texture2 + texture3 + texture4 ) * 0.25;
	
    //gl_FragColor = texture1 * ( 1.0 - alpha ) + superEllipseTexture * alpha + reflectionColor * 0.0 ;
    
    //gl_FragColor = texture1 * ( 1.0 - alpha ) + superEllipseTexture * alpha + reflectionColor * 10.0 * alpha;
    color = texture1 * ( 1.0 - alpha ) + superEllipseTexture * alpha + reflectionColor * 6.0 * alpha;
    
    gl_FragColor = color;
    //vec4(  vec4(1.0,1.0,1.0,1.0)*color );// + texture2 + texture3 + texture4 );
    // + texture2 + texture3 + texture4 + 
                         //texture5 + texture6 + texture7 + texture8 );	
}