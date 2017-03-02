varying vec2 texCoordVar;               // vertex texture coordinate varying
varying vec4 colorVar;                 // vertex color varying

uniform float CentreX;
uniform float CentreY;

uniform sampler2D TextureSampler;
uniform sampler2D NormalMap;
uniform sampler2D ReflectionMap;
uniform sampler2D SuperEllipseMap;

uniform float w;
uniform float h;

void main(void)
{
    float scale = 2.0;
    vec2 fragCoord = gl_FragCoord.xy;
    fragCoord.x /= w;
    fragCoord.y /= h;
    vec2 ds = vec2(1.0/w, 1.0/h);

    
    vec3 Normal = texture2D( NormalMap, texCoordVar ).rgb;
    vec4 superEllipseTexture = colorVar * texture2D( SuperEllipseMap, texCoordVar );

    Normal.x = 2.0 * Normal.x - 1.0;
    Normal.y = 2.0 * Normal.y - 1.0;
    Normal.z = 2.0 * Normal.z - 1.0;
    
    Normal = normalize(Normal);
   
    vec3 refractedRay = refract( vec3(0.0,0.0,-1.0), Normal, 1.9);
    vec4 reflectionColor = texture2D( ReflectionMap, fragCoord ).rgba * 6.0;

    vec2 Centre = vec2(CentreX/w, CentreY/h);
    vec2 d = (fragCoord)*0.2;
    d.xy = refractedRay.xy * 0.05;
    
    
    vec4 texture1 = texture2D( TextureSampler, vec2(fragCoord.x, fragCoord.y) + d ) * 0.5 * scale;
    //vec4 texture2 = texture2D( TextureSampler, vec2(fragCoord.x - ds.x, fragCoord.y) + d ) * 0.5 * scale;
	//vec4 texture3 = texture2D( TextureSampler, vec2(fragCoord.x, fragCoord.y + ds.y) + d ) * 0.5 * scale;
	//vec4 texture4 = texture2D( TextureSampler, vec2(fragCoord.x, fragCoord.y - ds.y) + d ) * 0.5 * scale;
    reflectionColor /= (reflectionColor+2.0);
    
    float alpha = superEllipseTexture.a;
	vec4 color = vec4(0.0,0.0,0.0,1.0);
    
    color = texture1 * ( 1.0 - alpha ) + superEllipseTexture * alpha + reflectionColor * alpha;
    
    gl_FragColor = color;
}