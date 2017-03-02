//
//  GUIGamePauseHUDController.h
//  Optiks
//
//  Created by Prashant on 12/03/14.
//
//

#ifndef __Optiks__GUIGamePauseHUDController__
#define __Optiks__GUIGamePauseHUDController__

#include <iostream>
#include "GUI.h"
#include "IGlowing.h"

using namespace spider3d::gui;

class GUIContinueButton;
class GUIMainMenuButton;

class GamePauseHUDController : public IActionListener, public IGlowing
{
public:
    GamePauseHUDController();
    
    ~GamePauseHUDController();
    
    // IActionListener
    virtual void         mousePressed(GUIComponent* pComponent, int x, int y);
    
    virtual void         mouseReleased(GUIComponent* pComponent, int x, int y);
    
    virtual void         mouseMoved(GUIComponent* pComponent, int x, int y);
    
    virtual void         keyPressed(GUIComponent* pComponent, const Event& event);
    
    void                 showGameLevelPauseView();
    void                 hideGameLevelPauseView();
    
    void                 setTransparency(float transparency);
    float                getTransparency() const;
    
    virtual vector<Ray>& getGlowingRays();

private:
    
    void                createGameLevelCompleteView();
    
private:
    GUIButton           *m_pMainMenuSelectionButton;
    GUIMainMenuButton   *m_pButtonGoToMainMenu;
    GUIContinueButton   *m_pButtonNextLevel;
    GUIPanel            *m_pGameCompleteView;
    vector<GUIButton*>  m_vecButtons;
    vector<Ray>         m_vecGlowingRays;
    float               m_Transparency;
    
};

#endif /* defined(__Optiks__GUIGamePauseHUDController__) */
