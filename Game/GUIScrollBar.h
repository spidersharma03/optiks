/*
 *  GUIScrollBar.h
 *  testCpp
 *
 *  Created by Prashant on 04/04/11.
 *  Copyright 2011 Sourcebits Technologies. All rights reserved.
 *
 */

#ifndef _GUISCROLL_BAR_H
#define _GUISCROLL_BAR_H

#include "GUIContainer.h"

namespace spider3d
{
	namespace gui 
	{
		
		class GUIPanel;
		class GUIButton;
		class GUIScrollPane;
		
		class GUIScrollBar : public GUIContainer
		{
		public:
			GUIScrollBar(GUIContainer* parent, si32 width, si32 height);
			
			virtual void render();
			
			virtual sbool OnEvent( Event& );
			
			friend class GUIScrollPane;
			
		protected:
			
			GUIPanel *m_scrollPanel;
			GUIButton *m_scrollUpButton;
			GUIButton *m_scrollDownButton;
			
            GUIButton *m_scrollButton;
			
			sbool upButtonPressed;
			sbool downButtonPressed;
		};
	}
}
#endif