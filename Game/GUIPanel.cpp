/*
 *  GUIPanel.cpp
 *  testCpp
 *
 *  Created by Prashant on 30/03/11.
 *  Copyright 2011 Sourcebits Technologies. All rights reserved.
 *
 */

#include "GUIPanel.h"
#include "TextureMaker.h"
#include "OglRenderer.h"
#include "Matrix4.h"
#include "Texture1D.h"
#include "Game.h"

using namespace NAMESPACE_DIRAC2D;

using namespace spider3d::render;

namespace spider3d
{
	namespace gui 
	{
		
        Texture* GUIPanel::m_pTexture = 0;
        
		GUIPanel::GUIPanel(si32 width, si32 height):GUIContainer(width, height)
		{
			createGeometry();
			m_Color.r = m_Color.g = m_Color.b  = m_Color.a = 255;
            Color4f c1, c2;
            c1.r = 10.0f; c1.g = 120.0f; c1.b = 120.0f; c1.a = 250.0f;
            c2.r = 220.0f; c2.g = 220.0f; c2.b = 220.0f; c2.a = 250.0f;
            
            if( !m_pTexture )
                m_pTexture = createGradientTexture(Vector2f(0.0f,0.0f), c1, Vector2f(64.0f,64.0f), c2);
            
            m_Name = "PANEL";
		}
		
        void GUIPanel::update()
		{
			GUIContainer::update();
		}
        
		GUIPanel::GUIPanel(GUIContainer* parent, si32 width, si32 height):GUIContainer(parent, width, height)
		{
			createGeometry();
			m_Color.r = m_Color.g = m_Color.b  = m_Color.a = 255;
            
            Color4f c1, c2;
            c1.r = 50.0f/255; c1.g = 50.0f/255; c1.b = 50.0f/255; c1.a = 255.0f/255;
            c2.r = 155.0f/255; c2.g = 155.0f/255; c2.b = 155.0f/255; c2.a = 255.0f/255;
            
            //texID = createGradientTexture(Vector2(0.0f,0.0f), c1, Vector2(256.0f,256), c2);
            if( !m_pTexture )
                m_pTexture = createGradientTexture(c1, c2);            
            m_Name = "PANEL";
		}
		
		void GUIPanel::render()
		{
            if( !m_bIsClipped )
                return;
            
            if( !m_bVisible  )
                return;
            
            NAMESPACE_DIRAC2D::Matrix4f xForm;
            
            xForm.translate(m_ClippedBounds.x, m_ClippedBounds.y);
            
            Game::getInstance()->getRenderer()->setTransform(xForm, spider3d::WORLD);
            
            Game::getInstance()->getRenderer()->setTextureObject(m_pTexture);
            
            Game::getInstance()->getRenderer()->drawVertexPrimitiveList(4, m_vertices, 2, m_Colors, 4, m_TexCoords, 2, 0, EPT_TRIANGLE_FAN);

			GUIContainer::render();
		}
		
		sbool GUIPanel::OnEvent( Event& event)
		{
            if( !m_bEnable )return false;

            OnAction(event);

            sbool bRes = false;
            
            if( event.eventType == SMOUSE_EVENT )
            {
                if( m_Parent )
                    bRes = m_Parent->OnEvent(event);
                
                if( event.mouseInput.Event == spider3d::LEFT_MOUSE_PRESSED_DOWN )
                    setActiveComponent(this);
                if( event.mouseInput.Event == spider3d::LEFT_MOUSE_LEFT_UP )
                    setActiveComponent(0);
            }
            return bRes;
		}
		
		
		void GUIPanel::createGeometry()
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