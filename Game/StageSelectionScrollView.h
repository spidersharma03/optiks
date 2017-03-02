//
//  StageSelectionScrollView.h
//  Optiks
//
//  Created by Prashant on 24/02/14.
//
//

#ifndef __Optiks__StageSelectionScrollView__
#define __Optiks__StageSelectionScrollView__

#include <iostream>
#include "GUIScrollView.h"

using namespace spider3d::gui;

class StageSelectionScrollView : public GUIScrollView
{
public:
    
    StageSelectionScrollView(GUIContainer* parent, si32 width, si32 height);
    
    virtual void render();
    
    virtual sbool OnEvent( Event& );
    
    void update();
    
protected:
    
};

#endif /* defined(__Optiks__StageSelectionScrollView__) */
