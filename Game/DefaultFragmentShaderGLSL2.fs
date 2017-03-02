//precision mediump float;        // set default precision for floats to medium

uniform sampler2D TextureSampler;      // shader texture uniform

varying vec2 texCoordVar;       // fragment texture coordinate varying
varying vec4 colorVar;                 // vertex color varying

void main()
{
    // sample the texture at the interpolated texture coordinate
    // and write it to gl_FragColor
    gl_FragColor = colorVar * texture2D( TextureSampler, texCoordVar);
}