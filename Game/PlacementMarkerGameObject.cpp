//
//  PlacementMarkerGameObject.cpp
//  Optiks
//
//  Created by Prashant Sharma on 11/11/13.
//
//

#include "PlacementMarkerGameObject.h"
#include "OglRenderer.h"
#include "Game.h"
#include "Scene.h"
#include "TextureMaker.h"

using namespace spider3d;
using namespace spider3d::render;

void DrawAABB(float w, float h, float scaleX = 1.0f, float scaleY = 1.0f)
{
    float vertices[8];
    vertices[0] = w;
    vertices[1] = h;
    vertices[2] = 0;
    vertices[3] = h;
    vertices[4] = 0;
    vertices[5] = 0;
    vertices[6] = w;
    vertices[7] = 0;
    
    float texCoords[8];
    texCoords[0] = scaleX;
    texCoords[1] = scaleY;
    texCoords[2] = 0.0f;
    texCoords[3] = scaleY;
    texCoords[4] = 0.0f;
    texCoords[5] = 0.0f;
    texCoords[6] = scaleX;
    texCoords[7] = 0.0f;
    
    float colors[16];
    for(int i=0; i<16; i+=4 )
    {
        colors[i]   = 1.0f;
        colors[i+1] = 1.0f;
        colors[i+2] = 1.0f;
        colors[i+3] = 1.0f;
    }
    
    Game::getInstance()->getRenderer()->drawVertexPrimitiveList(4, vertices, 2, colors, 4, texCoords, 2, 0, EPT_TRIANGLE_FAN);
}

void PlacementMarkerGameObject::render()
{
    if( !m_bVisible )
        return;
    
    Matrix4f xForm;
    xForm.translate(m_Position);
    xForm.m03 -= m_Width*0.5f;
    xForm.m13 -= m_Height*0.5f;
    
    Game::getInstance()->getRenderer()->setTransform(xForm, spider3d::WORLD);
    Game::getInstance()->getRenderer()->setTextureObject(m_pTexture);
    DrawAABB(m_Width, m_Height);
    //Game::getInstance()->getRenderer()->setBlendingEnable(false);
}

void PlacementMarkerGameObject::update(float dt)
{
  
}

bool PlacementMarkerGameObject::pick(const Vector2f& point) const
{
    Vector2f p = point;
    p -= Vector2f(m_Transform.col3.x, m_Transform.col3.y);
    return p.x < m_Width*0.5f && p.x > -m_Width*0.5f && p.y < m_Height*0.5f && p.y > -m_Height*0.5f;
}

void PlacementMarkerGameObject::save(FILE* pFile)
{
    fprintf(pFile, "%s\n", "POSITION::");
    Vector2i gridPos = getGridCoordinate();
    fprintf(pFile, "%d  %d\n", gridPos.x, gridPos.y);
    fprintf(pFile, "%s\n", "DIMENSIONS::");
    fprintf(pFile, "%f  %f\n", m_Width, m_Height);
}

void PlacementMarkerGameObject::load(FILE* pFile)
{
    
}

PlacementMarkerGameObject::~PlacementMarkerGameObject()
{
//    if( m_pGameObject )
//    {
//        Game::getInstance()->getScene()->removeGameObject(m_pGameObject);
//        Game::getInstance()->getScene()->destroyGameObject(m_pGameObject);
//    }
    m_pGameObject = NULL;
}

PlacementMarkerGameObject::PlacementMarkerGameObject( const PlacementMarkerGameObjectInfo& vInfo )
                        :GameObject(vInfo)
{
    m_Position = vInfo.m_Position;
    
    m_Width  = vInfo.width;
    m_Height = vInfo.height;
    m_Transform.translate(m_Position);
    m_pGameObject = NULL;
    
    m_ObjectInfo.m_ObjectTextureID = EOT_PLACEMENT_MARKER;

    m_zOrder = 1;
    
    m_pTexture = Game::getInstance()->m_pPlacementMarkerTexture;
}