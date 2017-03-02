//
//  SourceGameObject.cpp
//  Optiks
//
//  Created by Prashant Sharma on 15/10/13.
//
//

#include "SourceGameObject.h"
#include "OglRenderer.h"
#include "Shape2D.h"
#include "Scene.h"
#include "Game.h"
#include "Grid.h"
#include <math.h>

using namespace spider3d::render;

int SourceGameObject::s_SourceCount = 0;

SourceGameObject::SourceGameObject(const SourceGameObjectInfo& sourceGameObjectInfo)
:GameObject(sourceGameObjectInfo)
{
    m_Ray.origin.x    = sourceGameObjectInfo.origin.x;
    m_Ray.origin.y    = sourceGameObjectInfo.origin.y;
    
    m_Ray.origin      = m_Position;
    
    m_Ray.direction.x = sourceGameObjectInfo.direction.x;
    m_Ray.direction.y = sourceGameObjectInfo.direction.y;
    m_Ray.direction.normalize();
    
    m_Color = sourceGameObjectInfo.color;
    m_Ray.setColor(m_Color);

    m_ObjectInfo.m_ObjectTextureID = EOT_LIGHT_SOURCE;

    m_ClosestGridCentre = sourceGameObjectInfo.closestGridCentre;
    
    m_PickingRadius   = 20.0f;
    m_Transform.translate(sourceGameObjectInfo.origin);
    m_zOrder = 100;
    
    m_NumInterReflections = 10;
    
    CircleInfo cInfo;
    cInfo.radius = 5.0f;
    m_pShape2D = new(Scene::s_pBlockAllocator->Allocate(sizeof(Circle))) Circle(cInfo);
    m_SourceID = s_SourceCount;
    m_Ray.m_SourceID = m_SourceID;

    s_SourceCount++;
}

SourceGameObject::~SourceGameObject()
{
    s_SourceCount--;
}

int SourceGameObject::getSourceID() const
{
    return m_SourceID;
}

void SourceGameObject::setOrigin(const Vector2f& origin)
{
    m_Ray.origin.x = origin.x;
    m_Ray.origin.y = origin.y;
    m_pShape2D->setPosition(origin);
}

void SourceGameObject::setDirection(const Vector2f& direction)
{
    m_Ray.direction.x = direction.x;
    m_Ray.direction.y = direction.y;
    m_Ray.direction.normalize();
}

void SourceGameObject::setColor(const Color4f& color)
{
    m_Color = color;
    m_Ray.setColor(m_Color);
}

Color4f SourceGameObject::getColor() const
{
    return m_Color;
}

void SourceGameObject::setTransform(const Matrix3f& transform)
{
    m_Transform = transform;
    Vector2f dir(-1,-1);
    m_Transform.transformAsVector(dir);
    m_Ray.origin = Vector2f(m_Transform.col3.x, m_Transform.col3.y);
    m_Ray.direction = dir;
    m_Position = m_Ray.origin;
    m_pShape2D->setPosition(m_Ray.origin);
}

void SourceGameObject::setPosition( const Vector2f& position )
{
    m_Position = position;
    m_Ray.origin = position;
    m_Transform.translate(m_Position);
    m_pShape2D->setPosition(m_Ray.origin);
}

void SourceGameObject::setRotation( float rotation )
{
    m_Rotation = rotation;
    m_Transform.rotate(rotation);
    Vector2f dir(-1,-1);
    m_Transform.transformAsVector(dir);
    m_Ray.direction = dir;
}

void SourceGameObject::update(float dt)
{
    rayTraceGlowRays();
}

void SourceGameObject::render()
{
    //Matrix4f xForm;
    //Game::getInstance()->getRenderer()->setTransform(xForm, spider3d::WORLD);
    //Game::getInstance()->getRenderer()->setTextureObject(NULL);
    //Game::getInstance()->getRenderer()->DrawPoint(m_Position, Color4f(1.0f,0.7f,0.0f,0.0f));
}

void SourceGameObject::renderGlowRays(float width )
{
    for( int i=0; i<m_vecInnerGlowRays.size(); i++)
    {
        width *= 0.95f;
        m_vecInnerGlowRays[i].render(width, Color4f(1.0f,0.9f,0.70f,1.0f));
    }
}

void SourceGameObject::rayTraceGlowRays()
{
    m_vecInnerGlowRays.clear();
    
    m_SourceRay.direction = Vector2f(1.0f,0.95f);
    m_SourceRay.direction.normalize();
    m_SourceRay.origin = Vector2f(m_Ray.origin - Vector2f(0.0f,4.0f));
    
    Ray startRay = m_SourceRay;
        
    Intersect_Info intersectInfo;
    
    for( int i=0; i<m_NumInterReflections; i++)
    {
        m_pShape2D->intersectRay(startRay, intersectInfo);
        
        Vector2f ReflectionDir;
        Vector2f V(-intersectInfo.hitPoints[0].x + startRay.origin.x, -intersectInfo.hitPoints[0].y + startRay.origin.y);
        V.normalize();
        float NdotV = intersectInfo.hitNormal[0].dot(V);
        ReflectionDir.x = 2 * NdotV * intersectInfo.hitNormal[0].x - V.x;
        ReflectionDir.y = 2 * NdotV * intersectInfo.hitNormal[0].y - V.y;
        Vector2f rayOrigin(intersectInfo.hitPoints[0].x + ReflectionDir.x * 0.01f, intersectInfo.hitPoints[0].y + ReflectionDir.y * 0.01f);
        Ray reflectedRay(rayOrigin , ReflectionDir);
        
        startRay.endPoint = startRay.origin + startRay.direction * intersectInfo.T;
        Color4f color;
        color.set(m_Color.r + 0.2f, m_Color.g + 0.2f, m_Color.b + 0.2f,1.0f);

        startRay.setColor(color);
        
        m_vecInnerGlowRays.push_back(startRay);
        
        startRay = reflectedRay;
    }
}

bool SourceGameObject::pick(const Vector2f& point) const
{
    bool bRes = m_Ray.origin.distanceSquared(point) < m_PickingRadius*m_PickingRadius;
    return bRes;
}

void SourceGameObject::setClosestGridCentre(const Vector2f& centre)
{
    m_ClosestGridCentre = centre;
}

Vector2f SourceGameObject::getClosestGridCentre() const
{
    return m_ClosestGridCentre;
}

void SourceGameObject::save(FILE* pFile)
{
    fprintf(pFile, "%s\n", "SOURCE_COLOR::");
    fprintf(pFile, "%d %d %d\n",(int)(m_Color.r*255), (int)(m_Color.g*255), (int)(m_Color.b*255) );
    fprintf(pFile, "%s\n", "SOURCE_ORIGIN::");
    Vector2i gridPosition = getGridCoordinate();
    fprintf(pFile, "%d  %d\n", gridPosition.x, gridPosition.y);
    
    fprintf(pFile, "%s\n", "SOURCE_DIRECTION::");
    fprintf(pFile, "%f  %f\n", m_Ray.direction.x, m_Ray.direction.y);
    fprintf(pFile, "%s\n", "CLOSEST_GRID_CENTRE::");
    fprintf(pFile, "%f %f\n", m_ClosestGridCentre.x, m_ClosestGridCentre.y);
}

void SourceGameObject::load(FILE* pFile)
{
    
}

// Source Grid Coordinates are given at half intervals.
Vector2i SourceGameObject::getGridCoordinate() const
{
    Grid* pGrid = Game::getInstance()->getScene()->getGrid();
    float halfCellSize = pGrid->getCellSize() * 0.5f;
    Vector2i gridCoord( nearbyintf(m_Position.x/halfCellSize), nearbyintf(m_Position.y/halfCellSize) );
    return gridCoord;
}