
#include "FileLoader.h"
#include "Image.h"
#include "Texture2D.h"
#include <math.h>
#include <fstream>
#include <stdlib.h>
#include "FreeImage.h"

long int getWidthHeight(int pix1, int pix2) {
	long int num=0 ,rem;
	int n = 8;
	do {   //high byte only
		rem = pix2 % 2;
		num = num + rem * (long)(pow((float)2, (float)n));
		pix2 = pix2 /2;
		n++;
	} while(n <= 16);
	num = num + pix1;
	return(num);
}


TextureLoader* TextureLoader::m_pTextureLoader = 0;

TextureLoader::TextureLoader()
{
    FreeImage_Initialise();
}

TextureLoader* TextureLoader::Instance()
{
    if( !m_pTextureLoader )
        m_pTextureLoader = new TextureLoader();
    
    return m_pTextureLoader;
}
		

Texture* TextureLoader::getTexture(const string& fileName)
{
    map<string, Texture*>::iterator it = m_TextureMap.find(fileName);
    
    if( it != m_TextureMap.end() )
    {
        return it->second;
    }
    
    Image* image = loadFile(fileName);
    
    Texture2D* texture = new Texture2D( image->getWidth(), image->getHeight(), image); 
    
    return texture;
}

Image* TextureLoader::loadFile(const string& fileName)
{

	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);
	
	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(fileName.c_str(), 0);
	//if still unknown, try to guess the file format from the file extension
	if(fif == FIF_UNKNOWN) 
		fif = FreeImage_GetFIFFromFilename(fileName.c_str());
	//if still unkown, return failure
	if(fif == FIF_UNKNOWN)
		return 0;
    
	//check that the plugin has reading capabilities and load the file
	if(FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, fileName.c_str());
	//if the image failed to load, return failure
	if(!dib)
		return 0;
    
    // Premultiply alpha to support particle system blending mode.
    FreeImage_PreMultiplyWithAlpha(dib);
	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if((bits == 0) || (width == 0) || (height == 0))
		return 0;
    
    bool bTransparent = FreeImage_IsTransparent(dib);
    
    IMAGE_FORMAT imageFormat = EIF_RGB;

    int numChannels = bTransparent ? 4 : 3;
    imageFormat = bTransparent ? EIF_RGBA : EIF_RGB;
    imageFormat = EIF_RGBA;
//    for(int i = 0;i<width*height*numChannels;i+=numChannels)
//    {
//        BYTE temp = bits[i];
//        bits[i]   = bits[i+2];
//        bits[i+2]   = temp;
//    }
//
    int bpp = FreeImage_GetBPP(dib);
    
    Image* image = new Image(width , height , imageFormat );
    
    memcpy(image->getData() , bits , width*height*4);
    
	//Free FreeImage's copy of the data
	FreeImage_Unload(dib);
	//return success
	return image;    
}