//
//  TextureMaker.h
//  Optiks
//
//  Created by Prashant on 21/09/11.
//  Copyright 2011 Sourcebits Technologies. All rights reserved.
//

#pragma once

#include "MathUtil.h"
#include <vector>
#include "Texture2D.h"
#include "Texture1D.h"
#include "Image.h"

using namespace std;
using namespace NAMESPACE_DIRAC2D;

typedef void (*funcTexture)(void* data, int width, int height, void* userData); 

static int NSUBDIV = 2;

class TextureMaker
{
public:
    
    static Texture* createLaserBeamTexture1(int width, int height, Color4f color, float fallOff,funcTexture makeTexture = 0);

};


struct Triangle
{
    Triangle()
    {
        triangles[0] = triangles[1] = triangles[2] = triangles[3] = 0;
        childCount = 0;
    }
    
    ~Triangle()
    {
        delete triangles[0];    
        delete triangles[1];    
        delete triangles[2];    
        delete triangles[3];    
    }
    
    bool isPointInside(const Vector2f& testPoint)
    {
        Vector2f p;
        p = testPoint;
        Matrix3f InvXform;
        Transform.getInverse(InvXform);
        InvXform.transformAsPoint(p);
        float c = 1.0f/tanf(M_PI_4*0.9);
        return ( (p.x >= p.y*c ) && (p.x <= -p.y*c) && (p.y >= -Height) );
    }
    
    void findNormal()
    {
        Vector2f p;
        p.set(Apex.x, Apex.y);
        Matrix3f InvXform;
        Transform.getInverse(InvXform);
        InvXform.transformAsPoint(p);
        p.y -= Height*0.5f;
        Transform.transformAsPoint(p);
        float radius = powf(2.0f,NSUBDIV) * Height*sqrt(2);
        radius *= radius;
        float z = powf( radius - p.x*p.x - p.y*p.y , 1/2.0f);
        Normal.x = p.x;
        Normal.y = p.y;
        Normal.z = z;      
        Normal.normalize();
    }
    Triangle* triangles[4];
    Vector2f Apex;
    Vector3f Normal;
    float Height;
    float Width;
    float Angle;
    Matrix3f Transform;
    int depth;
    int childCount;
};

Texture* createNormalMapTexture(int width, int height );

Texture* createHaloTexture( int width, int height, const Color4f& centreColor, const Color4f& peripheryColor );

Texture* createSuperEllipseTexture(int width, int height, const Color4f color, bool bNormalMap = false, bool bFixed = false );

Texture* createPlacementMarkerTexture(int width, int height, const Color4f color, bool bNormalMap = false );

Texture* createAbsorptionRectTexture(int width, int height, const Color4f color, bool bNormalMap = false, bool bFixed = false );

Texture* createReflectiveRectShapeTexture( int width, int height, const Color4f& color, bool bNormalMap = false ,bool bFixed = false);

Texture* createFilterTexture( int width, int height, const Color4f& color, bool bNormalMap = false ,bool bFixed = false);

Texture* createRefractiveRectShapeTexture( int width, int height, const Color4f& color, bool bNormalMap = false, bool bFixed = false);

Texture* createPortalTexture( int width, int height, const Color4f& color, bool bNormalMap = false);

Texture* createGemTexture(int width, int height, const Color4f& color, bool bNormalMap = false);

void addPinImages( Image* pImage, int width, int height, const Vector3f& lightDir);

static Texture* createSinkTexture(int innerRadius, int outerRadius, float angle, int width, int height, const Color4f& color = Color4f(1.0f,1.0f,1.0f,1.0f) )
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
            
                    if( r2 >= innerRadius2 && r2 <= outerRadius2 )
                    {
                        float theta = atan2f(x, y);
                        if( fabs(theta) >= 0 && fabs(theta) < angle )
                        {
                            imageData[c+3] = 0.0f;
                            c += 4;
                            continue;
                        }
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
                        
                        imageData[c] = (unsigned char)color.r*255;
                        imageData[c+1] = (unsigned char)color.g*255;
                        imageData[c+2] = (unsigned char)color.b*255;
                        
                        float midRad = (innerRadius + outerRadius)*0.5f;
                        
                        float alpha;
                        float currentDist = sqrt(r2);
                        if( currentDist < midRad )
                          alpha = ( sqrt(r2) - innerRadius)/(midRad-innerRadius);
                        else
                          alpha = ( outerRadius - sqrt(r2) )/(outerRadius-midRad);

                        imageData[c+3] = alpha*alpha*255;//*sin(0.15f * sqrt(r2));
                    }
                    else
                    {
                        imageData[c+3] = 0.0f;
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

static Texture* createSinkTextureNew(int innerRadius, int outerRadius, int width, int height, int numSamples = 1 )
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

Texture* createSourceTexture(int innerRadius, int outerRadius, int width, int height, int numSamples = 1 );

static void antialias(int width, int height, unsigned char* pData)
{
    for( int i=0; i<width; i++ )
    {
        for( int j=0; j<height; j++ )
        {
            
        }
    }
}

static void Triangulate(Triangle* triangle)
{
    if( triangle->depth >= NSUBDIV )
        return;
    
    triangle->childCount = 4;

    float angles[] = {0.0f,0.0f,180.0f,0.0f};
    float ax = triangle->Apex.x;
    float ay = triangle->Apex.y;
    
    float h_w = triangle->Width/2.0f;
    float h_h = triangle->Height/2.0f;

    Vector2f apex[4];
    if( triangle->Angle == 0.0f )
    {
        apex[0].set(ax, ay);
        apex[1].set(ax - h_w*0.5f , ay - h_h);
        apex[2].set(ax, ay - 2.0f*h_h);
        apex[3].set(ax + h_w*0.5f , ay - h_h);
    }
    if( fabs(triangle->Angle - 180.0f ) < 1e-3 )
    {
        apex[0].set(ax, ay);
        apex[1].set(ax - h_w*0.5f , ay + h_h);
        apex[2].set(ax, ay + 2.0f*h_h);
        apex[3].set(ax + h_w*0.5f , ay + h_h);
        angles[0] = 180.0f;
        angles[1] = 180.0f;
        angles[2] = 0.0f;
        angles[3] = 180.0f;
    }
    
    for( int i=0; i<triangle->childCount; i++ )
    {
        Triangle* tri = new Triangle();
        tri->depth = triangle->depth + 1;
        tri->Width = triangle->Width * 0.5f;
        tri->Height = triangle->Height * 0.5f;
        tri->Apex = apex[i];
        tri->Angle = angles[i];
        tri->Transform.rotate(angles[i]);
        Vector2f t;
        t.x = tri->Apex.x;
        t.y = tri->Apex.y;

        tri->Transform.translate(t);
        tri->findNormal();
        triangle->triangles[i] = tri;
        Triangulate(tri);
    }
}

static Triangle* pointTest(Triangle* triangle, const Vector2f& p)
{
    Triangle* outTriangle = 0;
    vector<Triangle*> vecTris;
    vecTris.push_back(triangle);
    
    while( vecTris.size() )
    {
        Triangle* t = vecTris[vecTris.size()-1];
        vecTris.pop_back();
        
        if( t->isPointInside(p) )
        {
            //printf("Success\n");
            if( t->depth == NSUBDIV )
            {
                outTriangle = t;
                break;
            }
            else
            {
                if( t->triangles[0] )
                    vecTris.push_back(t->triangles[0]);
                if( t->triangles[1] )
                    vecTris.push_back(t->triangles[1]);
                if( t->triangles[2] )
                    vecTris.push_back(t->triangles[2]);
                if( t->triangles[3] )
                    vecTris.push_back(t->triangles[3]);
            }
        }
        else
        {
        }
    }
    return outTriangle;
}

static void rotate(Triangle* triangle, float angle)
{
    if( triangle->depth > NSUBDIV )
        return;
    
    Matrix3f ROT;
    ROT.rotate(angle);
    
    triangle->Angle += angle;
    ROT.transformAsPoint(triangle->Apex);
    triangle->Transform.setIdentity();
    triangle->Transform.rotate(triangle->Angle);
    Vector2f trans;
    trans.x = triangle->Apex.x;
    trans.y = triangle->Apex.y;
    
    triangle->Transform.translate(trans);
    triangle->findNormal();
    
    for( int i=0; i<triangle->childCount; i++ )
    {
        Triangle* t = triangle->triangles[i];
        rotate(t, angle);
    }
}


void gaussianLaser( void* data , int width, int height, void* userData);

void dottedLaser( void* data , int width, int height, void* userData);

static Texture2D* createGradientTexture( Vector2f point1, Color4f color1, Vector2f point2, Color4f color2)
{
    int width = 256;
    int height = 256;
    unsigned char* data = new unsigned char[4*width*height]; 
    Vector3f d;
    d.x = point2.x - point1.x;
    d.y = point2.y - point1.y;
    d.normalize();
    
    Vector3f dperp(d.x, d.y, 0.0f);
    float dx = point1.x - point2.x;
    float dy = point1.y - point2.y;
    
    float distance = sqrt( dx*dx + dy*dy);
    int c = 0;
   // printf("%f  \n", distance);

    for( int i=0; i<height; i++ )
    {
        for( int j=0; j<width; j++ )
        {
            Vector3f p(j,i,0.0f);
            Vector3f diff;
            diff.x = p.x - point1.x;
            diff.y = p.y - point1.y;
            diff.z = 0.0f;
            float sd = fabs(dperp.dot(diff));

            sd /= distance;
            //float floor_sd = floorf(sd);
            float ceil_sd = ceilf(sd);
            
            sd = sd > 1.0f ? ( ceil_sd - sd  ) : sd;
            
            //sd = sd < 0.0f ? ( ceil_sd + sd ) : sd;
            //printf("%f  ", sd);

            data[c]   = (color1.r * ( 1.0f - sd ) + sd * color2.r)*255;
            data[c+1] = (color1.g * ( 1.0f - sd ) + sd * color2.g)*255;
            data[c+2] = (color1.b * ( 1.0f - sd ) + sd * color2.b)*255;
            data[c+3] = (color1.a * ( 1.0f - sd ) + sd * color2.a)*255;
            c += 4;
        }
    }
    
    Image* pImage = new Image(width, height, EIF_RGBA);
    memcpy(pImage->getData(), data, width*height*4);

    Texture2D* pTexture = new Texture2D(width,height,pImage);
	return pTexture;
}

static Texture* createGradientTexture( Color4f color1, Color4f color2)
{
    int width = 256;
    unsigned char* data = new unsigned char[4*width]; 
    
    int c = 0;
    
    for( int i=0; i<width; i++ )
    {
        float sd = (float)i/width;
            
        data[c]   = (color1.r * ( 1.0f - sd ) + sd * color2.r)*255;
        data[c+1] = (color1.g * ( 1.0f - sd ) + sd * color2.g)*255;
        data[c+2] = (color1.b * ( 1.0f - sd ) + sd * color2.b)*255;
        data[c+3] = (color1.a * ( 1.0f - sd ) + sd * color2.a)*255;
        c += 4;
    }
    
    Image* pImage = new Image(width, 1, EIF_RGBA);
    memcpy(pImage->getData(), data, width*4);
    
    Texture2D* pTexture = new Texture2D(width, 1, pImage);
    
    delete[] data;

	return pTexture;
    
}