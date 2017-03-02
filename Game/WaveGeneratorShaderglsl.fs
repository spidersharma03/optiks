varying vec2 texCoordVar;               // vertex texture coordinate varying
varying vec4 colorVar;                 // vertex color varying

uniform float time;
uniform vec2 waveCentre;

float frequency = 60.0;
float Amplitude = 0.003;
float kmag = 100.0;


void main(void)
{
    vec2 d1;// = vec2(0.0,0.0);
    
    d1 = texCoordVar - waveCentre;
    float distance = length(d1);
    
    vec2 k1 = normalize(d1);
    
    float y1 = Amplitude * sin( dot(k1,d1) * kmag - frequency * time);// * exp(-5.0*length(d1));
    
    vec2 uvNew = texCoordVar;
    float waveSpeed = frequency/kmag;
    
    if( time > 0.0 &&  distance < waveSpeed * time )
        uvNew += k1 * y1;
    
	gl_FragColor = vec4( uvNew.x, uvNew.y, 0.0, 1.0);
}


