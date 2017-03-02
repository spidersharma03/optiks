//
//  GameMainMenuHUDController.h
//  Optiks
//
//  Created by Prashant Sharma on 04/12/13.
//
//

#ifndef __Optiks__GameMainMenuHUDController__
#define __Optiks__GameMainMenuHUDController__

#include <iostream>
#include "GUI.h"

using namespace spider3d::gui;

class GamePlayState;

class GameMainMenuHUDController : public IActionListener
{
public:
    GameMainMenuHUDController();
    
    ~GameMainMenuHUDController();
    
    // IActionListener
    virtual void        mousePressed(GUIComponent* pComponent, int x, int y);
    
    virtual void        mouseReleased(GUIComponent* pComponent, int x, int y);
    
    virtual void        mouseMoved(GUIComponent* pComponent, int x, int y);
    
    virtual void        keyPressed(GUIComponent* pComponent, const Event& event);
    
    void                hideAllViews();
    void                showAllViews();

private:
    
    void                createMainMenuView();

private:
    GUIPanel            *m_pMainPanelView;
    
    vector<GUIButton*>  m_vecButtons;
    
};

#endif /* defined(__Optiks__GameMainMenuHUDController__) */
