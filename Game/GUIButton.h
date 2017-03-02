/*
 *  GUIButton.h
 *  testCpp
 *
 *  Created by Prashant on 30/03/11.
 *  Copyright 2011 Sourcebits Technologies. All rights reserved.
 *
 */

#ifndef _GUI_BUTTON_H
#define _GUI_BUTTON_H

#include "GUIComponent.h"



namespace spider3d
{
	namespace gui 
	{
        
        enum TextAlignment
        {
            TextAlignment_Left = 0,
            TextAlignment_Top = 0,
            TextAlignment_Center = 1,
            TextAlignment_Right = 2,
            TextAlignment_Bottom = 2
        };
        
        class GUIPanel;
        
		class GUIButton: public GUIComponent
		{
		public:
			GUIButton(si32 width, si32 height);
			
			GUIButton(GUIContainer* parent, si32 width, si32 height);
			
			virtual void                render();
			virtual void                update();
			virtual sbool               OnEvent( Event& );
            void                        DrawTitle();
            void                        setTitle(const string& title);
            
            void                        setHorizontalAlignment(TextAlignment alignment);
            void                        setVerticalAlignment(TextAlignment alignment);
            void                        setPadding(float horizontal, float vertical);
            void                        adjustForPadding(Vector2f& pos);
            void                        adjustForAlignment(Vector2f& pos, float width, float height);
            void                        setFontScale( float fontScale );
            
            void                        setTexture( Texture* pTexture );
            
			inline sbool isPressed()
			{
				return m_bPressed;
			}
		protected:
			virtual void                createGeometry();
            
            TextAlignment               m_HorizontalAlignment;
            TextAlignment               m_VerticalAlignment;
            float                       m_HorzontalPadding;
            float                       m_VerticalPadding;
            Texture*                    m_pTexture;
            string                      m_Title;
            float                       m_FontScale;
            
		};
	}
}

#endif