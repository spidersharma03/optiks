//
//  Game.h
//  Optiks
//
//  Created by Prashant Sharma on 29/10/13.
//
//

#ifndef __Optiks__Game__
#define __Optiks__Game__

#include "StateMachine.h"
#include "EventListener.h"
#include "GameSettings.h"

using namespace spider3d;

class Scene;
class TaskManager;

namespace spider3d
{
    namespace render
    {
        class Renderer;
    }
    namespace gui
    {
        class GUIManager;
    }
}

class Texture;

using namespace spider3d::gui;
using namespace spider3d::render;

class Game : public EventListener
{
public:
    void update( float dt );
    
    void            changeState(IState<Game>* pState);
    
    virtual sbool   OnEvent( Event& );

    void            setGuiManager(GUIManager* pGuimanager);

    void            setScene(Scene* pScene)
                    {
                        m_pScene = pScene;
                    }
    
    void            setRenderer( Renderer* pRenderer)
                    {
                        m_pRenderer = pRenderer;
                    }
    
    Renderer*       getRenderer() const
                    {
                        return m_pRenderer;
                    }
    
    Scene*          getScene() const
                    {   return m_pScene;
                    }
    
    GUIManager*     getGUIManager() const
                    {
                        return m_pGUIManager;
                    }
    
    TaskManager*    getTaskManager() const
                    {
                        return m_pTaskManagerAboveGUI;
                    }
    
    TaskManager*    getTaskManagerBelowGUI() const
                    {
                        return m_pTaskManagerBelowGUI;
                    }
    
    static Game*    getInstance()
                    {
                        if( !s_pGame )
                        {
                            s_pGame = new Game();
                        }
                        return s_pGame;
                    }
    
    void            setLevelCompleteTaskFired(bool bLevelCompleteTaskFired)
                    {
                        m_bLevelCompleteTaskFired = bLevelCompleteTaskFired;
                    }
    
    void            initTextures();
    
    void            fireLevelCompleteTask();
    
    void            unlockNextLevel( int currentChapter, int currentStage, int currentLevel );
    
    void            initGameProgressionInfo();

    void            writeGameProgressionInfo();

    void            readGameProgressionInfo();

private:
    
    Game();
    
    static Game        *s_pGame;
    StateMachine<Game> *m_pStateMachine;
    Scene              *m_pScene;
    GUIManager         *m_pGUIManager;
    TaskManager        *m_pTaskManagerAboveGUI;
    TaskManager        *m_pTaskManagerBelowGUI;
    
    Renderer           *m_pRenderer;
    
    bool               m_bLevelCompleteTaskFired;
public:
    
    Texture*           m_pFilterTexture;
    Texture*           m_pFilterNormalTexture;
    Texture*           m_pFilterFixedTexture;

    Texture*           m_pReflectiveRectShapeTexture;
    Texture*           m_pReflectiveRectShapeNormalMapTexture;
    Texture*           m_pReflectiveRectShapeFixedTexture;
    
    Texture*           m_pTransmissiveRectShapeTexture;
    Texture*           m_pTransmissiveRectShapeNormalMapTexture;
    Texture*           m_pTransmissiveRectShapeFixedTexture;
    
    Texture*           m_pRefractiveRectShapeTexture;
    Texture*           m_pRefractiveRectShapeNormalMapTexture;

    Texture*           m_pChapterSelectionButtonTexture;
    Texture*           m_pChapterSelectionButtonFixedTexture;
    Texture*           m_pChapterSelectionButtonNormalMapTexture;
    
    Texture*           m_pAbsorptiveRectShapeTexture;
    Texture*           m_pAbsorptiveRectShapeFixedTexture;
    Texture*           m_pAbsorptiveRectShapeNormalMapTexture;

    Texture*           m_pPortalRectShapeTexture;
    Texture*           m_pPortalRectShapeNormalMapTexture;
    
    Texture*           m_pPlacementMarkerTexture;

    Texture*           m_pSinkTexture;
    
    Texture*           m_pSourceTexture;
    
    Texture*           m_pDefaultGradientTexture;
    
    Texture*           m_pSinkHaloTexture;
    Texture*           m_pGuiHaloTexture;
    Texture*           m_pReplayButtonTexture;
    Texture*           m_pSettingsButtonTexture;
    
    GameProgressionInfo gameProgressionInfo;

};

#endif /* defined(__Optiks__Game__) */
