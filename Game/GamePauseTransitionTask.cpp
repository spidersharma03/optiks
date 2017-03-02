//
//  SceneTransitionTask.cpp
//  Optiks
//
//  Created by Prashant on 09/03/14.
//
//

#include "GamePauseTransitionTask.h"
#include "DTimer.h"
#include "Game.h"
#include "Scene.h"
#include "Renderer.h"
#include "GamePlayState.h"
#include "ShaderProgram.h"
#include "GamePauseHUDController.h"

GamePauseTransitionTask::GamePauseTransitionTask( GamePauseHUDController* pHUDController, float sceneWidth, float sceneHeight, float lerpTime, float startTransparency, float endTransparency )
{
    m_pGamePauseHUDController = pHUDController;
    m_LerpTime = lerpTime * 1000;
    m_SceneWidth = sceneWidth;
    m_SceneHeight = sceneHeight;
    m_StartTime = DTimer::getTime();
    m_pGamePauseHUDController->setTransparency(startTransparency);
    m_pGamePauseHUDController->showGameLevelPauseView();
    m_StartTransparency = startTransparency;
    m_EndTransparency = endTransparency;
    
    m_Color.set(0.0f, 0.0f, 0.0f, 1.0f);
}

GamePauseTransitionTask::~GamePauseTransitionTask()
{
    
}

void GamePauseTransitionTask::reset()
{
    m_StartTime = DTimer::getTime();
}

void GamePauseTransitionTask::Update(float deltaSeconds)
{
    Matrix4f orthoMatrix, xForm;
    orthoMatrix.ortho(0, m_SceneWidth, 0, m_SceneHeight, -1, 1);
    Game::getInstance()->getScene()->m_pDefaultColorShaderProgram->OnSet();
    Game::getInstance()->getRenderer()->setTransform(orthoMatrix, PROJECTION);
    Game::getInstance()->getRenderer()->setTransform(xForm, WORLD);
    
    m_Time = (DTimer::getTime() - m_StartTime)/m_LerpTime;
    
    float time = m_Time;
    
    float transparency = m_StartTransparency * ( 1.0f - m_Time ) + m_Time * m_EndTransparency;
    
    
    Color4f color = m_Color;
    color.a = m_Time/2;
    
    if( time <= 1.0f )
    {
        m_pGamePauseHUDController->setTransparency(transparency);
        //Game::getInstance()->getRenderer()->DrawQuad(m_SceneWidth, m_SceneHeight, color);
    }
    
    if( time >= 0.5f )
    {
        //Game::getInstance()->changeState(m_pTansitionState);
    }
    if( time >= 1.0 )
    {
        Game::getInstance()->getScene()->m_pDefaultColorShaderProgram->UnSet();
        // Hide the UI when the transparency goes to zero
        if( transparency > 0.99 )
        {
            m_pGamePauseHUDController->hideGameLevelPauseView();
            Game::getInstance()->changeState(GamePlayState::getInstance());
        }

        Kill();
        
    }
}