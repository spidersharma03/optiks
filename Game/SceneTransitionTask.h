//
//  SceneTransitionTask.h
//  Optiks
//
//  Created by Prashant on 09/03/14.
//
//

#ifndef __Optiks__SceneTransitionTask__
#define __Optiks__SceneTransitionTask__

#include <iostream>
#include "TaskBase.h"
#include "Color4.h"
#include <string>
#include "IState.h"

using namespace std;
USE_NAMESPACE_DIRAC2D

class Game;

class SceneTransitionTask : public TaskBase
{
public:
    
    SceneTransitionTask(IState<Game>* pTansitionState, const char* levelFileName, float sceneWidth, float sceneHeight, float lerpTime, const Color4f& color = Color4f(0.0f,0.0f,0.0f,1.0f) );
    
    ~SceneTransitionTask();
    
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
    string                      m_LevelFileName;
    
    Color4f                     m_Color;
    bool                        m_bLevelLoaded;
    IState<Game>                *m_pTansitionState;
};

#endif /* defined(__Optiks__SceneTransitionTask__) */
