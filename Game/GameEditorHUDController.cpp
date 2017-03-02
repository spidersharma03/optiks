//
//  EditorHUDController.cpp
//  Optiks
//
//  Created by Prashant Sharma on 02/11/13.
//
//

#include "GameEditorHUDController.h"
#include "Game.h"
#include "GameEditorState.h"
#include "GameMenuState.h"
#include "EditorObjectCreateState.h"
#include "EditorOperationStates.h"
#include "Scene.h"
#include "ShapeGameObject.h"
#include "SinkGameObject.h"
#include "SourceGameObject.h"
#include "PropertiesPanel.h"
#include "GameChapterSelectionState.h"
#include "GameLevelSelectionState.h"

BasicPropertiesPanel* EditorHUDController::s_pBasicShapePropertiesPanel = NULL;
//SinkPropertiesPanel*  EditorHUDController::s_pSinkPropertiesPanel = NULL;
//SourcePropertiesPanel*  EditorHUDController::s_pSourcePropertiesPanel = NULL;

EditorHUDController::EditorHUDController(GameEditorState* pGameEditorState)
:m_pGameEditorState(pGameEditorState)
{
    Game::getInstance()->getGUIManager()->setActionListener(this);
    
    createShapeSelectionView();
    
    createShapeSelectionModeView();
    
    createSaveDialog();
    
    createLoadDialog();
    
    createPropertiesPanels();
}

void EditorHUDController::createPropertiesPanels()
{
    s_pBasicShapePropertiesPanel = new BasicPropertiesPanel(this);
    //s_pSinkPropertiesPanel       = new SinkPropertiesPanel(this);
    //s_pSourcePropertiesPanel     = new SourcePropertiesPanel(this);
    
    s_pBasicShapePropertiesPanel->hideGUI();
    //s_pSinkPropertiesPanel->hideGUI();
    //s_pSourcePropertiesPanel->hideGUI();
}

void EditorHUDController::createSaveDialog()
{
    m_pSaveDialogPanel = new GUIPanel(Game::getInstance()->getGUIManager(),250,120);
    m_pSaveDialogPanel->setPosition(500, 300);
    m_pLevelNameTextField = new GUITextField(m_pSaveDialogPanel,200,30);
    m_pLevelNameTextField->setActionListener(this);
    m_pLevelNameTextField->setPosition(30, 50);
    m_pLevelNameTextField->setTag(22);
    m_pSaveButton = new GUIButton(m_pSaveDialogPanel,50,30);
    m_pSaveButton->setTitle("Save");
    m_pSaveButton->setTag(20);
    m_pSaveButton->setPosition(30, 10);
    m_pCancelButton = new GUIButton(m_pSaveDialogPanel,80,30);
    m_pCancelButton->setTitle("Cancel");
    m_pCancelButton->setTag(21);
    m_pCancelButton->setPosition(160, 10);
    m_pCancelButton->setActionListener(this);
    m_pSaveButton->setActionListener(this);
    hideSaveDialog();
}

void EditorHUDController::createLoadDialog()
{
    m_pLoadDialogPanel = new GUIPanel(Game::getInstance()->getGUIManager(),250,120);
    m_pLoadDialogPanel->setPosition(500, 300);
    m_pLoadLevelNameTextField = new GUITextField(m_pLoadDialogPanel,200,30);
    m_pLoadLevelNameTextField->setActionListener(this);
    m_pLoadLevelNameTextField->setPosition(30, 50);
    m_pLoadLevelNameTextField->setTag(32);
    m_pLoadOkButton = new GUIButton(m_pLoadDialogPanel,50,30);
    m_pLoadOkButton->setTitle("Load");
    m_pLoadOkButton->setTag(30);
    m_pLoadOkButton->setPosition(30, 10);
    m_pLoadCancelButton = new GUIButton(m_pLoadDialogPanel,80,30);
    m_pLoadCancelButton->setTitle("Cancel");
    m_pLoadCancelButton->setTag(31);
    m_pLoadCancelButton->setPosition(160, 10);
    m_pLoadCancelButton->setActionListener(this);
    m_pLoadOkButton->setActionListener(this);
    hideLoadDialog();
}

void EditorHUDController::showLoadDialog()
{
    m_pLoadDialogPanel->setVisible(true);
}

void EditorHUDController::hideLoadDialog()
{
    m_pLoadDialogPanel->setVisible(false);
}

void EditorHUDController::showSaveDialog()
{
    m_pSaveDialogPanel->setVisible(true);
}

void EditorHUDController::hideSaveDialog()
{
    m_pSaveDialogPanel->setVisible(false);
}

void EditorHUDController::hideEditorUI()
{
    m_pShapeSelectionBasePanel->setVisible(false);
    m_pShapeSelectionModeView->setVisible(false);
    hideSaveDialog();
    hideLoadDialog();
    m_pMainMenuSelectionButton->setVisible(false);
    s_pBasicShapePropertiesPanel->hideGUI();
    //s_pSinkPropertiesPanel->hideGUI();
}

void EditorHUDController::showEditorUI()
{
    m_pShapeSelectionBasePanel->setVisible(true);
    m_pShapeSelectionModeView->setVisible(true);
    m_pMainMenuSelectionButton->setVisible(true);
}

void EditorHUDController::createShapeSelectionView()
{
    m_pShapeSelectionBasePanel = new GUIPanel(Game::getInstance()->getGUIManager(), 70, 500 );
    m_pShapeSelectionBasePanel->setPosition(0, 140);
    m_pShapeSelectionBasePanel->setTransparency(0.7);

    float x = 20.0f, y = 20.0f;
    float dy = 50;
    
    string names[] = {"1","2", "3", "4", "5", "6", "7", "8", "9"};
    for( int i=0; i< 9; i++ )
    {
        GUIButton* pButton = new GUIButton(m_pShapeSelectionBasePanel, 40,40);
        pButton->setFontScale(0.35f);
        pButton->setTag(i);
        pButton->setActionListener(this);
        m_vecButtons.push_back(pButton);
        pButton->setName(names[i]);
        pButton->setPosition(x,y);
        y += dy;
    }
    m_vecButtons[0]->setTitle("RECT");
    m_vecButtons[1]->setTitle("SOURCE");
    m_vecButtons[2]->setTitle("SINK");
    m_vecButtons[3]->setTitle("MARKER");
    m_vecButtons[4]->setTitle("GLASS");
    m_vecButtons[5]->setTitle("GEM");
    m_vecButtons[6]->setTitle("BLOCK");
    m_vecButtons[7]->setTitle("PORTAL");
    m_vecButtons[8]->setTitle("FILTER");
    
    for( int i=0; i<m_vecButtons.size(); i++ )
    {
        m_vecButtons[i]->setTexture((Game::getInstance()->m_pDefaultGradientTexture));
    }
}

void EditorHUDController::createShapeSelectionModeView()
{
    m_pShapeSelectionModeView = new GUIPanel(Game::getInstance()->getGUIManager(), 120, 40 );
    m_pShapeSelectionModeView->setTransparency(0.7);
    
    m_pShapeSelectionModeView->setPosition(0, 650);
    float x = 10.0f, y = 10.0f;
    GUIButton* m_pSelectObjectsButton = new GUIButton(m_pShapeSelectionModeView, 25,25);
    m_vecButtons.push_back(m_pSelectObjectsButton);
    m_pSelectObjectsButton->setPosition(x,y);
    m_pSelectObjectsButton->setActionListener(this);
    m_pSelectObjectsButton->setTag(10);
    m_pSelectObjectsButton->setTexture((Game::getInstance()->m_pDefaultGradientTexture));
    
    int horspace = 40;
    x += horspace;
    GUIButton* m_pMoveObjectsButton   = new GUIButton(m_pShapeSelectionModeView, 25,25);
    m_vecButtons.push_back(m_pMoveObjectsButton);
    m_pMoveObjectsButton->setPosition(x,y);
    m_pMoveObjectsButton->setActionListener(this);
    m_pMoveObjectsButton->setTag(11);
    m_pMoveObjectsButton->setTexture((Game::getInstance()->m_pDefaultGradientTexture));
    
    x += horspace;
    GUIButton* m_pRotateObjectsButton = new GUIButton(m_pShapeSelectionModeView, 25,25);
    m_vecButtons.push_back(m_pRotateObjectsButton);
    m_pRotateObjectsButton->setPosition(x,y);
    m_pRotateObjectsButton->setActionListener(this);
    m_pRotateObjectsButton->setTag(12);
    m_pRotateObjectsButton->setTexture((Game::getInstance()->m_pDefaultGradientTexture));
    
    m_pMainMenuSelectionButton = new GUIButton(Game::getInstance()->getGUIManager(), 50, 25 );
    m_pMainMenuSelectionButton->setPosition(600, 710);
    m_pMainMenuSelectionButton->setTag(13);
    m_pMainMenuSelectionButton->setActionListener(this);
    m_pMainMenuSelectionButton->setTitle("Menu");
    m_pMainMenuSelectionButton->setTexture((Game::getInstance()->m_pDefaultGradientTexture));
}

void EditorHUDController::mousePressed(GUIComponent* pComponent, int x, int y)
{
    if( pComponent )
    {
        switch (pComponent->getTag())
        {
            // GAME OBJECT CREATION BUTTONS
            case 0: // RECT SHAPE CREATE
                m_pGameEditorState->m_pEditorStateMachine->ChangeState(EditorRectShapeCreateState::getInstance());
                break;
            
            case 1: //  LIGHT SOURCE CREATE
                m_pGameEditorState->m_pEditorStateMachine->ChangeState(EditorSourceCreateState::getInstance());
                break;
            
            case 2: //  SINK CREATE
                m_pGameEditorState->m_pEditorStateMachine->ChangeState(EditorSinkCreateState::getInstance());
                break;
            case 3: //  PLACEMENT MARKER CREATE
                m_pGameEditorState->m_pEditorStateMachine->ChangeState(EditorPlacementMarkerCreateState::getInstance());
                break;
            case 4: //  TRANSMISSIVE RECT SHAPE CREATE
                m_pGameEditorState->m_pEditorStateMachine->ChangeState(EditorTransmissiveRectShapeCreateState::getInstance());
                break;
            case 5: //  REFRACTIVE RECT SHAPE CREATE
                m_pGameEditorState->m_pEditorStateMachine->ChangeState(EditorRefractiveRectShapeCreateState::getInstance());
                break;
            case 6: //  ABSORPTIVE RECT SHAPE CREATE
                m_pGameEditorState->m_pEditorStateMachine->ChangeState(EditorAbsorberRectShapeCreateState::getInstance());
                break;
            case 7: //  PORTAL CREATE
                m_pGameEditorState->m_pEditorStateMachine->ChangeState(EditorPortalCreateState::getInstance());
                break;
            case 8: //  COLOR FILTER CREATE
                m_pGameEditorState->m_pEditorStateMachine->ChangeState(EditorColorFilterRectShapeCreateState::getInstance());
                break;
                
            // GAME OBJECT OPERATION BUTTONS
            case 10: //  SELECT
                m_pGameEditorState->m_pEditorStateMachine->ChangeState(EditorOperationSelectState::getInstance());
                break;
            
            case 11: //  MOVE
                m_pGameEditorState->m_pEditorStateMachine->ChangeState(EditorOperationMoveState::getInstance());
                break;
                
            case 12: //  ROTATE
                m_pGameEditorState->m_pEditorStateMachine->ChangeState(EditorOperationRotateState::getInstance());
                break;

            case 13: //  MAIN MENU
                Game::getInstance()->changeState(GameMenuState::getInstance());
                break;
            // SourceObject/SinkGameObject Slider Color Red Component
            case 1000:
            {
                GUISlider* pSlider = (GUISlider*)pComponent;
                
                if( m_pSelectedGameObject->getGameObjectInfo().m_ObjectType == EOT_LIGHT_SOURCE )
                {
                    SourceGameObject *pSourceGameObject = (SourceGameObject*)m_pSelectedGameObject;
                    Color4f color = pSourceGameObject->getColor();
                    pSourceGameObject->setColor(Color4f(pSlider->getValue(),color.g,color.b,color.a));
                }
                if( m_pSelectedGameObject->getGameObjectInfo().m_ObjectType == EOT_SHAPE_OBJECT )
                {
                    ShapeGameObject *pShapeGameObject = (ShapeGameObject*)m_pSelectedGameObject;
                    Color4f color = pShapeGameObject->getColor();
                    pShapeGameObject->setColor(Color4f(pSlider->getValue(),color.g,color.b,color.a));
                }
                if( m_pSelectedGameObject->getGameObjectInfo().m_ObjectType == EOT_SINK )
                {
                    SinkGameObject *pSinkGameObject = (SinkGameObject*)m_pSelectedGameObject;
                    Color4f color = pSinkGameObject->getColor();
                    pSinkGameObject->setColor(Color4f(pSlider->getValue(),color.g,color.b,color.a));
                }
                break;
            }
            case 1001:
            {
                GUISlider* pSlider = (GUISlider*)pComponent;
                
                if( m_pSelectedGameObject->getGameObjectInfo().m_ObjectType == EOT_LIGHT_SOURCE )
                {
                    SourceGameObject *pSourceGameObject = (SourceGameObject*)m_pSelectedGameObject;
                    Color4f color = pSourceGameObject->getColor();
                    pSourceGameObject->setColor(Color4f(color.r,pSlider->getValue(),color.b,color.a));
                }
                if( m_pSelectedGameObject->getGameObjectInfo().m_ObjectType == EOT_SHAPE_OBJECT )
                {
                    ShapeGameObject *pShapeGameObject = (ShapeGameObject*)m_pSelectedGameObject;
                    Color4f color = pShapeGameObject->getColor();
                    pShapeGameObject->setColor(Color4f(color.r,pSlider->getValue(),color.b,color.a));
                }
                if( m_pSelectedGameObject->getGameObjectInfo().m_ObjectType == EOT_SINK )
                {
                    SinkGameObject *pSinkGameObject = (SinkGameObject*)m_pSelectedGameObject;
                    Color4f color = pSinkGameObject->getColor();
                    pSinkGameObject->setColor(Color4f(color.r,pSlider->getValue(),color.b,color.a));
                }
                break;
            }
            case 1002:
            {
                GUISlider* pSlider = (GUISlider*)pComponent;
                if( m_pSelectedGameObject->getGameObjectInfo().m_ObjectType == EOT_LIGHT_SOURCE )
                {
                    SourceGameObject *pSourceGameObject = (SourceGameObject*)m_pSelectedGameObject;
                    Color4f color = pSourceGameObject->getColor();
                    pSourceGameObject->setColor(Color4f(color.r,color.g,pSlider->getValue(),color.a));
                }
                if( m_pSelectedGameObject->getGameObjectInfo().m_ObjectType == EOT_SHAPE_OBJECT )
                {
                    ShapeGameObject *pShapeGameObject = (ShapeGameObject*)m_pSelectedGameObject;
                    Color4f color = pShapeGameObject->getColor();
                    pShapeGameObject->setColor(Color4f(color.r,color.g,pSlider->getValue(),color.a));
                }
                if( m_pSelectedGameObject->getGameObjectInfo().m_ObjectType == EOT_SINK )
                {
                    SinkGameObject *pSinkGameObject = (SinkGameObject*)m_pSelectedGameObject;
                    Color4f color = pSinkGameObject->getColor();
                    pSinkGameObject->setColor(Color4f(color.r,color.g,pSlider->getValue(),color.a));
                }
                break;
            }
            default:
                break;
        }
    }
}

void EditorHUDController::mouseReleased(GUIComponent* pComponent, int x, int y)
{
    if( pComponent )
    {
        switch (pComponent->getTag())
        {
            // Fixed Option for Shapes Check Box
            case  100:
            {
                GUICheckBox* pCheckBox = (GUICheckBox*)pComponent;
                bool bStatus = pCheckBox->isChecked();
                if( m_pSelectedGameObject->getGameObjectInfo().m_ObjectType == EOT_SHAPE_OBJECT )
                {
                    ((ShapeGameObject*)m_pSelectedGameObject)->setFixed(bStatus);
                }
                break;
            }
            case 20: // Save Button
                if( m_LevelName.size() )
                {
                    //Game::getInstance()->getScene()->setEnableForScreenShot(true);
                    Game::getInstance()->getScene()->renderScene();
                    Game::getInstance()->getScene()->save(m_LevelName.c_str());
                    hideSaveDialog();
                }
                break;
            case 21:
                hideSaveDialog();
                break;
            case 30: // Load Button
                if( m_LevelName.size() )
                {
                    Game::getInstance()->getScene()->load(m_LevelName.c_str());
                    hideLoadDialog();
                }
                break;
            case 31:
                hideLoadDialog();
                break;
            default:
                break;
        }
    }

}

void EditorHUDController::mouseMoved(GUIComponent* pComponent, int x, int y)
{
    if( pComponent )
    {
        switch (pComponent->getTag())
        {
            case 1000:
            {
                GUISlider* pSlider = (GUISlider*)pComponent;
                
                if( m_pSelectedGameObject->getGameObjectInfo().m_ObjectType == EOT_LIGHT_SOURCE )
                {
                    SourceGameObject *pSourceGameObject = (SourceGameObject*)m_pSelectedGameObject;
                    Color4f color = pSourceGameObject->getColor();
                    pSourceGameObject->setColor(Color4f(pSlider->getValue(),color.g,color.b,color.a));
                }
                if( m_pSelectedGameObject->getGameObjectInfo().m_ObjectType == EOT_SHAPE_OBJECT )
                {
                    ShapeGameObject *pShapeGameObject = (ShapeGameObject*)m_pSelectedGameObject;
                    Color4f color = pShapeGameObject->getColor();
                    pShapeGameObject->setColor(Color4f(pSlider->getValue(),color.g,color.b,color.a));
                }
                if( m_pSelectedGameObject->getGameObjectInfo().m_ObjectType == EOT_SINK )
                {
                    SinkGameObject *pSinkGameObject = (SinkGameObject*)m_pSelectedGameObject;
                    Color4f color = pSinkGameObject->getColor();
                    pSinkGameObject->setColor(Color4f(pSlider->getValue(),color.g,color.b,color.a));
                }
                break;
            }
            case 1001:
            {
                GUISlider* pSlider = (GUISlider*)pComponent;
                
                if( m_pSelectedGameObject->getGameObjectInfo().m_ObjectType == EOT_LIGHT_SOURCE )
                {
                    SourceGameObject *pSourceGameObject = (SourceGameObject*)m_pSelectedGameObject;
                    Color4f color = pSourceGameObject->getColor();
                    pSourceGameObject->setColor(Color4f(color.r,pSlider->getValue(),color.b,color.a));
                }
                if( m_pSelectedGameObject->getGameObjectInfo().m_ObjectType == EOT_SHAPE_OBJECT )
                {
                    ShapeGameObject *pShapeGameObject = (ShapeGameObject*)m_pSelectedGameObject;
                    Color4f color = pShapeGameObject->getColor();
                    pShapeGameObject->setColor(Color4f(color.r,pSlider->getValue(),color.b,color.a));
                }
                if( m_pSelectedGameObject->getGameObjectInfo().m_ObjectType == EOT_SINK )
                {
                    SinkGameObject *pSinkGameObject = (SinkGameObject*)m_pSelectedGameObject;
                    Color4f color = pSinkGameObject->getColor();
                    pSinkGameObject->setColor(Color4f(color.r,pSlider->getValue(),color.b,color.a));
                }
                break;
            }
            case 1002:
            {
                GUISlider* pSlider = (GUISlider*)pComponent;
                if( m_pSelectedGameObject->getGameObjectInfo().m_ObjectType == EOT_LIGHT_SOURCE )
                {
                    SourceGameObject *pSourceGameObject = (SourceGameObject*)m_pSelectedGameObject;
                    Color4f color = pSourceGameObject->getColor();
                    pSourceGameObject->setColor(Color4f(color.r,color.g,pSlider->getValue(),color.a));
                }
                if( m_pSelectedGameObject->getGameObjectInfo().m_ObjectType == EOT_SHAPE_OBJECT )
                {
                    ShapeGameObject *pShapeGameObject = (ShapeGameObject*)m_pSelectedGameObject;
                    Color4f color = pShapeGameObject->getColor();
                    pShapeGameObject->setColor(Color4f(color.r,color.g,pSlider->getValue(),color.a));
                }
                if( m_pSelectedGameObject->getGameObjectInfo().m_ObjectType == EOT_SINK )
                {
                    SinkGameObject *pSinkGameObject = (SinkGameObject*)m_pSelectedGameObject;
                    Color4f color = pSinkGameObject->getColor();
                    pSinkGameObject->setColor(Color4f(color.r,color.g,pSlider->getValue(),color.a));
                }
                break;
            }
        }
    }
}


void EditorHUDController::keyPressed(GUIComponent* pComponent, const Event& event)
{
    if( event.keyInput.Alt && (event.keyInput.Char == 'h' || event.keyInput.Char == 'H') && event.keyInput.PressedDown)
    {
        //showSaveDialog();
        int chapter = GameChapterSelectionState::getInstance()->getSelectedChapter();
        int stage = GameLevelSelectionState::getInstance()->getSelectedStage();
        int level = GameLevelSelectionState::getInstance()->getSelectedLevel();
        char levelFilename[32];
        sprintf(levelFilename,"ch%dstg%dlvl%d", chapter, stage, level);

        Game::getInstance()->getScene()->appendSolutionData(levelFilename);
    }
    if( event.keyInput.Alt && (event.keyInput.Char == 's' || event.keyInput.Char == 'S') )
    {
        showSaveDialog();
    }
    if( event.keyInput.Alt && (event.keyInput.Char == 'l' || event.keyInput.Char == 'L') )
    {
        showLoadDialog();
    }
    if( pComponent->getTag() == 22 )
    {
        GUITextField* pTextField = (GUITextField*)pComponent;
        m_LevelName = pTextField->getText();
        if( event.keyInput.Key == KEY_RETURN )
        {
            if( m_LevelName.size())
            {
                Game::getInstance()->getScene()->save(m_LevelName.c_str());
                hideSaveDialog();
            }
        }
    }
    if( pComponent->getTag() == 32 )
    {
        GUITextField* pTextField = (GUITextField*)pComponent;
        m_LevelName = pTextField->getText();
        if( event.keyInput.Key == KEY_RETURN )
        {
            if( m_LevelName.size())
            {
                Game::getInstance()->getScene()->load(m_LevelName.c_str());
                hideLoadDialog();
            }
        }
    }
}

EditorHUDController::~EditorHUDController()
{
    delete m_pShapeSelectionBasePanel;
    delete m_pShapeSelectionModeView;
    for( int i=0; i<m_vecButtons.size(); i++ )
    {
        delete m_vecButtons[i];
    }
    m_vecButtons.clear();
}

void EditorHUDController::fillGameObjectProperties(GameObject* pGameObject)
{
    m_pSelectedGameObject = pGameObject;
    
    switch (pGameObject->getGameObjectInfo().m_ObjectType)
    {
        case EOT_SHAPE_OBJECT:
            showBasicPropertiesPanel();
            fillShapeObjectProperties((ShapeGameObject*)pGameObject);
            break;
            
        case EOT_SINK:
            showBasicPropertiesPanel();
            fillSinkObjectProperties((SinkGameObject*)pGameObject);
            break;
            
        case EOT_LIGHT_SOURCE:
            showBasicPropertiesPanel();
            fillSourceObjectProperties((SourceGameObject*)pGameObject);
            break;
        default:
            break;
    }
}

void EditorHUDController::showBasicPropertiesPanel()
{
    s_pBasicShapePropertiesPanel->showGUI();
}

void EditorHUDController::hideBasicPropertiesPanel()
{
    s_pBasicShapePropertiesPanel->hideGUI();
}

void EditorHUDController::showSinkPropertiesPanel()
{
    s_pBasicShapePropertiesPanel->showGUI();
}

void EditorHUDController::hideSinkPropertiesPanel()
{
    s_pBasicShapePropertiesPanel->hideGUI();
}

void EditorHUDController::showSourcePropertiesPanel()
{
    s_pBasicShapePropertiesPanel->showGUI();
}

void EditorHUDController::hideSourcePropertiesPanel()
{
    s_pBasicShapePropertiesPanel->hideGUI();
}

void EditorHUDController::fillShapeObjectProperties(ShapeGameObject* pShapeGameObject)
{
    Vector2f pos = pShapeGameObject->getPosition();
    float    rot = pShapeGameObject->getRotation();
    
    s_pBasicShapePropertiesPanel->setPositionLabel(pos);
    s_pBasicShapePropertiesPanel->setRotationLabel(rot);
    s_pBasicShapePropertiesPanel->setFixedStatusForCheckBox(pShapeGameObject->isFixed());
    s_pBasicShapePropertiesPanel->setID(pShapeGameObject->getID());
    
    Color4f color = pShapeGameObject->getColor();
    
    s_pBasicShapePropertiesPanel->setRedSliderValue(color.r);
    s_pBasicShapePropertiesPanel->setGreenSliderValue(color.g);
    s_pBasicShapePropertiesPanel->setBlueSliderValue(color.b);
}

void EditorHUDController::fillSinkObjectProperties(SinkGameObject* pSinkGameObject)
{
    Vector2f pos = pSinkGameObject->getPosition();
    float    rot = pSinkGameObject->getRotation();
    
    s_pBasicShapePropertiesPanel->setPositionLabel(pos);
    s_pBasicShapePropertiesPanel->setRotationLabel(rot);
    //s_pSinkPropertiesPanel->setRayTerminateCheck(pSinkGameObject->getRayExitEnable());
    Color4f color = pSinkGameObject->getColor();

    s_pBasicShapePropertiesPanel->setRedSliderValue(color.r);
    s_pBasicShapePropertiesPanel->setGreenSliderValue(color.g);
    s_pBasicShapePropertiesPanel->setBlueSliderValue(color.b);
}

void EditorHUDController::fillSourceObjectProperties(SourceGameObject* pSourceGameObject)
{
    Vector2f pos = pSourceGameObject->getPosition();
    float    rot = pSourceGameObject->getRotation();
    
    s_pBasicShapePropertiesPanel->setPositionLabel(pos);
    s_pBasicShapePropertiesPanel->setRotationLabel(rot);
    Color4f color = pSourceGameObject->getColor();
    
    s_pBasicShapePropertiesPanel->setRedSliderValue(color.r);
    s_pBasicShapePropertiesPanel->setGreenSliderValue(color.g);
    s_pBasicShapePropertiesPanel->setBlueSliderValue(color.b);
}