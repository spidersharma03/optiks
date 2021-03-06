precision mediump float;
attribute vec2 position;                // vertex position attribute
attribute vec4 color;                  // vertex color attribute

// every GLSL2 shader must have these matrices passed in as uniforms.
uniform mat4 ModelViewMatrix;                 // shader modelview matrix uniform
uniform mat4 ProjectionMatrix;                // shader projection matrix uniform

varying vec4 colorVar;                 // vertex color varying

void main()
{
    vec4 pos4 = vec4(position.x, position.y, 0.0, 1.0);
    vec4 p = ModelViewMatrix * pos4;      // transform vertex position with modelview matrix
    gl_Position = ProjectionMatrix * p;       // project the transformed position and write it to gl_Position
    gl_PointSize = 1.0;
    colorVar    = color;
}