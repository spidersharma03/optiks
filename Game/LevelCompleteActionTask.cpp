//
//  LevelCompleteActionTask.cpp
//  Optiks
//
//  Created by Prashant on 28/12/13.
//
//

#include "LevelCompleteActionTask.h"
#include "DTimer.h"
#include "Game.h"
#include "Scene.h"
#include "GamePlayHUDController.h"
#include "GameLevelCompleteHUDController.h"
#include "GameLevelCompleteState.h"
#include "GamePlayState.h"
#include "SinkGameObject.h"

LevelCompleteActionTask::LevelCompleteActionTask()
{
    m_TaskCompletionTime = 0.6f * 1000;
    
    m_StartTime = DTimer::getTime();
    
    GameLevelCompleteHUDController* pHUDController = GameLevelCompleteState::getInstance()->getGameLevelCompleteHUDController();

    pHUDController->setTransparency(1.0f);
    
    m_bOnce = true;
}

LevelCompleteActionTask::~LevelCompleteActionTask()
{
    
}

void LevelCompleteActionTask::Update(float deltaSeconds)
{
    if( m_bOnce )
    {
        m_StartTime = DTimer::getTime();
        m_bOnce = false;
        Game::getInstance()->getScene()->fadeIn();
    }
    double time = (DTimer::getTime() - m_StartTime)/m_TaskCompletionTime;
    //GamePlayHUDController* pGamePlayHUDController = GamePlayState::getInstance()->getGamePlayHUDController();
    GameLevelCompleteHUDController* pHUDController = GameLevelCompleteState::getInstance()->getGameLevelCompleteHUDController();
    
    pHUDController->setTransparency( 1.0f - time );
   
//    GUILabel* pLabel = pGamePlayHUDController->getLevelCompleteLabel();
//    if( time < 1.0f )
//    {
//        float lerp = 0.2f * time;
//        pLabel->setTransparency(1.0-lerp);
//        Color4f color = pLabel->GetTextColor();
//        color.a = time;
//        pLabel->SetTextColor(color);
//    }
//    else
//    {
//        double time1 = ( time - 1.0f)/2.0f; // change from Range 1-3 to 0-1
//        pLabel->setTransparency( 0.8f + time1 * 0.2f);
//        Color4f color = pLabel->GetTextColor();
//        color.a = 1.0f-time1;
//        pLabel->SetTextColor(color);
//    }
    if( time >= 1.0 )
    {
        //m_bIsCompleted = true;
        //Game::getInstance()->setLevelCompleteTaskFired(false);
        Kill();
    }
    /*Scene* pScene = Game::getInstance()->getScene();
    vector<SinkGameObject*>& vecSinkGameObjects = pScene->getAllSinkObjects();
    for( int i=0; i<vecSinkGameObjects.size(); i++ )
    {
        SinkGameObject* pSinkGameObject = vecSinkGameObjects[i];
        pSinkGameObject->m_CurrentRayWidthMultFactor = 1.3f;
    }*/
    
}