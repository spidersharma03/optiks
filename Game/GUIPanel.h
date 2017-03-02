/*
 *  GUIPanel.h
 *  testCpp
 *
 *  Created by Prashant on 30/03/11.
 *  Copyright 2011 Sourcebits Technologies. All rights reserved.
 *
 */

#ifndef _GUI_PANEL_H
#define _GUI_PANEL_H

#include "GUIContainer.h"


namespace spider3d
{
	namespace gui 
	{
		class GUIPanel: public GUIContainer
		{
		public:
			GUIPanel(si32 width, si32 height);
			
			GUIPanel(GUIContainer* parent, si32 width, si32 height);
			
            virtual void update();

			virtual void render();
			
			virtual sbool OnEvent( Event& );

		protected:
			virtual void createGeometry();
            
            static Texture* m_pTexture;
		};
	}
}

#endif