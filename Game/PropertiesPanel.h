//
//  PropertiesPanel.h
//  Optiks
//
//  Created by Prashant Sharma on 06/12/13.
//
//

#ifndef __Optiks__PropertiesPanel__
#define __Optiks__PropertiesPanel__

#include <iostream>
#include "GUI.h"

using namespace spider3d::gui;

class EditorHUDController;

class BasicPropertiesPanel
{
public:
    BasicPropertiesPanel(EditorHUDController*);
    
    virtual ~BasicPropertiesPanel();

    void                    showGUI() const;
    void                    hideGUI() const;
    
    void                    setPositionLabel(const Vector2f& pos);
    void                    setRotationLabel(float rot);
    void                    setFixedStatusForCheckBox(bool bFixed);
    void                    setID( int id );

    void                    setRedSliderValue(float value);
    void                    setGreenSliderValue(float value);
    void                    setBlueSliderValue(float value);
    
protected:
    
    void                    initGUI();

protected:
    
    GUIPanel                *m_pPropertiesGuiPanel;
    GUILabel                *m_pPositionTextLabel;
    GUILabel                *m_pPositionValueLabel;
    GUILabel                *m_pRotationTextLabel;
    GUILabel                *m_pRotationValueLabel;

    GUILabel                *m_pFixedStatusTextLabel;
    GUILabel                *m_pIDTextLabel;
    GUILabel                *m_pIDValueLabel;

    GUICheckBox             *m_pFixedStatusCheckBox;
    
    EditorHUDController     *m_pEditorHUDController;
    
protected:
    
    GUISlider               *m_pRedColorValueSlider;
    GUISlider               *m_pGreenColorValueSlider;
    GUISlider               *m_pBlueColorValueSlider;
};

class SourcePropertiesPanel : public BasicPropertiesPanel
{
public:
    SourcePropertiesPanel(EditorHUDController*);
    
    virtual ~SourcePropertiesPanel();

};

class SinkPropertiesPanel : public SourcePropertiesPanel
{
public:
    SinkPropertiesPanel(EditorHUDController*);
    
    virtual ~SinkPropertiesPanel();
    
    void                    initGUI();
    void                    setRayTerminateCheck(bool bRayTerminate);
    
protected:
    
protected:
    GUICheckBox             *m_pRayTerminateCheckBox;
    GUILabel                *m_pRayTerminateTextLabel;
    bool                    m_bRayTerminate;
};


#endif /* defined(__Optiks__PropertiesPanel__) */
