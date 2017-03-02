/*
 *  GUIComponent.h
 *  testCpp
 *
 *  Created by Prashant on 28/03/11.
 *  Copyright 2011 Sourcebits Technologies. All rights reserved.
 *
 */

#ifndef _GUI_COMPONENT_H
#define _GUI_COMPONENT_H


#include <string>
#include "MathUtil.h"
#include "EventListener.h"
#include "Rectangle.h"
#include "IActionListener.h"

using namespace spider3d;
using namespace std;

USE_NAMESPACE_DIRAC2D

class Texture;

class Recti
{
public:
    Recti()
    {
    }
    
	float x,y, width, height;
	
	void set( float x_, float y_, float w, float h)
	{
		x = x_; y = y_;
		width = w;
		height = h;
	}
};

class IFocusListner
{
public:
	IFocusListner()
	{
	}
};

inline bool intersect(Recti& rect1, Recti& rect2, Recti& intersectRect)
{
	float x1 = rect1.x;
	float y1 = rect1.y;
	float x2 = rect2.x;
	float y2 = rect2.y;
	
	float gx = x1 > x2 ? x1 : x2;
	float gy = y1 > y2 ? y1 : y2;
    
	x1 += rect1.width;
	y1 += rect1.height;
	x2 += rect2.width;
	y2 += rect2.height;
	
	float lx = x1 < x2 ? x1 : x2;
	float ly = y1 < y2 ? y1 : y2;
    
	intersectRect.x = gx;
	intersectRect.y = gy;
	intersectRect.width  = lx-gx;
	intersectRect.height = ly-gy;
	
	bool result = (gx < lx) && (gy<ly) ? true : false;
	
	return result;
}

namespace spider3d
{
	namespace gui 
	{
		class GUIContainer;
		
		class GUIComponent : public EventListener
		{
		public:
			
			GUIComponent(si32 width, si32 height);
			
			GUIComponent(GUIComponent*, si32 width, si32 height);

			virtual void render() = 0;
				
			virtual void update() = 0;

			void setEnable(sbool);
			
            bool isEnable() const;

			void addEventListner(EventListener* );
			
			void addFocusListner(IFocusListner* );
			
			void setSize(si32, si32);
			
            void setScale(sf32 sx, sf32 sy);

			void setPosition(si32, si32);

            Vector2i getPosition() const
            {
                return m_Position;
            }
            
            Vector2i getWorldPosition() const
            {
                Vector2f v(m_Position.x, m_Position.y);
                m_GlobalTransform.transformAsPoint(v);
                return Vector2i(v.x, v.y);
            }

			void setTransform(Matrix3f&);
			
			void setColor(const Color4f&);
						
			void setTransparency(sf32);
			
            void setParent( GUIComponent* pParent)
            {
                m_Parent = pParent;
            }
            
            
            inline void setName( const string& name )
            {
                m_Name = name;
            }
            
            inline const string& getName() const
            {
                return m_Name;
            }
            
            inline void setTag( si32 tag )
            {
                m_Tag = tag;
            }
            
            inline si32 getTag() const
            {
                return m_Tag;
            }
            
            inline Vector2i getSize()
            {
                return m_Size;
            }
            
			inline GUIComponent* getParent()
			{    
				return m_Parent;
			}

			inline bool hasFocus()
			{
				return m_bFocused;
			}
			
			inline sbool isVisible()
            {
                return m_bVisible;
            }
            
            inline void setVisible(sbool bVisible)
            {
                m_bVisible = bVisible;
            }
            
			inline Matrix3f& getTransform()
			{
				return m_GlobalTransform;
			}
			
			inline const Matrix3f& getLocalTransform()
			{
				return m_LocalTransform;
			}
			
			inline Recti& getRectangle()
			{
				return m_Bounds;
			}
			
            virtual void setActiveComponent(GUIComponent* pComponent)
            {
                if( m_Parent && m_bActiveCompEnable)
                    m_Parent->setActiveComponent(pComponent);
            }
            
            void setActiveComponentEnable( sbool bEnable )
            {
                m_bActiveCompEnable = bEnable;
            }
            
            void OnAction( Event& event)
            {
                if(event.eventType == SMOUSE_EVENT )
                {
                    if( event.mouseInput.Event == spider3d::LEFT_MOUSE_PRESSED_DOWN )
                    {
                        m_bPressed = true;
                        if(m_pActionListener)
                            m_pActionListener->mousePressed(this, event.mouseInput.X,event.mouseInput.Y);
                    }
                    if( event.mouseInput.Event == spider3d::LEFT_MOUSE_LEFT_UP)
                    {
                        m_bPressed = false;
                        if(m_pActionListener)
                            m_pActionListener->mouseReleased(this, event.mouseInput.X,event.mouseInput.Y);
                    }
                    if( event.mouseInput.Event == spider3d::SMOUSE_DRAGGED)
                    {
                        if(m_pActionListener)
                            m_pActionListener->mouseMoved(this, event.mouseInput.X,event.mouseInput.Y);
                    }
                }
                if(event.eventType == SKEY_EVENT )
                {
                    if( m_pActionListener )
                        m_pActionListener->keyPressed(this,event);
                }
            }

			virtual GUIComponent* getComponentFromPoint(si32 x, si32 y)
			{
				if( isPointInside(x,y) )
				{
					return this;
				}
				else 
				{
					return 0;
				}
			}
			
			inline bool isPointInside(si32 x, si32 y)
			{
				return ( x < ( m_ClippedBounds.x + m_ClippedBounds.width) && x > (  m_ClippedBounds.x ) 
						&& y < (  m_ClippedBounds.y +  m_ClippedBounds.height ) && y > (  m_ClippedBounds.y) );
			}
			
            void aboutMe()
            {
                printf("%s\n",m_Name.c_str());
            }
			
            void setActionListener( IActionListener* pActionListener)
            {
                m_pActionListener = pActionListener;
            }
            
            void setUserData( void* pUserData)
            {
                m_pUserData = pUserData;
            }
            
            void* getUserData() const
            {
                return m_pUserData;
            }
            
            bool isClipped()
            {
                return m_bIsClipped;
            }
            
			friend class GUIContainer;
			
		protected:
			
            void setFocus(bool bFocus)
            {
                m_bFocused = bFocus;
            }
            
			virtual void createGeometry()
			{};
			
			Matrix3f            m_GlobalTransform;
			Matrix3f            m_LocalTransform;
			sbool               m_bFocused;
            sbool               m_bActiveCompEnable;
            Vector2i            m_Size;
			Vector2i            m_Position;
            
			Color4f             m_Color;
			sf32                m_Transparency;
			sbool               m_bEnable;
            sbool               m_bPressed;
            sbool               m_bVisible;
			GUIComponent        *m_Parent;
			            
			sf32                *m_vertices;
            sf32                *m_TexCoords;
			float               *m_Colors;
            int                 m_BorderStyle;
			
			Recti               m_Bounds;
			Recti               m_ClippedBounds;
            sbool               m_bIsClipped;
            string              m_Name;
            IActionListener     *m_pActionListener;
            
            void                *m_pUserData;
            si32                m_Tag;
            
            friend class        GUIManager;
		};
	}
	
}
#endif