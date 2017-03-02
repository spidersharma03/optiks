//
//  GameStageSelectionHUDController.cpp
//  Optiks
//
//  Created by Prashant Sharma on 05/12/13.
//
//

#include "FBO.h"
#include "GameChapterSelectionHUDController.h"
#include "GameMenuState.h"
#include "GameLevelSelectionState.h"
#include "Game.h"
#include "GameSettings.h"
#include "SceneTransitionTask.h"
#include "Scene.h"
#include "TaskManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "GLSL2ShaderProgram.h"
#include "GUIChapterSelectionButton.h"
#include "GUISettingsButton.h"
#include "ParticleBase.h"
#include "ParticleSystem.h"

GameChapterSelectionHUDController::GameChapterSelectionHUDController()
{
    m_DistanceFromMousePressedPosition = 0;
    m_SelectedChapter = -1;
    createGameChapterSelectionView();
    m_pParticleSystemDiffuse = new ParticleSystem(300, ParticleSystem::EPS_QUAD);
    ParticleEmitter *pEmitter = new ParticleEmitterRect( Game::getInstance()->getScene()->getSceneWidth(), Game::getInstance()->getScene()->getSceneHeight());
    pEmitter->setEmissionRate(100);
    pEmitter->setVelocityVariance(Vector2f(200,200));
    pEmitter->setSize(5);
    pEmitter->setSizeVariance(1);
    m_pParticleSystemDiffuse->setParticleEmitter(pEmitter);
    m_pParticleAnimatorDiffuse = new ParticleDiffuseAnimator(m_pParticleSystemDiffuse);
    m_pParticleSystemDiffuse->addParticleAnimator(m_pParticleAnimatorDiffuse);
    m_pParticleSystemDiffuse->setTexture(Game::getInstance()->m_pGuiHaloTexture);
    
    int bLocked;
    GameProgressionInfo& gInfo = Game::getInstance()->gameProgressionInfo;
    
    for( int i=0; i<NUM_CHAPTERS; i++ )
    {
        bLocked = gInfo.chapterProgInfo[i].bLocked;
        
        if( !bLocked )
            unLockChapter(i);
    }
}

GameChapterSelectionHUDController::~GameChapterSelectionHUDController()
{
}

// IActionListener
void GameChapterSelectionHUDController::mousePressed(GUIComponent* pComponent, int x, int y)
{
    m_MousePressedPosition.x = x;
    m_MousePressedPosition.y = y;
    m_DistanceFromMousePressedPosition = m_MousePressedPosition.distanceSquared(Vector2i(x,y));
}

void GameChapterSelectionHUDController::mouseReleased(GUIComponent* pComponent, int x, int y)
{
    if( m_DistanceFromMousePressedPosition > 16 )
    {
        m_DistanceFromMousePressedPosition = m_MousePressedPosition.distanceSquared(Vector2i(x,y));
        return;
    }
    if( pComponent )
    {
        int tag = pComponent->getTag();
        // Chapter Selection Buttons
        if( tag >= 0 && tag < 100 )
        {
            m_SelectedChapter = tag+1;
            SceneTransitionTask *pSceneTransitionTask = new SceneTransitionTask( GameLevelSelectionState::getInstance(), "", Game::getInstance()->getScene()->getSceneWidth(),
                                                                                Game::getInstance()->getScene()->getSceneHeight(),
                                                                                1.0f );
            
            Game::getInstance()->getTaskManager()->Attach(pSceneTransitionTask);
        }
        // Back to Main Menu
        if( tag == 100 )
        {
            SceneTransitionTask *pSceneTransitionTask = new SceneTransitionTask( GameMenuState::getInstance(), "", Game::getInstance()->getScene()->getSceneWidth(),
                                                                                Game::getInstance()->getScene()->getSceneHeight(),
                                                                                1.0f );
            
            Game::getInstance()->getTaskManager()->Attach(pSceneTransitionTask);
        }
    }
}

void GameChapterSelectionHUDController::mouseMoved(GUIComponent* pComponent, int x, int y)
{
    m_DistanceFromMousePressedPosition = m_MousePressedPosition.distanceSquared(Vector2i(x,y));
}

void GameChapterSelectionHUDController::keyPressed(GUIComponent* pComponent, const Event& event)
{
    
}

void GameChapterSelectionHUDController::showGameChapterSelectionView()
{
    m_pChapterSelectionView->setVisible(true);
    m_pMainMenuSelectionButton->setVisible(true);
    m_pSettingsButton->setVisible(true);
}

void GameChapterSelectionHUDController::hideGameChapterSelectionView()
{
    m_pChapterSelectionView->setVisible(false);
    m_pMainMenuSelectionButton->setVisible(false);
    m_pSettingsButton->setVisible(false);
}

int GameChapterSelectionHUDController::getSelectedChapter() const
{
    return m_SelectedChapter;
}

void GameChapterSelectionHUDController::createGameChapterSelectionView()
{
    m_pChapterSelectionView = new GUIScrollView(Game::getInstance()->getGUIManager(), 1500, 600);
    m_pChapterSelectionView->setTransparency(1.0f);
    m_pChapterSelectionView->setPosition(-200, 100);
    m_pChapterSelectionView->setPeriod(1024);
    
    m_pChapterSelectionView->setBoundaryLimits(0, 1);
    
    char buffer[20];
    
    int x = 512+50;
    int y = 150;
    int dx = 1024;
    for( int i=0; i<NUM_CHAPTERS; i++ )
    {
        GUIButton* pButton = new GUIChapterSelectionButton(m_pChapterSelectionView, 300,300);
        pButton->setPosition(x, y);
        pButton->setTag(i);
        pButton->setActionListener(this);
        m_vecChapterSelectButtons.push_back(pButton);
        
        sprintf(buffer, "Chapter %d", i+1);
        pButton->setTitle(buffer);
        
        x += dx;
    }
    
    //((GUIChapterSelectionButton*)m_vecChapterSelectButtons[1])->setRayColor(Color4f(0.0f,1.0f,0.5f,1.0f));
    
    m_pMainMenuSelectionButton = new GUIButton(Game::getInstance()->getGUIManager(), 50, 25 );
    m_pMainMenuSelectionButton->setPosition(20, 640);
    m_pMainMenuSelectionButton->setTag(100);
    m_pMainMenuSelectionButton->setActionListener(this);
    m_pMainMenuSelectionButton->setTitle("Menu");
    m_pMainMenuSelectionButton->setFontScale(0.45f);
    m_pMainMenuSelectionButton->setTexture((Game::getInstance()->m_pDefaultGradientTexture));
    m_pMainMenuSelectionButton->setVisible(true);

    m_pSettingsButton = new GUISettingsButton(Game::getInstance()->getGUIManager(), 40, 40);
    m_pSettingsButton->setPosition(20, 690);
    m_pSettingsButton->setTitle("<=");
    m_pSettingsButton->setTag(101);
    m_pSettingsButton->setActionListener(this);
    
    hideGameChapterSelectionView();
}

void GameChapterSelectionHUDController::renderUI()
{
    //return;
    Game::getInstance()->getRenderer()->setBlendingEnable(true);
    Game::getInstance()->getRenderer()->setBlendingFactors(spider3d::EBF_SRC_ALPHA, spider3d::EBF_ONE);
 
    m_pParticleSystemDiffuse->update();
    // render glowing rays
    renderGlowingRays();
    
    // Blur Texture Pass
    Game::getInstance()->getScene()->blurTexturePass();
    
    // Glow Pass
    Game::getInstance()->getScene()->glowPass();
    
    // Draw Reflection
    reflectionDrawPass();
    
    // Draw Background
    drawBackgroundPass();
    
    // Water ripple pass
    Game::getInstance()->getScene()->waterRipplePass();
    
    // Final Composite Pass
    Game::getInstance()->getScene()->finalCompositePass();
    
    // PASS 8  :: Draw the quad with final composite Texture
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT );
    
    int screenScale = Game::getInstance()->getScene()->getScreenScaleFactor();
    
    glViewport(0, 0, screenScale*Game::getInstance()->getScene()->getSceneWidth(), screenScale*Game::getInstance()->getScene()->getSceneHeight());

    Game::getInstance()->getRenderer()->setTextureObject(Game::getInstance()->getScene()->m_FinalCompositeTexture);
    
    Game::getInstance()->getScene()->m_pDefaultShaderProgram->OnSet();
    
    Game::getInstance()->getScene()->DrawQuad(Game::getInstance()->getScene()->m_SceneWidth, Game::getInstance()->getScene()->m_SceneHeight);
    
    Game::getInstance()->getRenderer()->setTextureObject(0);
    
    //renderAllRays(10.0f);
    
    Game::getInstance()->getScene()->m_pDefaultShaderProgram->UnSet();
    
    // Draw Buttons on top
}

void GameChapterSelectionHUDController::renderAllRays(float width )
{
    // render glowing rays
    for( int i=0; i<m_vecChapterSelectButtons.size(); i++ )
    {
        GUIChapterSelectionButton* pButton = (GUIChapterSelectionButton*)m_vecChapterSelectButtons[i];
        pButton->renderRays(width);
    }
    
    m_pParticleSystemDiffuse->render();
    
//    for( int i=0; i<NUM_STAGES; i++ )
//    {
//        GUIStageSelectionButton* pButton = (GUIStageSelectionButton*)m_pStageSelectionButtons[i];
//        if( pButton->isClipped() )
//            pButton->renderRays(width);
//    }
//    for( int i=0; i<NUM_STAGES; i++ )
//    {
//        GUILevelSelectionPanel* pPanel = (GUILevelSelectionPanel*)m_pLevelSelectionViewPanels[i];
//        pPanel->renderRays(3*width);
//    }
}

void GameChapterSelectionHUDController::renderGlowingRays()
{
    ShaderProgram* pDefaultShaderProgram = Game::getInstance()->getScene()->m_pDefaultShaderProgram;
    pDefaultShaderProgram->OnSet();
    
    Matrix4f orthoMatrix;
    orthoMatrix.ortho(0, Game::getInstance()->getScene()->getSceneWidth(), 0, Game::getInstance()->getScene()->getSceneHeight(), -1, 1);
    Game::getInstance()->getRenderer()->setTransform(orthoMatrix, spider3d::PROJECTION);
    
    Matrix4f xForm;
    Game::getInstance()->getRenderer()->setTransform(xForm, spider3d::WORLD);
    
    // Render All the rays for glow in FBO
    int width = Game::getInstance()->getScene()->getSceneWidth()/2;
    int height = Game::getInstance()->getScene()->getSceneHeight()/2;
    
    FrameBufferObjectTexture_2D* pFbo = Game::getInstance()->getScene()->m_Fbo;
    pFbo->onSet();
    // Draw all the rays in the texture
    glViewport(0, 0, width, height);
    pFbo->attachTexture(Game::getInstance()->getScene()->m_RenderTextureGlowCurrent[0]);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // render glowing rays
    renderAllRays(10.0f);
    
    width /= 2;
    height /= 2;
    
    //Draw smaller textures for glow pass
    for( int i=1; i<NUM_TEXTURES; i++ )
    {
        glViewport(0, 0, width, height);
        pFbo->attachTexture(Game::getInstance()->getScene()->m_RenderTextureGlowCurrent[i]);
        Game::getInstance()->getRenderer()->setTextureObject(Game::getInstance()->getScene()->m_RenderTextureGlowCurrent[i-1]);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        Game::getInstance()->getScene()->DrawQuad(Game::getInstance()->getScene()->getSceneWidth(), Game::getInstance()->getScene()->getSceneHeight());
        
        width /= 2;
        height /= 2;
    }
    
    pDefaultShaderProgram->UnSet();
    
    for( int i=0; i<NUM_TEXTURES; i++ )
    {
        SWAP_TEXTURES(Game::getInstance()->getScene()->m_RenderTextureGlowPrevious[i], Game::getInstance()->getScene()->m_RenderTextureGlowCurrent[i]);
    }
}

void GameChapterSelectionHUDController::drawBackgroundPass()
{
    glViewport(0, 0, Game::getInstance()->getScene()->getSceneWidth(), Game::getInstance()->getScene()->getSceneHeight());
    
    FrameBufferObjectTexture_2D* pFbo = Game::getInstance()->getScene()->m_Fbo;
    
    pFbo->attachTexture(Game::getInstance()->getScene()->m_RenderTextureCurrent);
    
    Game::getInstance()->getScene()->m_pDefaultShaderProgram->OnSet();
    
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT );
    
    // Draw Into FBO the background.

    Game::getInstance()->getRenderer()->setTextureObject(Game::getInstance()->getScene()->m_BackGroundTexture);

    Game::getInstance()->getScene()->DrawQuad(Game::getInstance()->getScene()->getSceneWidth(), Game::getInstance()->getScene()->getSceneHeight(),1.0f,1.0f/1.5);
    //glDisable(GL_TEXTURE_2D);
    Game::getInstance()->getRenderer()->setTextureObject(0);
    
    Matrix4f orthoMatrix;
    orthoMatrix.ortho(0, Game::getInstance()->getScene()->getSceneWidth(), 0, Game::getInstance()->getScene()->getSceneHeight(), -1, 1);
    Game::getInstance()->getRenderer()->setTransform(orthoMatrix, spider3d::PROJECTION);
    
    
    // Draw Into the FBO all the rays
    renderAllRays(10.0f);
    
    Game::getInstance()->getScene()->m_pDefaultShaderProgram->UnSet();
}

void GameChapterSelectionHUDController::reflectionDrawPass()
{
    // Draw objects in reflection map with appropriate scaling
    glViewport(0, 0, Game::getInstance()->getScene()->m_TextureSizeX/Game::getInstance()->getScene()->m_ReflectionTextureScale, Game::getInstance()->getScene()->m_TextureSizeY/Game::getInstance()->getScene()->m_ReflectionTextureScale);
    Game::getInstance()->getScene()->m_Fbo->attachTexture(Game::getInstance()->getScene()->m_ReflectionTexture);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    Matrix4f orthoMatrix;
    orthoMatrix.ortho(0, Game::getInstance()->getScene()->m_SceneWidth, 0, Game::getInstance()->getScene()->m_SceneHeight, -1, 1);
    Game::getInstance()->getScene()->m_pDefaultShaderProgram->OnSet();
    
    Game::getInstance()->getRenderer()->setTransform(orthoMatrix, spider3d::PROJECTION);
    
    renderAllRays(10.0);
    
    Game::getInstance()->getScene()->m_pDefaultShaderProgram->UnSet();
    
    //    Game::getInstance()->getScene()->m_ReflectionMapProgram->OnSet();
    
    //    Matrix4f orthoMatrix;
    //    orthoMatrix.ortho(0, Game::getInstance()->getScene()->m_SceneWidth, 0, Game::getInstance()->getScene()->m_SceneHeight, -1, 1);
    //    Game::getInstance()->getRenderer()->setTransform(orthoMatrix, spider3d::PROJECTION);
    //
    //
    //    Game::getInstance()->getRenderer()->setTextureObject(0);
    
    //    Game::getInstance()->getScene()->m_ReflectionMapProgram->UnSet();
}

void GameChapterSelectionHUDController::unLockChapter(int chapter)
{
    m_vecChapterSelectButtons[chapter]->setEnable(true);
}

void GameChapterSelectionHUDController::setChapterTo(int chapter)
{
    m_SelectedChapter = chapter;
}