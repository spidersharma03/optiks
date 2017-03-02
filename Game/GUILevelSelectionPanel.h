//
//  GUIStageSelectionPanel.h
//  Optiks
//
//  Created by Prashant on 08/03/14.
//
//

#ifndef __Optiks__GUIStageSelectionPanel__
#define __Optiks__GUIStageSelectionPanel__

#include <iostream>
#include "GUIPanel.h"
#include "Ray.h"

using namespace spider3d::gui;

class ParticleSystem;
class ParticleAnimator;

class GUILevelSelectionPanel: public GUIPanel
{
public:
    GUILevelSelectionPanel(si32 width, si32 height);
    
    GUILevelSelectionPanel(GUIContainer* parent, si32 width, si32 height);
    
    virtual void update();
    
    virtual void render();
    
    virtual sbool OnEvent( Event& );
  
    void          renderRays( float width );
  
    void          setRayAtIndexEnable( int index, bool bEnable);
    
    void          setTitle( const string& title )
    {
        m_Title = title;
    }
    
private:
    
    void   initParticleSystem();

    void   pushRayDataToRayVertexBuffer(float rayWidth, Ray& ray, int rayCount);

    float         m_pAllRayVertices[6*2*NUM_LEVELS_PER_STAGE];
    float         m_pAllRayColors[6*4*NUM_LEVELS_PER_STAGE];
    float         m_pAllRayTexCoords[6*2*NUM_LEVELS_PER_STAGE ];
    
    bool          m_bRayEnable[NUM_LEVELS_PER_STAGE];
    
    ParticleSystem      *m_pParticleSystem;
    ParticleAnimator    *m_pParticleAnimator;
    
    string               m_Title;
};


#endif /* defined(__Optiks__GUIStageSelectionPanel__) */
