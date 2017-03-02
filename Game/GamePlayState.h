//
//  GamePlayState.h
//  Optiks
//
//  Created by Prashant Sharma on 30/10/13.
//
//

#ifndef __Optiks__GamePlayState__
#define __Optiks__GamePlayState__

#include "IState.h"
#include "StateMachine.h"

class Game;
class GamePlayHUDController;

class GamePlayState : public IState<Game>
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
    
    static GamePlayState* getInstance()
    {
        if( !s_pGamePlayState )
        {
            s_pGamePlayState = new GamePlayState();
        }
        return s_pGamePlayState;
    }
    
    GamePlayHUDController* getGamePlayHUDController() const
    {
        return m_pGamePlayHUDController;
    }
    
private:
    GamePlayState();
    
    static GamePlayState   *s_pGamePlayState;
    StateMachine<Game>     *m_pPlayStateMachine;

    GamePlayHUDController  *m_pGamePlayHUDController;
    
    friend class GamePlayHUDController;
};

#endif /* defined(__Optiks__GamePlayState__) */
