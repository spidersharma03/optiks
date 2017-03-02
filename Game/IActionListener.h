//
//  ActionListener.h
//  Optiks
//
//  Created by Prashant Sharma on 02/04/13.
//

#ifndef _ACTION_LISTENER_H_
#define _ACTION_LISTENER_H_

#include "EventListener.h"

namespace spider3d
{
	namespace gui
	{
        class GUIComponent;
        
        class IActionListener
        {
            public:
                virtual void mousePressed(GUIComponent* pComponent, int x, int y) = 0;
            
                virtual void mouseReleased(GUIComponent* pComponent, int x, int y) = 0;

                virtual void mouseMoved(GUIComponent* pComponent, int x, int y) = 0;
            
                virtual void keyPressed(GUIComponent* pComponent, const Event& event) = 0;
        };
    }
}

#endif
