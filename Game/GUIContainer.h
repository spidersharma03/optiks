/*
 *  GUIContainer.h
 *  testCpp
 *
 *  Created by Prashant on 28/03/11.
 *  Copyright 2011 Sourcebits Technologies. All rights reserved.
 *
 */

#ifndef GUI_CONTAINER_H
#define GUI_CONTAINER_H

#include "GUIComponent.h"
#include "LayoutManager.h"
#include <vector>

using namespace std;

namespace spider3d
{
	namespace gui 
	{
		
		class GUIContainer : public GUIComponent 
		{
		public:
			GUIContainer();

			GUIContainer(si32 width, si32 height);
						
			GUIContainer(GUIContainer* parent, si32 width, si32 height);
			
			virtual void addComponent(GUIComponent*);
			
			void addComponent(GUIComponent* component, si32 constraints);

			GUIComponent* getComponentAt(si32);
			
			vector<GUIComponent*>& getComponents();
			
			GUIComponent* remove(si32);
			
			GUIComponent* remove(GUIComponent*);
			
			void setLayout(LayOutManager*);
			
			void doLayout();
			
			virtual void update();

			virtual void render() = 0;
			
			virtual GUIComponent* getComponentFromPoint(si32 x, si32 y)
			{
				GUIComponent* component = 0;
				// query components in reverse order. this makes sure that the components which are on top will process the events first. 
                 
				for (int i=m_vecComponents.size()-1; i>=0; i--)
				{
                    if( !m_vecComponents[i]->isVisible() || !m_vecComponents[i]->isEnable() )
                        continue;
                    
					component = m_vecComponents[i]->getComponentFromPoint(x,y);
					if( component )
						return component;
				}
				
				return isPointInside(x, y) ? this : 0;
			}
			
		protected:
			LayOutManager* m_layOutManager;
			vector<GUIComponent*> m_vecComponents;
		};
	}
}
#endif