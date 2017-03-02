//
//  GameLevelSelectionState.cpp
//  Optiks
//
//  Created by Prashant Sharma on 05/12/13.
//
//

#include "GameLevelSelectionState.h"
#include "Game.h"
#include "Scene.h"
#include "GameLevelSelectionHUDController.h"

GameLevelSelectionState* GameLevelSelectionState::s_pGameLevelSelectionState = NULL;


GameLevelSelectionState::GameLevelSelectionState()
{
    m_pGameLevelSelectionHUDController = new GameLevelSelectionHUDController();
}

void GameLevelSelectionState::Enter(Game* pOwner)
{
    m_pGameLevelSelectionHUDController->showGameLevelSelectionView();
}

void GameLevelSelectionState::Execute(Game* pOwner)
{
    Game::getInstance()->getScene()->reset();
    
    //Render GUI here
    m_pGameLevelSelectionHUDController->renderUI();
}

void GameLevelSelectionState::Exit(Game* pOwner)
{
    m_pGameLevelSelectionHUDController->hideGameLevelSelectionView();
}

sbool GameLevelSelectionState::OnEvent( Event& event)
{
    bool bRes = Game::getInstance()->getGUIManager()->processEvents(event);
    
    return bRes;
}

int GameLevelSelectionState::getSelectedLevel() const
{
    return m_pGameLevelSelectionHUDController->getSelectedLevel();
}

int GameLevelSelectionState::getSelectedStage() const
{
    return m_pGameLevelSelectionHUDController->getSelectedStage();
}

void GameLevelSelectionState::setLevelTo(int level)
{
    m_pGameLevelSelectionHUDController->setLevelTo(level);
}

void GameLevelSelectionState::setStageTo(int stage)
{
    m_pGameLevelSelectionHUDController->setStageTo(stage);
}