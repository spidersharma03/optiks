/*
 *  GUIWindow.h
 *  testCpp
 *
 *  Created by Prashant on 28/03/11.
 *  Copyright 2011 Sourcebits Technologies. All rights reserved.
 *
 */

#ifndef GUI_WINDOW_H
#define GUI_WINDOW_H

#include "GUIContainer.h"

namespace spider3d
{
	namespace gui 
	{
		class GUIPanel;
		class GUIButton;
		
		class GUIWindow : public GUIContainer
		{
		public:
			
			static const int  TITLE_BORDER     = 0X1;
			static const int  STATUS_BAR       = 0X2;
			static const int  CLOSE_BUTTON     = 0X4;
			static const int  MINMIZE_BUTTON   = 0x8;
			static const int  MAXIMIZE_BUTTON  = 0x10;
			
			
			GUIWindow(GUIContainer* parent, si32 width, si32 height);
			
			GUIWindow(GUIContainer* parent, si32 width, si32 height, si32 flags);

			virtual void render();
			
			virtual sbool OnEvent( Event& );

		protected:

			virtual void createGeometry();
			
			void createWindow();
			
			GUIPanel*  m_TitleBorderPanel;
			GUIPanel*  m_TitleBorderOptionButtonsPanel;
			GUIButton* m_MinimizeButton;
			GUIButton* m_MaximizeButton;
			GUIButton* m_CloseButton;
			
			GUIPanel* m_StatusBarPanel;
			si32 m_Flags;
			
		};
		
	}
}

#endif
