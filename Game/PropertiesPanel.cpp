//
//  PropertiesPanel.cpp
//  Optiks
//
//  Created by Prashant Sharma on 06/12/13.
//
//

#include "PropertiesPanel.h"
#include "Game.h"
#include "GameEditorHUDController.h"

BasicPropertiesPanel::BasicPropertiesPanel(EditorHUDController* pEditorHUDController)
{
    m_pEditorHUDController = pEditorHUDController;
    initGUI();
}

BasicPropertiesPanel::~BasicPropertiesPanel()
{
    
}

void BasicPropertiesPanel::setPositionLabel(const Vector2f& pos)
{
    char buffer[32];
    sprintf(buffer, "X:%d  Y:%d", (int)(pos.x/BASIC_ELEMENT_SIZE), (int)(pos.y/BASIC_ELEMENT_SIZE));
    m_pPositionValueLabel->SetTextScale(0.3f);
    m_pPositionValueLabel->SetText(buffer);
}

void BasicPropertiesPanel::setRotationLabel(float rot)
{
    char buffer[32];
    sprintf(buffer, "%d", (int)rot);
    m_pRotationValueLabel->SetTextScale(0.3f);
    m_pRotationValueLabel->SetText(buffer);
}

void BasicPropertiesPanel::setID( int id )
{
    char buffer[10];
    sprintf(buffer, "%d", (int)id);
    m_pIDValueLabel->SetTextScale(0.3f);
    m_pIDValueLabel->SetText(buffer);
}

void BasicPropertiesPanel::setFixedStatusForCheckBox(bool bFixed)
{
    m_pFixedStatusCheckBox->setCheckEnable(bFixed);
}

void BasicPropertiesPanel::initGUI()
{
    m_pPropertiesGuiPanel = new GUIPanel(Game::getInstance()->getGUIManager(), 150, 250);
    m_pPropertiesGuiPanel->setPosition(200, 10);
    m_pPropertiesGuiPanel->setTransparency(0.7f);
    int yOffset = 200;
    int xOffset = 10;
    m_pPositionTextLabel  = new GUILabel(m_pPropertiesGuiPanel, 60,20);
    m_pPositionTextLabel->setPosition(xOffset, yOffset);
    m_pPositionTextLabel->SetText("Position");
    m_pPositionTextLabel->SetTextScale(0.3);
    
    m_pPositionValueLabel = new GUILabel(m_pPropertiesGuiPanel, 60,20);
    m_pPositionValueLabel->setPosition(xOffset+70, yOffset);
    m_pPositionValueLabel->SetTextScale(0.3);

    m_pRotationTextLabel  = new GUILabel(m_pPropertiesGuiPanel, 60,20);
    m_pRotationTextLabel->SetText("Rotation");
    m_pRotationTextLabel->setPosition(xOffset, yOffset-30);
    m_pRotationTextLabel->SetTextScale(0.3);

    m_pRotationValueLabel = new GUILabel(m_pPropertiesGuiPanel, 60,20);
    m_pRotationValueLabel->setPosition(xOffset+70, yOffset-30);
    m_pRotationValueLabel->SetTextScale(0.3f);
    
    m_pFixedStatusTextLabel = new GUILabel(m_pPropertiesGuiPanel, 60,20);
    m_pFixedStatusTextLabel->SetText("Fixed");
    m_pFixedStatusTextLabel->setPosition(xOffset, yOffset-60);
    m_pFixedStatusTextLabel->SetTextScale(0.3f);
    
    m_pIDTextLabel = new GUILabel(m_pPropertiesGuiPanel, 60,20);
    m_pIDTextLabel->SetText("ID");
    m_pIDTextLabel->setPosition(xOffset, yOffset-90);
    m_pIDTextLabel->SetTextScale(0.3f);
    
    m_pIDValueLabel = new GUILabel(m_pPropertiesGuiPanel, 60,20);
    m_pIDValueLabel->setPosition(xOffset+70, yOffset-90);
    m_pIDValueLabel->SetTextScale(0.3);
    
    m_pFixedStatusCheckBox = new GUICheckBox(m_pPropertiesGuiPanel);
    m_pFixedStatusCheckBox->setPosition(xOffset+70, yOffset-60);
    m_pFixedStatusCheckBox->setTag(100);
    m_pFixedStatusCheckBox->setActionListener(m_pEditorHUDController);
    
    m_pRedColorValueSlider   = new GUISlider(m_pPropertiesGuiPanel,100,20);
    m_pRedColorValueSlider->setTag(1000);
    m_pRedColorValueSlider->setPosition(20, 80);
    m_pGreenColorValueSlider = new GUISlider(m_pPropertiesGuiPanel,100,20);
    m_pGreenColorValueSlider->setTag(1001);
    m_pGreenColorValueSlider->setPosition(20, 50);
    m_pBlueColorValueSlider  = new GUISlider(m_pPropertiesGuiPanel,100,20);
    m_pBlueColorValueSlider->setTag(1002);
    m_pBlueColorValueSlider->setPosition(20, 20);
    m_pRedColorValueSlider->setActionListener(m_pEditorHUDController);
    m_pGreenColorValueSlider->setActionListener(m_pEditorHUDController);
    m_pBlueColorValueSlider->setActionListener(m_pEditorHUDController);
}

void BasicPropertiesPanel::showGUI() const
{
    m_pPropertiesGuiPanel->setVisible(true);
}

void BasicPropertiesPanel::hideGUI() const
{
    m_pPropertiesGuiPanel->setVisible(false);
}


SinkPropertiesPanel::SinkPropertiesPanel(EditorHUDController* pEditorHUDController):SourcePropertiesPanel(pEditorHUDController)
{
}

SinkPropertiesPanel::~SinkPropertiesPanel()
{
    
}

void SinkPropertiesPanel::setRayTerminateCheck(bool bRayTerminate)
{
    m_bRayTerminate = bRayTerminate;
}

void SinkPropertiesPanel::initGUI()
{
//    m_pRayTerminateCheckBox = new GUICheckBox(m_pPropertiesGuiPanel);
//    m_pRayTerminateCheckBox->setPosition(80, 140);
//
//    m_pRayTerminateTextLabel = new GUILabel(m_pPropertiesGuiPanel, 60,20);
//    m_pRayTerminateTextLabel->setPosition(10, 140);
//    m_pRayTerminateTextLabel->SetText("RayExit");
//    m_pRayTerminateTextLabel->SetTextScale(0.75f);
}


SourcePropertiesPanel::SourcePropertiesPanel(EditorHUDController* pEditorHUDController):BasicPropertiesPanel(pEditorHUDController)
{
}

SourcePropertiesPanel::~SourcePropertiesPanel()
{
    
}

void BasicPropertiesPanel::setRedSliderValue(float value)
{
    m_pRedColorValueSlider->setValue(value);
}

void BasicPropertiesPanel::setGreenSliderValue(float value)
{
    m_pGreenColorValueSlider->setValue(value);
}

void BasicPropertiesPanel::setBlueSliderValue(float value)
{
    m_pBlueColorValueSlider->setValue(value);
}
