/*
 *  FBO.cpp
 *  FluidSimulationGPU
 *
 *  Created by Conf on 28/07/11.
 *
 */

#include "FBO.h"
#include "Texture2D.h"

#ifndef IOS
    #include <Opengl/gl.h>
#endif

#ifdef IOS
    #include <OpenGLES/ES2/gl.h>
    #include <OpenGLES/ES2/glext.h>
#endif

//#include "Game.h"

#include <cassert>

FrameBufferObject::FrameBufferObject(unsigned width, unsigned height):fboWidth(width),fboHeight(height)
{		
	m_BufferAttachmentType = EBAT_COLOR;
	m_ColorAttachmentPoint = ECAP0;
	m_bMRT_Enabled = false;
	fboTexture = 0;
}

Texture* FrameBufferObject::getFboTexture()
{
	return fboTexture;
}

void FrameBufferObject::setColorAttachmentPoint(COLOR_ATTACHMENT_POINT colorAttachment)
{
	m_ColorAttachmentPoint = colorAttachment;
}

void FrameBufferObject::setBufferAttachmentType(BUFFER_ATTACHMENT_TYPE bufferAttachmentType)
{
	m_BufferAttachmentType = bufferAttachmentType;
}

void FrameBufferObject::attachMRTArray(int arrayLength, COLOR_ATTACHMENT_POINT* bufferArray)
{
#ifndef IOS
	GLenum buffers[4];
	for( int i=0; i<arrayLength; i++ )
		buffers[i] = GL_COLOR_ATTACHMENT0 + bufferArray[i];
	
	m_bMRT_Enabled = true;
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferName);
	glDrawBuffers(arrayLength, buffers);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
}
void FrameBufferObject::attachTexture(Texture* texture, COLOR_ATTACHMENT_POINT colorAttchment)
{
	fboTexture = texture;
	m_ColorAttachmentPoint = colorAttchment;
	GLint col_att_point = GL_COLOR_ATTACHMENT0;
	col_att_point += colorAttchment;
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferName);
	if( m_BufferAttachmentType == EBAT_COLOR)
		glFramebufferTexture2D(GL_FRAMEBUFFER, col_att_point, GL_TEXTURE_2D, *fboTexture->getTextureName(), 0);
	if( m_BufferAttachmentType == EBAT_DEPTH)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, *fboTexture->getTextureName(), 0);
    
	//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void FrameBufferObject::getFrameBufferObjectName(unsigned* fboName)
{
	*fboName = frameBufferName;
}


FrameBufferObjectTexture_2D::FrameBufferObjectTexture_2D(unsigned width, unsigned height, bool depthBuffer, bool stencilBuffer):FrameBufferObject(width,height)
{
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFramebuffername);
    
    GLint oldRBO;
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &oldRBO);
    
	//if( GLEW_ARB_framebuffer_object )
	{
		frameBufferName   = 0;
		depthBufferName   = 0;
		stencilBufferName = 0;
		// create a framebuffer object, you need to delete them when program exits.
		glGenFramebuffers(1, &frameBufferName);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferName);
		
#ifdef  IOS
        //GLuint colorRenderbuffer;
        //glGenRenderbuffers(1, &colorRenderbuffer);
        //glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
        //glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height);
        //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
#endif
		// create a renderbuffer object to store depth info
		// NOTE: A depth renderable image should be attached the FBO for depth test.
		// If we don't attach a depth renderable image to the FBO, then
		// the rendering output will be corrupted because of missing depth test.
		// If you also need stencil test for your rendering, then you must
		// attach additional image to the stencil attachement point, too.
		if(depthBuffer)
		{
			glGenRenderbuffers(1, &depthBufferName);
			glBindRenderbuffer(GL_RENDERBUFFER, depthBufferName);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, fboWidth, fboHeight);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		}
		// attach a texture to FBO color attachement point
		fboTexture = new Texture2D(fboWidth, fboHeight,0);
		//fboTextureUnit = new TextureUnitState(fboTexture);
		//fboTextureUnit->getTextureAttributes()->texture_mode = ETEXM_REPLACE;
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *fboTexture->getTextureName(), 0);
		
		// attach a renderbuffer to depth attachment point
		if( depthBuffer )
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferName);
        
        
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        
        if(status != GL_FRAMEBUFFER_COMPLETE)
        {
            assert(0);
        }
        
		m_BufferAttachmentType = EBAT_COLOR;
        
        glBindFramebuffer(GL_FRAMEBUFFER, oldFramebuffername);
        glBindRenderbuffer(GL_RENDERBUFFER, oldRBO);
	}
}

void FrameBufferObjectTexture_2D::onSet()
{
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFramebuffername);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferName);
	if( !m_bMRT_Enabled )
	{
#ifndef IOS
		glDrawBuffer(GL_COLOR_ATTACHMENT0 + m_ColorAttachmentPoint);
#endif
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_ColorAttachmentPoint, GL_TEXTURE_2D, *fboTexture->getTextureName(), 0);
	}
}

void FrameBufferObjectTexture_2D::unSet()
{
#ifndef IOS
	glDrawBuffer(GL_BACK);
#endif
	glBindFramebuffer(GL_FRAMEBUFFER, oldFramebuffername);
}