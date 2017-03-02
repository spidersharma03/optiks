//
//  TaskManager.cpp
//  RTSGame
//
//  Created by Joe Engalan on 7/18/13.
//  Copyright (c) 2013 Sourcebits. All rights reserved.
//

#include "TaskManager.h"
#include "ITask.h"

TaskManager::TaskManager()
{
    
}

TaskManager::~TaskManager()
{
    for(TaskList::iterator it = m_Tasks.begin(); it != m_Tasks.end(); ++it)
    {
        ITask *task = *it;
        task->Destroy();
    }
    m_Tasks.clear();
}

void TaskManager::Update(float deltaSeconds)
{
    TaskList deadTasks;
    
    for(TaskList::iterator it = m_Tasks.begin(); it != m_Tasks.end(); ++it)
    {
        ITask* task = *it;
        if(!task->IsDead())
            task->Update(deltaSeconds);
        
        if(task->IsDead())
            deadTasks.push_back(task);
    }
    
    PruneDeadTasks(deadTasks);
}

void TaskManager::PruneDeadTasks(const TaskList& deadTasks)
{
    for(TaskList::const_iterator it = deadTasks.begin(); it != deadTasks.end(); ++it)
    {
        ITask* task = *it;
        if(task->IsDead())
        {
            m_Tasks.remove(task);
            ITask* next = task->PopNextTask();
            if(next != NULL)
                Attach(next);
            
            task->Destroy();
        }
    }
}

void TaskManager::Attach(ITask* task)
{
    m_Tasks.push_back(task);
}

int TaskManager::NumTasks() const
{
    return (int)m_Tasks.size();
}

TaskList& TaskManager::Tasks()
{
    return m_Tasks;
}

