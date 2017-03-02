//
//  GamePauseState.h
//  Optiks
//
//  Created by Prashant on 12/03/14.
//
//

#ifndef __Optiks__GamePauseState__
#define __Optiks__GamePauseState__

#include <iostream>
#include "IState.h"
#include "StateMachine.h"

class Game;
class GamePauseHUDController;

class GamePauseState : public IState<Game>
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
    
    static GamePauseState* getInstance()
    {
        if( !s_pGamePauseState )
        {
            s_pGamePauseState = new GamePauseState();
        }
        return s_pGamePauseState;
    }
    
    GamePauseHUDController* getGamePauseHUDController() const
    {
        return m_pGamePauseHUDController;
    }
    
private:
    GamePauseState();
    
    static GamePauseState   *s_pGamePauseState;
    
    GamePauseHUDController  *m_pGamePauseHUDController;
    
    friend class GamePauseHUDController;
};

#endif /* defined(__Optiks__GamePauseState__) */
