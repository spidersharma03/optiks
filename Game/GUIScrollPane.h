/*
 *  GUIScrollPane.h
 *  testCpp
 *
 *  Created by Prashant on 04/04/11.
 *  Copyright 2011 Sourcebits Technologies. All rights reserved.
 *
 */

#ifndef _GUI_SCROLLPANE_H
#define _GUI_SCROLLPANE_H

#include "GUIContainer.h"

namespace spider3d
{
	
	namespace gui 
	{
		class GUIScrollBar;
		class GUIPanel;
		
		class GUIScrollPane : public GUIContainer
		{
		public:
			
			GUIScrollPane(GUIContainer* parent, si32 width, si32 height);
			
			virtual void render();
			
			virtual sbool OnEvent( Event& );
			
            virtual void createGeometry();

		protected:
			
			void scrollComponents(si32 dx, si32 dy);
			
			GUIScrollBar *m_HorizontalBar;
			GUIScrollBar *m_VerticalBar;
		};
		
	}
}
#endif