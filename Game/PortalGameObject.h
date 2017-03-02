//
//  PortalGameObject.h
//  Optiks
//
//  Created by Prashant on 03/01/14.
//
//

#ifndef __Optiks__PortalGameObject__
#define __Optiks__PortalGameObject__

#include "GameObject.h"
#include "Ray.h"
#include "Shape2D.h"
#include <vector>

using namespace std;

struct PortalGameObjectInfo : public GameObjectInfo
{
    PortalGameObjectInfo()
    {
        m_ObjectType = EOT_PORTAL;
        width        = 0.0f;
        height       = 0.0f;
    }
    float width;
    float height;
    ShapeInfo *shapeInfo1;
    ShapeInfo *shapeInfo2;
};

class Shape2D;

class PortalGameObject : public GameObject
{
public:
    
    virtual void setTransform(const Matrix3f& transform);
    
    virtual void setPosition( const Vector2f& position );
    
    virtual void setRotation( float rotation );
    
    void         reset();
    
    void         increaseRayHitCount(Ray& ray);
    
    virtual bool intersectRay( Ray& ray, IntersectionInformation& intersectInfo );
    
    Shape2D*     getPortalShape1() const
    {
        return m_pPortalShape1;
    }
    
    Shape2D*     getPortalShape2() const
    {
        return m_pPortalShape2;
    }
    
    Shape2D*     getGlowShape1() const
    {
        return m_pGlowShape1;
    }
    
    Shape2D*     getGlowShape2() const
    {
        return m_pGlowShape2;
    }

    Shape2D*     getSelectedShape() const
    {
        return m_pSelectedShape;
    }
    
    // GameObject virtual methods
    virtual void render();
    
    void renderGlow();

    virtual Vector2f getClosestGridCentre() const;

    virtual void update(float dt);
        
    virtual bool pick(const Vector2f& point) const;
    
    virtual void save(FILE* pFile);
    
    virtual void load(FILE* pFile);
    
    Ray          getPortalRay() const;
    
    void        rayTraceForGlow();
    void        renderGlowRays(float width = 1.0f);
    
    virtual ~PortalGameObject();
    
    vector<Ray>&            getInnerGlowRays()
    {
        return m_vecInnerGlowRays;
    }
protected:
    
    PortalGameObject( const PortalGameObjectInfo& pInfo );
    
    friend class Scene;
    
    Shape2D     *m_pPortalShape1;
    Shape2D     *m_pPortalShape2;
    mutable Shape2D     *m_pSelectedShape;
    
    // Number of Rays reflected from the Shapes edges.
    int     m_NumRaysReflectedFromShape1Edge[MAX_SOURCES][4];
    int     m_NumRaysReflectedFromShape2Edge[MAX_SOURCES][4];
    
protected:
    
    void                rayTraceForGlow(const Vector2f& centre, Shape2D* pShape);

    float               m_Width;
    float               m_Height;
    Vector2f            m_PortalRayOrigin;
    Vector2f            m_PortalRayDirection;
    int                 m_PortalEdgeMap[4];
    Vector2f            m_LocalPortalRayOriginPoints[4];
    
    vector<Ray>         m_vecInnerGlowRays;
    Ray                 m_SourceRay;
    float               m_RotAngle;
    Shape2D             *m_pGlowShape1;
    Shape2D             *m_pGlowShape2;
    int                 m_NumInterReflections;
    float               m_GlowShapeRadius;
    
    int                 m_FirstHitPortalShapeIndex;
};

#endif /* defined(__Optiks__PortalGameObject__) */
