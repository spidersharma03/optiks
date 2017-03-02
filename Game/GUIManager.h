/*
 *  GUIManager.h
 *  testCpp
 *
 *  Created by Prashant on 28/03/11.
 *  Copyright 2011 Sourcebits Technologies. All rights reserved.
 *
 */

#ifndef _GUI_MANAGER_H
#define _GUI_MANAGER_H

#include "GUIContainer.h"

class ShaderProgram;

namespace spider3d
{
	namespace gui 
	{
	
		class GUIManager: public GUIContainer
		{
		public:
			
			GUIManager(int viewPortWidth, int viewPortHeight);
			
			void drawGUI();
			
            virtual void setActiveComponent(GUIComponent* pComponent)
            {
                m_pActiveComponent = pComponent;
            }
            
			virtual void render();

			sbool processEvents( Event& event);

			sbool OnEvent(Event& event)
			{
				return true;
			}
			
            int getViewportWidth()
            {
                return m_ViewPortWidth;
            }
            
            int getViewportHeight()
            {
                return m_ViewPortHeight;
            }
            
            void findFocusComponent( Event& event);

            ShaderProgram*  m_pDefaultShaderProgram;

		protected:
			
			void processEvents();
            
            GUIComponent* m_pActiveComponent;
            // This component is responsible for recieving the key events.
            GUIComponent* m_pCurrFocusComponent;
            GUIComponent* m_pPrevFocusComponent;
            
            int m_ViewPortWidth;
            int m_ViewPortHeight;
            
		};
		
	}
}
#endif