//
//  EditorState.h
//  Optiks
//
//  Created by Prashant Sharma on 29/10/13.
//
//

#ifndef __Optiks__EditorState__
#define __Optiks__EditorState__

#include "IState.h"
#include "StateMachine.h"

class Game;
class EditorHUDController;

class GameEditorState : public IState<Game>
{
public:
    //this will execute when the state is entered
    virtual void        Enter(Game* pOwner);
    
    //this is the states normal update function
    virtual void        Execute(Game* pOwner);
    
    //this will execute when the state is exited.
    virtual void        Exit(Game* pOwner);
    
    // EventListener
    virtual sbool       OnEvent( Event& event);
    
    static GameEditorState* getInstance()
    {
        if( !s_pGameEditorState )
        {
            s_pGameEditorState = new GameEditorState();
        }
        return s_pGameEditorState;
    }
    
    EditorHUDController* getEditorHUDController() const
    {
        return m_pEditorHUDController;
    }
    
private:
    
    GameEditorState();
    
    static GameEditorState   *s_pGameEditorState;
    
    StateMachine<Game>       *m_pEditorStateMachine;
    EditorHUDController      *m_pEditorHUDController;
    
    friend class EditorHUDController;
};

#endif /* defined(__Optiks__EditorState__) */
