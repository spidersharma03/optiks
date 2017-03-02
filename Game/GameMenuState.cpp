//
//  GameMenuState.cpp
//  Optiks
//
//  Created by Prashant Sharma on 30/10/13.
//
//

#include "GameMenuState.h"
#include "Game.h"
#include "Scene.h"
#include "GUI.h"
#include "GameMainMenuHUDController.h"

GameMenuState* GameMenuState::s_pGameMenuState = NULL;

GameMenuState::GameMenuState()
{
    m_pGameMainMenuHUDController = new GameMainMenuHUDController();
}

void GameMenuState::Enter(Game* pOwner)
{
    m_pGameMainMenuHUDController->showAllViews();
}

void GameMenuState::Execute(Game* pOwner)
{
    Game::getInstance()->getScene()->reset();
    
    //Game::getInstance()->getScene()->update();
    
    Game::getInstance()->getScene()->renderGuiScene();
}

void GameMenuState::Exit(Game* pOwner)
{
    m_pGameMainMenuHUDController->hideAllViews();
}

// EventListener
sbool GameMenuState::OnEvent( Event& event)
{
    return Game::getInstance()->getGUIManager()->processEvents(event);
}

