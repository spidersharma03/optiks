//
//  GUILabel.h
//  RTSGame
//
//  Created by Arshad on 12/06/13.
//  Copyright (c) 2013 Sourcebits. All rights reserved.
//

#ifndef RTSGame_GUILabel_h
#define RTSGame_GUILabel_h


#include "GUIComponent.h"
#include <iostream>

using namespace std;

namespace spider3d
{
	namespace gui
	{
        class IActionListener;
        
		class GUILabel: public GUIComponent
		{
            
		public:
            
			GUILabel(GUIContainer*, int width, int height);
			
			virtual void render();
			
			virtual void update();
			
            virtual bool OnEvent( Event& event);
            
            void SetText(const string& text);
            const string& GetText() const;
                        
            void SetTextColor(const Color4f& color);
            
            Color4f GetTextColor() const;

            void SetTextScale(float scale);
            
            enum TextAlignment
            {
                TextAlignment_Left = 0,
                TextAlignment_Top = 0,
                TextAlignment_Center = 1,
                TextAlignment_Right = 2,
                TextAlignment_Bottom = 2
            };
            
            void SetHorizontalAlignment(TextAlignment alignment);
            
            void SetVerticalAlignment(TextAlignment alignment);
            
            void SetPadding(float horizontal = 0.0f, float vertical = 0.0f);
            
            void SetVisible( bool bVisible );
            
            bool IsVisible();
            
		private:
            
            virtual void AdjustForAlignment(Vector2f& pos, float width, float height);
            
            virtual void AdjustForPadding(Vector2f& pos);
            
			virtual void createGeometry();
            
            string m_LabelText;
            
            Color4f m_TextColor;
            
            float m_TextScale;
            
            TextAlignment m_HorizontalAlignment;
            TextAlignment m_VerticalAlignment;
            
            float m_HorzontalPadding;
            float m_VerticalPadding;
            
            static Texture* m_pTexture;
            
            bool m_bVisible;
            
		};
	}
}


#endif
