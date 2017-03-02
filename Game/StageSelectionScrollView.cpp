//
//  StageSelectionScrollView.cpp
//  Optiks
//
//  Created by Prashant on 24/02/14.
//
//

#include "StageSelectionScrollView.h"


StageSelectionScrollView::StageSelectionScrollView(GUIContainer* parent, si32 width, si32 height):
GUIScrollView(parent, width, height )
{
}

void StageSelectionScrollView::render()
{
    GUIScrollView::render();
}

sbool StageSelectionScrollView::OnEvent( Event& event)
{
    return GUIScrollView::OnEvent(event);
}

void StageSelectionScrollView::update()
{
    GUIScrollView::update();
}