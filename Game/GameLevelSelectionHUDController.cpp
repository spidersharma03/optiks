//
//  GameLevelSelectionHUDController.cpp
//  Optiks
//
//  Created by Prashant Sharma on 05/12/13.
//
//

#include "FBO.h"
#include "GameLevelSelectionHUDController.h"
#include "GameChapterSelectionState.h"
#include "GameMenuState.h"
#include "GamePlayState.h"
#include "Game.h"
#include "Scene.h"
#include "GameSettings.h"
#include "FileUtil.h"
#include "StageSelectionScrollView.h"
#include "GUILevelSelectionButton.h"
#include "GUIStageSelectionButton.h"
#include "GLSL2ShaderProgram.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "GUILevelSelectionPanel.h"
#include "SceneTransitionTask.h"
#include "TaskManager.h"
#include "GUIBackButton.h"
#include "GUIContinueButton.h"
#include "GameChapterSelectionHUDController.h"
#include "ParticleBase.h"
#include "ParticleSystem.h"

GameLevelSelectionHUDController::GameLevelSelectionHUDController()
{
    m_SelectedLevel = -1;
    m_SelectedStage = 1;
    m_DistanceFromMousePressedPosition = 0;
    
    m_pLevelSelectionScrollView = new GUIScrollView(Game::getInstance()->getGUIManager(), 900, 600);
    m_pLevelSelectionScrollView->setPeriod(1024);
    m_pLevelSelectionScrollView->setTransparency(1.0f);
    m_pLevelSelectionScrollView->setPosition(60, 84);
    m_pLevelSelectionScrollView->setTag(800);
    m_pLevelSelectionScrollView->setActionListener(this);
    m_pLevelSelectionScrollView->setBoundaryLimits(0, NUM_STAGES-1);

    m_pStageSelectionScrollView = new StageSelectionScrollView(Game::getInstance()->getGUIManager(), 600,120);
    m_pStageSelectionScrollView->setPosition(200, 30);
    m_pStageSelectionScrollView->setTransparency(1.0f);
    m_pStageSelectionScrollView->setPeriod(200);
    m_pStageSelectionScrollView->setTag(900);
    m_pStageSelectionScrollView->setActionListener(this);
    m_pStageSelectionScrollView->setBoundaryLimits(1, NUM_STAGES-2);

    int x = 50, y = 10;
    int dx = 200;
    char buffer[10];
    for( int i=0; i<NUM_STAGES; i++ )
    {
        sprintf(buffer,"%d", i+1);
        m_pStageSelectionButtons[i] = new GUIStageSelectionButton(m_pStageSelectionScrollView, 100, 100 );
        m_pStageSelectionButtons[i]->setPosition(x, y);
        m_pStageSelectionButtons[i]->setTag(1000+i);
        m_pStageSelectionButtons[i]->setTitle(buffer);
        m_pStageSelectionButtons[i]->setFontScale(0.8f);
        m_pStageSelectionButtons[i]->setActionListener(this);
        x += dx;
    }
    x = 0, y = 0;
    for( int i=0; i<NUM_STAGES; i++ )
    {
        GUIPanel* pPanel = createGameLevelSelectionView(i);
        pPanel->setPosition(x, y);
        pPanel->setTransparency(1.0f);
        x += 1024;
    }
    
    m_pBackToStageSelectionButton = new GUIBackButton(Game::getInstance()->getGUIManager(), 40,40);
    m_pBackToStageSelectionButton->setPosition(20, 690);
    m_pBackToStageSelectionButton->setTitle("<=");
    m_pBackToStageSelectionButton->setTag(101);
    m_pBackToStageSelectionButton->setActionListener(this);
    m_pBackToStageSelectionButton->setTexture((Game::getInstance()->m_pDefaultGradientTexture));
    
    hideGameLevelSelectionView();
    
    unlockStageAndLevelButtonsFromGameProgression();
    
}


void GameLevelSelectionHUDController::unlockStageAndLevelButtonsFromGameProgression()
{
    int bLocked;
    GameProgressionInfo& gInfo = Game::getInstance()->gameProgressionInfo;
    
    for( int i=0; i<NUM_CHAPTERS; i++ )
    {
        for( int j=0; j<NUM_STAGES; j++ )
        {
            bLocked = gInfo.chapterProgInfo[i].stageProgInfo[j].bLocked;
            
            for( int k=0; k<NUM_LEVELS_PER_STAGE; k++ )
            {
                bLocked = gInfo.chapterProgInfo[i].stageProgInfo[j].levelProgInfo[k].bLocked;
                
                if( bLocked == 0 )
                {
                    unlockLevel(j+1,k+1);
                }
                else if ( bLocked == 2 )
                {
                    unlockLevel(j+1,k+1);
                    ((GUIStageSelectionButton*)m_pStageSelectionButtons[j])->setButtonState(2);
                    ((GUILevelSelectionButton*)m_pLevelSelectionButtons[j][k])->setButtonState(2);
                }
            }
        }
    }
}

GameLevelSelectionHUDController::~GameLevelSelectionHUDController()
{
    
}

// IActionListener
void GameLevelSelectionHUDController::mousePressed(GUIComponent* pComponent, int x, int y)
{
    m_MousePressedPosition.x = x;
    m_MousePressedPosition.y = y;
    m_DistanceFromMousePressedPosition = m_MousePressedPosition.distanceSquared(Vector2i(x,y));
}

void GameLevelSelectionHUDController::mouseReleased(GUIComponent* pComponent, int x, int y)
{
    if( pComponent )
    {
        int tag = pComponent->getTag();
        // Level Selection Scroll View. Scroll the Stage Selection View from meanPos of Level Scroll View
        if( tag == 800 )
        {
            int levelPeriod = m_pLevelSelectionScrollView->getPeriod();
            int meanPosition = m_pLevelSelectionScrollView->getMeanPosition();
            float index = (float)meanPosition/levelPeriod;
            int stagePeriod = m_pStageSelectionScrollView->getPeriod();
            m_pStageSelectionScrollView->setMeanPosition((index+1)*stagePeriod);
            m_SelectedStage = -index+1;
        }
        // Stage Selection Scroll View. Scroll the Level Selection View from meanPos of Level Scroll View
        if( tag == 900 )
        {
            int stagePeriod = m_pStageSelectionScrollView->getPeriod();
            int meanPosition = m_pStageSelectionScrollView->getMeanPosition();
            float index = (float)meanPosition/stagePeriod;
            int levelPeriod = m_pLevelSelectionScrollView->getPeriod();
            m_pLevelSelectionScrollView->setMeanPosition((index-1)*levelPeriod);
            setStageTo(-(index-2));
        }
        if( m_DistanceFromMousePressedPosition > 16 )
        {
            m_DistanceFromMousePressedPosition = m_MousePressedPosition.distanceSquared(Vector2i(x,y));
            return;
        }

        // Stage Selection. Scroll the Level Selection from selected Stage.
        if( tag >= 1000 )
        {
            int period = m_pLevelSelectionScrollView->getPeriod();
            int index = tag - 1000;
            m_pLevelSelectionScrollView->setMeanPosition( -(index)*period );
            
            //int stagePeriod = m_pStageSelectionScrollView->getPeriod();
            //m_pStageSelectionScrollView->setMeanPosition((1-index)*stagePeriod);
            
            m_SelectedStage = index+1;
        }
        // Level Selection
        if(tag < 100 )
        {
            m_SelectedLevel = tag+1;
            char levelFilename[32];
            int selectedChapter = GameChapterSelectionState::getInstance()->getSelectedChapter();
            sprintf(levelFilename,"ch%dstg%dlvl%d", selectedChapter, m_SelectedStage, m_SelectedLevel);
            string levelFilePath;
#ifdef IOS
            FileUtil fu;
            levelFilePath = fu.getAbsolutePath(levelFilename);
#else
            levelFilePath = levelFilename;
#endif
            SceneTransitionTask *pSceneTransitionTask = new SceneTransitionTask( GamePlayState::getInstance(), levelFilePath.c_str(), Game::getInstance()->getScene()->getSceneWidth(),
                                                             Game::getInstance()->getScene()->getSceneHeight(),
                                                             1.0f );
            
            Game::getInstance()->getTaskManager()->Attach(pSceneTransitionTask);
            //Game::getInstance()->getScene()->load(levelFilePath.c_str());
            //Game::getInstance()->changeState(GamePlayState::getInstance());
            
        }
        // Back to Chapter Selection
        if( tag == 101 )
        {
            SceneTransitionTask *pSceneTransitionTask = new SceneTransitionTask( GameChapterSelectionState::getInstance(), "", Game::getInstance()->getScene()->getSceneWidth(),
                                                                                Game::getInstance()->getScene()->getSceneHeight(),
                                                                                1.0f );
            
            Game::getInstance()->getTaskManager()->Attach(pSceneTransitionTask);
            //Game::getInstance()->changeState(GameChapterSelectionState::getInstance());
        }
        // Previous Level
        if( tag == 102 )
        {
        }
        // Next Level
        if( tag == 103 )
        {
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

int GameLevelSelectionHUDController::getSelectedLevel() const
{
    return m_SelectedLevel;
}

int GameLevelSelectionHUDController::getSelectedStage() const
{
    return m_SelectedStage;
}

void GameLevelSelectionHUDController::setLevelTo(int level)
{
    assert(level <= NUM_LEVELS_PER_STAGE && level > 0);
    m_SelectedLevel = level;
}

void GameLevelSelectionHUDController::setStageTo(int stage)
{
    assert(stage <= NUM_STAGES && stage > 0);
    m_SelectedStage = stage;
}

void GameLevelSelectionHUDController::mouseMoved(GUIComponent* pComponent, int x, int y)
{
    m_DistanceFromMousePressedPosition = m_MousePressedPosition.distanceSquared(Vector2i(x,y));
}

void GameLevelSelectionHUDController::keyPressed(GUIComponent* pComponent, const Event& event)
{
    
}

void GameLevelSelectionHUDController::showGameLevelSelectionView()
{
    m_pLevelSelectionScrollView->setVisible(true);
    //m_pMainMenuSelectionButton->setVisible(true);
    m_pBackToStageSelectionButton->setVisible(true);
    //m_pNextLevelSelectionButton->setVisible(true);
    //m_pPreviousLevelSelectionButton->setVisible(true);
    m_pStageSelectionScrollView->setVisible(true);
}

void GameLevelSelectionHUDController::hideGameLevelSelectionView()
{
    m_pLevelSelectionScrollView->setVisible(false);
    //m_pMainMenuSelectionButton->setVisible(false);
    m_pBackToStageSelectionButton->setVisible(false);
    //m_pNextLevelSelectionButton->setVisible(false);
    //m_pPreviousLevelSelectionButton->setVisible(false);
    m_pStageSelectionScrollView->setVisible(false);
}

//GUIPanel* GameLevelSelectionHUDController::createGameLevelSelectionView(int index)
//{
//    m_pLevelSelectionViewPanels[index] = new GUILevelSelectionPanel(m_pLevelSelectionScrollView, 1000,600);
//
//    int x = 100;
//    int y = 100;
//    int dx = 200;
//    int dy = 150;
//    
//    int nRows = 3;
//    int nCols = 4;
//    int lvlCount = 0;
//    char buffer[10];
//
//    for( int i=0; i<nRows ; i++ )
//    {
//        x = 100;
//        for( int j=0; j<nCols ; j++ )
//        {
//            GUIButton* pButton = new GUILevelSelectionButton(m_pLevelSelectionViewPanels[index], 45,45);
//            pButton->setPosition(x, y);
//            pButton->setTag(lvlCount++);
//            pButton->setActionListener(this);
//            
//            sprintf(buffer,"LEVEL %d", lvlCount);
//            pButton->setTitle(buffer);
//            
//            m_vecLevelSelectionButtons.push_back((GUILevelSelectionButton*)pButton);
//
//            x += dx;
//        }
//        y += dy;
//    }
//    
//    return m_pLevelSelectionViewPanels[index];
//}

GUIPanel* GameLevelSelectionHUDController::createGameLevelSelectionView(int index)
{
    m_pLevelSelectionViewPanels[index] = new GUILevelSelectionPanel(m_pLevelSelectionScrollView, 900,600);
    
    char buffer[20];
    sprintf(buffer, "Stage %d", index+1);
    ((GUILevelSelectionPanel*)m_pLevelSelectionViewPanels[index])->setTitle(buffer);
    int x = 0;
    int y = 0;
    
    int lvlCount = 0;
    float radius = 230;
    Vector2i centre(430,320);
    float theta = 0.0f;
    float dTheta = M_PI * 2.0/NUM_LEVELS_PER_STAGE;
    
    for( int i=0; i<NUM_LEVELS_PER_STAGE ; i++ )
    {
        x = centre.x + radius * cos(theta);
        y = centre.y + radius * sin(theta);
        
        GUIButton* pButton = new GUILevelSelectionButton(m_pLevelSelectionViewPanels[index], 45,45);
        pButton->setPosition(x, y);
        pButton->setTag(lvlCount++);
        pButton->setActionListener(this);
        
        sprintf(buffer,"%d", lvlCount);
        pButton->setTitle(buffer);
        pButton->setFontScale(0.6);
        theta += dTheta;
        m_vecLevelSelectionButtons.push_back((GUILevelSelectionButton*)pButton);
        m_pLevelSelectionButtons[index][i] = pButton;
    }
    
    return m_pLevelSelectionViewPanels[index];
}

void GameLevelSelectionHUDController::renderUI()
{
    //return;
    Game::getInstance()->getRenderer()->setBlendingEnable(true);
    Game::getInstance()->getRenderer()->setBlendingFactors(spider3d::EBF_SRC_ALPHA, spider3d::EBF_ONE);
    

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
    
    Game::getInstance()->getRenderer()->setTextureObject(Game::getInstance()->getScene()->m_FinalCompositeTexture);
    
    int screenScale = Game::getInstance()->getScene()->getScreenScaleFactor();
    
    glViewport(0, 0, screenScale*Game::getInstance()->getScene()->getSceneWidth(), screenScale*Game::getInstance()->getScene()->getSceneHeight());
    
    Game::getInstance()->getScene()->m_pDefaultShaderProgram->OnSet();
    
    Game::getInstance()->getScene()->DrawQuad(Game::getInstance()->getScene()->m_SceneWidth, Game::getInstance()->getScene()->m_SceneHeight);
    
    Game::getInstance()->getRenderer()->setTextureObject(0);
    
    renderAllRays(5.0, Color4f(0.6f,0.6f,0.6f,1.0f));
    
    Game::getInstance()->getScene()->m_pDefaultShaderProgram->UnSet();
    
    // Draw Buttons on top
}

void GameLevelSelectionHUDController::renderAllRays(float width, const Color4f& addColor )
{
    // render glowing rays
    for( int i=0; i<m_vecLevelSelectionButtons.size(); i++ )
    {
        GUILevelSelectionButton* pButton = m_vecLevelSelectionButtons[i];
        pButton->renderRays(width, addColor);
    }
    for( int i=0; i<NUM_STAGES; i++ )
    {
        GUIStageSelectionButton* pButton = (GUIStageSelectionButton*)m_pStageSelectionButtons[i];
        if( pButton->isClipped() )
            pButton->renderRays(width);
    }
    for( int i=0; i<NUM_STAGES; i++ )
    {
        GUILevelSelectionPanel* pPanel = (GUILevelSelectionPanel*)m_pLevelSelectionViewPanels[i];
        pPanel->renderRays(width);
    }
}

void GameLevelSelectionHUDController::renderBackground()
{
    
}

void GameLevelSelectionHUDController::renderGlowingRays()
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

void GameLevelSelectionHUDController::drawBackgroundPass()
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

void GameLevelSelectionHUDController::reflectionDrawPass()
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

    renderAllRays(20.0);
    
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

void GameLevelSelectionHUDController::markAsSolved(int stage, int level)
{
    ((GUIStageSelectionButton*)m_pStageSelectionButtons[stage-1])->setButtonState(2);
    ((GUIStageSelectionButton*)m_pLevelSelectionButtons[stage-1][level-1])->setButtonState(2);
}

void GameLevelSelectionHUDController::unlockStage( int stage )
{
    m_pStageSelectionButtons[stage-1]->setEnable(true);
}

void GameLevelSelectionHUDController::unlockLevel( int stage, int level )
{
    m_pLevelSelectionButtons[stage-1][level-1]->setEnable(true);
    m_pStageSelectionButtons[stage-1]->setEnable(true);
    ((GUILevelSelectionPanel*)m_pLevelSelectionViewPanels[stage-1])->setRayAtIndexEnable(level-1, true);
    
    Game::getInstance()->writeGameProgressionInfo();
}

void GameLevelSelectionHUDController::setLevelSelectionUiAt( int stage )
{
    if( stage > NUM_STAGES )
        return;
    int stageScrollViewPeriod = m_pStageSelectionScrollView->getPeriod();

    m_pStageSelectionScrollView->setMeanPosition((-stage+2)*stageScrollViewPeriod);

    int levelScrollViewPeriod = m_pLevelSelectionScrollView->getPeriod();
    
    m_pLevelSelectionScrollView->setMeanPosition((-stage+1)*levelScrollViewPeriod);
}
