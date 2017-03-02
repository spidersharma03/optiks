//
//  TextureMaker.cpp
//  Optiks
//
//  Created by Prashant on 21/09/11.
//  Copyright 2011 Sourcebits Technologies. All rights reserved.
//

#include "TextureMaker.h"
#include <cassert>


Texture* TextureMaker::createLaserBeamTexture1(int width, int height, Color4f color, float fallOff,funcTexture makeTexture)
{
//	glGenTextures(1, &texture);
//	glBindTexture(GL_TEXTURE_2D, texture);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	
	unsigned char* data = new unsigned char[4*width*height];
	
	if( makeTexture )
		makeTexture( data, width, height, 0 );
	
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//    
//	delete[] data;
//	glBindTexture(GL_TEXTURE_2D, 0);
//	return texture;
    
    Image* pImage = new Image(width, height, EIF_RGBA);
    memcpy(pImage->getData(), data, 4*width*height);
    Texture2D* pTexture = new Texture2D(width, height, pImage);
    
    return pTexture;
}

Texture* createNormalMapTexture(int width, int height )
{
    unsigned char* data = new unsigned char[4*width*height];
    
    float power = 4;
    
    float radius = (float)width/1.5f;
    float radius4 = powf(radius,power);
    unsigned char* imageData = (unsigned char*)data;
    int c = 0;
    //float alpha = 1.0f;
    float midx = width/2.0f - 0;
    float midy = height/2.0f - 1;
    
    Vector3f lightDir(0.0,0.0,1.0);
    lightDir.normalize();
    Vector3f ambientColor(40,40,40);
    Vector3f diffuseColor(250,250,250);
    
    
	for( int i=0; i<height; i++ )
	{
		for( int j=0; j<width; j++ )
		{
            float x = i - midx; float y = j - midy;
            float r4 = powf(x, power) + powf(y, power);
            
			imageData[c] = 0.0f;
			imageData[c+1] = 0.0f;
			imageData[c+2] = 0.0f;
            imageData[c+3] = 1.0f * 255;
            
            if( r4 <= radius4 )
            {
                float z = powf(radius4 - r4, 1.0f/power);
                Vector3f Normal(power*powf(x, power-1), power*powf(y, power-1), 0.5f*powf(z, power-1));
                Normal.normalize();
                float NdotL = Normal.dot(lightDir) * 0.6f;
                NdotL = NdotL < 0.0f ? 0.0f : NdotL;
                float r = NdotL * diffuseColor.x + ambientColor.x;
                float g = NdotL * diffuseColor.y + ambientColor.y;
                float b = NdotL * diffuseColor.z + ambientColor.z;
                
                r = r > 255 ? 255 : r;
                g = g > 255 ? 255 : g;
                b = b > 255 ? 255 : b;
                
                r = Normal.x * 0.5f + 0.5f;
                g = Normal.y * 0.5f + 0.5f;
                b = Normal.z * 0.5f + 0.5f;
                r *= 255;
                g *= 255;
                b *= 255;
                
                imageData[c] = (unsigned char)r;
                imageData[c+1] = (unsigned char)g;
                imageData[c+2] = (unsigned char)b;
                imageData[c+3] = 255;//0.25*(x*x + y*y);
            }
            else
            {
                imageData[c+3] = 0.0f;
            }
			c += 4;
		}
	}
    Image* pImage = new Image(width, height, EIF_RGBA);
    memcpy(pImage->getData(), data, 4*width*height);
    Texture2D* pTexture = new Texture2D(width, height, pImage);
    
    return pTexture;
}

Texture* createSourceTexture(int innerRadius, int outerRadius, int width, int height, int numSamples  )
{
    unsigned char* data = new unsigned char[4*width*height];
    
    float radius = (float)width/2.0f;
    float radius2 = radius*radius;
    float innerRadius2 = innerRadius*innerRadius;
    float outerRadius2 = outerRadius*outerRadius;
    
    unsigned char* imageData = (unsigned char*)data;
    int c = 0;
    float alpha = 1.0f;
    float midx = width/2.0f;
    float midy = height/2.0f;
    
    Vector3f lightDir(0.0,0.0,1.0);
    lightDir.normalize();
    Vector3f ambientColor(40,40,40);
    Vector3f diffuseColor(250,250,250);
    
	for( int i=0; i<height; i++ )
	{
		for( int j=0; j<width; j++ )
		{
            //for( int k=0; k<numSamples; k++ )
            {
                
                float x = i - midx; float y = j - midy;
                float r2 = x*x + y*y;
                
                imageData[c] = 0.0f;
                imageData[c+1] = 0.0f;
                imageData[c+2] = 0.0f;
                imageData[c+3] = alpha * 255;
                
                if( r2 > innerRadius2 && r2 <= outerRadius2 )
                {
                    float z = powf(radius2 - r2, 1.0f/2.0f);
                    Vector3f Normal(x, y, 0.5f*z);
                    Normal.normalize();
                    float NdotL = Normal.dot(lightDir) * 0.6f;
                    NdotL = NdotL < 0.0f ? 0.0f : NdotL;
                    float r = NdotL * diffuseColor.x + ambientColor.x;
                    float g = NdotL * diffuseColor.y + ambientColor.y;
                    float b = NdotL * diffuseColor.z + ambientColor.z;
                    
                    r = r > 255 ? 255 : r;
                    g = g > 255 ? 255 : g;
                    b = b > 255 ? 255 : b;
                    
                    imageData[c] = (unsigned char)r;
                    imageData[c+1] = (unsigned char)g;
                    imageData[c+2] = (unsigned char)b;
                    
                    imageData[c+3] = 255;//*sin(0.15f * sqrt(r2));
                }
                else
                {
                    imageData[c+3] = 0;
                }
            }
			c += 4;
		}
	}
    Image* pImage = new Image(width, height, EIF_RGBA);
    memcpy(pImage->getData(), imageData, width*height*4);
    
    Texture2D* pTexture = new Texture2D(width,height,pImage);
	
    delete[] data;
	return pTexture;
}


Texture* createPlacementMarkerTexture(int width, int height, const Color4f color, bool bNormalMap )
{
    unsigned char* data = new unsigned char[4*width*height];
    
    float radius = (float)width/2.0f;
    float power = 4;
    float max_radius4 = powf(radius, power);
    float min_radius4 = powf(radius-radius/20, power);
    unsigned char* imageData = (unsigned char*)data;
    int c = 0;
    //float alpha = 1.0f;
    float midx = width/2.0f - 0;
    float midy = height/2.0f - 1;
    
    Vector3f lightDir(0.0,0.0,1.0);
    lightDir.normalize();
    Vector3f ambientColor(40,40,40);
    Vector3f diffuseColor(color.r*255, color.g*255,color.b*255);
    
	for( int i=0; i<height; i++ )
	{
		for( int j=0; j<width; j++ )
		{
            float x = i - midx; float y = j - midy;
            float r4 = powf(x, power) + powf(y, power);
            
			imageData[c] = 0.0f;
			imageData[c+1] = 0.0f;
			imageData[c+2] = 0.0f;
            imageData[c+3] = 1.0f * 255;
            
            if( r4 <= max_radius4 && r4 >= min_radius4)
            {
                float z = powf(max_radius4 - r4, 1.0f/power);
                Vector3f Normal((power-1.0f)*powf(x, power-1.0f),(power-1.0f)*powf(y, power-1.0f), 0.5f*z*z*z);
                Normal.normalize();
                float NdotL = Normal.dot(lightDir) * 0.6f;
                NdotL = NdotL < 0.0f ? 0.0f : NdotL;
                float r = NdotL * diffuseColor.x + ambientColor.x;
                float g = NdotL * diffuseColor.y + ambientColor.y;
                float b = NdotL * diffuseColor.z + ambientColor.z;
                
                r = r > 255 ? 255 : r;
                g = g > 255 ? 255 : g;
                b = b > 255 ? 255 : b;
                
                if( bNormalMap )
                {
                    r = 0.5f;
                    g = 0.5f;
                    b = 1.0f;
                    r *= 255;
                    g *= 255;
                    b *= 255;
                }
                
                imageData[c] = (unsigned char)r;
                imageData[c+1] = (unsigned char)g;
                imageData[c+2] = (unsigned char)b;
                imageData[c+3] = color.a*255;
            }
            else
            {
                imageData[c+3] = 0.0f;
            }
			c += 4;
		}
	}
    
    
    Image* pImage = new Image(width, height, EIF_RGBA);
    memcpy(pImage->getData(), imageData, width*height*4);
    
    Texture2D* pTexture = new Texture2D(width,height,pImage);
	
    delete[] data;
	return pTexture;
}

Texture* createAbsorptionRectTexture(int width, int height, const Color4f color, bool bNormalMap, bool bFixed )
{
    unsigned char* data = new unsigned char[4*width*height];
    
    float radius = (float)width/2.0f;
    float power = 4;
    float radius4 = powf(radius, power);
    unsigned char* imageData = (unsigned char*)data;
    int c = 0;
    //float alpha = 1.0f;
    float midx = width/2.0f - 0;
    float midy = height/2.0f - 1;
    
    Vector3f lightDir(0.0,0.0,1.0);
    lightDir.normalize();
    Vector3f ambientColor(0,0,0);
    Vector3f diffuseColor(color.r*255, color.g*255,color.b*255);
    
	for( int i=0; i<height; i++ )
	{
		for( int j=0; j<width; j++ )
		{
            float x = i - midx; float y = j - midy;
            float r4 = powf(x, power) + powf(y, power);
            
			imageData[c] = 0.0f;
			imageData[c+1] = 0.0f;
			imageData[c+2] = 0.0f;
            imageData[c+3] = 1.0f * 255;
            
            if( r4 <= radius4 )
            {
                float z = powf(radius4 - r4, 1.0f/power);
                Vector3f Normal((power-1.0f)*powf(x, power-1.0f), (power-1.0f)*powf(y, power-1.0f), 0.5f*z*z*z);
                Normal.normalize();
                float NdotL = Normal.dot(lightDir) * 0.6f;
                NdotL = powf(NdotL, 3)*2;
                NdotL = NdotL < 0.0f ? 0.0f : NdotL;
                float r = NdotL * diffuseColor.x + ambientColor.x;
                float g = NdotL * diffuseColor.y + ambientColor.y;
                float b = NdotL * diffuseColor.z + ambientColor.z;
                
                r = r > 255 ? 255 : r;
                g = g > 255 ? 255 : g;
                b = b > 255 ? 255 : b;
                
                if( bNormalMap )
                {
                    r = 0.5f;
                    g = 0.5f;
                    b = 1.0f;
                    r *= 255;
                    g *= 255;
                    b *= 255;
                }
                
                imageData[c] = (unsigned char)r;
                imageData[c+1] = (unsigned char)g;
                imageData[c+2] = (unsigned char)b;
                imageData[c+3] = color.a*255;
            }
            else
            {
                imageData[c+3] = 0.0f;
            }
			c += 4;
		}
	}
    
    Image* pImage = new Image(width, height, EIF_RGBA);
    memcpy(pImage->getData(), imageData, width*height*4);
    
    if( bFixed )
        addPinImages(pImage, width, height, lightDir);
    
    Texture2D* pTexture = new Texture2D(width,height,pImage);
	
    delete[] data;
	return pTexture;
}

void addPinImages( Image* pImage, int width, int height, const Vector3f& lightDir)
{
    Vector3f lightDirection(0.0,0.0,1.0);
    Vector3f diffuseColor;
    diffuseColor.x = 255;
    diffuseColor.y = 255;
    diffuseColor.z = 255.0f;
    Vector3f ambientColor;
    ambientColor.x = ambientColor.y = ambientColor.z = 0;
    
    int newwidth = width/6.0f;
    int newheight = height/6.0f;
    float radius = (float)newwidth/2.0f;
    float minradius = 0;//(float)newwidth/2.0f;
    float power = 2;
    float radius4 = powf(radius, power);
    float minradius4 = powf(minradius, power);
    
    unsigned char* dataNew = new unsigned char[4*newwidth*newheight];
    unsigned char* imageData = (unsigned char*)dataNew;
    
    int c = 0;
    
    float midx = newwidth/2.0f;
    float midy = newheight/2.0f;
    
    for( int i=0; i<newheight; i++ )
	{
		for( int j=0; j<newwidth; j++ )
		{
            float x = i - midx; float y = j - midy;
            float r4 = powf(x, power) + powf(y, power);
            
			imageData[c] = 0.0f;
			imageData[c+1] = 0.0f;
			imageData[c+2] = 0.0f;
            imageData[c+3] = 1.0f * 255;
            
            if( r4 <= radius4 && r4 > minradius4 )
            {
                float z = powf(radius4 - r4, 1.0f/power);
                Vector3f Normal((power-1.0f)*powf(x, power-1.0f), (power-1.0f)*powf(y, power-1.0f), z);
                Normal.normalize();
                float NdotL = Normal.dot(lightDirection) * 0.6f;
                NdotL = powf(NdotL, 5)*10;
                NdotL = NdotL < 0.0f ? 0.0f : NdotL;
                float r = NdotL * diffuseColor.x + ambientColor.x;
                float g = NdotL * diffuseColor.y + ambientColor.y;
                float b = NdotL * diffuseColor.z + ambientColor.z;
                
                r = r > 255 ? 255 : r;
                g = g > 255 ? 255 : g;
                b = b > 255 ? 255 : b;
                
                imageData[c] = (unsigned char)r;
                imageData[c+1] = (unsigned char)g;
                imageData[c+2] = (unsigned char)b;
                imageData[c+3] = 255;
            }
            else
            {
                imageData[c+3] = 0.0f;
            }
			c += 4;
		}
	}
    
    int offset = 5;
    pImage->copySubImage(imageData, AABB2i(Vector2i(offset,offset), Vector2i(newwidth+offset,newheight+offset)));
    pImage->copySubImage(imageData, AABB2i(Vector2i(width-newwidth-offset,offset), Vector2i(width-offset,newheight+offset)));
    pImage->copySubImage(imageData, AABB2i(Vector2i(width-newwidth-offset,height-newheight-offset), Vector2i(width-offset,height-offset)));
    pImage->copySubImage(imageData, AABB2i(Vector2i(offset,height-newheight-offset), Vector2i(newwidth+offset,height-offset)));
    
}

Texture* createSuperEllipseTexture(int width, int height, const Color4f color, bool bNormalMap, bool bFixed )
{
    unsigned char* data = new unsigned char[4*width*height];
    
    float radius = (float)width/2.0f;
    float power = 4;
    float radius4 = powf(radius, power);
    unsigned char* imageData = (unsigned char*)data;
    int c = 0;
    //float alpha = 1.0f;
    float midx = width/2.0f - 0;
    float midy = height/2.0f - 1;
    
    Vector3f lightDir(0.0,0.0,1.0);
    lightDir.normalize();
    Vector3f ambientColor(40,40,40);
    Vector3f diffuseColor(color.r*255, color.g*255,color.b*255);
    
	for( int i=0; i<height; i++ )
	{
		for( int j=0; j<width; j++ )
		{
            float x = i - midx; float y = j - midy;
            float r4 = powf(x, power) + powf(y, power);
            
			imageData[c] = 0.0f;
			imageData[c+1] = 0.0f;
			imageData[c+2] = 0.0f;
            imageData[c+3] = 1.0f * 255;
            
            if( r4 <= radius4 )
            {
                float z = powf(radius4 - r4, 1.0f/power);
                Vector3f Normal((power-1.0f)*powf(x, power-1.0f), (power-1.0f)*powf(y, power-1.0f), 0.5f*z*z*z);
                Normal.normalize();
                float NdotL = Normal.dot(lightDir) * 0.6f;
                NdotL = NdotL < 0.0f ? 0.0f : NdotL;
                float r = NdotL * diffuseColor.x + ambientColor.x;
                float g = NdotL * diffuseColor.y + ambientColor.y;
                float b = NdotL * diffuseColor.z + ambientColor.z;
                
                r = r > 255 ? 255 : r;
                g = g > 255 ? 255 : g;
                b = b > 255 ? 255 : b;
                
                if( bNormalMap )
                {
                    r = 0.5f;
                    g = 0.5f;
                    b = 1.0f;
                    r *= 255;
                    g *= 255;
                    b *= 255;
                }
                
                imageData[c] = (unsigned char)r;
                imageData[c+1] = (unsigned char)g;
                imageData[c+2] = (unsigned char)b;
                imageData[c+3] = color.a*255;
            }
            else
            {
                imageData[c+3] = 0.0f;
            }
			c += 4;
		}
	}
    
    
    Image* pImage = new Image(width, height, EIF_RGBA);
    memcpy(pImage->getData(), imageData, width*height*4);
    
    if( bFixed )
        addPinImages(pImage, width, height, lightDir); 
    
    Texture2D* pTexture = new Texture2D(width,height,pImage);
	
    delete[] data;
	return pTexture;
}

Texture* createFilterTexture( int width, int height, const Color4f& color, bool bNormalMap ,bool bFixed)
{
    unsigned char* data = new unsigned char[4*width*height];
    
    float radius = (float)width/2.0f;
    float power = 4;
    float radius4 = powf(radius, power);
    unsigned char* imageData = (unsigned char*)data;
    int c = 0;
    //float alpha = 1.0f;
    float midx = width/2.0f - 0;
    float midy = height/2.0f - 1;
    
    Vector3f lightDir1(1.0,-1.0,1.0);
    Vector3f lightDir2(-1.0,-1.0,1.0);
    
    lightDir1.normalize();
    lightDir2.normalize();
    Vector3f ambientColor(50,50,50);
    Vector3f diffuseColor(color.r*255, color.g*255,color.b*255);
    Vector3f specularColor(155, 155,155);
    
	for( int i=0; i<height; i++ )
	{
		for( int j=0; j<width; j++ )
		{
            float x = i - midx; float y = j - midy;
            float r4 = powf(x, power) + powf(y, power);
            
			imageData[c] = 0.0f;
			imageData[c+1] = 0.0f;
			imageData[c+2] = 0.0f;
            imageData[c+3] = 1.0f * 255;
            
            if( r4 <= radius4 )
            {
                float z = powf(radius4 - r4, 1.0f/power);
                Vector3f Normal((power-1.0f)*powf(x, power-1.0f), (power-1.0f)*powf(y, power-1.0f), 0.05*z*z*z*z);
                Normal.normalize();
                float NdotL1 = Normal.dot(lightDir1) * 0.6f;
                NdotL1 = powf(NdotL1, 15)*4000;
                NdotL1 = NdotL1 < 0.0f ? 0.0f : NdotL1;
                
                float NdotL2 = Normal.dot(lightDir2) * 0.6f;
                NdotL2 = powf(NdotL2, 15)*4000;
                NdotL2 = NdotL2 < 0.0f ? 0.0f : NdotL2;
                
                NdotL1 += NdotL2;
                
                float r = NdotL1 * diffuseColor.x + ambientColor.x;
                float g = NdotL1 * diffuseColor.y + ambientColor.y;
                float b = NdotL1 * diffuseColor.z + ambientColor.z;
                
                r = r > 255 ? 255 : r;
                g = g > 255 ? 255 : g;
                b = b > 255 ? 255 : b;
                
                if( bNormalMap )
                {
                    r = Normal.x * 0.5f + 0.5f;
                    g = Normal.y * 0.5f + 0.5f;
                    b = Normal.z * 0.5f + 0.5f;
                    r = 0.5f;
                    g = 0.5f;
                    b = 1.0f;
                    
                    r *= 255;
                    g *= 255;
                    b *= 255;
                }
                
                imageData[c] = (unsigned char)r;
                imageData[c+1] = (unsigned char)g;
                imageData[c+2] = (unsigned char)b;
                
                float r2 = fabs(x*x + y*y);
                imageData[c+3] = (r2)/(radius*radius)*255;
            }
            else
            {
                imageData[c+3] = 0.0f;
            }
			c += 4;
		}
	}
    
    
    Image* pImage = new Image(width, height, EIF_RGBA);
    memcpy(pImage->getData(), imageData, width*height*4);
    
    if( bFixed )
        addPinImages(pImage, width, height, Vector3f(0.0,0.0,1.0));
        
        Texture2D* pTexture = new Texture2D(width,height,pImage);
        
        delete[] data;
	return pTexture;
}

Texture* createReflectiveRectShapeTexture( int width, int height, const Color4f& color, bool bNormalMap, bool bFixed)
{
    unsigned char* data = new unsigned char[4*width*height];
    
    float radius = (float)width/2.0f;
    float power = 4;
    float radius4 = powf(radius, power);
    unsigned char* imageData = (unsigned char*)data;
    int c = 0;
    //float alpha = 1.0f;
    float midx = width/2.0f - 0;
    float midy = height/2.0f - 1;
    
    Vector3f lightDir1(1.0,-1.0,1.0);
    Vector3f lightDir2(-1.0,-1.0,1.0);

    lightDir1.normalize();
    lightDir2.normalize();
    Vector3f ambientColor(30,30,30);
    Vector3f diffuseColor(color.r*255, color.g*255,color.b*255);
    Vector3f specularColor(55, 55,55);
    
	for( int i=0; i<height; i++ )
	{
		for( int j=0; j<width; j++ )
		{
            float x = i - midx; float y = j - midy;
            float r4 = powf(x, power) + powf(y, power);
            
			imageData[c] = 0.0f;
			imageData[c+1] = 0.0f;
			imageData[c+2] = 0.0f;
            imageData[c+3] = 1.0f * 255;
            
            if( r4 <= radius4 )
            {
                float z = powf(radius4 - r4, 1.0f/power);
                Vector3f Normal((power-1.0f)*powf(x, power-1.0f), (power-1.0f)*powf(y, power-1.0f), 0.05*z*z*z*z);
                Normal.normalize();
                float NdotL1 = Normal.dot(lightDir1) * 0.6f;
                NdotL1 = powf(NdotL1, 15)*4000;
                NdotL1 = NdotL1 < 0.0f ? 0.0f : NdotL1;
                
                float NdotL2 = Normal.dot(lightDir2) * 0.6f;
                NdotL2 = powf(NdotL2, 15)*4000;
                NdotL2 = NdotL2 < 0.0f ? 0.0f : NdotL2;
                
                NdotL1 += NdotL2;
                
                float r = NdotL1 * diffuseColor.x + ambientColor.x;
                float g = NdotL1 * diffuseColor.y + ambientColor.y;
                float b = NdotL1 * diffuseColor.z + ambientColor.z;
                
                r = r > 255 ? 255 : r;
                g = g > 255 ? 255 : g;
                b = b > 255 ? 255 : b;
                
                if( bNormalMap )
                {
                    r = Normal.x * 0.5f + 0.5f;
                    g = Normal.y * 0.5f + 0.5f;
                    b = Normal.z * 0.5f + 0.5f;
                    r = 0.5f;
                    g = 0.5f;
                    b = 1.0f;
                    
                    r *= 255;
                    g *= 255;
                    b *= 255;
                }
                
                imageData[c] = (unsigned char)r;
                imageData[c+1] = (unsigned char)g;
                imageData[c+2] = (unsigned char)b;
                
                float r2 = fabs(x*x + y*y);
                imageData[c+3] = (r2)/(radius*radius)*255;
            }
            else
            {
                imageData[c+3] = 0.0f;
            }
			c += 4;
		}
	}
    
    
    Image* pImage = new Image(width, height, EIF_RGBA);
    memcpy(pImage->getData(), imageData, width*height*4);
    
    if( bFixed )
        addPinImages(pImage, width, height, Vector3f(0.0,0.0,1.0));

    Texture2D* pTexture = new Texture2D(width,height,pImage);
	
    delete[] data;
	return pTexture;
}

Texture* createRefractiveRectShapeTexture( int width, int height, const Color4f& color, bool bNormalMap, bool bFixed)
{
    unsigned char* data = new unsigned char[4*width*height];
    
    float radius = (float)width/2.0f;
    float power = 4;
    float radius4 = powf(radius, power);
    unsigned char* imageData = (unsigned char*)data;
    int c = 0;
    //float alpha = 1.0f;
    float midx = width/2.0f - 0;
    float midy = height/2.0f - 1;
    
    Vector3f lightDir1(1.0,-1.0,1.0);
    Vector3f lightDir2(-1.0,-1.0,1.0);
    
    lightDir1.normalize();
    lightDir2.normalize();
    Vector3f ambientColor(20,20,50);
    Vector3f diffuseColor(color.r*255, color.g*255,color.b*255);
    Vector3f specularColor(55, 55,55);
    
	for( int i=0; i<height; i++ )
	{
		for( int j=0; j<width; j++ )
		{
            float x = i - midx; float y = j - midy;
            float r4 = powf(x, power) + powf(y, power);
            
			imageData[c] = 0.0f;
			imageData[c+1] = 0.0f;
			imageData[c+2] = 0.0f;
            imageData[c+3] = 1.0f * 255;
            
            if( r4 <= radius4 )
            {
                float z = powf(radius4 - r4, 1.0f/power);
                Vector3f Normal((power-1.0f)*powf(x, power-1.0f), (power-1.0f)*powf(y, power-1.0f), 0.05*z*z*z*z);
                Normal.normalize();
                float NdotL1 = Normal.dot(lightDir1) * 0.6f;
                NdotL1 = powf(NdotL1, 15)*4000;
                NdotL1 = NdotL1 < 0.0f ? 0.0f : NdotL1;
                
                float NdotL2 = Normal.dot(lightDir2) * 0.6f;
                NdotL2 = powf(NdotL2, 15)*4000;
                NdotL2 = NdotL2 < 0.0f ? 0.0f : NdotL2;
                
                NdotL1 += NdotL2;
                
                float r = NdotL1 * diffuseColor.x + ambientColor.x;
                float g = NdotL1 * diffuseColor.y + ambientColor.y;
                float b = NdotL1 * diffuseColor.z + ambientColor.z;
                
                r = r > 255 ? 255 : r;
                g = g > 255 ? 255 : g;
                b = b > 255 ? 255 : b;
                
                if( bNormalMap )
                {
                    r = Normal.x * 0.5f + 0.5f;
                    g = Normal.y * 0.5f + 0.5f;
                    b = Normal.z * 0.5f + 0.5f;
                    r = 0.5f;
                    g = 0.5f;
                    b = 1.0f;
                    
                    r *= 255;
                    g *= 255;
                    b *= 255;
                }
                
                imageData[c] = (unsigned char)r;
                imageData[c+1] = (unsigned char)g;
                imageData[c+2] = (unsigned char)b;
                
                float r2 = fabs(x*x + y*y);
                imageData[c+3] = (r2)/(radius*radius)*255;
            }
            else
            {
                imageData[c+3] = 0.0f;
            }
			c += 4;
		}
	}
    
    
    Image* pImage = new Image(width, height, EIF_RGBA);
    memcpy(pImage->getData(), imageData, width*height*4);
    
    if( bFixed )
        addPinImages(pImage, width, height, Vector3f(0.0,0.0,1.0));

    Texture2D* pTexture = new Texture2D(width,height,pImage);
	
    delete[] data;
	return pTexture;
}

Texture* createPortalTexture( int width, int height, const Color4f& color, bool bNormalMap)
{
    unsigned char* data = new unsigned char[4*width*height];
    
    float radius = (float)width/2.0f;
    float power = 4;
    float radius4 = powf(radius, power);
    unsigned char* imageData = (unsigned char*)data;
    int c = 0;
    //float alpha = 1.0f;
    float midx = width/2.0f - 0;
    float midy = height/2.0f - 1;
    
    Vector3f lightDir1(1.0,-1.0,1.0);
    Vector3f lightDir2(-1.0,-1.0,1.0);
    
    lightDir1.normalize();
    lightDir2.normalize();
    Vector3f ambientColor(20,20,50);
    Vector3f diffuseColor(color.r*255, color.g*255,color.b*255);
    Vector3f specularColor(55, 55,55);
    
	for( int i=0; i<height; i++ )
	{
		for( int j=0; j<width; j++ )
		{
            float x = i - midx; float y = j - midy;
            float r4 = powf(x, power) + powf(y, power);
            
			imageData[c] = 0.0f;
			imageData[c+1] = 0.0f;
			imageData[c+2] = 0.0f;
            imageData[c+3] = 1.0f * 255;
            
            if( r4 <= radius4 )
            {
                float z = powf(radius4 - r4, 1.0f/power);
                Vector3f Normal((power-1.0f)*powf(x, power-1.0f), (power-1.0f)*powf(y, power-1.0f), 0.05*z*z*z*z);
                Normal.normalize();
                float NdotL1 = Normal.dot(lightDir1) * 0.6f;
                NdotL1 = powf(NdotL1, 15)*4000;
                NdotL1 = NdotL1 < 0.0f ? 0.0f : NdotL1;
                
                float NdotL2 = Normal.dot(lightDir2) * 0.6f;
                NdotL2 = powf(NdotL2, 15)*4000;
                NdotL2 = NdotL2 < 0.0f ? 0.0f : NdotL2;
                
                NdotL1 += NdotL2;
                
                float r = NdotL1 * diffuseColor.x + ambientColor.x;
                float g = NdotL1 * diffuseColor.y + ambientColor.y;
                float b = NdotL1 * diffuseColor.z + ambientColor.z;
                
                r = r > 255 ? 255 : r;
                g = g > 255 ? 255 : g;
                b = b > 255 ? 255 : b;
                
                if( bNormalMap )
                {
                    r = Normal.x * 0.5f + 0.5f;
                    g = Normal.y * 0.5f + 0.5f;
                    b = Normal.z * 0.5f + 0.5f;
                    r = 0.5f;
                    g = 0.5f;
                    b = 1.0f;
                    
                    r *= 255;
                    g *= 255;
                    b *= 255;
                }
                
                imageData[c] = (unsigned char)r;
                imageData[c+1] = (unsigned char)g;
                imageData[c+2] = (unsigned char)b;
                
                float r2 = fabs(x*x + y*y);
                imageData[c+3] = (r2)/(radius*radius)*255;
            }
            else
            {
                imageData[c+3] = 0.0f;
            }
			c += 4;
		}
	}
    
    
    Image* pImage = new Image(width, height, EIF_RGBA);
    memcpy(pImage->getData(), imageData, width*height*4);
    
    addPinImages(pImage, width, height, lightDir1);

    Texture2D* pTexture = new Texture2D(width,height,pImage);
	
    delete[] data;
	return pTexture;
}

Texture* createGemTexture(int width, int height, const Color4f& color, bool bNormalMap)
{
    unsigned char* data = new unsigned char[4*width*height];
    
    float radius = (float)width/2.0f;
    float radius4 = radius*radius*radius*radius;
    unsigned char* imageData = (unsigned char*)data;
    int c = 0;
    float midx = width/2.0f - 0;
    float midy = height/2.0f - 1;
    
    Vector3f lightDir(0.0,-1.0,1.0);
    lightDir.normalize();
    Vector3f ambientColor(0,0,0);
    Vector3f diffuseColor(color.r*255, color.g*255, color.b*255);
    
    //int nTriangles = 4 * powf(4.0f, NSUBDIV);
    
    Triangle* triangle1 = new Triangle();
    Triangle* triangle2 = new Triangle();
    Triangle* triangle3 = new Triangle();
    Triangle* triangle4 = new Triangle();
    
    triangle1->Angle = 0.0f;
    triangle1->Apex.set(0.0f,0.0f);
    triangle1->depth = 0;
    triangle1->Width = width;
    triangle1->Height = height/2.0f;
    
    triangle2->Angle = 0.0f;
    triangle2->Apex.set(0.0f,0.0f);
    triangle2->depth = 0;
    triangle2->Width = width;
    triangle2->Height = height/2.0f;
    
    triangle3->Angle = 0.0f;
    triangle3->Apex.set(0.0f,0.0f);
    triangle3->depth = 0;
    triangle3->Width = width;
    triangle3->Height = height/2.0f;
    
    triangle4->Angle = 0.0f;
    triangle4->Apex.set(0.0f,0.0f);
    triangle4->depth = 0;
    triangle4->Width = width;
    triangle4->Height = height/2.0f;
    
    Triangulate(triangle1);
    Triangulate(triangle2);
    Triangulate(triangle3);
    Triangulate(triangle4);
    
    float beta = 90.0f;
    rotate(triangle2, beta);
    rotate(triangle3, -1.0f*beta);
    rotate(triangle4, 180.0f);
    
    float alpha = color.a * 255;
    
	for( int i=0; i<height; i++ )
	{
		for( int j=0; j<width; j++ )
		{
            float x = i - midx; float y = j - midy;
            
            Triangle* outTriangle = pointTest(triangle1, Vector2f(x,y));
            if( !outTriangle )
            outTriangle = pointTest(triangle2, Vector2f(x,y));
            if( !outTriangle )
            outTriangle = pointTest(triangle3, Vector2f(x,y));
            if( !outTriangle )
            outTriangle = pointTest(triangle4, Vector2f(x,y));
            
            float r4 = x*x*x*x + y*y*y*y;
            
			imageData[c] = 0.0f;
			imageData[c+1] = 0.0f;
			imageData[c+2] = 0.0f;
            imageData[c+3] =  0.0f;
            
            if( r4 <= radius4 && outTriangle)
            {
                //float z = powf(radius4 - r4, 1.0f/4.0f);
                Vector3f Normal;
                Normal = outTriangle->Normal;
                Normal.normalize();
                float NdotL = Normal.dot(lightDir);
                //NdotL = powf(NdotL, 15)*40000;
                NdotL = NdotL < 0.0f ? 0.0f : NdotL;
                
                float r = NdotL * diffuseColor.x + ambientColor.x;
                float g = NdotL * diffuseColor.y + ambientColor.y;
                float b = NdotL * diffuseColor.z + ambientColor.z;
                
                if( bNormalMap )
                {
                    r = Normal.x * 0.5f + 0.5f;
                    g = Normal.y * 0.5f + 0.5f;
                    b = Normal.z * 0.5f + 0.5f;
                }
                
                r *= 255;
                g *= 255;
                b *= 255;
                
                r = r > 255 ? 255 : r;
                g = g > 255 ? 255 : g;
                b = b > 255 ? 255 : b;
                
                
                imageData[c] = (unsigned char)r;
                imageData[c+1] = (unsigned char)g;
                imageData[c+2] = (unsigned char)b;
                imageData[c+3] = alpha;//(x*x + y*y);
            }
            else
            {
                float r = 0.5f;
                float g = 0.5f;
                float b = 1.0f;
                r *= 255;
                g *= 255;
                b *= 255;
                
                imageData[c] = (unsigned char)r;
                imageData[c+1] = (unsigned char)g;
                imageData[c+2] = (unsigned char)b;
                imageData[c+3] = 0.0f;
            }
			c += 4;
		}
	}
    Image* pImage = new Image(width, height, EIF_RGBA);
    memcpy(pImage->getData(), imageData, width*height*4);
    
    Texture2D* pTexture = new Texture2D(width,height,pImage);
	
    delete[] data;
	return pTexture;
}

void gaussianLaser( void* data , int width, int height, void* userData)
{
	int mid = height/2;
	int c = 0;
	
	unsigned char* imageData = (unsigned char*)data;
    
    for( int i=0; i<width*height; i++ )
    {
        imageData[c] = 0.0f;
        imageData[c+1] = 0.0f;
        imageData[c+2] = 0.0f;
        imageData[c+3] = 0.0f;
        c += 4;
    }
    c= 0;
	for( int i=0; i<height; i++ )
	{
		float d = fabs( (float)(mid - i)/height );
		float alpha = exp( -45  * d );
		//float alpha = cos(  d * M_PI * 2);
		
		for( int j=0; j<width; j++ )
		{
			imageData[c] = 1.0f * 255;
			imageData[c+1] = 1.0f * 255;
			imageData[c+2] = 1.0f * 255;
			imageData[c+3] = alpha * 255;// * fabs( sin( 1.0 * M_PI * (float)(width-j)/width ));
			c += 4;
		}
	}
    
//    float alpha = 0.84f;
//    c = 0;
//    int n = 100;
//    for( int i=0; i<n; i++ )
//	{
//        int s = (float)rand()/RAND_MAX * width * height;
//        
//        imageData[4*s] = 1.0f * 255;
//        imageData[4*s+1] = 1.0f * 255;
//        imageData[4*s+2] = 1.0f * 255;
//        imageData[4*s+3] = alpha * 255;
//	}
}

void dottedLaser( void* data , int width, int height, void* userData)
{
	int mid = height/2;
	int c = 0;
	
	unsigned char* imageData = (unsigned char*)data;
	for( int i=0; i<height; i++ )
	{
		//float d = fabs( (float)(mid - i)/height );
		//float alpha = cos(  d * M_PI * 2);
		
		for( int j=0; j<width; j++ )
		{
			imageData[c] = 1.0f * 255;
			imageData[c+1] = 1.0f * 255;
			imageData[c+2] = 1.0f * 255;
			imageData[c+3] = 0.05 * 255 * fabs( sin( 10.0 * M_PI * (float)(width-j)/width ));
			c += 4;
		}
	}
}

Texture* createHaloTexture( int width, int height, const Color4f& centreColor, const Color4f& peripheryColor )
{
    unsigned char* imageData = new unsigned char[4*width*height];

    int midx = width/2;
    int midy = height/2;
    float r2 = height * height * 0.25f * 1.0f;
    int c = 0;
    
    for( int i=0; i<height; i++ )
    {
        for( int j=0; j<width; j++ )
        {
            int dx = i - midx;
            int dy = j - midy;
            float d2 = dx*dx + dy*dy;
            if( d2 <= r2 )
            {
                float t = d2/r2;
                imageData[c] = (centreColor.r * (1-t) + peripheryColor.r * t)*255;
                imageData[c+1] = (centreColor.g * (1-t) + peripheryColor.g * t)*255;
                imageData[c+2] = (centreColor.b * (1-t) + peripheryColor.b * t)*255;
                imageData[c+3] = (centreColor.a * (1-t) + peripheryColor.a * t)*255;
            }
            else
            {
                imageData[c] = 255;
                imageData[c+1] = 255;
                imageData[c+2] = 255;
                imageData[c+3] = 0;
            }
            c += 4;
        }
    }
    Image* pImage = new Image(width, height, EIF_RGBA);
    memcpy(pImage->getData(), imageData, width*height*4);
    
    Texture2D* pTexture = new Texture2D(width,height,pImage);
	
    delete[] imageData;
	return pTexture;
}