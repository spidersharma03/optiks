
varying vec2 uv;

void main(void)
{
   uv = gl_MultiTexCoord0.xy;
   gl_Position = ftransform();
}