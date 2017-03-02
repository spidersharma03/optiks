//
//  HUD_Controler.h
//  Optiks
//
//  Created by Prashant Sharma on 29/10/13.
//
//

#ifndef __Optiks__HUD_Controler__
#define __Optiks__HUD_Controler__

#include "IActionListener.h"

using namespace spider3d::gui;

class HUDController : IActionListener
{
public:
    HUDController();
    
    // IActionListener Interface
    
    virtual void mousePressed(int x, int y);
    
    virtual void mouseReleased(int x, int y);
    
    virtual void mouseMoved(int x, int y);
    
    virtual void OnClicked( GUIComponent* );
    
private:
    
};

#endif /* defined(__Optiks__HUD_Controler__) */
