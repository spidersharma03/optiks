//
//  GUISlider.cpp
//  Optiks
//
//  Created by Prashant Sharma on 03/04/13.
//

#include "TextureMaker.h"
#include "GUISlider.h"
#include "GUIPanel.h"
#include "GUIButton.h"
#include "OglRenderer.h"
#include "Game.h"

using namespace spider3d::render;

namespace spider3d
{
	namespace gui 
	{
        Texture* GUISlider::m_pTexture = 0;
        
        GUISlider::GUISlider(si32 width, si32 height):GUIComponent(width, height)
        {
            m_Name = "SLIDER";
        }

        GUISlider::GUISlider(GUIContainer* parent, si32 width, si32 height):GUIComponent((GUIComponent*)parent, width, height)
        {
            m_Bounds.set(0, 0, width, 10);
            parent->addComponent(this);
			createGeometry();
            m_MinValue = 0.0f;
            m_MaxValue = 1.0f;
            
            Color4f c1, c2;
            
            c1.r = 150.0f/255; c1.g = 150.0f/255; c1.b = 150.0f/255; c1.a = 255.0f/255;
            c2.r = 55.0f/255; c2.g = 55.0f/255; c2.b = 55.0f/255; c2.a = 255.0f/255;
            
            if( !m_pTexture )
            {
                m_pTexture = createGradientTexture(c1, c2);
            }
            m_Name = "SLIDER";
        }
        
        void GUISlider::render()
        {
            if( !m_bIsClipped )
                return;
           
            float width = m_ClippedBounds.width;
            float height = m_ClippedBounds.height;
            
            // Darw Slider Base

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
            
            
            // Draw Slider Button
            
            vertices[0] = m_SliderButtonPos.x;
            vertices[1] = m_SliderButtonPos.y;
            vertices[2] = m_SliderButtonPos.x + 15;
            vertices[3] = m_SliderButtonPos.y;
            vertices[4] = m_SliderButtonPos.x + 15;
            vertices[5] = m_SliderButtonPos.y + 15;
            vertices[6] = m_SliderButtonPos.x;
            vertices[7] = m_SliderButtonPos.y + 15;
                                    
            Game::getInstance()->getRenderer()->drawVertexPrimitiveList(4, vertices, 2, m_Colors, 4, m_TexCoords, 1, 0, EPT_TRIANGLE_FAN);
        }
        
        void GUISlider::update()
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
                
                //m_SliderButtonPosX = m_ClippedBounds.x;
                if( m_SliderButtonPos.x > m_ClippedBounds.width )
                    m_SliderButtonPos.x = m_ClippedBounds.width;
                if( m_SliderButtonPos.x < 0 )
                    m_SliderButtonPos.x = 0;
                
                m_Value = (float)m_SliderButtonPos.x/m_ClippedBounds.width * ( m_MaxValue - m_MinValue ) + m_MinValue;
                
                m_Value = m_Value > m_MaxValue ? m_MaxValue : m_Value;
                m_Value = m_Value < m_MinValue ? m_MinValue : m_Value;
                //m_pSliderButton->m_Bounds;
			}
        }
        
        sbool GUISlider::OnEvent( Event& event)
        {      
            if( !m_bEnable )return false;

            OnAction(event);
            
            sbool bRes = false;

            if( event.eventType == SMOUSE_EVENT )
            {
                if( m_bPressed )
                {
                    m_SliderButtonPos.x = event.mouseInput.X - 8 - m_ClippedBounds.x;
                    //m_SliderButtonPos.y = event.mouseInput.Y -  m_ClippedBounds.y;
                }
                
                m_Value = (float)m_SliderButtonPos.x/m_ClippedBounds.width * ( m_MaxValue - m_MinValue ) + m_MinValue;

                m_Value = m_Value > m_MaxValue ? m_MaxValue : m_Value;
                m_Value = m_Value < m_MinValue ? m_MinValue : m_Value;
                
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
        
        void GUISlider::createGeometry()
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

            //m_pSliderButton = new GUIButton((GUIContainer*)m_Parent, 10, 10);
        }
        
        void GUISlider::setValue( float val )
        {
            m_Value = val;
            m_Value = m_Value > m_MaxValue ? m_MaxValue : m_Value;
            m_Value = m_Value < m_MinValue ? m_MinValue : m_Value;
            m_SliderButtonPos.x = m_ClippedBounds.width * (float)( m_Value - m_MinValue )/(m_MaxValue - m_MinValue);
        }
        
        float GUISlider::getValue() const
        {
            return m_Value;
        }

        
    }
}