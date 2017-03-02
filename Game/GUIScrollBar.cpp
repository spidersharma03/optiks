/*
 *  GUIScrollBar.cpp
 *  testCpp
 *
 *  Created by Prashant on 04/04/11.
 *  Copyright 2011 Sourcebits Technologies. All rights reserved.
 *
 */

#include "GUIScrollBar.h"
#include "GUIButton.h"
#include "GUIPanel.h"

namespace spider3d
{
	namespace gui 
	{
		
		
		GUIScrollBar::GUIScrollBar(GUIContainer* parent, si32 width, si32 height):GUIContainer(parent, width, height)
		{
			m_scrollPanel = new GUIPanel(this, width, height);
			m_scrollUpButton = new GUIButton(m_scrollPanel, 15, 15);
			m_scrollDownButton = new GUIButton(m_scrollPanel, 15, 15);
			m_scrollUpButton->setPosition(0, height-15);
			
			//m_scrollButton = new GUIButton(m_scrollPanel, 15, 15);
			
			upButtonPressed = false;
			downButtonPressed = false;
		}
		
		void GUIScrollBar::render()
		{
			GUIContainer::render();
		}
			
		sbool GUIScrollBar::OnEvent( Event& event)
		{      
            if( !m_bEnable )return false;

            OnAction(event);

            sbool bRes = false;

            if( event.eventType == SMOUSE_EVENT )
            {
                if( m_scrollUpButton->isPressed() )
                {
                    upButtonPressed = true;
                }
                else 
                {
                    upButtonPressed = false;
                }

                if( m_scrollDownButton->isPressed() )
                {
                    downButtonPressed = true;
                }
                else 
                {
                    downButtonPressed = false;
                }
                if( m_Parent )
                    bRes = m_Parent->OnEvent(event);
                
                if( event.mouseInput.Event == spider3d::LEFT_MOUSE_PRESSED_DOWN )
                    setActiveComponent(this);
                if( event.mouseInput.Event == spider3d::LEFT_MOUSE_LEFT_UP )
                    setActiveComponent(0);
            }
            return bRes;
		}
		
		
	}
}
