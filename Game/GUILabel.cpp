//
//  GUILabel.cpp
//  RTSGame
//
//  Created by Arshad on 12/06/13.
//  Copyright (c) 2013 Sourcebits. All rights reserved.
//

#include "GUILabel.h"
#include "GUIContainer.h"
#include "TextureManager.h"
#include "OglRenderer.h"
#include "FileUtil.h"
#include "TextureMaker.h"
#include "FontRenderer.h"
#include "Game.h"

namespace spider3d
{
	namespace gui
	{
		
        Texture* GUILabel::m_pTexture = 0;

		GUILabel::GUILabel(GUIContainer* pParent,int width, int height)
        : GUIComponent((GUIContainer*)pParent, width, height),
          m_TextScale(1.0f),
          m_HorizontalAlignment(TextAlignment_Center),
          m_VerticalAlignment(TextAlignment_Center),
          m_HorzontalPadding(0.0f),
          m_VerticalPadding(0.0f),
          m_bVisible(true)
		{
            if( pParent )
                pParent->addComponent(this);

            m_BorderStyle = 0;
            
			createGeometry();
                                    
            m_LabelText = " ";
            
            Color4f c1, c2;
            
            c1.r = 250.0f/255; c1.g = 250.0f/255; c1.b = 250.0f/255; c1.a = 255.0f/255;
            c2.r = 185.0f/255; c2.g = 185.0f/255; c2.b = 185.0f/255; c2.a = 255.0f/255;
            m_TextColor = Color4f(0.0f,0.0f,0.0f,1.0f);
            if( !m_pTexture )
                m_pTexture = createGradientTexture(c1, c2);
		}
        
		void GUILabel::render()
		{
            if( ! IsVisible() )
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
            
            
            // Render text
            FNTRenderer->SetColor(m_TextColor);
            
            Vector2f pos(m_GlobalTransform.col3.x,m_GlobalTransform.col3.y);
            
            float text_w = FNTRenderer->GetTextWidth(m_LabelText) * m_TextScale;
            float text_h = FNTRenderer->GetTextHeight(m_LabelText) * m_TextScale;
            
            AdjustForAlignment(pos, text_w, text_h);
            AdjustForPadding(pos);
            
            //FNTRenderer->SetFont("UI_LvlBar16empty");
            FNTRenderer->SetPosition(pos);
            FNTRenderer->SetScale(m_TextScale);
            
            FNTRenderer->DrawText(m_LabelText);
            
		}
        
        void GUILabel::AdjustForAlignment(Vector2f &pos, float width, float height)
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
        
        void GUILabel::AdjustForPadding(Vector2f& pos)
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
        
		void GUILabel::update()
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
		
        bool GUILabel::OnEvent( Event& event)
		{
            if( !m_bEnable )return false;
            
            sbool bRes = false;
            
            if( event.eventType == SMOUSE_EVENT )
            {
                OnAction(event);
                
                if( m_bPressed )
                {
                    setColor(Color4f(0.7f,0.7f,0.7f,1.0f));
                }
                else
                {
                    setColor(Color4f(1.0f,1.0f,1.0f,1.0f));
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
		
		void GUILabel::createGeometry()
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
        
        void GUILabel::SetText(const string& text)
        {
            m_LabelText = text;
        }
        
        const string& GUILabel::GetText() const
        {
            return m_LabelText;
        }
        
        
        void GUILabel::SetTextColor(const Color4f& color)
        {
            m_TextColor = color;
        }
        
        Color4f GUILabel::GetTextColor() const
        {
            return m_TextColor;
        }
        
        void GUILabel::SetTextScale(float scale)
        {
            m_TextScale = scale;
        }
        
        void GUILabel::SetHorizontalAlignment(TextAlignment alignment)
        {
            m_HorizontalAlignment = alignment;
        }
        
        void GUILabel::SetVerticalAlignment(TextAlignment alignment)
        {
            m_VerticalAlignment = alignment;
        }
        
        void GUILabel::SetPadding(float horizontal, float vertical)
        {
            m_HorzontalPadding = horizontal;
            m_VerticalPadding = vertical;
        }
        
        void GUILabel:: SetVisible( bool bVisible )
        {
            m_bVisible = bVisible;
        }
        
        bool GUILabel:: IsVisible()
        {
            return m_bVisible;
        }

        
	}
}