//
//  GameStageSelectionState.cpp
//  Optiks
//
//  Created by Prashant Sharma on 05/12/13.
//
//

#include "GameChapterSelectionState.h"
#include "Game.h"
#include "Scene.h"
#include "GameChapterSelectionHUDController.h"

GameChapterSelectionState* GameChapterSelectionState::s_pGameChapterSelectionState = NULL;


GameChapterSelectionState::GameChapterSelectionState()
{
    m_pGameChapterSelectionHUDController = new GameChapterSelectionHUDController();
}

void GameChapterSelectionState::Enter(Game* pOwner)
{
    m_pGameChapterSelectionHUDController->showGameChapterSelectionView();
}

void GameChapterSelectionState::Execute(Game* pOwner)
{
    Game::getInstance()->getScene()->reset();
    
    //Game::getInstance()->getScene()->update();
    m_pGameChapterSelectionHUDController->renderUI();
}

void GameChapterSelectionState::Exit(Game* pOwner)
{
    m_pGameChapterSelectionHUDController->hideGameChapterSelectionView();
}

sbool GameChapterSelectionState::OnEvent( Event& event)
{
    bool bRes = Game::getInstance()->getGUIManager()->processEvents(event);
    
    return bRes;
}

int GameChapterSelectionState::getSelectedChapter() const
{
    return m_pGameChapterSelectionHUDController->getSelectedChapter();
}

void GameChapterSelectionState::setChapterTo(int chapter)
{
    m_pGameChapterSelectionHUDController->setChapterTo(chapter);
}