//
//  SZGLTextureLoader.h
//  Texture Loading Sample
//
//  Created by numata on 09/08/16.
//  Copyright 2009 Satoshi Numata. All rights reserved.
//

#import <Foundation/Foundation.h>

#ifndef IOS
    #import <GLUT/glUT.h>
#endif

#ifdef IOS
    #import <OpenGLES/ES2/gl.h>
#endif

GLuint SZGLLoadTexture(NSString *imagePath);


