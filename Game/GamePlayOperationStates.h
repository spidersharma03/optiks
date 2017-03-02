//
//  GamePlayOperationStates.h
//  Optiks
//
//  Created by Prashant on 23/12/13.
//
//

#ifndef __Optiks__GamePlayOperationStates__
#define __Optiks__GamePlayOperationStates__

#include "IState.h"

class Game;

class GamePlayOperationMoveState : public IState<Game>
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
    
    static GamePlayOperationMoveState* getInstance()
    {
        if( !s_pGamePlayOperationMoveState )
        {
            s_pGamePlayOperationMoveState = new GamePlayOperationMoveState();
        }
        return s_pGamePlayOperationMoveState;
    }
    
protected:
    
    GamePlayOperationMoveState();
    
    static GamePlayOperationMoveState   *s_pGamePlayOperationMoveState;
};


class GamePlayOperationRotateState
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
    
    static GamePlayOperationRotateState* getInstance()
    {
        if( !s_pGamePlayOperationRotateState )
        {
            s_pGamePlayOperationRotateState = new GamePlayOperationRotateState();
        }
        return s_pGamePlayOperationRotateState;
    }
    
protected:
    
    GamePlayOperationRotateState();
    
    static GamePlayOperationRotateState   *s_pGamePlayOperationRotateState;
};


#endif /* defined(__Optiks__GamePlayOperationStates__) */
