//
//  GameMenuBackgroundDecorObject.cpp
//  Optiks
//
//  Created by Prashant on 02/02/14.
//
//

#include "GameMenuBackgroundDecorObject.h"
#include "Shape2D.h"
#include "Scene.h"

void GameMenuBackgroundDecorObject::render()
{
    
}

void GameMenuBackgroundDecorObject::update(float dt)
{
    rayTraceForSinkGlow();
}

void GameMenuBackgroundDecorObject::renderGlowRays(float width)
{
    
}

void GameMenuBackgroundDecorObject::setEnable(bool bEnable)
{
    
}

bool GameMenuBackgroundDecorObject::isEnable() const
{
    return m_bEnable;
}

void GameMenuBackgroundDecorObject::setColor( const Color4f& color )
{
    
}

Color4f GameMenuBackgroundDecorObject::getColor() const
{
    return m_Color;
}

void GameMenuBackgroundDecorObject::rayTraceForSinkGlow()
{
    m_SourceRay.direction = Vector2f(1.0f,0.92f);
    m_SourceRay.direction.normalize();
    m_SourceRay.origin = Vector2f(getPosition() - Vector2f(0.0f,m_DisableStateRadius*0.9f));
    Matrix3f xForm, rotXform, T, TInv;
    rotXform.rotate(m_RotAngle);
    T.translate(getPosition());
    TInv.translate(-getPosition());
    xForm = T;
    xForm *= rotXform;
    xForm *= TInv;
    
    xForm.transformAsPoint(m_SourceRay.origin);
    xForm.transformAsVector(m_SourceRay.direction);
    
    Ray startRay = m_SourceRay;
    
    m_RotAngle += 0.1f;
    
    Intersect_Info intersectInfo, intersectInfo1;
    
    m_vecInnerGlowRays.clear();
    
    for( int i=0; i<m_NumInterReflections; i++)
    {
        m_pShape2D->intersectRay(startRay, intersectInfo);
        m_pShape2D1->intersectRay(startRay, intersectInfo1);
        
        if( intersectInfo.T > intersectInfo1.T && intersectInfo1.T > 0.0)
        {
            intersectInfo = intersectInfo1;
        }
        //intersectInfo = intersectInfo.T < intersectInfo1.T ? intersectInfo : intersectInfo1;
        
        Vector2f ReflectionDir;
        Vector2f V(-intersectInfo.hitPoints[0].x + startRay.origin.x, -intersectInfo.hitPoints[0].y + startRay.origin.y);
        V.normalize();
        float NdotV = intersectInfo.hitNormal[0].dot(V);
        ReflectionDir.x = 2 * NdotV * intersectInfo.hitNormal[0].x - V.x;
        ReflectionDir.y = 2 * NdotV * intersectInfo.hitNormal[0].y - V.y;
        Vector2f rayOrigin(intersectInfo.hitPoints[0].x + ReflectionDir.x * 0.01f, intersectInfo.hitPoints[0].y + ReflectionDir.y * 0.01f);
        Ray reflectedRay(rayOrigin , ReflectionDir);
        reflectedRay.setColor(startRay.getColor());
        
        startRay.endPoint = startRay.origin + startRay.direction * intersectInfo.T;
        m_vecInnerGlowRays.push_back(startRay);
        
        startRay = reflectedRay;
    }
}

GameMenuBackgroundDecorObject::GameMenuBackgroundDecorObject(const GameMenuBackgroundDecorObjectInfo& decorGameObjectInfo) : GameObject(decorGameObjectInfo)
{
    CircleInfo cInfo;
    cInfo.radius = decorGameObjectInfo.radius;
    m_pShape2D = new(Scene::s_pBlockAllocator->Allocate(sizeof(Circle))) Circle(cInfo);
    m_pShape2D->setPosition(m_Position);
    m_pTexture = 0;
    
    m_DisableStateRadius = cInfo.radius;
    m_NumInterReflections = decorGameObjectInfo.numInterReflections;
    
    cInfo.radius *= 0.15f;
    m_pShape2D1 = new(Scene::s_pBlockAllocator->Allocate(sizeof(Circle))) Circle(cInfo);
    m_pShape2D1->setPosition(m_Position);

    m_Color = decorGameObjectInfo.color;
    m_SourceRay.setColor(m_Color);
    
}

GameMenuBackgroundDecorObject::~GameMenuBackgroundDecorObject()
{
    
}

void GameMenuBackgroundDecorObject::interpolateRadius()
{
    
}
