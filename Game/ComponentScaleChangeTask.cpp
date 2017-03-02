//
//  ComponentScaleChangeTask.cpp
//  Optiks
//
//  Created by Prashant on 08/03/14.
//
//

#include "ComponentScaleChangeTask.h"
#include "DTimer.h"
#include "GUIComponent.h"

ComponentScaleChangeTask::ComponentScaleChangeTask( GUIComponent* pGUIComponent, float startScale, float endScale, float lerpTime )
{
    m_pGUIComponent = pGUIComponent;
    m_StartScale = startScale;
    m_EndScale   = endScale;
    m_LerpTime   = lerpTime * 1000;
    m_StartTime  = DTimer::getTime();
}

ComponentScaleChangeTask::~ComponentScaleChangeTask()
{
    
}

void ComponentScaleChangeTask::Update(float deltaSeconds)
{
    m_Time = (DTimer::getTime() - m_StartTime)/m_LerpTime;
    //m_Time = 3.0f * m_Time * m_Time - 2.0f * m_Time * m_Time * m_Time;
    
    float lerpFactor = m_StartScale * ( 1.0 - m_Time ) + m_EndScale * m_Time;
    //printf("lerp = %f\n", m_StartTime);
    
    m_pGUIComponent->setScale(lerpFactor, lerpFactor);
    
    if( m_Time >= 1.0 )
        Kill();
}