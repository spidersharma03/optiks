//
//  SinkObjectGlowInterpolatorTask.cpp
//  Optiks
//
//  Created by Prashant on 27/12/13.
//
//

#include "SinkObjectGlowInterpolatorTask.h"
#include "SinkGameObject.h"
#include "DTimer.h"

SinkObjectGlowInterpolatorTask::SinkObjectGlowInterpolatorTask( SinkGameObject* pSinkGameObject, const Color4f& startColor, const Color4f& endColor, float startFactor, float endFactor, float startRadius, float endRadius, float lerpTime )
{
    m_pSinkGameObject = pSinkGameObject;
    m_StartTime = DTimer::getTime();
    m_LerpTime  = lerpTime * 1000;
    m_StartColor = startColor;
    m_EndColor = endColor;
    m_StartFactor = startFactor;
    m_EndFactor = endFactor;
    m_StartRadius = startRadius;
    m_EndRadius = endRadius;
}

SinkObjectGlowInterpolatorTask::~SinkObjectGlowInterpolatorTask()
{
    
}

void SinkObjectGlowInterpolatorTask::Update(float deltaSeconds)
{
    m_Time = (DTimer::getTime() - m_StartTime)/m_LerpTime;
    //m_Time = 3.0f * m_Time * m_Time - 2.0f * m_Time * m_Time * m_Time;
    
    float lerpFactor = m_StartFactor * ( 1.0 - m_Time ) + m_EndFactor * m_Time;
    m_pSinkGameObject->m_CurrentRayWidthMultFactor = lerpFactor;
    
    Color4f lerpColor = m_StartColor * ( 1.0 - m_Time ) + m_EndColor * m_Time;
    m_pSinkGameObject->m_CurrentRayColor = lerpColor;
    
    float lerpRadius = m_StartRadius * ( 1.0 - m_Time ) + m_EndRadius * m_Time;
    float minRadius = m_StartRadius, maxRadius = m_EndRadius;
    if( minRadius > maxRadius )
    {
        float temp = maxRadius;
        maxRadius = minRadius;
        minRadius = temp;
    }
    lerpRadius = lerpRadius < minRadius ? minRadius : lerpRadius;
    lerpRadius = lerpRadius > maxRadius ? maxRadius : lerpRadius;
    
    m_pSinkGameObject->m_CurrentRadius = lerpRadius;
    
    if( m_Time >= 1.0 )
        Kill();
}