precision mediump float;        // set default precision for floats to medium

varying vec4 colorVar;                 // vertex color varying

void main()
{
    // sample the texture at the interpolated texture coordinate
    // and write it to gl_FragColor
    gl_FragColor = colorVar;
}