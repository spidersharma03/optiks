//
//  GameMainMenuHUDController.cpp
//  Optiks
//
//  Created by Prashant Sharma on 04/12/13.
//
//

#include "GameMainMenuHUDController.h"
#include "Game.h"
#include "GameEditorState.h"
#include "GamePlayState.h"
#include "GameChapterSelectionState.h"
#include "PropertiesPanel.h"
#include "SceneTransitionTask.h"
#include "Scene.h"
#include "TaskManager.h"

GameMainMenuHUDController::GameMainMenuHUDController()
{
    createMainMenuView();    
}

GameMainMenuHUDController::~GameMainMenuHUDController()
{
    
}

// IActionListener
void GameMainMenuHUDController::mousePressed(GUIComponent* pComponent, int x, int y)
{
    
}

void GameMainMenuHUDController::mouseReleased(GUIComponent* pComponent, int x, int y)
{
    if( pComponent )
    {
        // Switch to Game Editor State
        if(pComponent->getTag() == 0)
        {
            hideAllViews();
            SceneTransitionTask *pSceneTransitionTask = new SceneTransitionTask( GameEditorState::getInstance(), "", Game::getInstance()->getScene()->getSceneWidth(),
                                                                                Game::getInstance()->getScene()->getSceneHeight(),
                                                                                1.0f );
            
            Game::getInstance()->getTaskManager()->Attach(pSceneTransitionTask);
        }
        // Switch to Game Play State
        else if(pComponent->getTag() == 1)
        {
            hideAllViews();
            SceneTransitionTask *pSceneTransitionTask = new SceneTransitionTask( GameChapterSelectionState::getInstance(), "", Game::getInstance()->getScene()->getSceneWidth(),
                                                                                Game::getInstance()->getScene()->getSceneHeight(),
                                                                                1.0f );
            
            Game::getInstance()->getTaskManager()->Attach(pSceneTransitionTask);
        }
    }
}

void GameMainMenuHUDController::mouseMoved(GUIComponent* pComponent, int x, int y)
{
    
}

void GameMainMenuHUDController::keyPressed(GUIComponent* pComponent, const Event& event)
{
    
}

void GameMainMenuHUDController::hideAllViews()
{
    m_pMainPanelView->setVisible(false);
}

void GameMainMenuHUDController::showAllViews()
{
    m_pMainPanelView->setVisible(true);
}

void GameMainMenuHUDController::createMainMenuView()
{
    m_pMainPanelView = new GUIPanel(Game::getInstance()->getGUIManager(), 1100, 600);
    
    m_pMainPanelView->setPosition(60, 100);
    m_pMainPanelView->setTransparency(0.5f);
    GUIButton* pEditorSelectionButton = new GUIButton(m_pMainPanelView, 200, 200);
    pEditorSelectionButton->setTitle("EDIT");
    pEditorSelectionButton->setTag(0);
    pEditorSelectionButton->setPosition(150, 200);
    pEditorSelectionButton->setActionListener(this);
    pEditorSelectionButton->setTexture((Game::getInstance()->m_pDefaultGradientTexture));
    
    GUIButton* pPlaySelectionButton = new GUIButton(m_pMainPanelView, 200, 200);
    pPlaySelectionButton->setTitle("PLAY");
    pPlaySelectionButton->setTag(1);
    pPlaySelectionButton->setPosition(700, 200);
    pPlaySelectionButton->setActionListener(this);
    pPlaySelectionButton->setTexture((Game::getInstance()->m_pDefaultGradientTexture));
}


