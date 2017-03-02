/*
 *  GUIButton.cpp
 *  testCpp
 *
 *  Created by Prashant on 30/03/11.
 *  Copyright 2011 Sourcebits Technologies. All rights reserved.
 *
 */

#include "GUIButton.h"
#include "GUIContainer.h"
#include "TextureMaker.h"
#include "FontRenderer.h"
#include "Texture1D.h"
#include "OglRenderer.h"
#include "Game.h"
#include "FileLoader.h"

namespace spider3d
{
	namespace gui 
	{
		
		GUIButton::GUIButton(si32 width, si32 height):GUIComponent(width, height)
		{
			createGeometry();
			m_bPressed = false;
            m_pActionListener = 0;
            m_Name = "BUTTON";
            m_FontScale = 1.0f;
		}

		GUIButton::GUIButton(GUIContainer* parent, si32 width, si32 height):GUIComponent((GUIComponent*)parent, width, height)
		{
			parent->addComponent(this);
			createGeometry();
			m_bPressed = false;
            
            Color4f c1, c2;
            
            c1.r = 150.0f/255; c1.g = 150.0f/255; c1.b = 150.0f/255; c1.a = 255.0f/255;
            c2.r = 85.0f/255; c2.g = 85.0f/255; c2.b = 85.0f/255; c2.a = 255.0f/255;
            
            m_pTexture = 0;//createGradientTexture(c1, c2);
                //m_pTexture = TextureLoader::Instance()->getTexture("/Users/prashant/Desktop/Optiks/ScreenShot.png");
            m_Name = "BUTTON";
            m_HorizontalAlignment = TextAlignment_Center;
            m_VerticalAlignment   = TextAlignment_Center;
            m_Color               = Color4f(0.0f,0.0f,0.0f,1.0f);
            m_FontScale = 1.0f;
		}

		void GUIButton::render()
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
            

            DrawTitle();
		}

		void GUIButton::update()
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
		
		sbool GUIButton::OnEvent( Event& event)
		{
            if( !m_bEnable )
            {
                m_Parent->OnEvent(event);
                return false;
            }
            
            sbool bRes = false;

            if( event.eventType == SMOUSE_EVENT )
            {
                OnAction(event);
                
                if( m_bPressed )
                {
                    //setColor(Color4f(0.7f,0.7f,0.7f,1.0f));
                }
                else
                {
                    //setColor(Color4f(1.0f,1.0f,1.0f,1.0f));
                }
                            
                if( m_Parent )
                    bRes = m_Parent->OnEvent(event);
                
                if( event.mouseInput.Event == spider3d::LEFT_MOUSE_PRESSED_DOWN )
                    setActiveComponent(this);
                if( event.mouseInput.Event == spider3d::LEFT_MOUSE_LEFT_UP )
                    setActiveComponent(0);
            }
            return bRes;
		}
		
        void GUIButton::DrawTitle()
        {
            Vector2f pos(m_GlobalTransform.col3.x,m_GlobalTransform.col3.y);
            float text_w = FNTRenderer->GetTextWidth(m_Title) * m_FontScale;
            float text_h = FNTRenderer->GetTextHeight(m_Title) * m_FontScale;
            
            adjustForAlignment(pos, text_w, text_h);
            adjustForPadding(pos);
            
            Color4f textColor(0.0f,0.0f,0.0f,1.0f-m_Transparency);
            
            FNTRenderer->SetPosition(pos);
            FNTRenderer->SetScale(m_FontScale);
            FNTRenderer->SetColor(textColor);
            FNTRenderer->DrawText(m_Title);
        }
        
        void GUIButton::adjustForAlignment( Vector2f& pos, float width, float height)
        {
            switch(m_HorizontalAlignment)
            {
                case TextAlignment_Center:
                    pos.x += ((m_Size.x - width)/2.0f);
                    break;
                case TextAlignment_Right:
                    pos.x += (m_Size.x - width);
                    break;
                default:
                    break;
            }
            
            switch(m_VerticalAlignment)
            {
                case TextAlignment_Center:
                    pos.y += m_Size.y - ((m_Size.y - height)/2.0f);
                    break;
                case TextAlignment_Bottom:
                    pos.y += m_Size.y - (m_Size.y - height);
                    break;
                default:
                    break;
            }
        }
        
        void GUIButton::adjustForPadding(Vector2f& pos)
        {
            switch(m_HorizontalAlignment)
            {
                case TextAlignment_Left:
                    pos.x += m_HorzontalPadding;
                    break;
                case TextAlignment_Right:
                    pos.x -= m_HorzontalPadding;
                    break;
                default:
                    break;
            }
            
            switch(m_VerticalAlignment)
            {
                case TextAlignment_Top:
                    pos.y -= m_VerticalPadding;
                    break;
                case TextAlignment_Bottom:
                    pos.y -= m_VerticalPadding;
                    break;
                default:
                    break;
            }
        }
        
        void GUIButton::setHorizontalAlignment(TextAlignment alignment)
        {
            m_HorizontalAlignment = alignment;
        }
        
        void GUIButton::setVerticalAlignment(TextAlignment alignment)
        {
            m_VerticalAlignment = alignment;
        }
        
        void GUIButton::setPadding(float horizontal, float vertical)
        {
            m_HorzontalPadding = horizontal;
            m_VerticalPadding = vertical;
        }

		void GUIButton::createGeometry()
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
            m_TexCoords[3] = 0.0f;
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
        
        void GUIButton::setTitle(const string& title)
        {
            m_Title = title;
        }
        
        void GUIButton::setFontScale( float fontScale )
        {
            m_FontScale = fontScale;
        }
        
        void GUIButton::setTexture( Texture* pTexture )
        {
            m_pTexture = pTexture;
        }
	}
}