/*
 *  LayoutManager.h
 *  testCpp
 *
 *  Created by Prashant on 06/04/11.
 *  Copyright 2011 Sourcebits Technologies. All rights reserved.
 *
 */

#ifndef _LAYOUT_MANAGER_H
#define _LAYOUT_MANAGER_H
#include <vector>

using namespace std;

namespace spider3d
{
	namespace gui 
	{
		class GUIComponent;
		class GUIContainer;
		
		class LayOutManager
		{
		public:
			virtual void layoutContainer() = 0;
			
			virtual void addLayoutComponent(GUIComponent*, si32) = 0;
			
			friend class GUIContainer;
			
		protected:
			vector<GUIComponent*> m_vecLayoutComponents;
			GUIContainer *m_TargetContainer;
		};
		
	}
	
}

#endif