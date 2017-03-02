/*
 *  GUIWindow.cpp
 *  testCpp
 *
 *  Created by Prashant on 28/03/11.
 *  Copyright 2011 Sourcebits Technologies. All rights reserved.
 *
 */

#include "GUIWindow.h"
#include "GUIPanel.h"
#include "GUIButton.h"
#include "OglRenderer.h"
#include "Game.h"

using namespace spider3d::render;

namespace spider3d
{
	namespace gui 
	{
		GUIWindow::GUIWindow(GUIContainer* parent, si32 width, si32 height):GUIContainer(parent, width,height)
		{
			createWindow();
			createGeometry();
			m_bFocused = true;
            m_Name = "WINDOW";
		}
		
		GUIWindow::GUIWindow(GUIContainer* parent, si32 width, si32 height, si32 flags):GUIContainer(parent, width,height),m_Flags(flags)
		{
			createWindow();
			createGeometry();
			m_bFocused = true;
            m_Name = "WINDOW";
		}
		
		void GUIWindow::render()
		{

            Matrix4f xForm;
            
            xForm.translate(m_GlobalTransform.col3.x, m_GlobalTransform.col3.y);
            
            Game::getInstance()->getRenderer()->setTextureObject(0);
            
            Game::getInstance()->getRenderer()->setTransform(xForm, spider3d::WORLD);
            
            Game::getInstance()->getRenderer()->drawVertexPrimitiveList(4, m_vertices, 2, m_Colors, 4, m_TexCoords, 2, 0, EPT_TRIANGLE_FAN);
            
			/*glPushMatrix();
			
            glTranslatef(m_ClippedBounds.x, m_ClippedBounds.y, 0.0f);

		
			float x1, y1, x2, y2;

			x1 = m_ClippedBounds.x;
			y1 = m_ClippedBounds.y;
			x2 = m_ClippedBounds.x;
			y2 = y1 + m_ClippedBounds.height;
			
            
            glBegin(GL_QUADS);
             
             glColor4ub(m_Colors[0], m_Colors[1], m_Colors[2], m_Colors[3]);
             for( int i=0; i<8; i+=2)
             {
                 glVertex2f(m_vertices[i], m_vertices[i+1]);
             }
             glEnd();
             
						
			glPopMatrix();*/
			
			GUIContainer::render(); 
		}

		void GUIWindow::createGeometry()
		{
			m_vertices = new sf32[4*2];
			m_Colors   = new float[4*4];
		
			m_vertices[0] = 0;
			m_vertices[1] = 0;
			m_vertices[2] = m_Size.x;
			m_vertices[3] = 0;
			m_vertices[4] = m_Size.x;
			m_vertices[5] = m_Size.y;
			m_vertices[6] = 0.0f;
			m_vertices[7] = m_Size.y;
			
			for( int i=0; i<16; i++)
			{
				m_Colors[i] = 0.7f;
			}
		}
		
		sbool GUIWindow::OnEvent( Event& event)
		{
			static int currx, prevx;
			static int curry, prevy;
            static bool bPressedInsideTitleBar = false;

            if( !m_bEnable )return false;

            sbool bRes = false;

            if( event.eventType == SMOUSE_EVENT )
            {
                OnAction(event);
                
                if( m_TitleBorderPanel 
                   && event.mouseInput.Event == spider3d::LEFT_MOUSE_PRESSED_DOWN )
                {
                    sbool bInside = m_TitleBorderPanel->isPointInside(event.mouseInput.X, event.mouseInput.Y);
                    
                    if( bInside )
                        setActiveComponent(this);
                    
                    bPressedInsideTitleBar = bInside;
                    
                    if ( !bInside )return false;
                }
                
                if( event.mouseInput.Event == spider3d::LEFT_MOUSE_PRESSED_DOWN )
                {
                    currx = event.mouseInput.X;
                    curry = event.mouseInput.Y;
                    prevx = event.mouseInput.X;
                    prevy = event.mouseInput.Y;
                }
                if( event.mouseInput.Event == spider3d::SMOUSE_DRAGGED )
                {
                    currx = event.mouseInput.X;
                    curry = event.mouseInput.Y;
                }

                if( bPressedInsideTitleBar  && m_bPressed )
                    setPosition(m_Position.x + (currx-prevx) , m_Position.y + (curry-prevy));

                prevx = currx;
                prevy = curry;
                    
                sbool bRes = false;
                
                if( m_Parent )
                    bRes = m_Parent->OnEvent(event);
                
                if( event.mouseInput.Event == spider3d::LEFT_MOUSE_LEFT_UP )
                    setActiveComponent(0);
            }
            
            return bRes;
		}
		
		void GUIWindow::createWindow()
		{
			float titleBorderHeight = 0.05f;
			float titleBorderOptionButtonsPanelWidth = 0.12f;
			//m_Bounds.height -= titleBorderHeight;
			//m_Bounds.width  -= 10;
			
			if( m_Flags & TITLE_BORDER )
			{
				m_TitleBorderPanel = new GUIPanel(this, m_Size.x, m_Size.y * titleBorderHeight);
                //m_TitleBorderPanel->setActiveComponentEnable(false);
				m_TitleBorderPanel->setPosition(0, m_Size.y - m_Size.y * titleBorderHeight);
				
				m_TitleBorderOptionButtonsPanel = new GUIPanel(m_TitleBorderPanel, m_Size.x*titleBorderOptionButtonsPanelWidth, m_Size.y*0.05f);
				m_TitleBorderOptionButtonsPanel->setPosition(m_Size.x - m_Size.x*titleBorderOptionButtonsPanelWidth, 0);
				
				Color4f color;
				color.r = 1.0f;
				m_TitleBorderOptionButtonsPanel->setColor(color);

				if(m_Flags & CLOSE_BUTTON)
				{
					m_CloseButton = new GUIButton(m_TitleBorderOptionButtonsPanel, m_Size.x*titleBorderOptionButtonsPanelWidth*0.4f, m_Size.y*0.05f);
					m_CloseButton->setPosition(m_Size.x * titleBorderOptionButtonsPanelWidth - m_Size.x*titleBorderOptionButtonsPanelWidth*0.4f, 0);
					color.g = 1.0f;
					//m_CloseButton->setColor(color);
				}
				if(m_Flags & MINMIZE_BUTTON)
				{
					m_MinimizeButton = new GUIButton(m_TitleBorderOptionButtonsPanel,m_Size.x*titleBorderOptionButtonsPanelWidth*0.4f, m_Size.y*0.05f);
					m_MinimizeButton->setPosition(m_Size.x * titleBorderOptionButtonsPanelWidth - 2*m_Size.x*titleBorderOptionButtonsPanelWidth*0.4f, 0);
					color.r = 0.0f;
					//m_MinimizeButton->setColor(color);
				}
				if(m_Flags & MAXIMIZE_BUTTON)
				{
					m_MaximizeButton = new GUIButton(m_TitleBorderOptionButtonsPanel, m_Size.x*titleBorderOptionButtonsPanelWidth*0.4f, m_Size.y*0.05f);
					m_MaximizeButton->setPosition(m_Size.x * titleBorderOptionButtonsPanelWidth - 3*m_Size.x*titleBorderOptionButtonsPanelWidth*0.4f, 0);
					color.r = 0.9f;
					color.g = 0.3f;
					//m_MaximizeButton->setColor(color);
				}
			}
			
			if( m_Flags )//& STATUS_BAR )
			{
				m_StatusBarPanel = new GUIPanel(this, m_Size.x, m_Size.y*0.05f);
				m_StatusBarPanel->setPosition(0, 0);
			}
		}
		
		
		
	}
}