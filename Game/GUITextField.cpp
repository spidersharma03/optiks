//
//  GUITextField.cpp
//  Optiks
//
//  Created by Prashant Sharma on 18/11/13.
//
//

#include "GUITextField.h"
#include "GUIContainer.h"
#include "TextureMaker.h"
#include "FontRenderer.h"
#include "Texture1D.h"
#include "OglRenderer.h"
#include "GUILabel.h"
#include <assert.h>
#include "Game.h"

namespace spider3d
{
	namespace gui
	{
		
        Texture* GUITextField::m_pTexture = 0;
        
		GUITextField::GUITextField(si32 width, si32 height):GUIComponent(width, height)
		{
			createGeometry();
            m_pLabel = new GUILabel(0,width, height);
            m_pLabel->setParent(this);
			m_bPressed = false;
            m_pActionListener = 0;
            m_Name = "BUTTON";
		}
        
		GUITextField::GUITextField(GUIContainer* parent, si32 width, si32 height):GUIComponent((GUIComponent*)parent, width, height)
		{
			parent->addComponent(this);
			createGeometry();
            m_pLabel = new GUILabel(0,width, height);
            m_pLabel->setParent(this);
            //m_pLabel->SetHorizontalAlignment();
			m_bPressed = false;
            
            Color4f c1, c2;
            
            c1.r = 150.0f/255; c1.g = 150.0f/255; c1.b = 150.0f/255; c1.a = 255.0f/255;
            c2.r = 185.0f/255; c2.g = 185.0f/255; c2.b = 185.0f/255; c2.a = 255.0f/255;
            
            if( !m_pTexture )
                m_pTexture = createGradientTexture(c1, c2);
            m_Name = "BUTTON";
		}
        
		void GUITextField::render()
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
            
            //m_pLabel->setPosition(m_ClippedBounds.x, m_ClippedBounds.y);
            m_pLabel->SetTextColor(Color4f(0.0f,0.0f,0.0f,1.0f));
            //m_pLabel->SetTextScale(0.3f);
            m_pLabel->SetText(m_Text);
            m_pLabel->render();
		}
        
		void GUITextField::update()
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
            m_pLabel->update();
		}
		
		sbool GUITextField::OnEvent( Event& event)
		{
            if( !m_bEnable )return false;
            
            OnAction(event);
            
            sbool bRes = false;
            
            if( m_Parent )
                bRes = m_Parent->OnEvent(event);

            if( event.eventType == SMOUSE_EVENT )
            {
                if( event.mouseInput.Event == spider3d::LEFT_MOUSE_PRESSED_DOWN )
                    setActiveComponent(this);
                if( event.mouseInput.Event == spider3d::LEFT_MOUSE_LEFT_UP )
                    setActiveComponent(0);
            }
            if( event.eventType == SKEY_EVENT && hasFocus() )
            {
                wchar_t character = event.keyInput.Char;
                if( event.keyInput.PressedDown && event.keyInput.Key != KEY_DELETE )
                {
                    pushChar(character);
                }
                if( event.keyInput.PressedDown && event.keyInput.Key == KEY_DELETE )
                {
                    popChar();
                }
            }
            return bRes;
		}
		
        void GUITextField::setText(const string& text)
        {
            m_Text = text;
        }
        
        string GUITextField::getText() const
        {
            return m_Text;
        }

        void GUITextField::insertChar(wchar_t character, int index)
        {
            m_Text.insert(m_Text.begin()+index, character);
        }
        
        void GUITextField::removeChar(wchar_t character, int index)
        {
            assert(index < m_Text.length());
            m_Text.erase(m_Text.begin()+index);
        }
        
        void GUITextField::pushChar(wchar_t character)
        {
            m_Text.push_back(character);
        }
        
        void GUITextField::popChar()
        {
            if( m_Text.size() > 0 )
                m_Text.erase(m_Text.end()-1);
        }
        
		void GUITextField::createGeometry()
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