//
//  FadeSceneTask.cpp
//  Optiks
//
//  Created by Prashant on 15/03/14.
//
//

#include "FadeSceneTask.h"
#include "DTimer.h"
#include "Game.h"
#include "Scene.h"
#include "Renderer.h"
#include "GamePlayState.h"
#include "ShaderProgram.h"

FadeSceneTask::FadeSceneTask(float sceneWidth, float sceneHeight, float startTransparency, float endTransparency , float lerpTime)
{
    m_StartTransparency = startTransparency;
    m_EndTransparency = endTransparency;
    m_LerpTime = lerpTime * 1000;
    m_SceneWidth = sceneWidth;
    m_SceneHeight = sceneHeight;
    m_StartTime = DTimer::getTime();
}

FadeSceneTask::~FadeSceneTask()
{
    
}

void FadeSceneTask::reset()
{
    m_StartTime = DTimer::getTime();
}

void FadeSceneTask::Update(float deltaSeconds)
{
    Game::getInstance()->getRenderer()->setBlendingEnable(true);
    Game::getInstance()->getRenderer()->setBlendingFactors(EBF_SRC_ALPHA, EBF_ONE_MINUS_SRC_ALPHA);

    Matrix4f orthoMatrix, xForm;
    orthoMatrix.ortho(0, m_SceneWidth, 0, m_SceneHeight, -1, 1);
    Game::getInstance()->getScene()->m_pDefaultColorShaderProgram->OnSet();
    Game::getInstance()->getRenderer()->setTransform(orthoMatrix, PROJECTION);
    Game::getInstance()->getRenderer()->setTransform(xForm, WORLD);
    
    m_Time = (DTimer::getTime() - m_StartTime)/m_LerpTime;
    
    float transparency = m_StartTransparency * ( 1.0f - m_Time ) + m_EndTransparency * m_Time;
    
    if( transparency < 0.99 )
    {
        if( transparency < 0.3f )
            transparency = 0.3f;
        
        Game::getInstance()->getRenderer()->DrawQuad(m_SceneWidth, m_SceneHeight, Color4f(0.0f,0.0f,0.0f,1.0f-transparency) );
        //printf("%f \n", transparency);
    }
    
    //Game::getInstance()->getScene()->m_pDefaultColorShaderProgram->UnSet();
}