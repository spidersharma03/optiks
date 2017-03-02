//
//  GamePlayOperationStates.cpp
//  Optiks
//
//  Created by Prashant on 23/12/13.
//
//

#include "GamePlayOperationStates.h"
#include "Game.h"
#include "GameObject.h"
#include "ShapeGameObject.h"
#include "Scene.h"

GamePlayOperationMoveState* GamePlayOperationMoveState::s_pGamePlayOperationMoveState = NULL;

///////////////////////////////////////////////////////////////////////////
/////////////////////////MOVE STATE///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

GamePlayOperationMoveState::GamePlayOperationMoveState()
{
    
}

void GamePlayOperationMoveState::Enter(Game* pOwner)
{
    
}

//this is the states normal update function
void GamePlayOperationMoveState::Execute(Game* pOwner)
{
    
}

//this will execute when the state is exited.
void GamePlayOperationMoveState::Exit(Game* pOwner)
{
    
}

sbool GamePlayOperationMoveState::OnEvent( Event& event)
{
    GameObject* pSelectedObject = NULL;
    
    if( event.mouseInput.Event == spider3d::LEFT_MOUSE_PRESSED_DOWN )
    {
        int x = event.mouseInput.X;
        int y = event.mouseInput.Y;
        pSelectedObject = Game::getInstance()->getScene()->pickGameObject(Vector2f(x,y));
        
        if( pSelectedObject )
        {
            GAME_OBJECT_TYPE objectType = pSelectedObject->getGameObjectInfo().m_ObjectType;
            
            if(  objectType == EOT_SHAPE_OBJECT )
            {
                ShapeGameObject* pShapeGameObject = (ShapeGameObject*)pSelectedObject;
                
                if( !pShapeGameObject->isFixed() )
                {
                    pSelectedObject = Game::getInstance()->getScene()->mousePressed(event);
                }
            }
        }
    }
    if( event.mouseInput.Event == spider3d::LEFT_MOUSE_LEFT_UP )
    {
        GameObject* pCurrentSelectedObject = Game::getInstance()->getScene()->getCurrentlySelectedObject();
        
        if( pCurrentSelectedObject && pCurrentSelectedObject->getGameObjectInfo().m_ObjectType == EOT_SHAPE_OBJECT )
        {
             ShapeGameObject* pShapeGameObject = (ShapeGameObject*)pCurrentSelectedObject;

            if( !pShapeGameObject->isFixed() )
            {
                Game::getInstance()->getScene()->mousePressed(event);
            }
        }
    }
    if( event.mouseInput.Event == spider3d::SMOUSE_DRAGGED )
    {
        GameObject* pCurrentSelectedObject = Game::getInstance()->getScene()->getCurrentlySelectedObject();
        
        if( pCurrentSelectedObject && pCurrentSelectedObject->getGameObjectInfo().m_ObjectType == EOT_SHAPE_OBJECT )
        {
            ShapeGameObject* pShapeGameObject = (ShapeGameObject*)pCurrentSelectedObject;
            
            if( !pShapeGameObject->isFixed() )
                Game::getInstance()->getScene()->mouseMoved(event);
        }
    }
    return true;
}


///////////////////////////////////////////////////////////////////////////
/////////////////////////ROTATE STATE///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////


