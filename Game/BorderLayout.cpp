/*
 *  BorderLayout.cpp
 *  testCpp
 *
 *  Created by Prashant on 06/04/11.
 *  Copyright 2011 Sourcebits Technologies. All rights reserved.
 *
 */

#include "BorderLayout.h"
#include "GUIComponent.h"


namespace spider3d
{
	namespace gui
	{
		
		BorderLayout::BorderLayout()
		{
			m_hgap = m_vgap = 0;
			m_vecLayoutComponents.resize(5);
		}
		
		BorderLayout::BorderLayout(si32 hgap, si32 vgap)
		{
			m_hgap = m_vgap = 0;
		}
		
		void BorderLayout::addLayoutComponent(GUIComponent* component, si32 constraints)
		{
			//m_vecLayoutComponents.push_back(component);
			m_vecLayoutComponents[constraints] = component;
		}
		
		void BorderLayout::layoutContainer()
		{
			// Find location of South Component
			GUIComponent *southComponent = m_vecLayoutComponents[SOUTH];
			if( southComponent )
			{
				southComponent->setPosition(0, 0);
			}
		}
		
	}
}