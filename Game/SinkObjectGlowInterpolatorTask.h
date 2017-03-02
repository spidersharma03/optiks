//
//  SinkObjectGlowInterpolatorTask.h
//  Optiks
//
//  Created by Prashant on 27/12/13.
//
//

#ifndef __Optiks__SinkObjectGlowInterpolatorTask__
#define __Optiks__SinkObjectGlowInterpolatorTask__

#include "TaskBase.h"
#include "Color4.h"

USE_NAMESPACE_DIRAC2D

class SinkGameObject;

class SinkObjectGlowInterpolatorTask : public TaskBase
{
public:
    
    SinkObjectGlowInterpolatorTask( SinkGameObject* pSinkGameObject, const Color4f& startColor, const Color4f& endColor, float startFactor, float endFactor, float startRadius, float endRadius, float lerpTime );
    
    ~SinkObjectGlowInterpolatorTask();
    
    virtual void Update(float deltaSeconds);

private:
    SinkGameObject              *m_pSinkGameObject;
    double                      m_Time;
    double                      m_StartTime;
    
    float                       m_LerpTime;
    Color4f                     m_StartColor;
    Color4f                     m_EndColor;
    float                       m_StartFactor;
    float                       m_EndFactor;
    float                       m_StartRadius;
    float                       m_EndRadius;
};

#endif /* defined(__Optiks__SinkObjectGlowInterpolatorTask__) */
