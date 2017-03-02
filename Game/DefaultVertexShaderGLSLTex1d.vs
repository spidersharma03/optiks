varying float texCoordVar;
varying vec4 colorVar;
//uniform sampler2D DensitySampler;

void main(void)
{   
   texCoordVar = gl_MultiTexCoord0.x;
   colorVar = gl_Color;
   gl_Position = ftransform();
}