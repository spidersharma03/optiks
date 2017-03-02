//
//  GameStageSelectionHUDController.h
//  Optiks
//
//  Created by Prashant Sharma on 05/12/13.
//
//

#ifndef __Optiks__GameStageSelectionHUDController__
#define __Optiks__GameStageSelectionHUDController__

#include <iostream>
#include "GUI.h"
#include "GameSettings.h"

using namespace spider3d::gui;

class GameMenuBackgroundDecorObject;
class ParticleSystem;
class ParticleEmitter;
class ParticleAnimator;

class GameChapterSelectionHUDController : public IActionListener
{
public:
    GameChapterSelectionHUDController();
    
    ~GameChapterSelectionHUDController();
    
    // IActionListener
    virtual void        mousePressed(GUIComponent* pComponent, int x, int y);
    
    virtual void        mouseReleased(GUIComponent* pComponent, int x, int y);
    
    virtual void        mouseMoved(GUIComponent* pComponent, int x, int y);
    
    virtual void        keyPressed(GUIComponent* pComponent, const Event& event);
    
    void                showGameChapterSelectionView();
    void                hideGameChapterSelectionView();
    int                 getSelectedChapter() const;
    void                renderUI();
    void                renderAllRays(float width );
    void                setChapterTo(int chapter);
    
    void                unLockChapter(int chapter);
    
private:

    void                renderGlowingRays();
    void                reflectionDrawPass();
    void                drawBackgroundPass();
    void                createGameChapterSelectionView();
    
private:
    GUIButton           *m_pMainMenuSelectionButton;
    GUIButton           *m_pSettingsButton;
    
    GUIScrollView       *m_pChapterSelectionView;
    vector<GUIButton*>  m_vecChapterSelectButtons;
    
    int                 m_SelectedChapter;
    
    ParticleSystem      *m_pParticleSystemDiffuse;
    ParticleAnimator    *m_pParticleAnimatorDiffuse;
    
    Vector2i            m_MousePressedPosition;
    int                 m_DistanceFromMousePressedPosition;
};

#endif /* defined(__Optiks__GameStageSelectionHUDController__) */
