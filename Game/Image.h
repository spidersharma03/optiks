#ifndef _IMAGE2D_H
#define _IMAGE2D_H

#include "Rectangle.h"
#include "AABB2.h"

enum IMAGE_FORMAT { EIF_RGB , EIF_RGBA, EIF_RGB_FLOAT32, EIF_RGBA_FLOAT32 };

class Image
{
private:
	unsigned width, height;
	void* data;
	IMAGE_FORMAT format;
	bool m_bIsTransparent;
	int cpp;
public:
	Image(unsigned width, unsigned height);
	Image(unsigned width, unsigned height, IMAGE_FORMAT format );
	unsigned getWidth();
	unsigned getHeight();
	IMAGE_FORMAT getFormat();
	void fillPixels(int cx, int cy, int radius, void* dst, float r=0.0f, float g=0.0f, float b=0.0f);
	void copySubImage( float value, void* dst, Rectangle2i rect);
    void copySubImage( unsigned char* src, const AABB2i& rect);
	void* getData();
	void flipVertical();
	void pad(unsigned int padding , unsigned char r , unsigned char g ,unsigned char b ,unsigned char a);
	bool isTransparent();
    Image*  getScaledImage(float width, float height);
    void    save(const char* imageName);
	virtual ~Image();
};

#endif