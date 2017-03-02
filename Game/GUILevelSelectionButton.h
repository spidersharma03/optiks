//
//  GUILevelSelectionButton.h
//  Optiks
//
//  Created by Prashant on 03/03/14.
//
//

#ifndef __Optiks__GUILevelSelectionButton__
#define __Optiks__GUILevelSelectionButton__

#include "GUIButton.h"
#include <vector>
#include "Ray.h"

using namespace std;
using namespace spider3d::gui;

class Shape2D;

class GUILevelSelectionButton : public GUIButton
{
public:
    
    GUILevelSelectionButton(si32 width, si32 height);
    
    GUILevelSelectionButton(GUIContainer* parent, si32 width, si32 height);
    
    virtual void                render();
    virtual void                update();
    virtual sbool               OnEvent( Event& );
    
    void                        renderRays(float rayWidth = 20.0f, const Color4f& addColor = Color4f(0.0f,0.0f,0.0f,0.0f));
    
    vector<Ray>&                getAllRays()
    {
        return m_vecInnerGlowRays;
    }
    
    void                        setButtonState(int state);

private:
    
    void                        rayTraceForSinkGlow();
    void                        pushRayDataToRayVertexBuffer(float rayWidth, Ray& ray, int rayCount, const Color4f& addColor);

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

#endif /* defined(__Optiks__GUILevelSelectionButton__) */
