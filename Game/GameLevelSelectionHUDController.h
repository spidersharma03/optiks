//
//  GameLevelSelectionHUDController.h
//  Optiks
//
//  Created by Prashant Sharma on 05/12/13.
//
//

#ifndef __Optiks__GameLevelSelectionHUDController__
#define __Optiks__GameLevelSelectionHUDController__

#include <iostream>
#include "GUI.h"
#include "GameSettings.h"
#include "Ray.h"

using namespace spider3d::gui;


class GUILevelSelectionButton;
class GUIStageSelectionButton;
class SceneTransitionTask;
class ParticleSystem;
class ParticleAnimator;

class GameLevelSelectionHUDController : public IActionListener
{
public:
    GameLevelSelectionHUDController();
    
    ~GameLevelSelectionHUDController();
    
    // IActionListener
    virtual void        mousePressed(GUIComponent* pComponent, int x, int y);
    
    virtual void        mouseReleased(GUIComponent* pComponent, int x, int y);
    
    virtual void        mouseMoved(GUIComponent* pComponent, int x, int y);
    
    virtual void        keyPressed(GUIComponent* pComponent, const Event& event);
    
    void                showGameLevelSelectionView();
    void                hideGameLevelSelectionView();
    int                 getSelectedLevel() const;
    int                 getSelectedStage() const;
    void                markAsSolved(int stage, int level);
    
    void                setLevelTo(int level);
    void                setStageTo(int stage);
    
    void                renderUI();
    void                renderBackground();
    void                renderGlowingRays();
    void                drawBackgroundPass();
    void                reflectionDrawPass();
    void                renderAllRays(float width, const Color4f& addColor = Color4f(0.0f,0.0f,0.0f,0.0f));

    void                unlockStage( int stage );
    void                unlockLevel( int stage, int level );
    
    vector<Ray>&        getAllRays()
    {
        collectRaysFromAllUIButtons();
        return m_vecRays;
    }
    
    void                setLevelSelectionUiAt( int stage );
    
private:
    
    void                collectRaysFromAllUIButtons();
    
    GUIPanel*           createGameLevelSelectionView(int index);
    
    void                unlockStageAndLevelButtonsFromGameProgression();
    
private:
    GUIButton           *m_pMainMenuSelectionButton;
    GUIButton           *m_pBackToStageSelectionButton;
    
    GUIButton           *m_pNextLevelSelectionButton;
    GUIButton           *m_pPreviousLevelSelectionButton;
    
    GUIScrollView      *m_pLevelSelectionScrollView;
    GUIScrollView      *m_pStageSelectionScrollView;
    
    GUIButton          *m_pStageSelectionButtons[NUM_STAGES];
    GUIButton          *m_pLevelSelectionButtons[NUM_STAGES][NUM_LEVELS_PER_STAGE];
    
    GUIPanel           *m_pLevelSelectionViewPanels[NUM_STAGES];
        
    int                 m_SelectedLevel;
    int                 m_SelectedStage;
    
    vector<GUILevelSelectionButton*>  m_vecLevelSelectionButtons;
    vector<Ray>         m_vecRays;
    Ray                 m_vecSourceRays[NUM_LEVELS_PER_STAGE];
    SceneTransitionTask * m_pSceneTransitionTask;
    
    Vector2i            m_MousePressedPosition;
    int                 m_DistanceFromMousePressedPosition;
};

#endif /* defined(__Optiks__GameLevelSelectionHUDController__) */
