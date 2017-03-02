//
//  GameStageSelectionState.h
//  Optiks
//
//  Created by Prashant Sharma on 05/12/13.
//
//

#ifndef __Optiks__GameStageSelectionState__
#define __Optiks__GameStageSelectionState__

#include <iostream>
#include "IState.h"

class Game;
class GameChapterSelectionHUDController;

class GameChapterSelectionState : public IState<Game>
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
    
    void                setChapterTo(int chapter);
    
    static GameChapterSelectionState* getInstance()
    {
        if( !s_pGameChapterSelectionState )
        {
            s_pGameChapterSelectionState = new GameChapterSelectionState();
        }
        return s_pGameChapterSelectionState;
    }
    
    GameChapterSelectionHUDController* getGameChapterSelectionHUDController()
    {
        return m_pGameChapterSelectionHUDController;
    }
    
    int                 getSelectedChapter() const;
    
private:
    GameChapterSelectionState();
    
    static GameChapterSelectionState     *s_pGameChapterSelectionState;
    
    GameChapterSelectionHUDController  *m_pGameChapterSelectionHUDController;
    
    friend class GameChapterSelectionHUDController;
};

#endif /* defined(__Optiks__GameStageSelectionState__) */
