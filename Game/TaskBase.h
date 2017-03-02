//
//  TaskBase.h
//  RTSGame
//
//  Created by Joe Engalan on 7/18/13.
//  Copyright (c) 2013 Sourcebits. All rights reserved.
//

#ifndef __RTSGame__TaskBase__
#define __RTSGame__TaskBase__

#include <iostream>
#include "ITask.h"

class TaskBase : public ITask
{
public:
    TaskBase();
    virtual ~TaskBase();
    
    virtual void Init();
    
    // ITask
    virtual void Update(float deltaSeconds);
    bool IsDead() const;
    void Destroy();
    ITask* PopNextTask();
    void Add(ITask* task);
    
    void Kill();
    
private:
    bool m_isInitialized;
    bool m_isDead;
    ITask* m_pNextTask;
};

#endif /* defined(__RTSGame__TaskBase__) */
