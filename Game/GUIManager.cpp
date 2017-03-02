/*
 *  GUIManager.cpp
 *  testCpp
 *
 *  Created by Prashant on 28/03/11.
 *  Copyright 2011 Sourcebits Technologies. All rights reserved.
 *
 */

#include "GUIManager.h"
#include "OglRenderer.h"
#include "Game.h"
#include "ShaderProgram.h"

using namespace spider3d::render;

namespace spider3d
{
	namespace gui 
	{
		
		GUIManager::GUIManager(int viewPortWidth, int viewPortHeight)
		{
            m_ViewPortWidth = viewPortWidth;
            m_ViewPortHeight = viewPortHeight;
			m_Bounds.x = 0;
			m_Bounds.y = 0;
			m_Bounds.width = viewPortWidth;
			m_Bounds.height = viewPortHeight;
            m_pActiveComponent = 0;
            m_pPrevFocusComponent = 0;
            m_pCurrFocusComponent = 0;
            m_pDefaultShaderProgram = NULL;
		}
		
		void GUIManager::drawGUI()
		{
			// Update All the GUI Components
			update();
						
			// Render All GUI Components
			render();
		}
		
         
		void GUIManager::render()
		{
            Game::getInstance()->getRenderer()->setTextureObject(0);
            
            m_pDefaultShaderProgram->OnSet();
            
            //glDepthFunc(GL_ALWAYS);
            Matrix4f orthoMatrix;
            orthoMatrix.ortho(0, m_ViewPortWidth, 0, m_ViewPortHeight, -1, 1);
            Game::getInstance()->getRenderer()->setTransform(orthoMatrix, spider3d::PROJECTION);
            
            Matrix4f xForm;
            Game::getInstance()->getRenderer()->setTransform(xForm, spider3d::WORLD);
            
			//glDisable(GL_DEPTH_TEST);
			
            
            Game::getInstance()->getRenderer()->setBlendingEnable(true);
            Game::getInstance()->getRenderer()->setBlendingFactors(spider3d::EBF_SRC_ALPHA, spider3d::EBF_ONE_MINUS_SRC_ALPHA);
			
			GUIContainer::render();
			//glDisable(GL_BLEND);
			//glEnable(GL_DEPTH_TEST);
            //glEnable(GL_TEXTURE_2D);
            
            m_pDefaultShaderProgram->UnSet();
		}
		
		sbool GUIManager::processEvents( Event& event)
		{
            bool bRes = false;
            
            OnAction(event);
            
            if( m_pActiveComponent )
            {
                return m_pActiveComponent->OnEvent(event);
            }
            
            findFocusComponent(event);

            if( m_pCurrFocusComponent )
            {
                m_pCurrFocusComponent->OnEvent(event);
            }
            
            GUIComponent *pComponent = 0;
            
            if( event.eventType == SMOUSE_EVENT )
            {
                pComponent = getComponentFromPoint(event.mouseInput.X, event.mouseInput.Y);
            }
            if( pComponent )
            {
				bRes = pComponent->OnEvent(event);
            }
            return bRes;
		}
        
        void GUIManager::findFocusComponent( Event& event)
        {
            if( event.eventType == SMOUSE_EVENT )
            {
                if( event.mouseInput.Event == LEFT_MOUSE_PRESSED_DOWN ||
                   event.mouseInput.Event == RIGHT_MOUSE_PRESSED_DOWN)
                {
                    m_pPrevFocusComponent = m_pCurrFocusComponent;
                    
                    if( m_pPrevFocusComponent )
                        m_pPrevFocusComponent->setFocus(false);
                    
                    m_pCurrFocusComponent = getComponentFromPoint(event.mouseInput.X, event.mouseInput.Y);
                    
                    if( m_pCurrFocusComponent )
                        m_pCurrFocusComponent->setFocus(true);
                }
            }
        }
		
	}
}