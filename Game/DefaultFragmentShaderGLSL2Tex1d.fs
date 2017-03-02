uniform sampler1D TextureSampler;      // shader texture uniform

varying float texCoordVar;       // fragment texture coordinate varying
varying vec4 colorVar;                 // vertex color varying

void main()
{
    // sample the texture at the interpolated texture coordinate
    // and write it to gl_FragColor
    gl_FragColor = colorVar * texture1D( TextureSampler, texCoordVar);
}