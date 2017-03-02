//
//  GamePauseTransitionTask.h
//  Optiks
//
//  Created by Prashant on 12/03/14.
//
//

#ifndef __Optiks__GamePauseTransitionTask__
#define __Optiks__GamePauseTransitionTask__

#include <iostream>
#include "TaskBase.h"
#include "Color4.h"

using namespace std;
USE_NAMESPACE_DIRAC2D

class GamePauseHUDController;

class GamePauseTransitionTask : public TaskBase
{
public:
    
    GamePauseTransitionTask( GamePauseHUDController* pHUDController, float sceneWidth, float sceneHeight, float lerpTime, float startTransparency, float endTransparency );
    
    ~GamePauseTransitionTask();
    
    virtual void Update(float deltaSeconds);
    
    void         reset();
    
    bool         isTaskCopleted()
    {
        return m_bIsCompleted;
    }
    
    float       getCurrentLerpTime()
    {
        return m_Time;
    }
    
private:
    double                      m_LerpTime;
    double                      m_StartTime;
    double                      m_Time;
    
    bool                        m_bIsCompleted;
    float                       m_TaskCompletionTime;
    float                       m_SceneWidth;
    float                       m_SceneHeight;
    
    Color4f                     m_Color;
    float                       m_StartTransparency;
    float                       m_EndTransparency;
    
    GamePauseHUDController      *m_pGamePauseHUDController;
};

#endif /* defined(__Optiks__GamePauseTransitionTask__) */
