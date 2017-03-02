//
//  GUITextField.h
//  Optiks
//
//  Created by Prashant Sharma on 18/11/13.
//
//

#ifndef __Optiks__GUITextField__
#define __Optiks__GUITextField__

#include <iostream>
#include "GUIComponent.h"

namespace spider3d
{
	namespace gui
	{
        class GUILabel;
        
		class GUITextField: public GUIComponent
		{
		public:
			GUITextField(si32 width, si32 height);
			
			GUITextField(GUIContainer* parent, si32 width, si32 height);
			
			virtual void            render();
			virtual void            update();
			virtual sbool           OnEvent( Event& );
			
            void                    setText(const string& text);
            void                    insertChar(wchar_t character, int index);
            void                    removeChar(wchar_t character, int index);
            void                    pushChar(wchar_t character);
            void                    popChar();
            string                  getText() const;
            
			inline sbool isPressed()
			{
				return m_bPressed;
			}
		protected:
			virtual void createGeometry();
            static Texture* m_pTexture;
            string          m_Text;
            GUILabel        *m_pLabel;
		};
	}
}

#endif /* defined(__Optiks__GUITextField__) */
