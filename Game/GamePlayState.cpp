//
//  GamePlayState.cpp
//  Optiks
//
//  Created by Prashant Sharma on 30/10/13.
//
//

#include "GamePlayState.h"
#include "Game.h"
#include "Scene.h"
#include "GamePlayHUDController.h"
#include "GamePlayOperationStates.h"
#include "GameLevelCompleteState.h"
#include "GameLevelSelectionState.h"
#include "GameChapterSelectionState.h"
#include "FontRenderer.h"

GamePlayState* GamePlayState::s_pGamePlayState = NULL;


GamePlayState::GamePlayState()
{
    m_pGamePlayHUDController = new GamePlayHUDController();
    
    m_pPlayStateMachine = new StateMachine<Game>(NULL);
    m_pPlayStateMachine->ChangeState(GamePlayOperationMoveState::getInstance());
}

void GamePlayState::Enter(Game* pOwner)
{
    m_pGamePlayHUDController->showAllViews();
    
    Game::getInstance()->getScene()->addGlowingObjects(m_pGamePlayHUDController);
    // Disable Debug Grid
    Game::getInstance()->getScene()->setDebugGridEnable(false);
    // Disable Markers for Source and Sink.
}

void GamePlayState::Execute(Game* pOwner)
{
    Game::getInstance()->getScene()->reset();
    
    Game::getInstance()->getScene()->update();
    
    m_pPlayStateMachine->CurrentState()->Execute(pOwner);
    
    Game::getInstance()->getScene()->renderScene();

    // If All the Sinks are Intersecting, Level is solved!!!
    if( Game::getInstance()->getScene()->areAllSinkIntersecting() )
    {
        int currentStage = GameLevelSelectionState::getInstance()->getSelectedStage();
        int currentLevel = GameLevelSelectionState::getInstance()->getSelectedLevel();
        int currentChapter = GameChapterSelectionState::getInstance()->getSelectedChapter();
        
        Game::getInstance()->unlockNextLevel(currentChapter, currentStage, currentLevel);
        
        Game::getInstance()->changeState(GameLevelCompleteState::getInstance());
    }
    
    Matrix4f xForm1;
    xForm1.m00 = xForm1.m11 = 0.6f;
    FontRenderer::Instance()->SetTransform(xForm1);
    FontRenderer::Instance()->DrawText("Level");
}

void GamePlayState::Exit(Game* pOwner)
{
    m_pGamePlayHUDController->hideAllViews();
    
    Game::getInstance()->getScene()->removeGlowingObjects(m_pGamePlayHUDController);

    Game::getInstance()->getScene()->setDebugGridEnable(true);
    
    // Enable Markers.
    Game::getInstance()->getScene()->enableMarkerObjects();
}

sbool GamePlayState::OnEvent( Event& event)
{
    bool bRes = Game::getInstance()->getGUIManager()->processEvents(event);
    
    if( bRes )
        return true;
    
    if(m_pPlayStateMachine->CurrentState())
        m_pPlayStateMachine->CurrentState()->OnEvent(event);
    
    return bRes;
}