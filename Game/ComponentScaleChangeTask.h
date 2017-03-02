//
//  ComponentScaleChangeTask.h
//  Optiks
//
//  Created by Prashant on 08/03/14.
//
//

#ifndef __Optiks__ComponentScaleChangeTask__
#define __Optiks__ComponentScaleChangeTask__

#include <iostream>
#include "TaskBase.h"
#include "Color4.h"

USE_NAMESPACE_DIRAC2D

namespace spider3d {
    namespace gui{
        class GUIComponent;
    }
}

using namespace spider3d::gui;

class ComponentScaleChangeTask : public TaskBase
{
public:
    
    ComponentScaleChangeTask( GUIComponent* pGUIComponent, float startScale, float endScale, float lerpTime );
    
    ~ComponentScaleChangeTask();
    
    virtual void Update(float deltaSeconds);
    
private:
    GUIComponent                *m_pGUIComponent;
    double                      m_Time;
    double                      m_StartTime;
    
    float                       m_LerpTime;
    float                       m_StartScale;
    float                       m_EndScale;
};

#endif /* defined(__Optiks__ComponentScaleChangeTask__) */
