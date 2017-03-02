//
//  Shader.fsh
//  Helios
//
//  Created by Prashant on 25/01/14.
//
//

varying mediump vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
