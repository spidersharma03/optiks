//
//  Game.cpp
//  Optiks
//
//  Created by Prashant Sharma on 29/10/13.
//
//

#include "Game.h"
#include "GameEditorState.h"
#include "GameMenuState.h"
#include "GamePlayState.h"
#include "Scene.h"
#include "GUI.h"
#include "TextureMaker.h"
#include "TaskManager.h"
#include "LevelCompleteActionTask.h"
#include "OglRenderer.h"
#include "OGL2Renderer.h"
#include "DTimer.h"
#include "GameLevelSelectionState.h"
#include "GameLevelSelectionHUDController.h"
#include "FileUtil.h"
#include "TimeElapsedTask.h"
#include <errno.h>

Game* Game::s_pGame = NULL;

Game::Game()
{
    m_pStateMachine = new StateMachine<Game>(this);
    m_pTaskManagerAboveGUI  =  new TaskManager();
    m_pTaskManagerBelowGUI  = new TaskManager();
    m_pGUIManager = NULL;
    m_bLevelCompleteTaskFired = false;
    initTextures();
    m_pRenderer = new Ogl2Renderer();
    
    initGameProgressionInfo();
}

void Game::changeState(IState<Game>* pState)
{
    m_pStateMachine->ChangeState(pState);
}

void Game::setGuiManager(GUIManager* pGuimanager)
{
    m_pGUIManager = pGuimanager;
}

DTimer dtimer;

void Game::update( float dt )
{
    m_pStateMachine->Update();
    
    m_pTaskManagerBelowGUI->Update(dt);
    
    m_pGUIManager->drawGUI();
    
    m_pTaskManagerAboveGUI->Update(dt);

    //dtimer.tick();
}

sbool Game::OnEvent( Event& event)
{
    return m_pStateMachine->CurrentState()->OnEvent(event);
}

void Game::initTextures()
{
    m_pReflectiveRectShapeTexture = createSuperEllipseTexture(128,128, Color4f(0.8f,1.0f,1.0f,1.0f));
    m_pReflectiveRectShapeNormalMapTexture = createSuperEllipseTexture(128,128, Color4f(0.8f,1.0f,1.0f,1.0f),true);
    m_pReflectiveRectShapeFixedTexture = createSuperEllipseTexture(128,128, Color4f(0.8f,1.0f,1.0f,1.0f), false, true);
    
    m_pTransmissiveRectShapeTexture = createReflectiveRectShapeTexture(128, 128, Color4f(1.0f,1.0f,1.0f,1.0f));
    m_pTransmissiveRectShapeNormalMapTexture = createReflectiveRectShapeTexture(128, 128, Color4f(1.0f,1.0f,1.0f,0.3f), true);
    m_pTransmissiveRectShapeFixedTexture = createReflectiveRectShapeTexture(128, 128, Color4f(1.0f,1.0f,1.0f,0.3f), false, true);
    
    m_pRefractiveRectShapeTexture = createRefractiveRectShapeTexture(128, 128, Color4f(0.0f,1.0f,1.0f,0.3f));
    m_pRefractiveRectShapeNormalMapTexture = createGemTexture(128, 128,Color4f(0.8f,1.0f,1.0f,1.0f), true);
    
    m_pAbsorptiveRectShapeTexture = createAbsorptionRectTexture(128,128, Color4f(0.5f,0.5f,0.5f,0.8f));
    m_pAbsorptiveRectShapeFixedTexture = createAbsorptionRectTexture(128,128, Color4f(0.5f,0.5f,0.5f,0.8f), false, true);
    m_pAbsorptiveRectShapeNormalMapTexture = m_pReflectiveRectShapeNormalMapTexture;
    
    m_pPortalRectShapeTexture = createPortalTexture(128, 128, Color4f(0.0f,1.0f,1.0f,0.3f));
    m_pPortalRectShapeNormalMapTexture = m_pReflectiveRectShapeNormalMapTexture;
    
    m_pPlacementMarkerTexture = createPlacementMarkerTexture(128,128, Color4f(1.0f,1.0f,1.0f,1.0f));
    
    m_pChapterSelectionButtonTexture = createReflectiveRectShapeTexture(256, 256, Color4f(0.0f,1.0f,1.0f,0.3f));
    m_pChapterSelectionButtonFixedTexture = createReflectiveRectShapeTexture(256, 256, Color4f(0.0f,1.0f,1.0f,0.3f), false, true);
    
    m_pChapterSelectionButtonNormalMapTexture = createSuperEllipseTexture(256,256, Color4f(0.8f,1.0f,1.0f,1.0f), false, true);
    
    Color4f c1, c2;
    
    c1.r = 150.0f/255; c1.g = 150.0f/255; c1.b = 150.0f/255; c1.a = 255.0f/255;
    c2.r = 85.0f/255; c2.g = 85.0f/255; c2.b = 85.0f/255; c2.a = 255.0f/255;
    
    m_pDefaultGradientTexture = createGradientTexture(c1, c2);
    
    m_pFilterTexture = createFilterTexture(128, 128, Color4f(1.0f,1.0f,1.0f,1.0f));
    m_pFilterNormalTexture = createFilterTexture(128, 128, Color4f(1.0f,1.0f,1.0f,0.3f), true);
    m_pFilterFixedTexture = createFilterTexture(128, 128, Color4f(1.0f,1.0f,1.0f,0.3f), false, true);
    
    m_pSinkHaloTexture = createHaloTexture(64, 64, Color4f(1.0f,1.0f,1.0f,0.2f), Color4f(1.0f,1.0f,1.0f,0.0f));
    m_pGuiHaloTexture = createHaloTexture(64, 64, Color4f(1.0f,0.5f,0.0f,0.8f), Color4f(1.0f,0.5f,0.0f,0.0f));
    
    m_pReplayButtonTexture = createSinkTexture(15, 22, M_PI_4/2, 64, 64, Color4f(1.0f,1.0f,0.0f,0.8f) );
    m_pSettingsButtonTexture = createSinkTexture(15, 22, 0, 64, 64, Color4f(1.0f,1.0f,0.0f,0.8f) );
}

void Game::fireLevelCompleteTask()
{
    if( !m_bLevelCompleteTaskFired )
    {
        m_bLevelCompleteTaskFired = true;
        TimeElapsedTask *pRoot = new TimeElapsedTask(0.7f);
        LevelCompleteActionTask* pTask = new LevelCompleteActionTask();
        pRoot->Add(pTask);
        Game::getInstance()->getTaskManager()->Attach(pRoot);
    }
}

void Game::unlockNextLevel( int currentChapter, int currentStage, int currentLevel )
{
    int newChapter = currentChapter;
    int newStage = currentStage;
    int newLevel = currentLevel+1;
    
    newLevel = newLevel > NUM_LEVELS_PER_STAGE ? 1 : newLevel;
    
    // if it is a new level of next Stage, Unlock the stage also.
    if( newLevel == 1 )
    {
        newStage++;
        newStage = newStage > NUM_STAGES ? 1 : newStage;
        gameProgressionInfo.chapterProgInfo[newChapter].stageProgInfo[newStage].bLocked = 0;
        gameProgressionInfo.chapterProgInfo[newChapter].numStagesCleared++;
        
        // Set the UI
        GameLevelSelectionState::getInstance()->getGameLevelSelectionHUDController()->setLevelSelectionUiAt(newStage);
    }
    // if it is a new Stage of next Chapter, Unlock the Chapter also.
    if( newStage == 2 )
    {
        newChapter++;
        newChapter = newChapter > NUM_CHAPTERS ? 1 : newChapter;
        gameProgressionInfo.chapterProgInfo[newChapter].bLocked = 0;
    }
    // Unlock Next Level
    gameProgressionInfo.chapterProgInfo[newChapter-1].stageProgInfo[newStage-1].levelProgInfo[newLevel-1].bLocked = 0;
    gameProgressionInfo.chapterProgInfo[newChapter-1].stageProgInfo[newStage-1].numLevelsCleared++;
    
    // Mark Current Level Solved
    gameProgressionInfo.chapterProgInfo[currentChapter-1].stageProgInfo[currentStage-1].levelProgInfo[currentLevel-1].bLocked = 2;
    // Send Notification to GUI to mark this level as solved.
    GameLevelSelectionState::getInstance()->getGameLevelSelectionHUDController()->markAsSolved(currentStage, currentLevel);
    // Send Notification to GUI to unlock next level.
    GameLevelSelectionState::getInstance()->getGameLevelSelectionHUDController()->unlockLevel(newStage, newLevel);
}

void Game::writeGameProgressionInfo()
{
    string fileName("GameProggression");
    
    string path = "/Users/prashantsharma/Downloads/Backup_MacBookPro/Optiks/";
    
//    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
//    NSString *docs_dir = [paths objectAtIndex:0];
//    NSString* aFile = [docs_dir stringByAppendingPathComponent: @"somedocthatdefinitelyexists.doc"];
//    
//    FILE* fp = fopen([aFile fileSystemRepresentation], "r");
    
#ifdef IOS
    FileUtil fu;
    path = fu.getAbsolutePath(fileName).c_str();
#else
    path += fileName;
#endif
    
	FILE* pFile = fopen(path.c_str(), "w");

    if( !pFile )
    {
        //printf("fopen failed, errno = %d\n", errno);
        return;
    }
    
//    for( int i=0; i<NUM_CHAPTERS; i++ )
//    {
//        fprintf(pFile, "\nCHAPTER");
//        fprintf(pFile, "\n%d", gameProgressionInfo.chapterProgInfo[i].bLocked);
//        for( int j=0; j<NUM_STAGES; j++ )
//        {
//            fprintf(pFile, "\nSTAGE::");
//            fprintf(pFile, "\n%d\n", gameProgressionInfo.chapterProgInfo[i].stageProgInfo[j].bLocked);
//            for( int k=0; k<NUM_LEVELS_PER_STAGE; k++ )
//            {
//                fprintf(pFile, "%d ", gameProgressionInfo.chapterProgInfo[i].stageProgInfo[j].levelProgInfo[k].bLocked);
//            }
//        }
//    }
    
    fclose(pFile);
    
}

void Game::readGameProgressionInfo()
{
    string fileName("GameProggression");
    
    string path = "/Users/prashantsharma/Downloads/Backup_MacBookPro/Optiks/";
    
#ifdef IOS
    FileUtil fu;
    path = fu.getAbsolutePath(fileName).c_str();
#else
    path += fileName;
#endif
    
	FILE* pFile = fopen(path.c_str(), "r");
    
    char dummyBuffer[200];
    
    for( int i=0; i<NUM_CHAPTERS; i++ )
    {
        fscanf(pFile, "\n%s", dummyBuffer);
        fscanf(pFile, "\n%d", (int*)&gameProgressionInfo.chapterProgInfo[i].bLocked);
        for( int j=0; j<NUM_STAGES; j++ )
        {
            fscanf(pFile, "\n%s", dummyBuffer);
            int bLocked;
            fscanf(pFile, "\n%d\n", &bLocked);
            gameProgressionInfo.chapterProgInfo[i].stageProgInfo[j].bLocked = bLocked;
            for( int k=0; k<NUM_LEVELS_PER_STAGE; k++ )
            {
                fscanf(pFile, "%d ", &bLocked);
                gameProgressionInfo.chapterProgInfo[i].stageProgInfo[j].levelProgInfo[k].bLocked = bLocked;
            }
        }
    }
    
    fclose(pFile);
}

void Game::initGameProgressionInfo()
{
    //writeGameProgressionInfo();
    //readGameProgressionInfo();

    // Unlock All Levels HACK!!!!
    for( int c=0; c<NUM_CHAPTERS; c++ )
    {
        gameProgressionInfo.chapterProgInfo[c].bLocked = 0;
        
        for( int s=0; s<NUM_STAGES; s++ )
        {
            gameProgressionInfo.chapterProgInfo[c].stageProgInfo[s].bLocked = 0;
            
            for( int l=0; l<NUM_LEVELS_PER_STAGE; l++ )
            {
                gameProgressionInfo.chapterProgInfo[c].stageProgInfo[s].levelProgInfo[l].bLocked = 0;
            }
        }
    }
}