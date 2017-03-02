//
//  GUIChapterSelectionButton.h
//  Optiks
//
//  Created by Prashant on 09/03/14.
//
//

#ifndef __Optiks__GUIChapterSelectionButton__
#define __Optiks__GUIChapterSelectionButton__

#include <iostream>
#include "GUIButton.h"
#include <vector>
#include "Ray.h"

using namespace std;
using namespace spider3d::gui;

class Shape2D;
class ParticleSystem;
class ParticleAnimator;

class GUIChapterSelectionButton : public GUIButton
{
public:
    
    GUIChapterSelectionButton(si32 width, si32 height);
    
    GUIChapterSelectionButton(GUIContainer* parent, si32 width, si32 height);
    
    virtual void                render();
    virtual void                update();
    virtual sbool               OnEvent( Event& );
    
    void                        setRayColor( const Color4f& rayColor );
    
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
    
    Shape2D                 *m_pShape2D1;
    Shape2D                 *m_pShape2D2;
    
    float                   *m_pAllRayVertices;
    float                   *m_pAllRayColors;
    float                   *m_pAllRayTexCoords;
    
    
    ParticleSystem      *m_pParticleSystemOrbit;
    ParticleAnimator    *m_pParticleAnimatorOrbit;
};

#endif /* defined(__Optiks__GUIChapterSelectionButton__) */
