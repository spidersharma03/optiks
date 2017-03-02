//
//  GUICheckBox.cpp
//  Optiks
//
//  Created by Prashant Sharma on 06/12/13.
//
//

#include "GUICheckBox.h"
#include "GUIContainer.h"
#include "TextureMaker.h"
#include "OglRenderer.h"
#include "Game.h"

using namespace spider3d::render;

namespace spider3d
{
	namespace gui
	{
     
        Texture* GUICheckBox::m_pTexture = 0;

        GUICheckBox::GUICheckBox():GUIComponent(CHECK_BOX_SIZE,CHECK_BOX_SIZE)
        {
            m_bCheckOn = false;
        }

        GUICheckBox::GUICheckBox(GUIContainer* parent):GUIComponent((GUIComponent*)parent,CHECK_BOX_SIZE,CHECK_BOX_SIZE)
        {
            parent->addComponent(this);
			createGeometry();
			m_bPressed = false;
            m_bCheckOn = false;
            
            Color4f c1, c2;
            
            c1.r = 150.0f/255; c1.g = 150.0f/255; c1.b = 150.0f/255; c1.a = 255.0f/255;
            c2.r = 85.0f/255; c2.g = 85.0f/255; c2.b = 85.0f/255; c2.a = 255.0f/255;
            
            if( !m_pTexture )
                m_pTexture = createGradientTexture(c1, c2);
            m_Name = "CHECK BOX";
            m_Color               = Color4f(0.0f,0.0f,0.0f,1.0f);
        }

        void GUICheckBox::render()
        {
            if( !m_bIsClipped )
                return;
            
            float width = m_ClippedBounds.width;
            float height = m_ClippedBounds.height;
            
            float vertices[8];
            vertices[0] = 0.0f;
            vertices[1] = 0.0f;
            vertices[2] = width;
            vertices[3] = 0.0f;
            vertices[4] = width;
            vertices[5] = height;
            vertices[6] = 0.0f;
            vertices[7] = height;
            
            NAMESPACE_DIRAC2D::Matrix4f xForm;
            
            xForm.translate(m_ClippedBounds.x, m_ClippedBounds.y);
            
            Game::getInstance()->getRenderer()->setTransform(xForm, spider3d::WORLD);
            
            Game::getInstance()->getRenderer()->setTextureObject(m_pTexture);
            
            Game::getInstance()->getRenderer()->drawVertexPrimitiveList(4, vertices, 2, m_Colors, 4, m_TexCoords, 2, 0, EPT_TRIANGLE_FAN);
            
            // Draw the Check
            if( m_bCheckOn )
            {
                xForm.setIdentity();
                xForm.translate(m_ClippedBounds.x + CHECK_BOX_SIZE*0.25f, m_ClippedBounds.y + CHECK_BOX_SIZE*0.25f);
                xForm.m00 = xForm.m11 = 0.5f;
                
                Game::getInstance()->getRenderer()->setTextureObject(NULL);
                
                Game::getInstance()->getRenderer()->setTransform(xForm, spider3d::WORLD);
                
                float colors[16];
                for( int i=0; i<16; i+=4 )
                {
                    colors[i]   = 0.2f;
                    colors[i+1] = 0.2f;
                    colors[i+2] = 0.2f;
                    colors[i+3] = 1.0f;
                }
                Game::getInstance()->getRenderer()->drawVertexPrimitiveList(4, vertices, 2, colors, 4, m_TexCoords, 1, 0, EPT_TRIANGLE_FAN);
            }
		}
        
        void GUICheckBox::update()
        {
			if(m_Parent)
			{
				Matrix3f& parentTransform = m_Parent->getTransform();
				m_GlobalTransform.multiply(parentTransform, m_LocalTransform);
				m_Bounds.x = m_GlobalTransform.col3.x;
				m_Bounds.y = m_GlobalTransform.col3.y;
				
				// Check for Clipping against its parent
				m_ClippedBounds = m_Bounds;
				Recti& parentRect = m_Parent->getRectangle();
				m_bIsClipped = intersect(parentRect, m_Bounds, m_ClippedBounds);
			}
		}
        
        void GUICheckBox::setCheckEnable( bool bCheckEnable )
        {
            m_bCheckOn = bCheckEnable;
        }
        
        sbool GUICheckBox::OnEvent( Event& event)
        {
            if( !m_bEnable )return false;
            
            sbool bRes = false;
            
            if( event.eventType == SMOUSE_EVENT )
            {
                if( m_Parent )
                    bRes = m_Parent->OnEvent(event);
                
                if( event.mouseInput.Event == spider3d::LEFT_MOUSE_LEFT_UP )
                {
                    m_bCheckOn = !m_bCheckOn;
                }
                if( event.mouseInput.Event == spider3d::LEFT_MOUSE_PRESSED_DOWN )
                    setActiveComponent(this);
                if( event.mouseInput.Event == spider3d::LEFT_MOUSE_LEFT_UP )
                    setActiveComponent(0);
                
                OnAction(event);
            }
            return bRes;
		}

        void  GUICheckBox::createGeometry()
        {
			m_vertices = new sf32[4*2];
			m_Colors   = new float[4*4];
            m_TexCoords = new sf32[4*2];
            
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