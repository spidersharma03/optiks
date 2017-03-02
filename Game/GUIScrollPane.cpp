/*
 *  GUIScrollPane.cpp
 *  testCpp
 *
 *  Created by Prashant on 04/04/11.
 *  Copyright 2011 Sourcebits Technologies. All rights reserved.
 *
 */

#include "GUIScrollPane.h"
#include "GUIScrollBar.h"
#include "GUIPanel.h"
#include "GUIButton.h"
#include "GUIWindow.h"
#include "Matrix4.h"
#include "OglRenderer.h"
#include "Game.h"

using namespace spider3d::render;
using namespace NAMESPACE_DIRAC2D;

namespace spider3d
{
	
	namespace gui 
	{
		
		GUIScrollPane::GUIScrollPane(GUIContainer* parent, si32 width, si32 height):
        GUIContainer(parent, width, height )
		{
			m_HorizontalBar = new GUIScrollBar(this, parent->getSize().x-15, 15);
			m_HorizontalBar->m_scrollUpButton->setPosition(parent->getSize().x-30, 0);
			//m_HorizontalBar->m_scrollButton->setPosition(width-50, 0);
			
			m_VerticalBar   = new GUIScrollBar(this, 15, parent->getSize().y);
			m_VerticalBar->setPosition(parent->getSize().x-15, 0);
			//m_VerticalBar->m_scrollButton->setPosition(width-15, 50);
            
            createGeometry();
		}
		
		
		void GUIScrollPane::render()
		{
            if( !m_bVisible )
                return;
            
            NAMESPACE_DIRAC2D::Matrix4f xForm;
            
            xForm.translate(m_ClippedBounds.x, m_ClippedBounds.y);
            
            Game::getInstance()->getRenderer()->setTransform(xForm, spider3d::WORLD);
            
            Game::getInstance()->getRenderer()->setTextureObject(0);
            
            Game::getInstance()->getRenderer()->drawVertexPrimitiveList(4, m_vertices, 2, m_Colors, 4, m_TexCoords, 1, 0, EPT_TRIANGLE_FAN);
            
			GUIContainer::render();
		}
			
		sbool GUIScrollPane::OnEvent( Event& event)
		{
            OnAction(event);

            sbool bRes = false;

            if( event.eventType == SMOUSE_EVENT )
            {
                if( m_VerticalBar->downButtonPressed )
                {
                    scrollComponents(0, -3);
                }
                if( m_VerticalBar->upButtonPressed )
                {
                    scrollComponents(0, 3);
                }
                if( m_HorizontalBar->downButtonPressed )
                {
                    scrollComponents(3, 0);
                }
                if( m_HorizontalBar->upButtonPressed )
                {
                    scrollComponents(-3, 0);
                }
                sbool bRes = false;
                
                if( m_Parent )
                    bRes = m_Parent->OnEvent(event);
                
                if( event.mouseInput.Event == spider3d::LEFT_MOUSE_PRESSED_DOWN )
                    setActiveComponent(this);
                if( event.mouseInput.Event == spider3d::LEFT_MOUSE_LEFT_UP )
                    setActiveComponent(0);
            }
            return bRes;
		}
		
		
		void GUIScrollPane::scrollComponents(si32 dx, si32 dy)
		{
			vector<GUIComponent*>& vecComponents = getComponents();
			for (unsigned c=0; c<vecComponents.size(); c++)
			{
				GUIComponent *component = vecComponents[c];
                if( component == m_HorizontalBar || component == m_VerticalBar )
                    continue;
				si32 x = component->getLocalTransform().col3.x;
				si32 y = component->getLocalTransform().col3.y;
				component->setPosition(x+dx, y+dy);
			}
		}
        
        void GUIScrollPane::createGeometry()
		{
			m_vertices = new sf32[4*2];
            m_TexCoords = new sf32[4*2];
			m_Colors   = new float[4*4];
			m_vertices[0] = 0;
			m_vertices[1] = 0;
			m_vertices[2] = m_Size.x;
			m_vertices[3] = 0;
			m_vertices[4] = m_Size.x;
			m_vertices[5] = m_Size.y;
			m_vertices[6] = 0.0f;
			m_vertices[7] = m_Size.y;
            
            m_TexCoords[0] = 0.0f;
            m_TexCoords[1] = 0.0f;
			m_TexCoords[2] = 1.0f;
            m_TexCoords[3] = 1.0f;
            m_TexCoords[4] = 1.0f;
            m_TexCoords[5] = 1.0f;
            m_TexCoords[6] = 0.0f;
            m_TexCoords[7] = 1.0f;
            
			for( int i=0; i<16; i+=4)
			{
				m_Colors[i]   = 1.0f;
                m_Colors[i+1] = 1.0f;
                m_Colors[i+2] = 1.0f;
                m_Colors[i+3] = 1.0f;
			}
		}
	}
}
