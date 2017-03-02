//
//  FontRenderer.h
//  RTSGame
//
//  Created by Arshad on 11/06/13.
//  Copyright (c) 2013 Sourcebits. All rights reserved.
//

#ifndef RTSGame_FontRenderer_h
#define RTSGame_FontRenderer_h

#include "Vector2.h"
#include <string>
#include "FNTLoader.h"
#include "Matrix4.h"
#include "Renderer.h"

using namespace std;
using namespace spider3d::render;

#define FNTRenderer FontRenderer::Instance()

USE_NAMESPACE_DIRAC2D

class FontRenderer
{
  
public:
    
    static FontRenderer* Instance();
    
    void DrawText(const string& text);
    
    void SetPosition(const Vector2f& pos);
    
    void SetScale(const float& scale);
    
    void SetRotation(const float& rotation);
    
    void SetFont(string filename);

    void SetFontTexture(Texture* pTexture);

    void SetTransform(const NAMESPACE_DIRAC2D::Matrix4f& transform);
    
    void SetColor(const Color4f& color);
    
    float GetTextWidth(string text);
    
    float GetTextHeight(string text);
    
private:
    
    FontRenderer();
    
    void GenCharBuffer(const string& text);
    
private:
    
    Font m_Font;
    
    Texture* m_pTexture;
    
    NAMESPACE_DIRAC2D::Matrix4f m_Transform;
    
    Vector2f m_Position;
    float m_Scale;
    float m_Rotation;
    
    Color4f m_Color;

    static float* m_pCoordBuffer;
    static float* m_pTexBuffer;
    static float* m_pColorBuffer;
    
    static FontRenderer* m_pFontRenderer;
};


#endif
