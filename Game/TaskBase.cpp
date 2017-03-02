//
//  TaskBase.cpp
//  RTSGame
//
//  Created by Joe Engalan on 7/18/13.
//  Copyright (c) 2013 Sourcebits. All rights reserved.
//

#include "TaskBase.h"


TaskBase::TaskBase()
: m_isInitialized(false),
  m_isDead(false),
  m_pNextTask(NULL)
{
}

TaskBase::~TaskBase()
{
    Kill();
}

void TaskBase::Init()
{
    m_isDead = false;
    m_isInitialized = true;
}

void TaskBase::Update(float deltaSeconds)
{
    if(m_isInitialized == false)
        Init();
}

bool TaskBase::IsDead() const
{
    return m_isDead;
}

void TaskBase::Destroy()
{
    if(m_pNextTask)
        m_pNextTask->Destroy();
    
    delete this;
}

ITask* TaskBase::PopNextTask()
{
    ITask *next = m_pNextTask;
    m_pNextTask = NULL;
    return next;
}

void TaskBase::Add(ITask* task)
{
    if(m_pNextTask == NULL)
        m_pNextTask = task;
    else
        m_pNextTask->Add(task);
}

void TaskBase::Kill()
{
    m_isDead = true;
}
