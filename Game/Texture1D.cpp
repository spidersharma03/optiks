
#ifndef IOS

#include "Texture1D.h"
#include "Image.h"

#ifndef IOS
    #include <OpenGL/gl.h>
#endif

#ifdef IOS
    #include <OpenGLES/ES2/gl.h>
#endif

Texture1D::Texture1D(Image* image)
:Texture(image->getWidth(),0)
{
	type = ETT_TEXTURE_1D;
	boundaryWidth = 0;
	this->image = image;
	if( image &&  image->getData() != 0 )
	{
		if( image->getFormat() == EIF_RGB )
		{
			glGenTextures (1, &textureName);
			glBindTexture(GL_TEXTURE_1D,textureName);
			
			// set texture parameters
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			
			glTexImage1D(GL_TEXTURE_1D,0,GL_RGB, width, 0, GL_RGB, GL_UNSIGNED_BYTE, this->image->getData());
			
			glBindTexture(GL_TEXTURE_1D,0);
		}
		if( image->getFormat() == EIF_RGBA )
		{
			glGenTextures (1, &textureName);
			glBindTexture(GL_TEXTURE_1D,textureName);
			
			// set texture parameters
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			
            glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, image->getWidth(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image->getData());
			
			glBindTexture(GL_TEXTURE_1D,0);
		}
	}
	else {
		glGenTextures (1, &textureName);
		glBindTexture(GL_TEXTURE_1D,textureName);
		
		// set texture parameters
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, width, 0, GL_RGBA, GL_FLOAT, 0);
		glBindTexture(GL_TEXTURE_1D,0);
	}
    
	//m_vecMipMapImages.resize(12);
}


Image* Texture1D::getImage()
{
	return image;
}

#endif
