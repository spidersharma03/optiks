#ifndef _TEXTURE2D_H
#define _TEXTURE2D_H

#include "Texture.h"

class Image;

class Texture2D:public Texture
{
public:
	Texture2D(unsigned width, unsigned height, Image* image);
	virtual Image* getImage();
	virtual ~Texture2D()
	{
		//delete image;
	}
private:
	vector<Image*> m_vecMipMapImages;
};

#endif