//
//  PhysicalGameObject.cpp
//  Optiks
//
//  Created by Prashant Sharma on 13/10/13.
//
//

#include "ShapeGameObject.h"
#include "Scene.h"
#include "Game.h"
#include "Grid.h"
#include "Shape2D.h"
#include "Renderer.h"
#include <new>

ShapeGameObject::ShapeGameObject(const ShapeGameObjectInfo& shapeGameObjectInfo)
:GameObject( shapeGameObjectInfo )
{
    Matrix3f xForm;
    xForm.translate(shapeGameObjectInfo.shapeInfo->position.x, shapeGameObjectInfo.shapeInfo->position.y);
    m_pShape2D = createShape2D(shapeGameObjectInfo);
    setTransform(xForm);
    m_eMaterialType = shapeGameObjectInfo.eMaterialType;
    
    m_Color = shapeGameObjectInfo.m_Color;
    
    if( shapeGameObjectInfo.eMaterialType == EMT_REFLECTIVE )
    {
        m_pTexture = Game::getInstance()->m_pReflectiveRectShapeTexture;
        m_pNormalMapTexture = Game::getInstance()->m_pRefractiveRectShapeNormalMapTexture;
        m_ObjectInfo.m_ObjectTextureID = EOT_SHAPE_OBJECT + EMT_REFLECTIVE;
    }
    if( shapeGameObjectInfo.eMaterialType == EMT_TRANSMISSIVE )
    {
        m_pTexture = Game::getInstance()->m_pTransmissiveRectShapeTexture;
        m_pNormalMapTexture = Game::getInstance()->m_pTransmissiveRectShapeNormalMapTexture;
        m_ObjectInfo.m_ObjectTextureID = EOT_SHAPE_OBJECT + EMT_TRANSMISSIVE;
    }
    if( shapeGameObjectInfo.eMaterialType == EMT_REFRACTIVE )
    {
        m_pTexture = Game::getInstance()->m_pRefractiveRectShapeTexture;
        m_pNormalMapTexture = Game::getInstance()->m_pRefractiveRectShapeNormalMapTexture;
        m_ObjectInfo.m_ObjectTextureID = EOT_SHAPE_OBJECT + EMT_REFRACTIVE;
    }
    if( shapeGameObjectInfo.eMaterialType == EMT_ABSORBITIVE )
    {
        m_pTexture = Game::getInstance()->m_pAbsorptiveRectShapeTexture;
        m_pNormalMapTexture = Game::getInstance()->m_pAbsorptiveRectShapeNormalMapTexture;
        m_ObjectInfo.m_ObjectTextureID = EOT_SHAPE_OBJECT + EMT_ABSORBITIVE;
    }
    if( shapeGameObjectInfo.eMaterialType == EMT_COLOR_FILTER )
    {
        m_pTexture = Game::getInstance()->m_pFilterTexture;
        m_pNormalMapTexture = Game::getInstance()->m_pFilterNormalTexture;
        m_ObjectInfo.m_ObjectTextureID = EOT_SHAPE_OBJECT + EMT_COLOR_FILTER;
    }
    m_zOrder = 1000;
    
    m_bFixed = shapeGameObjectInfo.bFixed;
    
    setFixed(m_bFixed);
    
    reset();
}

void ShapeGameObject::update(float dt)
{
    reset();
}

void ShapeGameObject::reset()
{
    // Assuming that the Shape is Rect
    for( int i=0; i<MAX_SOURCES; i++ )
    {
        for( int j=0; j<4; j++ )
        {
            m_NumRaysReflectedFromShapeEdge[i][j] = 0;
        }
    }
}

GameObject* ShapeGameObject::clone()
{
    return NULL;
}

ShapeGameObject:: ~ShapeGameObject()
{
    switch (m_pShape2D->getShapeType()) {
        case EST_CIRCLE:
        {
            Scene::s_pBlockAllocator->Free(m_pShape2D, sizeof(Circle));
            break;
        }
        case EST_CIRCULAR_ARC:
        {
            Scene::s_pBlockAllocator->Free(m_pShape2D, sizeof(CircularArc));
            break;
        }
        case EST_LENS:
        {
            Scene::s_pBlockAllocator->Free(m_pShape2D, sizeof(SphericalLens));
            break;
        }
        case EST_LINE:
        {
            Scene::s_pBlockAllocator->Free(m_pShape2D, sizeof(LineSegment));
            break;
        }
        case EST_PARABOLIC_SEGMENT:
        {
            Scene::s_pBlockAllocator->Free(m_pShape2D, sizeof(ParabolicSegment));
            break;
        }
        case EST_RECT:
        {
            Scene::s_pBlockAllocator->Free(m_pShape2D, sizeof(RectangleShape));
            break;
        }
        case EST_REGULAR_POLY:
        {
            Scene::s_pBlockAllocator->Free(m_pShape2D, sizeof(RegularPolygon));
            break;
        }
        default:
            break;
    }
}

Shape2D* ShapeGameObject::createShape2D(const ShapeGameObjectInfo& shapeGameObjectInfo)
{
    Shape2D* pShape2D = NULL;
    
    switch (shapeGameObjectInfo.shapeInfo->shapeType) {
        case EST_CIRCLE:
        {
            CircleInfo* cInfo = (CircleInfo*)shapeGameObjectInfo.shapeInfo;
            pShape2D = new(Scene::s_pBlockAllocator->Allocate(sizeof(Circle)))
            Circle(*cInfo);
            break;
        }
        case EST_CIRCULAR_ARC:
        {
            CircularArcInfo* cInfo = (CircularArcInfo*)shapeGameObjectInfo.shapeInfo;
            pShape2D = new(Scene::s_pBlockAllocator->Allocate(sizeof(CircularArc)))
            CircularArc(*cInfo);
            break;
        }
        case EST_LENS:
        {
            SphericalLensInfo* lensInfo = (SphericalLensInfo*)shapeGameObjectInfo.shapeInfo;
            pShape2D = new(Scene::s_pBlockAllocator->Allocate(sizeof(SphericalLens)))
            SphericalLens(*lensInfo);
            break;
        }
        case EST_LINE:
        {
            LineSegmentInfo* lInfo = (LineSegmentInfo*)shapeGameObjectInfo.shapeInfo;
            pShape2D = new(Scene::s_pBlockAllocator->Allocate(sizeof(LineSegment)))
            LineSegment(*lInfo);
            break;
        }
        case EST_PARABOLIC_SEGMENT:
        {
            ParabolicSegmentInfo* pInfo = (ParabolicSegmentInfo*)shapeGameObjectInfo.shapeInfo;
            pShape2D = new(Scene::s_pBlockAllocator->Allocate(sizeof(ParabolicSegment)))
            ParabolicSegment(*pInfo);
            break;
        }
        case EST_RECT:
        {
            RectangleInfo* rInfo = (RectangleInfo*)shapeGameObjectInfo.shapeInfo;
            pShape2D = new(Scene::s_pBlockAllocator->Allocate(sizeof(RectangleShape)))
            RectangleShape(*rInfo);
            break;
        }
        case EST_REGULAR_POLY:
        {
            RegularPolygonInfo* polyInfo = (RegularPolygonInfo*)shapeGameObjectInfo.shapeInfo;
            pShape2D = new(Scene::s_pBlockAllocator->Allocate(sizeof(RegularPolygon)))
            RegularPolygon(*polyInfo);
            break;
        }
        default:
            break;
    }
    return pShape2D;
}

void ShapeGameObject::setTransform(const Matrix3f& transform)
{
    m_Transform = transform;
    m_pShape2D->setTransform(transform);
}

void ShapeGameObject::setPosition( const Vector2f& position )
{
    m_Position = position;
    m_pShape2D->setPosition(position);
}

void ShapeGameObject::setRotation( float rotation )
{
    m_Rotation = rotation;
    m_pShape2D->rotate(rotation);
}

void ShapeGameObject::render()
{
    float scale = 0.5f;
    
    Matrix4f xForm;
    xForm.set(m_pShape2D->getTransform());
    Game::getInstance()->getRenderer()->setTransform(xForm, spider3d::WORLD);
    
    int width = ((RectangleShape*)m_pShape2D)->getWidth();
    int height = ((RectangleShape*)m_pShape2D)->getHeight();
    
    float vertices[8];
    float texCoords[8];
    float colors[16];
    
    vertices[0] = -width*scale;
    vertices[1] = -height*scale;
    
    texCoords[0] = 0.0f;
    texCoords[1] = 0.0f;
    
    vertices[2] =  width*scale;
    vertices[3] = -height*scale;
    
    texCoords[2] = 1.0f;
    texCoords[3] = 0.0f;
    
    vertices[4] =  width*scale;
    vertices[5] =  height*scale;
    
    texCoords[4] = 1.0f;
    texCoords[5] = 1.0f;
    
    vertices[6] = -width*scale;
    vertices[7] = height*scale;
    
    texCoords[6] = 0.0f;
    texCoords[7] = 1.0f;
    
    for( int i=0; i<16; i+=4 )
    {
        colors[i] = m_Color.r;
        colors[i+1] = m_Color.g;
        colors[i+2] = m_Color.b;
        colors[i+3] = m_Color.a;
    }
    
    Game::getInstance()->getRenderer()->drawVertexPrimitiveList(4, vertices, 2, colors, 4, texCoords, 2, 0, spider3d::EPT_TRIANGLE_FAN);
}

bool ShapeGameObject::pick(const Vector2f& point) const
{
    return m_pShape2D->pick(point);
}

// Determine no of hits for edges, and flag the ray to terminate.
void ShapeGameObject::increaseRayHitCount(Ray& ray)
{
    IntersectionInformation intersectInfo;
    
    m_pShape2D->intersectRay(ray, intersectInfo);

    int hitEdge = ((RectangleShape*)m_pShape2D)->getHitEdge();
    
    m_NumRaysReflectedFromShapeEdge[ray.m_SourceID][hitEdge]++;
    
    // if the Ray hits more than once for a given edge terminate it.
    if( m_NumRaysReflectedFromShapeEdge[ray.m_SourceID][hitEdge] > 1)
    {
        ray.m_bUserTerminate = true;
    }
}

bool ShapeGameObject::intersectRay( Ray& ray, IntersectionInformation& intersectInfo )
{
    bool bRes = m_pShape2D->intersectRay(ray, intersectInfo);
    int hitEdge = -1;
    if( bRes )
    {
        hitEdge = ((RectangleShape*)m_pShape2D)->getHitEdge();
        // Check if this ray from given source is hitting any edge more than once. if yes ignore this hit!!!
        if( m_NumRaysReflectedFromShapeEdge[ray.m_SourceID][hitEdge] > 1)
        {
            bRes = false;
        }
    }
    
    return bRes;
}

void ShapeGameObject::setFixed( bool bFixed )
{
    m_bFixed = bFixed;
    
    if( bFixed )
    {
        if( m_eMaterialType == EMT_ABSORBITIVE )
            m_pTexture = Game::getInstance()->m_pAbsorptiveRectShapeFixedTexture;
        if( m_eMaterialType == EMT_REFLECTIVE )
            m_pTexture = Game::getInstance()->m_pReflectiveRectShapeFixedTexture;
        if( m_eMaterialType == EMT_TRANSMISSIVE )
            m_pTexture = Game::getInstance()->m_pTransmissiveRectShapeFixedTexture;
        if( m_eMaterialType == EMT_COLOR_FILTER )
            m_pTexture = Game::getInstance()->m_pFilterFixedTexture;
    }
    else
    {
        if( m_eMaterialType == EMT_ABSORBITIVE )
            m_pTexture = Game::getInstance()->m_pAbsorptiveRectShapeTexture;
        if( m_eMaterialType == EMT_REFLECTIVE )
            m_pTexture = Game::getInstance()->m_pReflectiveRectShapeTexture;
        if( m_eMaterialType == EMT_TRANSMISSIVE )
            m_pTexture = Game::getInstance()->m_pTransmissiveRectShapeTexture;
        if( m_eMaterialType == EMT_COLOR_FILTER )
            m_pTexture = Game::getInstance()->m_pFilterTexture;
    }
}

void ShapeGameObject::save(FILE* pFile)
{
    fprintf(pFile, "IS_FIXED::\n");
    fprintf(pFile, "%d\n", m_bFixed);
    fprintf(pFile, "ID::\n");
    fprintf(pFile, "%d\n", m_ID);
    fprintf(pFile, "%s\n", "SHAPE_COLOR::");
    fprintf(pFile, "%d %d %d\n",(int)(m_Color.r*255), (int)(m_Color.g*255), (int)(m_Color.b*255) );
    m_pShape2D->save(pFile);
}

void ShapeGameObject::load(FILE* pFile)
{
    m_pShape2D->load(pFile);
}