//
//  GUIScrollView.h
//  Optiks
//
//  Created by Prashant on 11/02/14.
//
//

#ifndef __Optiks__GUIScrollView__
#define __Optiks__GUIScrollView__


#include "GUIContainer.h"

namespace spider3d
{
	
	namespace gui
	{
		class GUIScrollBar;
		class GUIPanel;
		
		class GUIScrollView : public GUIContainer
		{
		public:
			
			GUIScrollView(GUIContainer* parent, si32 width, si32 height);
			
			virtual void render();
			
			virtual sbool OnEvent( Event& );
			
            virtual void createGeometry();
            
            void update();

            void setPeriod( int period )
            {
                m_Period = period;
            }
            
            void setMeanPosition( int meanPosition)
            {
                m_MeanPosition = meanPosition;
            }
            
            int getPeriod() const
            {
                return m_Period;
            }
            
            int getMeanPosition() const
            {
                return m_MeanPosition;
            }
            
            int getVirtualPosition() const
            {
                return m_VirtualPosition;
            }
            
            void setBoundaryLimits(int leftLimit, int rightLimit )
            {
                m_LeftBoundaryLimit  = leftLimit;
                m_RightBoundaryLimit = rightLimit;
            }
            
		protected:
			
			void scrollComponents(si32 dx, si32 dy);
			
            void moveComponentsWithVelocity(const Vector2i& velocity);

            int m_PreviousX, m_PreviousY;
            
            Vector2f m_PanPosition;
            Vector2f m_RestoringForce;
            float    m_VelocityDampingFactor;
            float    m_RestoringForceFactor;
            
            int      m_MeanPosition;
            int      m_Period;
            int      m_VirtualPosition;
            float    m_RelativePosition;
            float    m_AccDisplacement;
            static Texture* m_pTexture;
            
            int      m_LeftBoundaryLimit, m_RightBoundaryLimit;
		};
		
	}
}

#endif /* defined(__Optiks__GUIScrollView__) */
