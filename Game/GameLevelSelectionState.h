//
//  GameLevelSelectionState.h
//  Optiks
//
//  Created by Prashant Sharma on 05/12/13.
//
//

#ifndef __Optiks__GameLevelSelectionState__
#define __Optiks__GameLevelSelectionState__

#include <iostream>
#include "IState.h"

class Game;
class GameLevelSelectionHUDController;

class GameLevelSelectionState : public IState<Game>
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
    

    GameLevelSelectionHUDController*  getGameLevelSelectionHUDController()
    {
        return m_pGameLevelSelectionHUDController;
    }
    
    static GameLevelSelectionState* getInstance()
    {
        if( !s_pGameLevelSelectionState )
        {
            s_pGameLevelSelectionState = new GameLevelSelectionState();
        }
        return s_pGameLevelSelectionState;
    }
    
    int                 getSelectedLevel() const;
    int                 getSelectedStage() const;

    void                setLevelTo(int);
    void                setStageTo(int stage);
    
private:
    GameLevelSelectionState();
    
    static GameLevelSelectionState   *s_pGameLevelSelectionState;
    
    GameLevelSelectionHUDController  *m_pGameLevelSelectionHUDController;
    
    friend class GameLevelSelectionHUDController;
};

#endif /* defined(__Optiks__GameLevelSelectionState__) */
