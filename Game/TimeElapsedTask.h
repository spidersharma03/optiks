//
//  TimeElapsedTask.h
//  Optiks
//
//  Created by Prashant on 05/04/14.
//
//

#ifndef __Optiks__TimeElapsedTask__
#define __Optiks__TimeElapsedTask__

#include <iostream>
//
//  LevelCompleteActionTask.h
//  Optiks
//
//  Created by Prashant on 28/12/13.
//
//

#include <iostream>
#include "TaskBase.h"
#include "Color4.h"


USE_NAMESPACE_DIRAC2D

class SinkGameObject;

class TimeElapsedTask : public TaskBase
{
public:
    
    TimeElapsedTask(float m_TaskElapseTime);
    
    ~TimeElapsedTask();
    
    virtual void Update(float deltaSeconds);
    
private:
    double                      m_Time;
    double                      m_StartTime;
    
    float                       m_TaskElapseTime;
    Color4f                     m_StartColor;
    Color4f                     m_EndColor;
};


#endif /* defined(__Optiks__TimeElapsedTask__) */
