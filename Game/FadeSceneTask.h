//
//  FadeSceneTask.h
//  Optiks
//
//  Created by Prashant on 15/03/14.
//
//

#ifndef __Optiks__FadeSceneTask__
#define __Optiks__FadeSceneTask__

#include <iostream>
#include "TaskBase.h"
#include "Color4.h"
#include <string>
#include "IState.h"

using namespace std;
USE_NAMESPACE_DIRAC2D

class Game;

class FadeSceneTask : public TaskBase
{
public:
    
    FadeSceneTask(float sceneWidth, float sceneHeight, float startTransparency, float endTransparency , float lerpTime);
    
    ~FadeSceneTask();
    
    virtual void Update(float deltaSeconds);
    
    void         reset();
    
    void setStartTransparency( float startTransparency )
    {
        m_StartTransparency = startTransparency;
    }
    
    void setEndTransparency( float endTransparency )
    {
        m_EndTransparency = endTransparency;
    }

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
    
    float                       m_StartTransparency;
    float                       m_EndTransparency;
    bool                        m_bIsCompleted;
    float                       m_SceneWidth;
    float                       m_SceneHeight;
};


#endif /* defined(__Optiks__FadeSceneTask__) */
