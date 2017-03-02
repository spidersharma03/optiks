//
//  SceneTransitionTask.cpp
//  Optiks
//
//  Created by Prashant on 09/03/14.
//
//

#include "SceneTransitionTask.h"
#include "DTimer.h"
#include "Game.h"
#include "Scene.h"
#include "Renderer.h"
#include "GamePlayState.h"
#include "ShaderProgram.h"

SceneTransitionTask::SceneTransitionTask(IState<Game>* pTansitionState, const char* levelFileName, float sceneWidth, float sceneHeight, float lerpTime, const Color4f& color)
{
    m_pTansitionState = pTansitionState;
    m_LevelFileName = levelFileName;
    m_LerpTime = lerpTime * 1000;
    m_SceneWidth = sceneWidth;
    m_SceneHeight = sceneHeight;
    m_Color = color;
    m_StartTime = DTimer::getTime();
    m_bLevelLoaded = false;
}

SceneTransitionTask::~SceneTransitionTask()
{
}

void SceneTransitionTask::reset()
{
    m_StartTime = DTimer::getTime();
}

void SceneTransitionTask::Update(float deltaSeconds)
{
    Game::getInstance()->getRenderer()->setBlendingFactors(spider3d::EBF_SRC_ALPHA, spider3d::EBF_ONE_MINUS_SRC_ALPHA);
    
    Matrix4f orthoMatrix, xForm;
    orthoMatrix.ortho(0, m_SceneWidth, 0, m_SceneHeight, -1, 1);
    Game::getInstance()->getScene()->m_pDefaultColorShaderProgram->OnSet();
    Game::getInstance()->getRenderer()->setTransform(orthoMatrix, PROJECTION);
    Game::getInstance()->getRenderer()->setTransform(xForm, WORLD);
    
    m_Time = (DTimer::getTime() - m_StartTime)/m_LerpTime;
    
    float time = m_Time;
    
    m_Time = m_Time <= 0.5f ? 2.0f * m_Time : 2.0f * ( 1 - m_Time );
    
    Color4f color = m_Color;
    color.a = m_Time;
    
    if( time <= 1.0f )
    {
        //printf("%f  %f\n", time, m_Time);
        if( time > 0.5 )
        {
            int i=0;
            i++;
        }
        Game::getInstance()->getRenderer()->DrawQuad(m_SceneWidth, m_SceneHeight, color);
    }
    
    if( time >= 0.5f && !m_bLevelLoaded)
    {
        m_bLevelLoaded = true;
        if( m_LevelFileName.size() > 0 )
            Game::getInstance()->getScene()->load(m_LevelFileName.c_str());
        Game::getInstance()->changeState(m_pTansitionState);
    }
    if( time >= 1.0 )
    {
        Game::getInstance()->getScene()->m_pDefaultColorShaderProgram->UnSet();
        Kill();
    }
}