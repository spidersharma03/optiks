//
//  GUISlider.h
//  Optiks
//
//  Created by Prashant Sharma on 03/04/13.
//

#ifndef _GUI_SLIDER_H_
#define _GUI_SLIDER_H_

#include "GUIComponent.h"

namespace spider3d
{
	namespace gui 
	{

        class GUIPanel;
        class GUIButton;
        
class GUISlider : public GUIComponent
{
public:
    GUISlider(si32 width, si32 height);
    
    GUISlider(GUIContainer* parent, si32 width, si32 height);
    
    virtual void render();
    
    virtual void update();
    
    virtual sbool OnEvent( Event& );

    void setValue( float val );

    float getValue() const;

    void setMinValue( float minVal );
    
    void setMaxValue( float maxVal );
    
protected:
    
    virtual void createGeometry();
    
protected:
    GUIPanel* m_pSliderPanel;
    GUIButton* m_pSliderButton;
    
    float m_Value;
    float m_MaxValue, m_MinValue;
    // Relative button position
    Vector2i m_SliderButtonPos;
    
    static Texture* m_pTexture;
};
        
    }
}

#endif
