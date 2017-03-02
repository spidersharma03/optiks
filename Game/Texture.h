#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "Color4.h"
#include <vector>

using namespace std;
using namespace NAMESPACE_DIRAC2D;

enum TEXTURE_BOUNDARY_MODE { ETEX_BM_CLAMP , ETEX_BM_WRAP, ETEX_BM_CLAMP_TO_EDGE, ETEX_BM_CLAMP_TO_BOUNDARY  }; 

enum TEXTURE_FORMAT { ETF_RGB , ETF_RGBA, ETF_DEPTH};

enum TEXTURE_TYPE { ETT_TEXTURE_1D, ETT_TEXTURE_2D, ETT_TEXTURE_POINT_SPRITE, ETT_TEXTURE_3D, ETT_TEXTURE_CUBE_MAP };

enum TEXTURE_MIN_FILTER { ETMINF_POINT, ETMINF_LINEAR };

enum TEXTURE_MAG_FILTER { ETMAXF_POINT, ETMAXF_LINEAR };

enum MIPMAP_MODE { E_MIPMAP_MODE_BASE_LEVEL, E_MIPMAP_MODE_MULTI_LEVEL };

enum E_TEXTURE_ENV_MODE { E_TEM_MODULATE, E_TEM_REPLACE };

class Image;

class Texture
{
public:
	Texture(unsigned width, unsigned height):width(width),height(height)
	{
		image = 0;
		m_NumMipMaps = 1;
		m_MipMapMode = E_MIPMAP_MODE_BASE_LEVEL;
        m_TexEnvMode = E_TEM_MODULATE;
	}
	
	virtual ~Texture()
	{
	}
	
	TEXTURE_FORMAT getFormat()
	{
		return format;
	}
	
	TEXTURE_BOUNDARY_MODE getBoundaryMode()
	{
		return texBoundaryMode;
	}
	
	TEXTURE_TYPE getTextureType()
	{
		return type;
	}
	
	TEXTURE_MIN_FILTER getMinFilter()
	{
		return minFilter;
	}
	
	TEXTURE_MAG_FILTER getMagFilter()
	{
		return magFilter;
	}
	
    E_TEXTURE_ENV_MODE getTextureEnvMode()  const
    {
        return m_TexEnvMode;
    }
    
	unsigned getWidth()
	{
		return width;
	}
	
	unsigned getHeight()
	{
		return height;
	}
	
	unsigned* getTextureName()
	{
		return &textureName;
	}
	
	Color4f& getBoundaryColor()
	{
		return boundaryColor;
	}
	
	void setBoundaryColor(Color4f& boundaryColor)
	{
		this->boundaryColor = boundaryColor;
	}
	
	unsigned getBoundaryWidth()
	{
		return boundaryWidth;
	}
	
	void setMipMapMode(MIPMAP_MODE mipMapMode)
	{
		m_MipMapMode = mipMapMode;
	}
	
	MIPMAP_MODE getMipMapMode()
	{
		return m_MipMapMode;
	}
	
	unsigned getNumMipMaps()
	{
		return m_NumMipMaps;
	}
	
	virtual Image* getImage() = 0;
		
protected:
	TEXTURE_BOUNDARY_MODE texBoundaryMode;
	TEXTURE_FORMAT format;
	TEXTURE_TYPE type;
	TEXTURE_MIN_FILTER minFilter;
	TEXTURE_MAG_FILTER magFilter;
    E_TEXTURE_ENV_MODE m_TexEnvMode;
	MIPMAP_MODE m_MipMapMode;
	unsigned width, height;
	unsigned boundaryWidth;
	unsigned textureName;
	Image* image;
	int m_NumMipMaps;
	Color4f boundaryColor;
	bool m_bIsMipMapped;
};

#endif