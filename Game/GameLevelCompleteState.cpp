//
//  GameLevelCompleteState.cpp
//  Optiks
//
//  Created by Prashant on 24/02/14.
//
//

#include "GameLevelCompleteState.h"
#include "Game.h"
#include "Scene.h"
#include "GameLevelCompleteHUDController.h"
#include "GamePlayOperationStates.h"


GameLevelCompleteState* GameLevelCompleteState::s_pGameLevelCompleteState = NULL;


GameLevelCompleteState::GameLevelCompleteState()
{
    m_pGameLevelCompleteHUDController = new GameLevelCompleteHUDController();
}

void GameLevelCompleteState::Enter(Game* pOwner)
{
    m_pGameLevelCompleteHUDController->showGameLevelCompleteView();
    // Fire Level Complete Task
    Game::getInstance()->fireLevelCompleteTask();    
}

void GameLevelCompleteState::Execute(Game* pOwner)
{
    Game::getInstance()->getScene()->reset();
    
    Game::getInstance()->getScene()->update();
    
    Game::getInstance()->getScene()->renderScene();
}

void GameLevelCompleteState::Exit(Game* pOwner)
{
    m_pGameLevelCompleteHUDController->hideGameLevelCompleteView();
    
    Game::getInstance()->getScene()->setDebugGridEnable(true);
    
    // Enable Markers.
    Game::getInstance()->getScene()->enableMarkerObjects();
    
    Game::getInstance()->getScene()->fadeOut();
}

sbool GameLevelCompleteState::OnEvent( Event& event)
{
    bool bRes = Game::getInstance()->getGUIManager()->processEvents(event);
    
    if( bRes )
        return true;
     
    return bRes;
}