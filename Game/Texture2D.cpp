#include "Texture2D.h"
#include "Image.h"

#ifndef IOS
    #include <OpenGL/gl.h>
#endif

#ifdef IOS
    #include <OpenGLES/EAGL.h>
#endif

Texture2D::Texture2D(unsigned width, unsigned height, Image* image)
:Texture(width,height)
{
	type = ETT_TEXTURE_2D;
	boundaryWidth = 0;
	
	this->image = image;
	if( image &&  image->getData() != 0 )
	{
		if( image->getFormat() == EIF_RGB )
		{
			glGenTextures (1, &textureName);
			glBindTexture(GL_TEXTURE_2D,textureName);
			
			// set texture parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->image->getData());
			
			glBindTexture(GL_TEXTURE_2D,0);
		}
		if( image->getFormat() == EIF_RGBA )
		{
			glGenTextures (1, &textureName);
			glBindTexture(GL_TEXTURE_2D,textureName);
			
			// set texture parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->image->getData());
			
			glBindTexture(GL_TEXTURE_2D,0);
		}
		if( image->getFormat() == EIF_RGB_FLOAT32 )
		{
			glGenTextures (1, &textureName);
			glBindTexture(GL_TEXTURE_2D,textureName);
			
			// set texture parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            
#ifndef IOS
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGB32F_ARB,width,height,0,GL_RGB,GL_FLOAT,this->image->getData());
#endif
#ifdef IOS
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_HALF_FLOAT_OES,this->image->getData());
#endif
			glBindTexture(GL_TEXTURE_2D,0);
		}
		if( image->getFormat() == EIF_RGBA_FLOAT32 )
		{
			glGenTextures (1, &textureName);
			glBindTexture(GL_TEXTURE_2D,textureName);
			
			// set texture parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			
#ifndef IOS
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA32F_ARB,width,height,0,GL_RGBA,GL_FLOAT,this->image->getData());
#endif
#ifdef IOS
            
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_HALF_FLOAT_OES,this->image->getData());
#endif
			glBindTexture(GL_TEXTURE_2D,0);
		}
	}
	else {
		glGenTextures (1, &textureName);
		glBindTexture(GL_TEXTURE_2D,textureName);
		
		// set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		        
		glBindTexture(GL_TEXTURE_2D,0);
	}
    delete image;
	//m_vecMipMapImages.resize(12);
}


Image* Texture2D::getImage()
{
	return image;
}

