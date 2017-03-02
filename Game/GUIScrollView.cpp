//
//  GUIScrollView.cpp
//  Optiks
//
//  Created by Prashant on 11/02/14.
//
//

#include "GUIScrollView.h"
#include "GUIPanel.h"
#include "GUIButton.h"
#include "GUIWindow.h"
#include "Matrix4.h"
#include "OglRenderer.h"
#include "Game.h"
#include "TextureMaker.h"

using namespace spider3d::render;
using namespace NAMESPACE_DIRAC2D;

namespace spider3d
{
	
	namespace gui
	{
        Texture* GUIScrollView::m_pTexture = 0;

		GUIScrollView::GUIScrollView(GUIContainer* parent, si32 width, si32 height):
        GUIContainer(parent, width, height )
		{
            m_VelocityDampingFactor = 0.8f;
            m_RestoringForceFactor = 0.1f;
            m_Period = 640;
            m_MeanPosition = 0;// + m_Size.x*0.5f;
            m_VirtualPosition = 0;
            m_AccDisplacement = 0.0f;
            
            m_Color.r = m_Color.g = m_Color.b  = m_Color.a = 255;
            
            Color4f c1, c2;
            c1.r = 50.0f/255; c1.g = 50.0f/255; c1.b = 50.0f/255; c1.a = 255.0f/255;
            c2.r = 155.0f/255; c2.g = 155.0f/255; c2.b = 155.0f/255; c2.a = 255.0f/255;
            
            if( !m_pTexture )
                m_pTexture = createGradientTexture(c1, c2);
            m_Name = "SCROLLVIEW";
            
            createGeometry();
            
            m_LeftBoundaryLimit = 1000;
            m_RightBoundaryLimit = 1000;
		}
		
		
		void GUIScrollView::render()
		{
            if( !m_bVisible )
                return;

            // Move the Child Components, only if the Accumulated Displacement is > 1
            if( !m_bPressed )
            {
                int   displacementFromEquilibrium = (m_MeanPosition - m_VirtualPosition);
                float displacementThisFrame = 0.0f;
                
                displacementThisFrame = (float)displacementFromEquilibrium * m_RestoringForceFactor;
                
                // Accumulate the Displacement over time
                if( displacementThisFrame*displacementThisFrame < 1 )
                {
                    m_AccDisplacement += displacementThisFrame;
                }
                else
                {
                    m_AccDisplacement = displacementThisFrame;
                }
                
                if( m_AccDisplacement*m_AccDisplacement >= 1.0f )
                {
                    // Move towards the equilibrium point
                    moveComponentsWithVelocity(Vector2i(m_AccDisplacement, 0.0f));
                    
                    m_AccDisplacement = 0.0f;
                }
            }

            NAMESPACE_DIRAC2D::Matrix4f xForm;
            
            xForm.translate(m_ClippedBounds.x, m_ClippedBounds.y);
            
            Game::getInstance()->getRenderer()->setTransform(xForm, spider3d::WORLD);
            
            Game::getInstance()->getRenderer()->setTextureObject(m_pTexture);
            
            Game::getInstance()->getRenderer()->drawVertexPrimitiveList(4, m_vertices, 2, m_Colors, 4, m_TexCoords, 1, 0, EPT_TRIANGLE_FAN);
            
			GUIContainer::render();
		}
        
		sbool GUIScrollView::OnEvent( Event& event)
		{
            // Decide the Mean Position towards which we will move.
            if( event.mouseInput.Event == LEFT_MOUSE_LEFT_UP )
            {
                int   displacementFromEquilibrium = (m_VirtualPosition - m_MeanPosition);
                
                int index = (float)displacementFromEquilibrium/m_Period;
                
                if( (displacementFromEquilibrium) > m_Period/10 )
                {
                    m_MeanPosition += (index+1)*m_Period;
                }
                else if( displacementFromEquilibrium < -m_Period/10 )
                {
                    m_MeanPosition -= (-index+1)*m_Period;
                }
                m_MeanPosition = m_MeanPosition > m_LeftBoundaryLimit*m_Period ? m_LeftBoundaryLimit*m_Period : m_MeanPosition;
                m_MeanPosition = m_MeanPosition < -m_RightBoundaryLimit*m_Period ? -m_RightBoundaryLimit*m_Period : m_MeanPosition;
            }
            
            OnAction(event);
            
            int currentX = -1;
            int currentY = -1;
            
            sbool bRes = false;
            
            if( event.eventType == SMOUSE_EVENT )
            {
                if( event.mouseInput.Event == LEFT_MOUSE_PRESSED_DOWN )
                {
                    currentX = event.mouseInput.X;
                    currentY = event.mouseInput.Y;
                    m_PreviousX = currentX;
                    m_PreviousY = currentY;
                    m_PanPosition.x = m_PanPosition.y = 0.0f;
                }
                if( event.mouseInput.Event == SMOUSE_DRAGGED )
                {
                    currentX = event.mouseInput.X;
                    currentY = event.mouseInput.Y;
                    m_PanPosition.x = (currentX - m_PreviousX);
                    m_PanPosition.y = (currentY - m_PreviousY);
                }
                if( m_bPressed )
                {
                    scrollComponents( m_PanPosition.x, 0.0f );
                }

                bRes = false;
                
                if( m_Parent )
                    bRes = m_Parent->OnEvent(event);
                
                if( event.mouseInput.Event == spider3d::LEFT_MOUSE_PRESSED_DOWN )
                    setActiveComponent(this);
                if( event.mouseInput.Event == spider3d::LEFT_MOUSE_LEFT_UP )
                    setActiveComponent(0);
                
                m_PreviousX = currentX;
                m_PreviousY = currentY;
            }
            return bRes;
		}
		
		void GUIScrollView::update()
		{
            //m_VirtualPosition = m_Position.x;
            
            GUIContainer::update();
		}
        
		void GUIScrollView::scrollComponents(si32 dx, si32 dy)
		{
			vector<GUIComponent*>& vecComponents = getComponents();
			for (unsigned c=0; c<vecComponents.size(); c++)
			{
				GUIComponent *component = vecComponents[c];
				si32 x = component->getLocalTransform().col3.x;
				si32 y = component->getLocalTransform().col3.y;
				component->setPosition(x+dx, y);
			}
            m_VirtualPosition += dx;
		}
        
        void GUIScrollView::moveComponentsWithVelocity(const Vector2i& velocity)
		{
			vector<GUIComponent*>& vecComponents = getComponents();
			for (unsigned c=0; c<vecComponents.size(); c++)
			{
				GUIComponent *component = vecComponents[c];
				float x = component->getPosition().x;
				float y = component->getPosition().y;
				component->setPosition(x + velocity.x, y + velocity.y );
                //printf("\nx = %f  %d  %d", x, m_VirtualPosition, velocity.x);
			}
            m_VirtualPosition += velocity.x;
		}
        
        void GUIScrollView::createGeometry()
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
