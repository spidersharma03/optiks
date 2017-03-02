//
//  PlayHUDController.h
//  Optiks
//
//  Created by Prashant Sharma on 20/11/13.
//
//

#ifndef __Optiks__PlayHUDController__
#define __Optiks__PlayHUDController__

#include <iostream>
#include "GUI.h"
#include "IGlowing.h"

using namespace spider3d::gui;

class GamePlayState;

class GamePlayHUDController : public IActionListener, public IGlowing
{
public:
    GamePlayHUDController();
    
    ~GamePlayHUDController();
    
    // IActionListener
    virtual void        mousePressed(GUIComponent* pComponent, int x, int y);
    
    virtual void        mouseReleased(GUIComponent* pComponent, int x, int y);
    
    virtual void        mouseMoved(GUIComponent* pComponent, int x, int y);
    
    virtual void        keyPressed(GUIComponent* pComponent, const Event& event);

    void                hideAllViews();
    void                showAllViews();
    
    GUILabel*           getLevelCompleteLabel()
                        {
                            return m_pLevelCompleteLabel;
                        }
    
    void                goToPrevLevel();
    void                goToNextLevel();
    
    string              getNextLevelName() const;
    string              getCurrentLevelName() const;

    vector<Ray>&        getGlowingRays();

private:
    
    void                createGamePlayView();

    void                createLevelCompletePanel();

private:
    GUIPanel            *m_pStageSelectionView;
    GUIButton           *m_pMainMenuSelectionButton;
    GUIButton           *m_pBackToLevelSelectionButton;
    GUIButton           *m_pHintButton;
    
    GUIButton           *m_pNextLevelSelectionButton;
    GUIButton           *m_pPreviousLevelSelectionButton;
    
    GUIPanel            *m_pLevelSelectionView;
    GUILabel            *m_pLevelCompleteLabel;

    vector<GUIButton*>  m_vecLevelSelectButtons;
    vector<GUIButton*>  m_vecStageSelectButtons;
    
    vector<Ray>         m_vecGlowingRays;
};


#endif /* defined(__Optiks__PlayHUDController__) */
