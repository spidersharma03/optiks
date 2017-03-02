/*
 *  GUIComponentAnimator.h
 *  testCpp
 *
 *  Created by Prashant on 29/03/11.
 *  Copyright 2011 Sourcebits Technologies. All rights reserved.
 *
 */

#ifndef _GUI_COMPONENT_ANIMATOR_H
#define _GUI_COMPONENT_ANIMATOR_H

#include "spiderDataTypes.h"

namespace spider3d
{
	namespace gui 
	{
		class GUIComponent;
		
		class GUIComponentAnimator
		{
		public:
			GUIComponentAnimator(GUIComponent* target):m_Target(target)
			{
			}
			
			virtual void animate(sf32 currentTime) = 0;
			
		private:
			GUIComponent* m_Target;
		};
	}
}
#endif