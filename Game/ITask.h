//
//  ITask.h
//  RTSGame
//
//  Created by Joe Engalan on 7/18/13.
//  Copyright (c) 2013 Sourcebits. All rights reserved.
//

#ifndef RTSGame_ITask_h
#define RTSGame_ITask_h

class ITask
{
public:
    virtual void Update(float deltaSeconds) = 0;
    virtual bool IsDead() const = 0;
    virtual void Destroy() = 0;
    virtual ITask* PopNextTask() = 0;
    virtual void Add(ITask* task) = 0;
};

#endif
