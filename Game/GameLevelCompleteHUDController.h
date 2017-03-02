//
//  GameLevelCompleteHUDController.h
//  Optiks
//
//  Created by Prashant on 24/02/14.
//
//
#ifndef __Optiks__GameLevelCompleteHUDController__
#define __Optiks__GameLevelCompleteHUDController__


#include <iostream>
#include "GUI.h"

using namespace spider3d::gui;


class GameLevelCompleteHUDController : public IActionListener
{
public:
    GameLevelCompleteHUDController();
    
    ~GameLevelCompleteHUDController();
    
    // IActionListener
    virtual void        mousePressed(GUIComponent* pComponent, int x, int y);
    
    virtual void        mouseReleased(GUIComponent* pComponent, int x, int y);
    
    virtual void        mouseMoved(GUIComponent* pComponent, int x, int y);
    
    virtual void        keyPressed(GUIComponent* pComponent, const Event& event);
    
    void                showGameLevelCompleteView();
    void                hideGameLevelCompleteView();
    
    void                setTransparency(float transparency);
    float               getTransparency() const;
    
private:
    
    void                createGameLevelCompleteView();
    
private:
    GUIButton           *m_pMainMenuSelectionButton;
    
    GUIPanel            *m_pGameCompleteView;
    vector<GUIButton*>  m_vecButtons;
    float               m_Transparency;
    
};


#endif /* defined(__Optiks__GameLevelCompleteHUDController__) */
