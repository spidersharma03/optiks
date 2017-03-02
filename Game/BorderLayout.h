/*
 *  BorderLayout.h
 *  testCpp
 *
 *  Created by Prashant on 06/04/11.
 *  Copyright 2011 Sourcebits Technologies. All rights reserved.
 *
 */

#ifndef _BORDER_LAYOUT_H
#define _BORDER_LAYOUT_H

#include "spiderDataTypes.h"
#include "LayoutManager.h"

using namespace spider3d;

namespace spider3d
{
	namespace gui
	{
		class BorderLayout : public LayOutManager
		{
		public:
			
			static const si32 CENTRE = 0;
			static const si32 NORTH  = 1;
			static const si32 SOUTH  = 2;
			static const si32 WEST   = 3;
			static const si32 EAST   = 4;

			BorderLayout();
			
			BorderLayout(si32 hgap, si32 vgap);
			
			virtual void layoutContainer();
			
			virtual void addLayoutComponent(GUIComponent* component, si32 constraints);

		protected:
			
			si32 m_hgap, m_vgap;
		};
	}
}
#endif