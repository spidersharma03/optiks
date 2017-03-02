//
//  State.h
//  Optiks
//
//  Created by Prashant Sharma on 29/10/13.
//
//

#ifndef Optiks_State_h
#define Optiks_State_h

#include "EventListener.h"

using namespace spider3d;

template <class T>
class IState : public EventListener
{
public:
        
    //this will execute when the state is entered
    virtual void Enter(T* pOwner)=0;
    
    //this is the states normal update function
    virtual void Execute(T* pOwner)=0;
    
    //this will execute when the state is exited.
    virtual void Exit(T* pOwner)=0;
    
    // EventListener
    virtual sbool OnEvent( Event& event)
    {
        return false;
    }
};


#endif
