//
//  TextRenderer.h
//  Optiks
//
//  Created by Prashant Sharma on 28/03/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef _TEXT_RENDERER_H_
#define _TEXT_RENDERER_H_

class Texture2D;
class TextureLoader;

class TextRenderer
{
public:
    
    TextRenderer( const char* fontFile, const char* textureFile );
    
    void print( int x, int y, const char* string, int set);
    
    unsigned getFontTexture()
    {
        return m_FontTextureName;
    }
    
private:
    
    void buildFont();
    
private:
    Texture2D* m_FontTexture;
    unsigned m_FontTextureName;
    TextureLoader* m_TextureLoader;
    int m_Base;
};


#endif
