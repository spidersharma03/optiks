//
//  EditorState.cpp
//  Optiks
//
//  Created by Prashant Sharma on 29/10/13.
//
//

#include "GameEditorState.h"
#include "Game.h"
#include "Scene.h"
#include "ShapeGameObject.h"
#include "GameEditorHUDController.h"
#include "GUIManager.h"

#include "EditorOperationStates.h"
#include "EditorObjectCreateState.h"

GameEditorState* GameEditorState::s_pGameEditorState = NULL;

GameEditorState::GameEditorState()
{
    m_pEditorStateMachine  = new StateMachine<Game>(NULL);
    m_pEditorHUDController = new EditorHUDController(this);
}

void GameEditorState::Enter(Game* pOwner)
{
    // Enable and show the Editor UI here.
    m_pEditorHUDController->showEditorUI();
}

void GameEditorState::Execute(Game* pOwner)
{
    if( m_pEditorStateMachine->CurrentState() )
        m_pEditorStateMachine->CurrentState()->Execute(pOwner);

    Game::getInstance()->getScene()->reset();
    
    Game::getInstance()->getScene()->update();
    
    Game::getInstance()->getScene()->renderScene();
}

void GameEditorState::Exit(Game* pOwner)
{
    // Disable the Editor UI here.
    m_pEditorHUDController->hideEditorUI();
}


sbool GameEditorState::OnEvent( Event& event)
{
    bool bRes = Game::getInstance()->getGUIManager()->processEvents(event);
 
    if( bRes )
        return true;
    
    if(m_pEditorStateMachine->CurrentState())
        m_pEditorStateMachine->CurrentState()->OnEvent(event);

    return true;
}