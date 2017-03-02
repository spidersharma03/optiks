//
//  StateMachine.h
//  Optiks
//
//  Created by Prashant Sharma on 29/10/13.
//
//

#ifndef Optiks_StateMachine_h
#define Optiks_StateMachine_h


#include <iostream>
#include "IState.h"
#include "assert.h"

template <class T>
class StateMachine
{
protected:
    
    //a pointer to the agent that owns this instance
    T*          m_pOwner;
    
    IState<T>*   m_pCurrentState;
    
    //a record of the last state the agent was in
    IState<T>*   m_pPreviousState;
    
    //this is called every time the FSM is updated
    IState<T>*   m_pGlobalState;
    
    
public:
    
    StateMachine(T* owner):m_pOwner(owner),
    m_pCurrentState(NULL),
    m_pPreviousState(NULL),
    m_pGlobalState(NULL)
    {}
    
    virtual ~StateMachine(){}
    
    //use these methods to initialize the FSM
    void SetCurrentState(IState<T>* s){m_pCurrentState = s;
        m_pCurrentState->Enter(m_pOwner);
    }
    void SetGlobalState(IState<T>* s) {m_pGlobalState = s;}
    void SetPreviousState(IState<T>* s){m_pPreviousState = s;}
    
    //call this to update the FSM
    void  Update()const
    {
        //if a global state exists, call its execute method, else do nothing
        if(m_pGlobalState)   m_pGlobalState->Execute(m_pOwner);
        
        //same for the current state
        if (m_pCurrentState) m_pCurrentState->Execute(m_pOwner);
    }
    
    //change to a new state
    void  ChangeState(IState<T>* pNewState)
    {
        assert(pNewState &&
               "<StateMachine::ChangeState>: trying to change to NULL state");
        
        //keep a record of the previous state
        m_pPreviousState = m_pCurrentState;
        
        //call the exit method of the existing state, make sure CurrentState is NOT NULL
        if( m_pCurrentState )
            m_pCurrentState->Exit(m_pOwner);
        
        //change state to the new state
        m_pCurrentState = pNewState;
        
        //call the entry method of the new state
        m_pCurrentState->Enter(m_pOwner);
    }
    
    //change state back to the previous state
    void  RevertToPreviousState()
    {
        ChangeState(m_pPreviousState);
    }
    
    //returns true if the current state's type is equal to the type of the
    //class passed as a parameter.
    //    bool  isInState(const State<entity_type>& st)const
    //    {
    //        return typeid(*m_pCurrentState) == typeid(st);
    //    }
    
    IState<T>*  CurrentState()  const{return m_pCurrentState;}
    IState<T>*  GlobalState()   const{return m_pGlobalState;}
    IState<T>*  PreviousState() const{return m_pPreviousState;}
};


#endif
