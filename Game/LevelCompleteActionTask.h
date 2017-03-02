//
//  LevelCompleteActionTask.h
//  Optiks
//
//  Created by Prashant on 28/12/13.
//
//

#ifndef __Optiks__LevelCompleteActionTask__
#define __Optiks__LevelCompleteActionTask__

#include <iostream>
#include "TaskBase.h"
#include "Color4.h"


USE_NAMESPACE_DIRAC2D

class SinkGameObject;

class LevelCompleteActionTask : public TaskBase
{
    public:
    
    LevelCompleteActionTask();
    
    ~LevelCompleteActionTask();
    
    virtual void Update(float deltaSeconds);
    
    bool         isTaskCopleted()
    {
        return m_bIsCompleted;
    }
    
    private:
    SinkGameObject              *m_pSinkGameObject;
    double                      m_Time;
    double                      m_StartTime;
    
    bool                        m_bIsCompleted;
    float                       m_TaskCompletionTime;
    Color4f                     m_StartColor;
    Color4f                     m_EndColor;
    
    bool                        m_bOnce;
};

#endif /* defined(__Optiks__LevelCompleteActionTask__) */
