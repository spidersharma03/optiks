//
//  GameMenuState.h
//  Optiks
//
//  Created by Prashant Sharma on 30/10/13.
//
//

#ifndef __Optiks__GameMenuState__
#define __Optiks__GameMenuState__

#include "IState.h"
#include "EventListener.h"

using namespace spider3d;

class Game;
class GameMainMenuHUDController;

class GameMenuState : public IState<Game>
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
    
    static GameMenuState* getInstance()
    {
        if( !s_pGameMenuState )
        {
            s_pGameMenuState = new GameMenuState();
        }
        return s_pGameMenuState;
    }
    
private:
    
    GameMenuState();
    
    GameMainMenuHUDController   *m_pGameMainMenuHUDController;
    static GameMenuState        *s_pGameMenuState;
};


#endif /* defined(__Optiks__GameMenuState__) */
