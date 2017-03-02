//
//  GamePauseState.cpp
//  Optiks
//
//  Created by Prashant on 12/03/14.
//
//

#include "GamePauseState.h"
#include "GameLevelCompleteState.h"
#include "Game.h"
#include "Scene.h"
#include "GamePauseHUDController.h"
#include "GamePlayOperationStates.h"


GamePauseState* GamePauseState::s_pGamePauseState = NULL;


GamePauseState::GamePauseState()
{
    m_pGamePauseHUDController = new GamePauseHUDController();
}

void GamePauseState::Enter(Game* pOwner)
{
    Game::getInstance()->getScene()->addGlowingObjects(m_pGamePauseHUDController);

    m_pGamePauseHUDController->showGameLevelPauseView();
}

void GamePauseState::Execute(Game* pOwner)
{
    Game::getInstance()->getScene()->reset();
    
    Game::getInstance()->getScene()->update();
    
    Game::getInstance()->getScene()->renderScene();
}

void GamePauseState::Exit(Game* pOwner)
{
    Game::getInstance()->getScene()->removeGlowingObjects(m_pGamePauseHUDController);

    m_pGamePauseHUDController->hideGameLevelPauseView();
    
    // Enable Markers.
    //Game::getInstance()->getScene()->enableMarkerObjects();
}

sbool GamePauseState::OnEvent( Event& event)
{
    bool bRes = Game::getInstance()->getGUIManager()->processEvents(event);
    
    if( bRes )
        return true;
    
    return bRes;
}