uniform sampler2D INITIALPOSITION;
uniform sampler2D INITIALVELOCITY;

varying vec2 uv;
float dt = 0.005;

void main(void)
{
   vec4 cp = texture2D(INITIALPOSITION, uv).rgba;
   vec4 cv = texture2D(INITIALVELOCITY, uv).rgba;
   
   float decay = 1.0;
   cp.a -= decay;
   
    vec3 g = vec3(0.0,-10.0,0.0);
   
    cv.rgb += g * dt;
	cp.rgb += cv.rgb * dt;
    
	float e = 0.98;
	
	if( cp.g < -1.0 )
	{
	  cv.g *= -e;
	  cp.g = -1.0;
	}
	if( cp.a <= 0.0 )
	{
	  cp = vec4(0.0);
	  cp.a = 200.0;
	}
		  
    gl_FragData[0] = vec4(cv);
    gl_FragData[1] = vec4(cp);
}