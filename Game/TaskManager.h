//
//  TaskManager.h
//  RTSGame
//
//  Created by Joe Engalan on 7/18/13.
//  Copyright (c) 2013 Sourcebits. All rights reserved.
//

#ifndef __RTSGame__TaskManager__
#define __RTSGame__TaskManager__

#include <iostream>
#include <list>

class ITask;

typedef std::list<ITask*> TaskList;

class TaskManager
{
public:
    TaskManager();
    ~TaskManager();
    
    void Update(float deltaSeconds);
    void Attach(ITask* task);

    int NumTasks() const;
    
protected:
    TaskList& Tasks();
    
private:
    void PruneDeadTasks(const TaskList& deadTasks);
    
    TaskList m_Tasks;
};

#endif /* defined(__RTSGame__TaskManager__) */
