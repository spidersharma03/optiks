#ifndef _TEXTURE1D_H
#define _TEXTURE1D_H

#include "Texture.h"

class Image;

class Texture1D:public Texture
{
public:
	Texture1D(Image* image);
	virtual Image* getImage();
	virtual ~Texture1D()
	{
		//delete image;
	}
private:
	vector<Image*> m_vecMipMapImages;
};

#endif