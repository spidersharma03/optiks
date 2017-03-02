/*
 *  GUIContainer.cpp
 *  testCpp
 *
 *  Created by Prashant on 28/03/11.
 *  Copyright 2011 Sourcebits Technologies. All rights reserved.
 *
 */

#include "GUIContainer.h"
#include "BorderLayout.h"

namespace spider3d
{
	namespace gui 
	{
		GUIContainer::GUIContainer():GUIComponent(0, 0)
		{
			m_layOutManager = new BorderLayout();
		}

		GUIContainer::GUIContainer(si32 width, si32 height):GUIComponent(width, height)
		{
			m_layOutManager = new BorderLayout();
		}
		
		GUIContainer::GUIContainer(GUIContainer* parent, si32 width, si32 height):GUIComponent(parent,width, height)
		{
			if( parent )
			parent->addComponent(this);
			
			m_layOutManager = new BorderLayout();
			m_layOutManager->m_TargetContainer = this;
		}
		
		void GUIContainer::addComponent(GUIComponent* component)
		{
			m_vecComponents.push_back(component);
			component->m_Parent = this;
			m_layOutManager->addLayoutComponent(component,0);
			doLayout();
		}
		
		void GUIContainer::addComponent(GUIComponent* component, si32 constraints)
		{
			m_vecComponents.push_back(component);
			component->m_Parent = this;
			m_layOutManager->addLayoutComponent(component,constraints);
			doLayout();
		}
		
		
		GUIComponent* GUIContainer::getComponentAt(si32 index)
		{
			GUIComponent* component = 0;
			if( index >= m_vecComponents.size()-1 )
				component = 0;
			else 
				component = m_vecComponents[index];
			
			return component;
		}
		
		vector<GUIComponent*>& GUIContainer::getComponents()
		{
			return m_vecComponents;
		}
		
		GUIComponent* GUIContainer::remove(si32 index)
		{
			return 0;
		}
		
		GUIComponent* GUIContainer::remove(GUIComponent* component)
		{
			return 0;
		}
		
		void GUIContainer::setLayout(LayOutManager* layOutManager)
		{
			m_layOutManager = layOutManager;
			if( m_layOutManager )
				m_layOutManager->m_TargetContainer = this;
		}
		
		void GUIContainer::doLayout()
		{
			if( m_layOutManager )
			{
				m_layOutManager->layoutContainer();
			}
		}
		
		void GUIContainer::update()
		{
			if(m_Parent)
			{
				Matrix3f& parentTransform = m_Parent->getTransform();
				m_GlobalTransform.multiply(parentTransform, m_LocalTransform);
				m_Bounds.x = m_GlobalTransform.col3.x;
				m_Bounds.y = m_GlobalTransform.col3.y;
				
				// Check for Clipping against its parent
				bool bClipped = false;
				m_ClippedBounds = m_Bounds;
				Recti& parentRect = m_Parent->getRectangle();
				bClipped = intersect(parentRect, m_Bounds, m_ClippedBounds);
                m_bIsClipped = bClipped;
				//if( !bClipped )
				//	m_ClippedBounds.set(0,0,0,0);
			}
			
			for( unsigned i=0; i<m_vecComponents.size(); i++ )
			{
				m_vecComponents[i]->update();
			}
		}
		
		void GUIContainer::render()
		{
            if( !m_bVisible )
                return;
            
			for( unsigned c=0; c<m_vecComponents.size(); c++ )
			{
				if(m_vecComponents[c]  && m_vecComponents[c]->isVisible())
					m_vecComponents[c]->render();
			}
		}
	}
}