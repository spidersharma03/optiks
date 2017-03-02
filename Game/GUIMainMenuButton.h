//
//  GUIMainMenuButton.h
//  Optiks
//
//  Created by Prashant on 25/03/14.
//
//

#ifndef __Optiks__GUIMainMenuButton__
#define __Optiks__GUIMainMenuButton__

#include <iostream>
#include "GUIButton.h"
#include <vector>
#include "Ray.h"

using namespace std;
using namespace spider3d::gui;

class Shape2D;

class GUIMainMenuButton : public GUIButton
{
public:
    
    GUIMainMenuButton(si32 width, si32 height);
    
    GUIMainMenuButton(GUIContainer* parent, si32 width, si32 height);
    
    virtual void                render();
    virtual void                update();
    virtual sbool               OnEvent( Event& );
    
    void                        renderRays(float rayWidth = 20.0f);
    
    vector<Ray>&                getAllRays()
    {
        return m_vecInnerGlowRays;
    }
    
private:
    
    void                        rayTraceForSinkGlow();
    void                        pushRayDataToRayVertexBuffer(float rayWidth, Ray& ray, int rayCount);
    
private:
    
    Ray                     m_SourceRay;
    vector<Ray>             m_vecInnerGlowRays;
    int                     m_NumInterReflections;
    float                   m_RotAngle;
    float                   m_DisableStateRadius, m_EnableStateRadius;
    
    float                   *m_pAllRayVertices;
    float                   *m_pAllRayColors;
    float                   *m_pAllRayTexCoords;
};


#endif /* defined(__Optiks__GUIMainMenuButton__) */
