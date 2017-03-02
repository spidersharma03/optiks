//
//  GameMenuBackgroundDecorObject.h
//  Optiks
//
//  Created by Prashant on 02/02/14.
//
//

#ifndef __Optiks__GameMenuBackgroundDecorObject__
#define __Optiks__GameMenuBackgroundDecorObject__

#include <iostream>
#include "GameObject.h"
#include "Color4.h"
#include "Ray.h"
#include <vector>

using namespace std;

struct GameMenuBackgroundDecorObjectInfo : public GameObjectInfo
{
    GameMenuBackgroundDecorObjectInfo()
    {
        numInterReflections = 20;
        radius = 100.0f;
        m_ObjectType = EOT_DECOR;
    }
    int numInterReflections;
    Color4f  color;
    float radius;
};

class GameMenuBackgroundDecorObject : public GameObject
{
public:
    
    // GameObject virtual methods
    virtual void            render();
    virtual void            update(float dt);
    
    void                    renderGlowRays(float width = 1.0f);
    
    void                    setEnable(bool bEnable);
    bool                    isEnable() const;
    
    void                    setColor( const Color4f& color );
    Color4f                 getColor() const;
    
    void                    rayTraceForSinkGlow();
    
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
    
    GameMenuBackgroundDecorObject(const GameMenuBackgroundDecorObjectInfo& decorGameObjectInfo);
    
    ~GameMenuBackgroundDecorObject();
    
    void                    interpolateRadius();
    
    friend class Scene;
    
    bool                    m_bEnable;
    Ray                     m_SourceRay;
    vector<Ray>             m_vecInnerGlowRays;
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
    Shape2D                 *m_pShape2D1;
    
    friend class SinkObjectGlowInterpolatorTask;
    friend class LevelCompleteActionTask;
};

#endif /* defined(__Optiks__GameMenuBackgroundDecorObject__) */
