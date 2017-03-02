//
//  GameLevelCompleteHUDController.cpp
//  Optiks
//
//  Created by Prashant on 24/02/14.
//
//

#include "GameLevelCompleteHUDController.h"
#include "GameMenuState.h"
#include "GameLevelCompleteState.h"
#include "GameLevelSelectionState.h"
#include "GameLevelSelectionHUDController.h"
#include "GamePlayState.h"
#include "Game.h"
#include "GameSettings.h"
#include "Scene.h"
#include "GamePlayHUDController.h"
#include "TextureMaker.h"
#include "GameChapterSelectionState.h"
#include "GUIContinueButton.h"
#include "GUIMainMenuButton.h"
#include "SceneTransitionTask.h"
#include "TaskManager.h"
#include "GUIGameThemeButton.h"
#include "GamePauseTransitionTask.h"
#include "GamePauseState.h"
#include "GamePauseHUDController.h"
#include "GUIReplayButton.h"

GameLevelCompleteHUDController::GameLevelCompleteHUDController()
{
    createGameLevelCompleteView();
}

GameLevelCompleteHUDController::~GameLevelCompleteHUDController()
{
    
}

// IActionListener
void GameLevelCompleteHUDController::mousePressed(GUIComponent* pComponent, int x, int y)
{
    
}

void GameLevelCompleteHUDController::mouseReleased(GUIComponent* pComponent, int x, int y)
{
    if( pComponent )
    {
        int tag = pComponent->getTag();
        if( tag == 0 )
        {
//            SceneTransitionTask *pSceneTransitionTask = new SceneTransitionTask( GamePlayState::getInstance(), "", Game::getInstance()->getScene()->getSceneWidth(),
//                                                                                Game::getInstance()->getScene()->getSceneHeight(),
//                                                                                1.0f );
//            
//            Game::getInstance()->getTaskManager()->Attach(pSceneTransitionTask);
        }
         // Replay Level
        if( tag == 1 )
        {
            string levelFilePath= GamePlayState::getInstance()->getGamePlayHUDController()->getCurrentLevelName();

            SceneTransitionTask *pSceneTransitionTask = new SceneTransitionTask( GamePlayState::getInstance(), levelFilePath.c_str(), Game::getInstance()->getScene()->getSceneWidth(),
                                                                                Game::getInstance()->getScene()->getSceneHeight(),
                                                                                1.0f );
            
            Game::getInstance()->getTaskManager()->Attach(pSceneTransitionTask);
        }
        // Next Level
        if( tag == 2 )
        {
            string levelFilePath= GamePlayState::getInstance()->getGamePlayHUDController()->getNextLevelName();

            SceneTransitionTask *pSceneTransitionTask = new SceneTransitionTask( GamePlayState::getInstance(), levelFilePath.c_str(), Game::getInstance()->getScene()->getSceneWidth(),
                                                                                Game::getInstance()->getScene()->getSceneHeight(),
                                                                                1.0f );
            
            Game::getInstance()->getTaskManager()->Attach(pSceneTransitionTask);
        }
        // Main Menu
        if( tag == 3 )
        {
            SceneTransitionTask *pSceneTransitionTask = new SceneTransitionTask( GameLevelSelectionState::getInstance(), "", Game::getInstance()->getScene()->getSceneWidth(),
                                                                                Game::getInstance()->getScene()->getSceneHeight(),
                                                                                1.0f );
            
            Game::getInstance()->getTaskManager()->Attach(pSceneTransitionTask);
        }
    }
}

void GameLevelCompleteHUDController::mouseMoved(GUIComponent* pComponent, int x, int y)
{
    
}

void GameLevelCompleteHUDController::keyPressed(GUIComponent* pComponent, const Event& event)
{
    
}

void GameLevelCompleteHUDController::showGameLevelCompleteView()
{
    m_pGameCompleteView->setVisible(true);
}

void GameLevelCompleteHUDController::hideGameLevelCompleteView()
{
    m_pGameCompleteView->setVisible(false);
}

void  GameLevelCompleteHUDController::setTransparency(float transparency)
{
    m_Transparency = transparency;
    for( int i=0; i<m_vecButtons.size(); i++ )
    {
        GUIButton* pButton = m_vecButtons[i];
        pButton->setTransparency(transparency);
    }
}

float GameLevelCompleteHUDController::getTransparency() const
{
    return m_Transparency;
}

void GameLevelCompleteHUDController::createGameLevelCompleteView()
{
    m_pGameCompleteView = new GUIPanel(Game::getInstance()->getGUIManager(), 420, 400);
    m_pGameCompleteView->setTransparency(1.0f);
    m_pGameCompleteView->setPosition(310, 170);
    
    int x = 125;
    int y = 125;

    GUIButton* pButtonLevelComplete = new GUIGameThemeButton(m_pGameCompleteView, 150,150);
    pButtonLevelComplete->setTitle("LEVEL COMPLETE");
    pButtonLevelComplete->setPosition(x, y);
    pButtonLevelComplete->setTag(0);
    pButtonLevelComplete->setActionListener(this);
    m_vecButtons.push_back(pButtonLevelComplete);
    pButtonLevelComplete->setTexture(Game::getInstance()->m_pDefaultGradientTexture);
    
    x = 0;
    y = 100;
    
    GUIButton* pButtonReplayLevel = new GUIReplayButton(m_pGameCompleteView, 50,50);
    pButtonReplayLevel->setTitle("Re");
    pButtonReplayLevel->setPosition(x, y);
    pButtonReplayLevel->setTag(1);
    pButtonReplayLevel->setActionListener(this);
    m_vecButtons.push_back(pButtonReplayLevel);
    pButtonReplayLevel->setTexture(Game::getInstance()->m_pDefaultGradientTexture);

    x = 360;
    y = 100;
    
    GUIButton* pButtonNextLevel = new GUIContinueButton(m_pGameCompleteView, 50,50);
    pButtonNextLevel->setTitle("Next");
    pButtonNextLevel->setPosition(x, y);
    pButtonNextLevel->setTag(2);
    pButtonNextLevel->setActionListener(this);
    m_vecButtons.push_back(pButtonNextLevel);

    x = 0;
    y = 270;
    
    GUIButton* pButtonBackToMainMenu = new GUIMainMenuButton(m_pGameCompleteView, 50,50);
    pButtonBackToMainMenu->setTitle("Next");
    pButtonBackToMainMenu->setPosition(x, y);
    pButtonBackToMainMenu->setTag(3);
    pButtonBackToMainMenu->setActionListener(this);
    m_vecButtons.push_back(pButtonBackToMainMenu);
    
    hideGameLevelCompleteView();
}