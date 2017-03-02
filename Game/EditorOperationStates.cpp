//
//  EditorOperationStatesStates.cpp
//  Optiks
//
//  Created by Prashant Sharma on 07/11/13.
//
//

#include "EditorOperationStates.h"
#include "Game.h"
#include "Scene.h"
#include "GameEditorState.h"
#include "GameEditorHUDController.h"

EditorOperationSelectState*  EditorOperationSelectState::s_pEditorOperationSelectState = 0;
EditorOperationMoveState*    EditorOperationMoveState::  s_pEditorOperationMoveState   = 0;
EditorOperationRotateState*  EditorOperationRotateState::s_pEditorOperationRotateState = 0;

///////////////////////////////////////////////////////////////////////////
/////////////////////////BASE STATE///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

EditorOperationBaseState::EditorOperationBaseState()
{
    
}

void EditorOperationBaseState::Enter(Game* pOwner)
{
    
}
    
    //this is the states normal update function
void EditorOperationBaseState::Execute(Game* pOwner)
{
    
}

//this will execute when the state is exited.
void EditorOperationBaseState::Exit(Game* pOwner)
{
    
}
    
sbool EditorOperationBaseState::OnEvent( Event& event)
{
    if( event.eventType == SKEY_EVENT )
    {
        if( event.keyInput.Key == KEY_DELETE )
        {
            GameObject* pGameObject = Game::getInstance()->getScene()->pickGameObject(Vector2f(event.mouseInput.X, event.mouseInput.Y));
            
            if( pGameObject )
            {
                Game::getInstance()->getScene()->destroyGameObject(pGameObject);
            }
        }
    }
    return false;
}



///////////////////////////////////////////////////////////////////////////
/////////////////////////MOVE STATE///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

EditorOperationMoveState::EditorOperationMoveState()
{
    
}

void EditorOperationMoveState::Enter(Game* pOwner)
{
    
}

//this is the states normal update function
void EditorOperationMoveState::Execute(Game* pOwner)
{
    
}

//this will execute when the state is exited.
void EditorOperationMoveState::Exit(Game* pOwner)
{
    
}

sbool EditorOperationMoveState::OnEvent( Event& event)
{
    GameObject* pSelectedObject = NULL;

    EditorOperationBaseState::OnEvent(event);
    
    if( event.mouseInput.Event == spider3d::LEFT_MOUSE_LEFT_UP ||
        event.mouseInput.Event == spider3d::LEFT_MOUSE_PRESSED_DOWN )
    {
        pSelectedObject = Game::getInstance()->getScene()->mousePressed(event);
    }
    if( event.mouseInput.Event == spider3d::SMOUSE_DRAGGED )
    {
        Game::getInstance()->getScene()->mouseMoved(event);
    }
    EditorHUDController* pEditorHUDController = GameEditorState::getInstance()->getEditorHUDController();
    
    if( event.mouseInput.Event == spider3d::LEFT_MOUSE_PRESSED_DOWN )
    {
        if(pSelectedObject )
        {
            pEditorHUDController->fillGameObjectProperties(pSelectedObject);
        }
        else
        {
            //pEditorHUDController->hideBasicPropertiesPanel();
            //pEditorHUDController->hideSourcePropertiesPanel();
            //pEditorHUDController->hideSinkPropertiesPanel();
        }
    }
    return true;
}


///////////////////////////////////////////////////////////////////////////
/////////////////////////ROTATE STATE///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

EditorOperationRotateState::EditorOperationRotateState()
{
    
}


void EditorOperationRotateState::Enter(Game* pOwner)
{
    
}

//this is the states normal update function
void EditorOperationRotateState::Execute(Game* pOwner)
{
    
}

//this will execute when the state is exited.
void EditorOperationRotateState::Exit(Game* pOwner)
{
    
}

sbool EditorOperationRotateState::OnEvent( Event& event)
{
    EditorOperationBaseState::OnEvent(event);

    if( event.mouseInput.Event == spider3d::LEFT_MOUSE_LEFT_UP )
    {
        Vector2f pickPoint(event.mouseInput.X, event.mouseInput.Y);
        GameObject *pGameObject = Game::getInstance()->getScene()->pickGameObject(pickPoint);
        Game::getInstance()->getScene()->rotateSelectedObject(pGameObject, 90.0f);
    }
    return true;
}



///////////////////////////////////////////////////////////////////////////
/////////////////////////SELECT STATE///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

EditorOperationSelectState::EditorOperationSelectState()
{
    
}

void EditorOperationSelectState::Enter(Game* pOwner)
{
    
}

//this is the states normal update function
void EditorOperationSelectState::Execute(Game* pOwner)
{
    
}

//this will execute when the state is exited.
void EditorOperationSelectState::Exit(Game* pOwner)
{
    
}

sbool EditorOperationSelectState::OnEvent( Event& event)
{
    EditorOperationBaseState::OnEvent(event);

    return false;
}