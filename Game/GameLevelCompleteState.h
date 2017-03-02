//
//  GameLevelCompleteState.h
//  Optiks
//
//  Created by Prashant on 24/02/14.
//
//

#ifndef __Optiks__GameLevelCompleteState__
#define __Optiks__GameLevelCompleteState__

#include <iostream>
#include "IState.h"
#include "StateMachine.h"

class Game;
class GameLevelCompleteHUDController;

class GameLevelCompleteState : public IState<Game>
{
public:
    //this will execute when the state is entered
    virtual void Enter(Game* pOwner);
    
    //this is the states normal update function
    virtual void Execute(Game* pOwner);
    
    //this will execute when the state is exited.
    virtual void Exit(Game* pOwner);
    
    // EventListener
    virtual sbool       OnEvent( Event& event);
    
    static GameLevelCompleteState* getInstance()
    {
        if( !s_pGameLevelCompleteState )
        {
            s_pGameLevelCompleteState = new GameLevelCompleteState();
        }
        return s_pGameLevelCompleteState;
    }
    
    GameLevelCompleteHUDController* getGameLevelCompleteHUDController() const
    {
        return m_pGameLevelCompleteHUDController;
    }
    
private:
    GameLevelCompleteState();
    
    static GameLevelCompleteState   *s_pGameLevelCompleteState;
    
    GameLevelCompleteHUDController  *m_pGameLevelCompleteHUDController;
    
    friend class GameLevelCompleteHUDController;
};

#endif /* defined(__Optiks__GameLevelCompleteState__) */
