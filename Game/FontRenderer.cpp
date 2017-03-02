//
//  FontRenderer.cpp
//  RTSGame
//
//  Created by Arshad on 11/06/13.
//  Copyright (c) 2013 Sourcebits. All rights reserved.
//

#include "FontRenderer.h"
#include "Texture.h"
#include "OglRenderer.h"
#include "Game.h"
#include "FileUtil.h"

#define MAX_CHAR    256

FontRenderer* FontRenderer::m_pFontRenderer = 0;
float* FontRenderer::m_pCoordBuffer = 0;
float* FontRenderer::m_pColorBuffer = 0;
float* FontRenderer::m_pTexBuffer = 0;

        FontRenderer::FontRenderer()
        {
            m_Scale = 1.0f;
            m_Rotation  = 0.0f;
            m_Color.r = 1.0f;
            m_Color.g = 1.0f;
            m_Color.b = 1.0f;
            m_Color.a = 1.0f;
            m_pTexture = 0;
            // Load the default engine font.
            
            SetFont("KingsFontnormal");
            
            m_pCoordBuffer = new float[MAX_CHAR * 6 * 2];
            m_pTexBuffer = new float[MAX_CHAR * 6 * 2];
            m_pColorBuffer = new float[MAX_CHAR * 6 * 4];
        }
        
        FontRenderer* FontRenderer:: Instance()
        {
            if( !m_pFontRenderer )
            {
                m_pFontRenderer = new FontRenderer();
            }

            return m_pFontRenderer;
        }
        
        void FontRenderer::DrawText(const string& text)
        {
#ifdef IOS
            //return;
#endif
            GenCharBuffer(text);
            
            Game::getInstance()->getRenderer()->setTransform(m_Transform, spider3d::WORLD);
            
            Game::getInstance()->getRenderer()->setTextureObject(m_pTexture);
            
            Game::getInstance()->getRenderer()->drawVertexPrimitiveList((int)text.length()*6, m_pCoordBuffer, 2, m_pColorBuffer, 4, m_pTexBuffer, 2, 0, spider3d::EPT_TRIANGLE);
        }
        
        void FontRenderer::SetPosition(const Vector2f& pos)
        {
            m_Position = pos;
            m_Transform.m03 = pos.x;
            m_Transform.m13 = pos.y;
        }
        
        void FontRenderer::SetScale(const float& scale)
        {
            m_Scale = scale;
            
            float theta = PI/180.0f * m_Rotation;
            float cs = cos(theta);
            float sn = sin(theta);
            
            m_Transform.m00 = cs * m_Scale;
            m_Transform.m10 = sn * m_Scale;
            m_Transform.m01 = -sn * m_Scale;
            m_Transform.m11 = cs * m_Scale;
        }
        
        void FontRenderer::SetRotation(const float& rotation)
        {
            m_Rotation = rotation;
            
            float theta = PI/180.0f * m_Rotation;
            float cs = cos(theta);
            float sn = sin(theta);
            
            m_Transform.m00 = cs * m_Scale;
            m_Transform.m10 = sn * m_Scale;
            m_Transform.m01 = -sn * m_Scale;
            m_Transform.m11 = cs * m_Scale;
        }
        
        void FontRenderer::SetColor(const Color4f& color)
        {
            m_Color = color;
        }
        
        void FontRenderer::SetTransform(const Matrix4f& transform)
        {
            m_Transform = transform;
        }

        void FontRenderer::SetFontTexture(Texture* pTexture)
        {
            m_pTexture = pTexture;
        }

        void FontRenderer::SetFont(string filename)
        {
#ifdef IOS
            m_Font = FNTLoader::Instance()->loadFont(filename);
#else
            m_Font = FNTLoader::Instance()->loadFont(filename);
            m_pTexture = m_Font.texture;
#endif
        }
        
        void FontRenderer::GenCharBuffer(const string& text)
        {
            int bufferIndex = 0;
            int colorIndex = 0;
            Texture* texture = m_pTexture;
            int t_width = texture->getWidth();
            int t_height = texture->getHeight();
            
            int xcursor = 0;
            
            for( int i=0; i<text.length(); i++ )
            {
                char ch = text[i];
                
                CharInfoMap::iterator it = m_Font.MapCharInfo.find(ch);
                
                if(it == m_Font.MapCharInfo.end())
                {
                    printf("Errror in generating char buffer\n");
                    return;
                    //assert(0);
                }
                
                CharInfo cinfo = it->second;
                
                float xOffset = xcursor + cinfo.xoffset;
                float yOffset = -cinfo.yoffset;
                
                // Coordinates.
                m_pCoordBuffer[bufferIndex+0] = xOffset                                                 ;m_pCoordBuffer[bufferIndex+1] = yOffset;
                m_pCoordBuffer[bufferIndex+2] = xOffset                                                 ;m_pCoordBuffer[bufferIndex+3] = yOffset - cinfo.height;
                m_pCoordBuffer[bufferIndex+4] = xOffset+cinfo.width                                     ;m_pCoordBuffer[bufferIndex+5] = yOffset - cinfo.height;
                m_pCoordBuffer[bufferIndex+6] = xOffset+cinfo.width                                     ;m_pCoordBuffer[bufferIndex+7] = yOffset - cinfo.height;
                m_pCoordBuffer[bufferIndex+8] = xOffset+cinfo.width                                     ;m_pCoordBuffer[bufferIndex+9] = yOffset;
                m_pCoordBuffer[bufferIndex+10] = xOffset                                                ;m_pCoordBuffer[bufferIndex+11] = yOffset;
                
                
                // tex coordinates.
                float _lbx = cinfo.x/(float)t_width ;
                float _lby = (t_height-cinfo.y-cinfo.height)/(float)t_height;
                
                float _rbx = (cinfo.x + cinfo.width)/(float)t_width;
                float _rby = _lby;
                
                float _rtx = _rbx;
                float _rty = (t_height - cinfo.y)/(float)t_height;
                
                float _ltx = _lbx;
                float _lty = _rty;
                
                m_pTexBuffer[bufferIndex+0] = _ltx          ;m_pTexBuffer[bufferIndex+1] = _lty;
                m_pTexBuffer[bufferIndex+2] = _lbx          ;m_pTexBuffer[bufferIndex+3] = _lby;
                m_pTexBuffer[bufferIndex+4] = _rbx          ;m_pTexBuffer[bufferIndex+5] = _rby;
                m_pTexBuffer[bufferIndex+6] = _rbx          ;m_pTexBuffer[bufferIndex+7] = _rby;
                m_pTexBuffer[bufferIndex+8] = _rtx          ;m_pTexBuffer[bufferIndex+9] = _rty;
                m_pTexBuffer[bufferIndex+10] = _ltx         ;m_pTexBuffer[bufferIndex+11] = _lty;
                
                // color coordinate.
                for(int i=0; i < 24; i+=4 )
                {
                    m_pColorBuffer[colorIndex+i+0] = m_Color.r;
                    m_pColorBuffer[colorIndex+i+1] = m_Color.g;
                    m_pColorBuffer[colorIndex+i+2] = m_Color.b;
                    m_pColorBuffer[colorIndex+i+3] = m_Color.a;
                }
                
                bufferIndex+=12;
                colorIndex+=24;
                
                xcursor +=cinfo.xadvance;
                
            }
        }

    float FontRenderer::GetTextWidth(string text)
    {
        int xcursor = 0;
        float width = 0;
        CharInfo cinfo;
        for( int i=0; i<text.length(); i++ )
        {
            char ch = text[i];
            
            CharInfoMap::iterator it = m_Font.MapCharInfo.find(ch);
          
            cinfo = it->second;
            
            width = xcursor + cinfo.xoffset;
            
            xcursor += cinfo.xadvance;
            
        }
        width = xcursor + cinfo.xoffset;
        
        return width;
    }

    float FontRenderer::GetTextHeight(string text)
    {
        float maxHeight = 0;
        
        for( int i=0; i<text.length(); i++ )
        {
            char ch = text[i];
            
            CharInfoMap::iterator it = m_Font.MapCharInfo.find(ch);
            
            //if(it == m_Font.MapCharInfo.end())
              //  assert(0);
            
            CharInfo cinfo = it->second;
            
            float h = cinfo.yoffset + cinfo.height;
            if( h > maxHeight)
                maxHeight = h;
        
    }
    
    return maxHeight;
}
