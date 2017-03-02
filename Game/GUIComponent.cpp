/*
 *  GUIComponent.cpp
 *  testCpp
 *
 *  Created by Prashant on 28/03/11.
 *  Copyright 2011 Sourcebits Technologies. All rights reserved.
 *
 */

#include "GUIComponent.h"

namespace spider3d
{
	namespace gui 
	{
		
		GUIComponent::GUIComponent(si32 width, si32 height):m_Size(width,height)
		{
			m_bFocused = false;
            m_bIsClipped = false;
            m_bActiveCompEnable = true;
            m_bVisible = true;
			m_Color.r = m_Color.g = m_Color.b = 128;
			m_Color.a = 255;
			m_Transparency = 0.0f;
			m_bEnable = true;
			m_Parent = 0;
			m_Bounds.x = m_Position.x;
			m_Bounds.y = m_Position.y;
			m_Bounds.width = width;
			m_Bounds.height = height;
			createGeometry();
            m_pActionListener = 0;
            m_bPressed = false;
            m_pUserData = NULL;
            m_Tag = -1;
            m_BorderStyle = 0;
		}
		
		GUIComponent::GUIComponent(GUIComponent* parent, si32 width, si32 height):m_Parent(parent),m_Size(width,height)
		{
			m_bFocused = false;
            m_bActiveCompEnable = true;
            m_bIsClipped = false;
            m_bVisible = true;
			m_Color.r = m_Color.g = m_Color.b = 128;
			m_Color.a = 255;
			m_Transparency = 0.0f;
			m_bEnable = true;
			m_Bounds.x = m_Position.x;
			m_Bounds.y = m_Position.y;
			m_Bounds.width = width;
			m_Bounds.height = height;
			createGeometry();
            m_pActionListener = 0;
            m_bPressed = false;
            m_pUserData = NULL;
            m_Tag = -1;
		}
		
		void GUIComponent::setEnable(sbool bEnable)
		{
			m_bEnable = bEnable;
		}
		
        bool GUIComponent::isEnable() const
		{
			return m_bEnable;
		}
        
		void GUIComponent::addEventListner(EventListener* eventListner)
		{
		}
		
		void GUIComponent::addFocusListner(IFocusListner* focusListner)
		{
		}
		
		void GUIComponent::setSize(si32 width, si32 height)
		{
			m_Size.x = width;
			m_Size.y = height;
			m_Bounds.width = width;
			m_Bounds.height = height;
		}
		
        void GUIComponent::setScale(sf32 sx, sf32 sy)
		{
            m_LocalTransform.col1.x = sx;
            m_LocalTransform.col2.y = sy;
		}
        
		void GUIComponent::setPosition(si32 x, si32 y)
		{
			m_Position.x = x;
			m_Position.y = y;
			//m_Bounds.x = m_X;
			//m_Bounds.y = m_Y;
			m_LocalTransform.col3.x = (sf32)m_Position.x;
			m_LocalTransform.col3.y = (sf32)m_Position.y;
		}
		
		void GUIComponent::setTransform(Matrix3f& transform)
		{
			m_LocalTransform = transform;
		}
		
		void GUIComponent::setColor(const Color4f& color)
		{
			m_Color = color;
			for( int i=0; i<16; i+=4)
			{
				m_Colors[i]   = m_Color.r;
				m_Colors[i+1] = m_Color.g;
				m_Colors[i+2] = m_Color.b;
				m_Colors[i+3] = m_Color.a;
			}
		}
		
		
		void GUIComponent::setTransparency(sf32 transparency)
		{
			if( transparency < 0.0f || transparency > 1.0f )
				return;
			
			m_Transparency = transparency;
			
			for( int i=0; i<16; i+=4)
			{
				m_Colors[i+3] = ( 1.0f - m_Transparency );
			}
		}
		
	}
}

