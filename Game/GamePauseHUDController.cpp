//
//  GUIGamePauseHUDController.cpp
//  Optiks
//
//  Created by Prashant on 12/03/14.
//
//

#include "GamePauseHUDController.h"
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
#include "SceneTransitionTask.h"
#include "TaskManager.h"
#include "GamePauseTransitionTask.h"
#include "GamePauseState.h"
#include "GUIContinueButton.h"
#include "GUIMainMenuButton.h"
#include "GUIGameThemeButton.h"

GamePauseHUDController::GamePauseHUDController()
{
    createGameLevelCompleteView();
}

GamePauseHUDController::~GamePauseHUDController()
{
    
}

// IActionListener
void GamePauseHUDController::mousePressed(GUIComponent* pComponent, int x, int y)
{
    
}

void GamePauseHUDController::mouseReleased(GUIComponent* pComponent, int x, int y)
{
    if( pComponent )
    {
        int tag = pComponent->getTag();
        if( tag == 0 )
        {
            
        }
        
        // Go to Main Menu
        if( tag == 1 )
        {
            SceneTransitionTask *pSceneTransitionTask = new SceneTransitionTask( GameLevelSelectionState::getInstance(), "", Game::getInstance()->getScene()->getSceneWidth(),Game::getInstance()->getScene()->getSceneHeight(),1.0f );
            
            Game::getInstance()->getTaskManager()->Attach(pSceneTransitionTask);
            Game::getInstance()->getScene()->fadeOut();
            
//            GamePauseTransitionTask *pTask = new GamePauseTransitionTask(GamePauseState::getInstance()->getGamePauseHUDController(), Game::getInstance()->getScene()->getSceneWidth(), Game::getInstance()->getScene()->getSceneHeight(), 1.0, 0.0f, 1.0f);
//            Game::getInstance()->getTaskManager()->Attach(pTask);

        }
        // Continue Level
        if( tag == 2 )
        {
            GamePauseTransitionTask *pTask = new GamePauseTransitionTask(GamePauseState::getInstance()->getGamePauseHUDController(), Game::getInstance()->getScene()->getSceneWidth(), Game::getInstance()->getScene()->getSceneHeight(), 1.0, 0.0f, 1.0f);
            
            Game::getInstance()->getTaskManager()->Attach(pTask);
            Game::getInstance()->getScene()->fadeOut();
        }
    }
}

void GamePauseHUDController::mouseMoved(GUIComponent* pComponent, int x, int y)
{
    
}

void GamePauseHUDController::keyPressed(GUIComponent* pComponent, const Event& event)
{
    
}

void GamePauseHUDController::showGameLevelPauseView()
{
    m_pGameCompleteView->setVisible(true);
}

void GamePauseHUDController::hideGameLevelPauseView()
{
    m_pGameCompleteView->setVisible(false);
}

void  GamePauseHUDController::setTransparency(float transparency)
{
    m_Transparency = transparency;
    for( int i=0; i<m_vecButtons.size(); i++ )
    {
        GUIButton* pButton = m_vecButtons[i];
        pButton->setTransparency(transparency);
    }
}

float GamePauseHUDController::getTransparency() const
{
    return m_Transparency;
}

void GamePauseHUDController::createGameLevelCompleteView()
{
    m_pGameCompleteView = new GUIPanel(Game::getInstance()->getGUIManager(), 420, 400);
    m_pGameCompleteView->setTransparency(1.0f);
    m_pGameCompleteView->setPosition(310, 170);
    
    int x = 125;
    int y = 125;
    
    GUIButton* pButtonLevelComplete = new GUIGameThemeButton(m_pGameCompleteView, 150,150);
    pButtonLevelComplete->setTitle("LEVEL PAUSED");
    pButtonLevelComplete->setPosition(x, y);
    pButtonLevelComplete->setTag(0);
    pButtonLevelComplete->setActionListener(this);
    m_vecButtons.push_back(pButtonLevelComplete);
    pButtonLevelComplete->setTexture(Game::getInstance()->m_pDefaultGradientTexture);
    
    x = 0;
    y = 100;
    
    m_pButtonGoToMainMenu = new GUIMainMenuButton(m_pGameCompleteView, 50,50);
    m_pButtonGoToMainMenu->setTitle("Main");
    m_pButtonGoToMainMenu->setPosition(x, y);
    m_pButtonGoToMainMenu->setTag(1);
    m_pButtonGoToMainMenu->setActionListener(this);
    m_vecButtons.push_back(m_pButtonGoToMainMenu);
    m_pButtonGoToMainMenu->setTexture(Game::getInstance()->m_pDefaultGradientTexture);
    
    x = 360;
    y = 100;
    
    m_pButtonNextLevel = new GUIContinueButton(m_pGameCompleteView, 50,50);
    m_pButtonNextLevel->setPosition(x, y);
    m_pButtonNextLevel->setTag(2);
    m_pButtonNextLevel->setActionListener(this);
    m_vecButtons.push_back(m_pButtonNextLevel);
    m_pButtonNextLevel->setTexture(Game::getInstance()->m_pDefaultGradientTexture);
    
    hideGameLevelPauseView();
}

vector<Ray>& GamePauseHUDController::getGlowingRays()
{
    m_vecGlowingRays.clear();
//    vector<Ray>& vecRays =  m_pButtonNextLevel->getAllRays();
//    for( int i=0; i<vecRays.size(); i++ )
//    {
//        m_vecGlowingRays.push_back(vecRays[i]);
//    }
//    vector<Ray>& vecRays1 =  m_pButtonGoToMainMenu->getAllRays();
//    for( int i=0; i<vecRays1.size(); i++ )
//    {
//        m_vecGlowingRays.push_back(vecRays1[i]);
//    }
    return m_vecGlowingRays;
}