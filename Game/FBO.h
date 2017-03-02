/*
 *  FBO.h
 *  FluidSimulationGPU
 *
 *  Created by Conf on 28/07/11.
 *
 */
#ifndef _FBO_H_
#define _FBO_H_

enum BUFFER_ATTACHMENT_TYPE { EBAT_COLOR, EBAT_DEPTH, EBAT_STENCIL };
enum COLOR_ATTACHMENT_POINT {ECAP0, ECAP1, ECAP2, ECAP3 }; 

class Texture;

//namespace spider3d {
//    namespace render
//    {
//        class Renderer;
//    }
//}
//using namespace spider3d::render;

class FrameBufferObject
{
public:
	
public:
	FrameBufferObject(unsigned width, unsigned height);
	Texture* getFboTexture();
	void attachTexture(Texture*, COLOR_ATTACHMENT_POINT colorAttchment = ECAP0);
	void setColorAttachmentPoint(COLOR_ATTACHMENT_POINT colorAttachment);
	void setBufferAttachmentType(BUFFER_ATTACHMENT_TYPE);
	void attachMRTArray(int, COLOR_ATTACHMENT_POINT*);
	void getFrameBufferObjectName(unsigned*);
	virtual void onSet() = 0;
	virtual void unSet() = 0;
protected:
	unsigned frameBufferName;
    int oldFramebuffername;
	unsigned depthBufferName;
	unsigned stencilBufferName;
	unsigned colorBuferName;
	unsigned fboTextureName;
	int      fboHeight, fboWidth;
	bool    bEnable;
	Texture* fboTexture;
	COLOR_ATTACHMENT_POINT m_ColorAttachmentPoint;
	bool m_bMRT_Enabled;
	BUFFER_ATTACHMENT_TYPE m_BufferAttachmentType;
    
    //Renderer *m_pRenderer;
};

class FrameBufferObjectTexture_2D: public FrameBufferObject
{
public:
	FrameBufferObjectTexture_2D(unsigned width, unsigned height, bool depthBuffer = false, bool stencilBuffer = false);
	virtual void onSet();
	virtual void unSet();
};

#endif