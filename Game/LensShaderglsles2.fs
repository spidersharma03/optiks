varying mediump vec2 texCoordVar;               // vertex texture coordinate varying
varying mediump vec4 colorVar;                 // vertex color varying

uniform mediump float CentreX;
uniform mediump float CentreY;

uniform sampler2D TextureSampler;
uniform sampler2D NormalMap;
uniform sampler2D ReflectionMap;
uniform sampler2D SuperEllipseMap;

uniform mediump float w;
uniform mediump float h;

void main(void)
{
    lowp float scale = 2.0;
    mediump vec2 fragCoord = gl_FragCoord.xy;
    fragCoord.x /= w;
    fragCoord.y /= h;
    mediump vec2 ds = 2.0*vec2(1.0/w, 1.0/h);

    mediump vec3 Normal = texture2D( NormalMap, texCoordVar ).rgb;
    mediump vec4 superEllipseTexture = colorVar * texture2D( SuperEllipseMap, texCoordVar );

    Normal.x = 2.0 * Normal.x - 1.0;
    Normal.y = 2.0 * Normal.y - 1.0;
    Normal.z = 2.0 * Normal.z - 1.0;
    
    Normal = normalize(Normal);
   
    mediump vec3 refractedRay = refract( vec3(0.0,0.0,-1.0), Normal, 1.9);
    mediump vec4 reflectionColor = texture2D( ReflectionMap, fragCoord + mediump vec2(ds.x,0.0) ).rgba *6.0;
    mediump vec4 reflectionColor2 = texture2D( ReflectionMap, fragCoord + mediump vec2(-ds.x,0.0)).rgba*6.0;
    mediump vec4 reflectionColor3 = texture2D( ReflectionMap, fragCoord + mediump vec2(0.0,ds.y)).rgba*6.0;
    mediump vec4 reflectionColor4 = texture2D( ReflectionMap, fragCoord + mediump vec2(0.0,-ds.y)).rgba*6.0;

    mediump vec2 Centre = vec2(CentreX/w, CentreY/h);
    mediump vec2 d = (fragCoord)*0.2;
    d.xy = refractedRay.xy * 0.05;
    
  	mediump vec4 texture1 = texture2D( TextureSampler, fragCoord + d ) * 0.5 * scale;
 
    mediump vec4 avgColor = ( reflectionColor + reflectionColor2 + reflectionColor3 + reflectionColor4 ) * 0.25;
    
    mediump float alpha = superEllipseTexture.a;
	mediump vec4 color = vec4(0.0,0.0,0.0,1.0);
    
    //reflectionColor = reflectionColor/(reflectionColor + 0.5);
    avgColor = avgColor/(avgColor + 2.0);
    
    color = texture1 * ( 1.0 - alpha ) + superEllipseTexture * alpha + avgColor * 1.0 * alpha;
    
    gl_FragColor = color;
}