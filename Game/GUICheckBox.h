//
//  GUICheckBox.h
//  Optiks
//
//  Created by Prashant Sharma on 06/12/13.
//
//

#ifndef __Optiks__GUICheckBox__
#define __Optiks__GUICheckBox__

#include <iostream>
#include "GUIComponent.h"



namespace spider3d
{
	namespace gui
	{
        #define CHECK_BOX_SIZE 16
        
		class GUICheckBox: public GUIComponent
		{
		public:
			GUICheckBox();
			
			GUICheckBox(GUIContainer* parent);
			
			virtual void                render();
			virtual void                update();
			virtual sbool               OnEvent( Event& );
             
			inline sbool isPressed()
			{
				return m_bPressed;
			}
            
            inline sbool isChecked() const
            {
                return m_bCheckOn;
            }
            
            void setCheckEnable( bool bCheckEnable );
            
		protected:
			virtual void                createGeometry();
            
            static Texture*             m_pTexture;
            sbool                       m_bCheckOn;
		};
	}
}

#endif /* defined(__Optiks__GUICheckBox__) */
