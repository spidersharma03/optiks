//
//  GUIStageSelectionButton.h
//  Optiks
//
//  Created by Prashant on 04/03/14.
//
//

#ifndef __Optiks__GUIStageSelectionButton__
#define __Optiks__GUIStageSelectionButton__

#include <iostream>

#include "GUIButton.h"
#include <vector>
#include "Ray.h"

using namespace std;
using namespace spider3d::gui;

class Shape2D;

class GUIStageSelectionButton : public GUIButton
{
public:
    
    GUIStageSelectionButton(si32 width, si32 height);
    
    GUIStageSelectionButton(GUIContainer* parent, si32 width, si32 height);
    
    virtual void                render();
    virtual void                update();
    virtual sbool               OnEvent( Event& );
    
    vector<Ray>&                getAllRays()
    {
        return m_vecInnerGlowRays;
    }
    
    void                        renderRays(float rayWidth);
    
    void                        setButtonState(int state);
    
private:
    
    void                        rayTraceForSinkGlow();
    void                        pushRayDataToRayVertexBuffer(float rayWidth, Ray& ray, int rayCount);
    
private:
    
    Ray                     m_SourceRay;
    vector<Ray>             m_vecInnerGlowRays;
    int                     m_NumInterReflections;
    float                   m_RotAngle;
    float                   m_DisableStateRadius, m_EnableStateRadius;
    
    Shape2D                 *m_pShape2D;
    
    float                   *m_pAllRayVertices;
    float                   *m_pAllRayColors;
    float                   *m_pAllRayTexCoords;
    
    int                     m_RenderState;
};


#endif /* defined(__Optiks__GUIStageSelectionButton__) */
