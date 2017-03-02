//
//  SinkGameObject.h
//  Optiks
//
//  Created by Prashant Sharma on 08/11/13.
//
//

#ifndef __Optiks__SinkGameObject__
#define __Optiks__SinkGameObject__

#include <iostream>

#include "ShapeGameObject.h"
#include "Ray.h"
#include <vector>

using namespace std;

struct SinkGameObjectInfo : public GameObjectInfo
{
    SinkGameObjectInfo()
    {
        m_ObjectType = EOT_SINK;
    }
    Vector2f closestGridCentre;
    Color4f  color;
};

class ParticleSystem;
class ParticleOrbitCentreAnimator;
class ParticleEmitter;

class SinkGameObject : public GameObject
{
public:
    
    // GameObject virtual methods
    virtual void            setTransform(const Matrix3f& transform);
    virtual void            setPosition( const Vector2f& position );
    virtual void            setRotation( float rotation );
    
    virtual void            render();
    virtual void            update(float dt);
    
    void                    renderGlowRays(float width = 1.0f);
    
    virtual bool            pick(const Vector2f& point) const;
    virtual bool            intersectRay( Ray& ray, IntersectionInformation& intersectInfo );

    void                    setEnable(bool bEnable);
    bool                    isEnable() const;

    Vector2i                getGridCoordinate() const;

    virtual void            save(FILE* pFile);
    virtual void            load(FILE* pFile);
    void                    setRayExitEnable( bool bEnable );// if true then the ray which exits from
                            // the Sink will not be processed further.
    bool                    getRayExitEnable() const
    {
        return m_bRayTerminate;
    }
    
    void                    setColor( const Color4f& color );
    Color4f                 getColor() const;
    
    virtual void            setClosestGridCentre(const Vector2f& centre);
    virtual Vector2f        getClosestGridCentre() const;
    void                    rayTraceForSinkGlow();
    void                    pushRayForSinkEnableCheck(Ray& ray);
    void                    checkForRayColorMatch();
    
    vector<Ray>&            getInnerGlowRays()
    {
        return m_vecInnerGlowRays;
    }
    
    float                   getCurrentRayWidthMultFactor()
    {
        return m_CurrentRayWidthMultFactor;
    }
    
    Shape2D*                getShape2D() const
    {
        return m_pShape2D;
    }
protected:
    
    SinkGameObject(const SinkGameObjectInfo& sinkGameObjectInfo);

    ~SinkGameObject();

    void                    interpolateRadius();

    friend class Scene;
    
    bool                    m_bRayTerminate;
    bool                    m_bEnable;
    Ray                     m_SourceRay;
    vector<Ray>             m_vecInnerGlowRays;
    vector<Ray>             m_vecSinkIntersectionRays;
    
    int                     m_NumInterReflections;
    float                   m_RotAngle;
    float                   m_Radius, m_CurrentRadius;
    float                   interPolateRadiusTime;
    
    float                   m_CurrentRayWidthMultFactor;
    float                   m_DisableStateRayWidthMultFactor;
    float                   m_EnableStateRayWidthMultFactor;
    float                   m_EnableStateRadius;
    float                   m_DisableStateRadius;
    
    Color4f                 m_DisableStateRayColor;
    Color4f                 m_EnableStateRayColor;
    Color4f                 m_CurrentRayColor;
    Vector2f                m_ClosestGridCentre;
    bool                    m_bFireSinkGlowEnableTask;
    bool                    m_bFireSinkGlowDisableTask;
    float                   m_PickingRadius;
    
    Color4f                 m_Color;
    
    Shape2D                 *m_pShape2D;
    
    ParticleSystem          *m_pParticleSystem;
    ParticleOrbitCentreAnimator *m_pAnimator;
    ParticleEmitter         *m_pEmitter;
    
    friend class SinkObjectGlowInterpolatorTask;
    friend class LevelCompleteActionTask;
};

#endif /* defined(__Optiks__SinkGameObject__) */
