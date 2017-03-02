//
//  EditorHUDController.h
//  Optiks
//
//  Created by Prashant Sharma on 02/11/13.
//
//

#ifndef __Optiks__EditorHUDController__
#define __Optiks__EditorHUDController__

#include "GUI.h"

using namespace spider3d::gui;

class GameEditorState;
class BasicPropertiesPanel;
class SinkPropertiesPanel;
class SourcePropertiesPanel;
class GameObject;
class ShapeGameObject;
class SinkGameObject;
class SourceGameObject;

class EditorHUDController : public IActionListener
{
public:
    EditorHUDController(GameEditorState* pGameEditorState);
    
    ~EditorHUDController();
    
    // IActionListener
    virtual void        mousePressed(GUIComponent* pComponent, int x, int y);
    
    virtual void        mouseReleased(GUIComponent* pComponent, int x, int y);
    
    virtual void        mouseMoved(GUIComponent* pComponent, int x, int y);

    virtual void        keyPressed(GUIComponent* pComponent, const Event& event);
    
    void                hideEditorUI();
    void                showEditorUI();
    
    void                fillGameObjectProperties(GameObject* pGameObject);
    void                showBasicPropertiesPanel();
    void                hideBasicPropertiesPanel();
    void                showSinkPropertiesPanel();
    void                hideSinkPropertiesPanel();
    void                showSourcePropertiesPanel();
    void                hideSourcePropertiesPanel();
    

private:
    
    void                fillShapeObjectProperties(ShapeGameObject*);
    void                fillSinkObjectProperties(SinkGameObject*);
    void                fillSourceObjectProperties(SourceGameObject* pSourceGameObject);

    void                createShapeSelectionView();
    void                createShapeSelectionModeView();
    void                createSaveDialog();
    void                showSaveDialog();
    void                hideSaveDialog();
    void                createLoadDialog();
    void                showLoadDialog();
    void                hideLoadDialog();
    void                createPropertiesPanels();
private:
    GUIPanel            *m_pShapeSelectionBasePanel;
    
    GUIPanel            *m_pShapeSelectionModeView;

    GUIPanel            *m_pSaveDialogPanel;
    GUITextField        *m_pLevelNameTextField;
    GUIButton           *m_pCancelButton;
    GUIButton           *m_pSaveButton;
    string              m_LevelName;
    
    GUIPanel            *m_pLoadDialogPanel;
    GUITextField        *m_pLoadLevelNameTextField;
    GUIButton           *m_pLoadCancelButton;
    GUIButton           *m_pLoadOkButton;
    string              m_LoadLevelName;
    
    GUIButton           *m_pMainMenuSelectionButton;
    
    vector<GUIButton*>  m_vecButtons;
    GameEditorState     *m_pGameEditorState;
    
    static  BasicPropertiesPanel           *s_pBasicShapePropertiesPanel;
    //static  SinkPropertiesPanel            *s_pSinkPropertiesPanel;
    //static  SourcePropertiesPanel          *s_pSourcePropertiesPanel;
    
    GameObject          *m_pSelectedGameObject;
};

#endif /* defined(__Optiks__EditorHUDController__) */
