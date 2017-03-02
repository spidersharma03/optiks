//
//  TimeElapsedTask.cpp
//  Optiks
//
//  Created by Prashant on 05/04/14.
//
//

#include "TimeElapsedTask.h"
#include "DTimer.h"


TimeElapsedTask::TimeElapsedTask(float timeElapsed)
{
    m_TaskElapseTime = 1000.0f * timeElapsed;
    
    m_StartTime = DTimer::getTime();
}

TimeElapsedTask::~TimeElapsedTask()
{
    
}

void TimeElapsedTask::Update(float deltaSeconds)
{
    double time = (DTimer::getTime() - m_StartTime)/m_TaskElapseTime;
    
    if( time >= 1.0 )
    {
        //m_bIsCompleted = true;
        //Game::getInstance()->setLevelCompleteTaskFired(false);
        Kill();
    }
}