//
//  PlayHUDController.cpp
//  Optiks
//
//  Created by Prashant Sharma on 20/11/13.
//
//

#include "GamePlayHUDController.h"
#include "Game.h"
#include "GameMenuState.h"
#include "GameLevelSelectionState.h"
#include "GameChapterSelectionState.h"
#include "Scene.h"
#include "GameSettings.h"
#include "Image.h"
#include "SceneTransitionTask.h"
#include "TaskManager.h"
#include "GUIPauseButton.h"
#include "GUIHintButton.h"
#include "GamePauseState.h"
#include "GamePauseTransitionTask.h"
#include "FileUtil.h"

GamePlayHUDController::GamePlayHUDController()
{
    createGamePlayView();
    
    createLevelCompletePanel();
}

GamePlayHUDController::~GamePlayHUDController()
{
    
}

void GamePlayHUDController::createGamePlayView()
{
//    m_pMainMenuSelectionButton = new GUIButton(Game::getInstance()->getGUIManager(), 50, 25 );
//    m_pMainMenuSelectionButton->setPosition(600, 710);
//    m_pMainMenuSelectionButton->setTag(0);
//    m_pMainMenuSelectionButton->setActionListener(this);
//    m_pMainMenuSelectionButton->setTitle("Menu");
//    m_pMainMenuSelectionButton->setTexture((Game::getInstance()->m_pDefaultGradientTexture));
    
    m_pBackToLevelSelectionButton = new GUIPauseButton(Game::getInstance()->getGUIManager(), 40,40);
    m_pBackToLevelSelectionButton->setPosition(20, 690);
    m_pBackToLevelSelectionButton->setTitle("<=");
    m_pBackToLevelSelectionButton->setTag(3);
    m_pBackToLevelSelectionButton->setActionListener(this);
    m_pHintButton = new GUIHintButton(Game::getInstance()->getGUIManager(), 40,40);
    m_pHintButton->setPosition(964, 690);
    m_pHintButton->setTitle("<=");
    m_pHintButton->setTag(4);
    m_pHintButton->setActionListener(this);
    
    m_pNextLevelSelectionButton = new GUIButton(Game::getInstance()->getGUIManager(), 50, 25);
    m_pNextLevelSelectionButton->setPosition(1050, 710);
    m_pNextLevelSelectionButton->setActionListener(this);
    m_pNextLevelSelectionButton->setTitle("<=");
    m_pNextLevelSelectionButton->setTag(1);
    m_pNextLevelSelectionButton->setTexture((Game::getInstance()->m_pDefaultGradientTexture));
    
    m_pPreviousLevelSelectionButton = new GUIButton(Game::getInstance()->getGUIManager(), 50, 25);
    m_pPreviousLevelSelectionButton->setPosition(1100, 710);
    m_pPreviousLevelSelectionButton->setActionListener(this);
    m_pPreviousLevelSelectionButton->setTitle("=>");
    m_pPreviousLevelSelectionButton->setTag(2);
    m_pPreviousLevelSelectionButton->setTexture((Game::getInstance()->m_pDefaultGradientTexture));
    
    hideAllViews();
}

void GamePlayHUDController::hideAllViews()
{
    //m_pMainMenuSelectionButton->setVisible(false);
    m_pBackToLevelSelectionButton->setVisible(false);
    m_pHintButton->setVisible(false);
    m_pNextLevelSelectionButton->setVisible(false);
    m_pPreviousLevelSelectionButton->setVisible(false);
}

void GamePlayHUDController::showAllViews()
{
    //m_pMainMenuSelectionButton->setVisible(true);
    m_pBackToLevelSelectionButton->setVisible(true);
    m_pHintButton->setVisible(true);
    m_pNextLevelSelectionButton->setVisible(true);
    m_pPreviousLevelSelectionButton->setVisible(true);
}

// IActionListener
void GamePlayHUDController::mousePressed(GUIComponent* pComponent, int x, int y)
{
    
}

void GamePlayHUDController::mouseReleased(GUIComponent* pComponent, int x, int y)
{
    if( pComponent )
    {
        int tag = pComponent->getTag();
        // Pause Level
        if( tag == 3 )
        {
            Game::getInstance()->getScene()->fadeIn();
            
            GamePauseTransitionTask *pTask = new GamePauseTransitionTask(GamePauseState::getInstance()->getGamePauseHUDController(), Game::getInstance()->getScene()->getSceneWidth(), Game::getInstance()->getScene()->getSceneHeight(), 1, 1.0f, 0.0f);
            
            Game::getInstance()->getTaskManagerBelowGUI()->Attach(pTask);
            
            Game::getInstance()->changeState(GamePauseState::getInstance());
        }
        // Give me some HINT!!
        if( tag == 4 )
        {
            Game::getInstance()->getScene()->applyHint();
        }
        // Previous Level
        if( tag == 1 )
        {
            goToPrevLevel();
        }
        // Next Level
        if( tag == 2 )
        {
            goToNextLevel();
        }
        // Back to Main Menu
        if( tag == 0 )
        {
            Game::getInstance()->changeState(GameMenuState::getInstance());
        }
    }
}

void GamePlayHUDController::mouseMoved(GUIComponent* pComponent, int x, int y)
{
    
}

void GamePlayHUDController::goToPrevLevel()
{
    int selectedStage = GameLevelSelectionState::getInstance()->getSelectedStage();
    int selectedLevel = GameLevelSelectionState::getInstance()->getSelectedLevel();
    int previousLevel = selectedLevel-1;
    int selectedChapter = GameChapterSelectionState::getInstance()->getSelectedChapter();

    if(previousLevel >= 0)
    {
        Game::getInstance()->getScene()->load(selectedChapter,selectedStage, previousLevel);
    }
}

void GamePlayHUDController::goToNextLevel()
{
    int selectedStage = GameLevelSelectionState::getInstance()->getSelectedStage();
    int selectedLevel = GameLevelSelectionState::getInstance()->getSelectedLevel();
    int nextLevel = selectedLevel+1;
    int selectedChapter = GameChapterSelectionState::getInstance()->getSelectedChapter();

    if(nextLevel <= NUM_LEVELS_PER_STAGE )
    {
        Game::getInstance()->getScene()->load(selectedChapter,selectedStage, nextLevel);
    }
    else
    {
        selectedStage++;
        nextLevel = 1;
        if( selectedStage < NUM_STAGES )
            Game::getInstance()->getScene()->load(selectedChapter,selectedStage, nextLevel);
    }
}

string GamePlayHUDController::getCurrentLevelName() const
{
    string currentLevelName;
    
    int selectedStage = GameLevelSelectionState::getInstance()->getSelectedStage();
    int selectedLevel = GameLevelSelectionState::getInstance()->getSelectedLevel();
    int selectedChapter = GameChapterSelectionState::getInstance()->getSelectedChapter();
    

    char levelFilename[32];
    sprintf(levelFilename,"ch%dstg%dlvl%d",selectedChapter, selectedStage, selectedLevel);
    
#ifdef IOS
    FileUtil fu;
    currentLevelName = fu.getAbsolutePath(levelFilename).c_str();
#else
    currentLevelName = levelFilename;
#endif
    
    return currentLevelName;
}

string GamePlayHUDController::getNextLevelName() const
{
    string nextLevelName;
    
    int selectedStage = GameLevelSelectionState::getInstance()->getSelectedStage();
    int selectedLevel = GameLevelSelectionState::getInstance()->getSelectedLevel();
    int nextLevel = selectedLevel+1;
    int selectedChapter = GameChapterSelectionState::getInstance()->getSelectedChapter();
    
    if(nextLevel <= NUM_LEVELS_PER_STAGE )
    {
    }
    else
    {
        selectedStage++;
        nextLevel = 1;
    }
    
    GameLevelSelectionState::getInstance()->setLevelTo(nextLevel);
    GameLevelSelectionState::getInstance()->setStageTo(selectedStage);

    char levelFilename[32];
    sprintf(levelFilename,"ch%dstg%dlvl%d",selectedChapter, selectedStage, nextLevel);
    
#ifdef IOS
    FileUtil fu;
    nextLevelName = fu.getAbsolutePath(levelFilename).c_str();
#else
    nextLevelName = levelFilename;
#endif
    
    return nextLevelName;
}

void GamePlayHUDController::keyPressed(GUIComponent* pComponent, const Event& event)
{
    if( event.keyInput.Key == KEY_LEFT )
    {
        goToPrevLevel();
    }
    if( event.keyInput.Key == KEY_RIGHT )
    {
        goToNextLevel();
    }
}

void GamePlayHUDController::createLevelCompletePanel()
{
    m_pLevelCompleteLabel = new GUILabel(Game::getInstance()->getGUIManager(), 250, 50);
    m_pLevelCompleteLabel->setTransparency(0.8);
    m_pLevelCompleteLabel->SetTextScale(1.5f);
    m_pLevelCompleteLabel->SetTextColor(Color4f(1.0f,0.5f,0.0f,1.0f));
    m_pLevelCompleteLabel->SetText("LEVEL COMPLETE");
    m_pLevelCompleteLabel->setPosition(500,400);
    m_pLevelCompleteLabel->SetVisible(false);
    m_pLevelCompleteLabel->setEnable(false);
}

vector<Ray>& GamePlayHUDController::getGlowingRays()
{
    return m_vecGlowingRays;//((GUIPauseButton*)m_pBackToLevelSelectionButton)->getAllRays();
}
