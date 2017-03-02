//
//  EditorOperationStatesStates.h
//  Optiks
//
//  Created by Prashant Sharma on 07/11/13.
//
//

#ifndef __Optiks__EditorOperationStatesStates__
#define __Optiks__EditorOperationStatesStates__

#include "IState.h"

class Game;

class EditorOperationBaseState : public IState<Game>
{
public:
    //this will execute when the state is entered
    virtual void            Enter(Game* pOwner);
    
    //this is the states normal update function
    virtual void            Execute(Game* pOwner);
    
    //this will execute when the state is exited.
    virtual void            Exit(Game* pOwner);
    
    sbool                   OnEvent( Event& event);
protected:
    EditorOperationBaseState();
};


class EditorOperationMoveState : public EditorOperationBaseState
{
public:
    //this will execute when the state is entered
    virtual void            Enter(Game* pOwner);
    
    //this is the states normal update function
    virtual void            Execute(Game* pOwner);
    
    //this will execute when the state is exited.
    virtual void            Exit(Game* pOwner);
    
    // EventListener
    sbool                   OnEvent( Event& event);
    
    static EditorOperationMoveState* getInstance()
    {
        if( !s_pEditorOperationMoveState )
        {
            s_pEditorOperationMoveState = new EditorOperationMoveState();
        }
        return s_pEditorOperationMoveState;
    }

protected:
    
    EditorOperationMoveState();
        
    static EditorOperationMoveState   *s_pEditorOperationMoveState;
};


class EditorOperationRotateState : public EditorOperationBaseState
{
public:
    //this will execute when the state is entered
    virtual void            Enter(Game* pOwner);
    
    //this is the states normal update function
    virtual void            Execute(Game* pOwner);
    
    //this will execute when the state is exited.
    virtual void            Exit(Game* pOwner);
    
    // EventListener
    sbool                   OnEvent( Event& event);
    
    static EditorOperationRotateState* getInstance()
    {
        if( !s_pEditorOperationRotateState )
        {
            s_pEditorOperationRotateState = new EditorOperationRotateState();
        }
        return s_pEditorOperationRotateState;
    }
    
protected:
    
    EditorOperationRotateState();
    
    static EditorOperationRotateState   *s_pEditorOperationRotateState;
};


class EditorOperationSelectState : public EditorOperationBaseState
{
public:
    //this will execute when the state is entered
    virtual void            Enter(Game* pOwner);
    
    //this is the states normal update function
    virtual void            Execute(Game* pOwner);
    
    //this will execute when the state is exited.
    virtual void            Exit(Game* pOwner);
    
    // EventListener
    sbool                   OnEvent( Event& event);
    
    static EditorOperationSelectState* getInstance()
    {
        if( !s_pEditorOperationSelectState )
        {
            s_pEditorOperationSelectState = new EditorOperationSelectState();
        }
        return s_pEditorOperationSelectState;
    }
protected:
    
    EditorOperationSelectState();
    
    static EditorOperationSelectState   *s_pEditorOperationSelectState;
};

#endif /* defined(__Optiks__EditorOperationStatesStates__) */
