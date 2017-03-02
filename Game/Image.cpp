#include "Image.h"
#include <math.h>
#include <string>

#ifndef IOS
#include "FreeImage.h"
#endif

Image::Image(unsigned width, unsigned height ):width(width),height(height),format(EIF_RGB)
{
	data = new unsigned char[width*height*3];
}

Image::Image(unsigned width, unsigned height, IMAGE_FORMAT format ):width(width),height(height),format(format)
{
	cpp = 3;
	switch(format)
	{
		case EIF_RGB:
			data = new unsigned char[width*height*3];
			m_bIsTransparent = false;
			break;
		case EIF_RGBA:
			cpp = 4;
			data = new unsigned char[width*height*4];
			m_bIsTransparent = true;
			break;
		case EIF_RGB_FLOAT32:
			data = new float[width*height*3];
			m_bIsTransparent = true;
			break;
		case EIF_RGBA_FLOAT32:
			cpp = 4;
			data = new float[width*height*4];
			m_bIsTransparent = true;
			break;
		default:
			m_bIsTransparent = false;
			break;
	}
	
	if( format == EIF_RGBA_FLOAT32 || format == EIF_RGB_FLOAT32 ) 
	{
		for( int i=0; i<width*height*cpp; i++ )
		{
			float* d = (float*)data;
			d[i] = 0.0f;//(float)rand()/RAND_MAX;
		}
	}
	else
	{
		for( int i=0; i<width*height*cpp; i++ )
		{
			//float* d = (float*)data;
			//d[i] = 0.0f;//(float)rand()/RAND_MAX;
		}
	}
	
	
	
	//fillPixels(64, 64, 3, this->data);
	//copySubImage(1.0f, this->data, rect);
}

void Image::fillPixels(int cx, int cy, int radius, void* dst, float red, float green, float blue)
{
	
	float* destinationImageData = (float*)dst;
	int count = 0;
	for( int i=0; i<height; i++ )
	{
		for( int j=0; j<cpp*width; j+=4 )
		{
			int dx = fabs( j/cpp - cy );
			int dy = fabs( i - cx );
			int r = dx*dx + dy*dy;
			if( r < radius*radius )
			{
				destinationImageData[count] = red;
				destinationImageData[count+1] = green;
				destinationImageData[count+2] = blue;
			}
			count += cpp;
		}
	}
}

void Image::copySubImage( float value, void* dst, Rectangle2i rect)
{
	int w = rect.endX - rect.startX;
	int h = rect.endY - rect.startY;
	
	float* destinationImageData = (float*)dst;
	
	int count = 0;
	for( int i=0; i<h; i++ )
	{
		count = cpp*((rect.startY+i)*width) + rect.startX*cpp;
		for( int j=0; j<cpp*w; j+=cpp)
		{
			destinationImageData[count]   = 1.0f;
			destinationImageData[count+1] = 1.0f;
			destinationImageData[count+2] = 1.0f;
			destinationImageData[count+3] = 1.0f;
			
			count += cpp;
		}
	}
}

void Image::copySubImage( unsigned char* src, const AABB2i& rect)
{
	int w = rect.m_UpperBounds.x - rect.m_LowerBounds.x;
	int h = rect.m_UpperBounds.y - rect.m_LowerBounds.y;
    
	unsigned char* destinationImageData = (unsigned char*)data;
	
	int count = 0;
    int srcCount = 0;
	for( int i=0; i<h; i++ )
	{
		count = cpp*((rect.m_LowerBounds.y+i)*width) + rect.m_LowerBounds.x*cpp;
		for( int j=0; j<cpp*w; j+=cpp)
		{
            float srcalpha = (float)src[srcCount+3]/255.0f;
            
			destinationImageData[count]   = destinationImageData[count]*(1-srcalpha) + src[srcCount]*srcalpha;
			destinationImageData[count+1] = destinationImageData[count+1]*(1-srcalpha) + src[srcCount+1]*srcalpha;
			destinationImageData[count+2] = destinationImageData[count+2]*(1-srcalpha) + src[srcCount+2]*srcalpha;
			destinationImageData[count+3] = destinationImageData[count+3]*(1-srcalpha) + src[srcCount+3]*srcalpha;
			
            srcCount += 4;
			count += cpp;
		}
	}
}

IMAGE_FORMAT Image::getFormat()
{
	return format;
}

void* Image::getData()
{
	return data;
}

unsigned Image::getWidth()
{
	return width;
}

unsigned Image::getHeight()
{
	return height;
}


void Image::flipVertical()
{
	int numColorComponents = ( format == EIF_RGB ? 3 : 4 );
	if ( data )
	{
		unsigned char* imageData = (unsigned char*)data;
		for ( int j = 0, k = height-1 ; j < height/2, k >= height/2 ; j ++ ,k--)
		{
			for ( int i = 0 ; i < numColorComponents*width ; i++ )
			{
				unsigned char temp = imageData[j*numColorComponents*width + i];
				imageData[j*numColorComponents*width + i] =  imageData[k*numColorComponents*width  + i];
				imageData[k*numColorComponents*width  + i] = temp; 
			}
		}
	}
}

void Image::pad(unsigned int padding , unsigned char r , unsigned char g ,unsigned char b ,unsigned char a)
{
	int numColorComponents = ( format == EIF_RGB ? 3 : 4 );
	
	unsigned char *pixelData = new unsigned char[numColorComponents*width*height];
	unsigned char *imageData = (unsigned char*)data;
	for ( unsigned i = 0 ; i < numColorComponents*width*height; i++ )
		pixelData[i] = imageData[i];
	
	delete[] data;
	data  = new unsigned char[numColorComponents*(width+2*padding)*(height+2*padding) ];
	unsigned char *newimageData = (unsigned char*)data;
	//unsigned int l = 0;
    unsigned int m = 0;
	for ( int j = 0 ; j < height + 2*padding ; j ++ )
	{
		for ( int i = 0 ; i < width + 2*padding ; i++ )
		{
			if ( i < padding || i >= ( width + padding) || j < padding || j >= ( height + padding) )
			{
				newimageData[0] = 255;
				newimageData[1] = 255;
				newimageData[2] = 255;
				if ( numColorComponents == 4 )
					newimageData[3] = 255;
			}
			else
			{
				newimageData[0]   = pixelData[m];
				newimageData[1]   = pixelData[m+1];
				newimageData[2]   = pixelData[m+2];
				if ( numColorComponents == 4 )
					newimageData[3] = 255;
				m += numColorComponents;
			}
			newimageData += numColorComponents;
		}
	}
	/*for (int j = 0; j < height + 2*padding; ++j) {
	 for (int i = 0; i < width + 2*padding; ++i) {
	 if (padding <= j && j <= height + padding  && padding <= i && i <= width + padding ) {
	 newimageData[0] = pixelData[4 * (j * width + i) + 0];
	 newimageData[1] = pixelData[4 * (j * width + i) + 1];
	 newimageData[2] = pixelData[4 * (j * width + i) + 2];
	 } else {
	 newimageData[0] = r * 0xff;
	 newimageData[1] = g * 0xff;
	 newimageData[2] = b * 0xff;
	 }
	 newimageData += 3;
	 }
	 }*/
	delete[] pixelData;
}

bool Image::isTransparent()
{
	return false;
}

Image* Image::getScaledImage(float width, float height)
{
#ifndef IOS
    FIBITMAP *dib = 0;
    int numChannelPerPixel = 3;
    numChannelPerPixel = m_bIsTransparent ? 4 : 3;
    int BPP = numChannelPerPixel == 3 ? 24 : 32;
    IMAGE_FORMAT imageFormat = EIF_RGB;
    imageFormat = m_bIsTransparent ? EIF_RGBA : EIF_RGB;
    
    dib = FreeImage_Allocate(this->width, this->height, BPP);
    BYTE* bytes = FreeImage_GetBits(dib);
    memcpy(bytes , data , this->width*this->height*numChannelPerPixel);
    
    FIBITMAP *dibscaled = FreeImage_Rescale(dib,width,height,FILTER_BICUBIC);
    //retrieve the image data
    BYTE* bits = FreeImage_GetBits(dibscaled);
    //FreeImage_Save(FIF_BMP, dib, "SkyDome.bmp");
    
//    for(int i = 0;i<width*height*4; i+=4)
//    {
//        BYTE temp = bits[i];
//        bits[i]   = bits[i+2];
//        bits[i+2]   = temp;
//    }
    

    Image *newImage = new Image(width , height , imageFormat);
    memcpy(newImage->getData() , bits , width*height*numChannelPerPixel);
    FreeImage_Unload(dib);
    FreeImage_Unload(dibscaled);
    return newImage;
#else
    return 0;
#endif
}

void Image::save(const char* imageName)
{
#ifndef IOS
    FIBITMAP *dib = 0;
    int numChannelPerPixel = 3;
    numChannelPerPixel = m_bIsTransparent ? 4 : 3;
    int BPP = numChannelPerPixel == 3 ? 24 : 32;
    IMAGE_FORMAT imageFormat = EIF_RGB;
    imageFormat = m_bIsTransparent ? EIF_RGBA : EIF_RGB;
    
    dib = FreeImage_Allocate(this->width, this->height, BPP);
    BYTE* bytes = FreeImage_GetBits(dib);
    
    memcpy(bytes , data , this->width*this->height*numChannelPerPixel);
    /*BYTE* rawData = (BYTE*)data;
     for(int i = 0;i<width*height*numChannelPerPixel;i+=numChannelPerPixel)
     {
     BYTE temp = rawData[i];
     bytes[i]   = rawData[i+2];
     rawData[i+2]   = temp;
     }*/
    
    
    FreeImage_Save(FIF_PNG, dib, imageName);
    
    FreeImage_Unload(dib);
#endif
}

Image::~Image()
{
    if( format == EIF_RGB || format == EIF_RGBA )
        delete[] (unsigned char*)data;
    if( format == EIF_RGB_FLOAT32 || format == EIF_RGBA_FLOAT32 )
        delete[] (float*)data;
}
