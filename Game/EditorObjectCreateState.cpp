//
//  EditorObjectCreateState.cpp
//  Optiks
//
//  Created by Prashant Sharma on 07/11/13.
//
//

#include "EditorObjectCreateState.h"
#include "Game.h"
#include "Scene.h"
#include "Grid.h"
#include "ShapeGameObject.h"
#include "SourceGameObject.h"
#include "SinkGameObject.h"
#include "PlacementMarkerGameObject.h"
#include "PortalGameObject.h"

EditorRectShapeCreateState* EditorRectShapeCreateState::s_pEditorRectShapeCreateState = NULL;
EditorSourceCreateState*    EditorSourceCreateState   ::s_pEditorSourceCreateState    = NULL;
EditorSinkCreateState*      EditorSinkCreateState     ::s_pEditorSinkCreateState    = NULL;
EditorPlacementMarkerCreateState* EditorPlacementMarkerCreateState::s_pEditorPlacementMarkerCreateState = NULL;
EditorTransmissiveRectShapeCreateState* EditorTransmissiveRectShapeCreateState::s_pEditorTransmissiveRectShapeCreateState = NULL;
EditorRefractiveRectShapeCreateState* EditorRefractiveRectShapeCreateState::s_pEditorRefractiveRectShapeCreateState = NULL;
EditorAbsorberRectShapeCreateState* EditorAbsorberRectShapeCreateState::s_pEditorAbsorberRectShapeCreateState = NULL;
EditorPortalCreateState* EditorPortalCreateState::s_pEditorPortalCreateState = NULL;
EditorColorFilterRectShapeCreateState* EditorColorFilterRectShapeCreateState::s_pEditorColorFilterRectShapeCreateState = NULL;

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////BASE STATE FOR OBJECT CREATION /////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

EditorObjectCreateState::EditorObjectCreateState()
{
    
}

void EditorObjectCreateState::Enter(Game* pOwner)
{
    
}

//this is the states normal update function
void EditorObjectCreateState::Execute(Game* pOwner)
{
    
}

//this will execute when the state is exited.
void EditorObjectCreateState::Exit(Game* pOwner)
{
    
}

sbool EditorObjectCreateState::OnEvent( Event& event)
{
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////STATE FOR RECT CREATION /////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

EditorRectShapeCreateState::EditorRectShapeCreateState()
{
    
}

void EditorRectShapeCreateState::Enter(Game* pOwner)
{
    
}

//this is the states normal update function
void EditorRectShapeCreateState::Execute(Game* pOwner)
{
    
}

//this will execute when the state is exited.
void EditorRectShapeCreateState::Exit(Game* pOwner)
{
    
}

sbool EditorRectShapeCreateState::OnEvent( Event& event)
{
    if(event.mouseInput.Event == spider3d::LEFT_MOUSE_LEFT_UP)
    {
        Scene* pScene = Game::getInstance()->getScene();
        
        ShapeGameObjectInfo sInfo;
        
        RectangleInfo rInfo;
        rInfo.m_IsReflective = true;
        rInfo.m_isRefractive = false;
        rInfo.m_RefractionIndex = 1.0f;
        
        rInfo.width  = BASIC_ELEMENT_SIZE;
        rInfo.height = BASIC_ELEMENT_SIZE;
        rInfo.position.x = event.mouseInput.X;
        rInfo.position.y = event.mouseInput.Y;
        
        sInfo.m_Position = rInfo.position;
        sInfo.m_Rotation = 0.0f;
        
        sInfo.shapeInfo = &rInfo;
        sInfo.eMaterialType = EMT_REFLECTIVE;
        
        if( pScene->isGameObjectPlaceable(Vector2f(event.mouseInput.X,event.mouseInput.Y), EOT_SHAPE_OBJECT ) )
        {
            pScene->addGameObject(pScene->createGameObject(sInfo));
        }

    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////STATE FOR COLOR FILTER RECT CREATION /////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

EditorColorFilterRectShapeCreateState::EditorColorFilterRectShapeCreateState()
{
    
}

void EditorColorFilterRectShapeCreateState::Enter(Game* pOwner)
{
    
}

//this is the states normal update function
void EditorColorFilterRectShapeCreateState::Execute(Game* pOwner)
{
    
}

//this will execute when the state is exited.
void EditorColorFilterRectShapeCreateState::Exit(Game* pOwner)
{
    
}

sbool EditorColorFilterRectShapeCreateState::OnEvent( Event& event)
{
    if(event.mouseInput.Event == spider3d::LEFT_MOUSE_LEFT_UP)
    {
        Scene* pScene = Game::getInstance()->getScene();
        
        ShapeGameObjectInfo sInfo;

        RectangleInfo rInfo;
        rInfo.m_IsReflective = false;
        rInfo.m_isRefractive = true;
        rInfo.m_RefractionIndex = 1.0f;
        
        rInfo.width  = BASIC_ELEMENT_SIZE;
        rInfo.height = BASIC_ELEMENT_SIZE;
        rInfo.position.x = event.mouseInput.X;
        rInfo.position.y = event.mouseInput.Y;
        
        sInfo.m_Position = rInfo.position;
        sInfo.m_Rotation = 0.0f;
        
        sInfo.shapeInfo = &rInfo;
        sInfo.eMaterialType = EMT_COLOR_FILTER;
        
        if( pScene->isGameObjectPlaceable(Vector2f(event.mouseInput.X,event.mouseInput.Y), EOT_SHAPE_OBJECT ) )
        {
            pScene->addGameObject(pScene->createGameObject(sInfo));
        }
        
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////STATE FOR TRANSMISSIVE RECT CREATION /////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

EditorTransmissiveRectShapeCreateState::EditorTransmissiveRectShapeCreateState()
{
    
}

void EditorTransmissiveRectShapeCreateState::Enter(Game* pOwner)
{
    
}

//this is the states normal update function
void EditorTransmissiveRectShapeCreateState::Execute(Game* pOwner)
{
    
}

//this will execute when the state is exited.
void EditorTransmissiveRectShapeCreateState::Exit(Game* pOwner)
{
    
}

sbool EditorTransmissiveRectShapeCreateState::OnEvent( Event& event)
{
    if(event.mouseInput.Event == spider3d::LEFT_MOUSE_LEFT_UP)
    {
        Scene* pScene = Game::getInstance()->getScene();
        
        ShapeGameObjectInfo sInfo;
        
        RectangleInfo rInfo;
        rInfo.m_IsReflective = true;
        rInfo.m_isRefractive = true;
        rInfo.m_RefractionIndex = 1.0f;
        
        rInfo.width  = BASIC_ELEMENT_SIZE;
        rInfo.height = BASIC_ELEMENT_SIZE;
        rInfo.position.x = event.mouseInput.X;
        rInfo.position.y = event.mouseInput.Y;
        
        sInfo.m_Position = rInfo.position;
        sInfo.m_Rotation = 0.0f;
        
        sInfo.shapeInfo = &rInfo;
        sInfo.eMaterialType = EMT_TRANSMISSIVE;
        
        if( pScene->isGameObjectPlaceable(Vector2f(event.mouseInput.X,event.mouseInput.Y), EOT_SHAPE_OBJECT ) )
        {
            pScene->addGameObject(pScene->createGameObject(sInfo));
        }

    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////STATE FOR REFRACTIVE RECT CREATION /////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

EditorRefractiveRectShapeCreateState::EditorRefractiveRectShapeCreateState()
{
    
}

void EditorRefractiveRectShapeCreateState::Enter(Game* pOwner)
{
    
}

//this is the states normal update function
void EditorRefractiveRectShapeCreateState::Execute(Game* pOwner)
{
    
}

//this will execute when the state is exited.
void EditorRefractiveRectShapeCreateState::Exit(Game* pOwner)
{
    
}

sbool EditorRefractiveRectShapeCreateState::OnEvent( Event& event)
{
    if(event.mouseInput.Event == spider3d::LEFT_MOUSE_LEFT_UP)
    {
        Scene* pScene = Game::getInstance()->getScene();
        
        ShapeGameObjectInfo sInfo;
        
        RectangleInfo rInfo;
        rInfo.m_IsReflective = true;
        rInfo.m_isRefractive = true;
        rInfo.m_RefractionIndex = 100.0f;
        
        rInfo.width  = BASIC_ELEMENT_SIZE;
        rInfo.height = BASIC_ELEMENT_SIZE;
        rInfo.position.x = event.mouseInput.X;
        rInfo.position.y = event.mouseInput.Y;
        
        sInfo.m_Position = rInfo.position;
        sInfo.m_Rotation = 0.0f;
        
        sInfo.shapeInfo = &rInfo;
        sInfo.eMaterialType = EMT_REFRACTIVE;
        
        if( pScene->isGameObjectPlaceable(Vector2f(event.mouseInput.X,event.mouseInput.Y), EOT_SHAPE_OBJECT ) )
        {
            pScene->addGameObject(pScene->createGameObject(sInfo));
        }

    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////STATE FOR PORTAL CREATION /////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

EditorPortalCreateState::EditorPortalCreateState()
{
    
}

void EditorPortalCreateState::Enter(Game* pOwner)
{
    
}

//this is the states normal update function
void EditorPortalCreateState::Execute(Game* pOwner)
{
    
}

//this will execute when the state is exited.
void EditorPortalCreateState::Exit(Game* pOwner)
{
    
}

sbool EditorPortalCreateState::OnEvent( Event& event)
{
    if(event.mouseInput.Event == spider3d::LEFT_MOUSE_LEFT_UP)
    {
        Scene* pScene = Game::getInstance()->getScene();
        
        PortalGameObjectInfo pInfo;
        
        RectangleInfo rInfo1;
        rInfo1.m_IsReflective = false;
        rInfo1.m_isRefractive = false;
        rInfo1.m_RefractionIndex = 1.0f;
        
        rInfo1.width  = BASIC_ELEMENT_SIZE;
        rInfo1.height = BASIC_ELEMENT_SIZE;
        rInfo1.position.x = event.mouseInput.X;
        rInfo1.position.y = event.mouseInput.Y;
        
        pInfo.m_Position = rInfo1.position;
        pInfo.m_Rotation = 0.0f;
        
        pInfo.shapeInfo1 = &rInfo1;
        
        RectangleInfo rInfo2;
        rInfo2.m_IsReflective = false;
        rInfo2.m_isRefractive = false;
        rInfo2.m_RefractionIndex = 1.0f;
        
        rInfo2.width  = BASIC_ELEMENT_SIZE;
        rInfo2.height = BASIC_ELEMENT_SIZE;
        rInfo2.position.x = event.mouseInput.X;
        rInfo2.position.y = event.mouseInput.Y;
        
        pInfo.m_Position = rInfo2.position;
        pInfo.m_Rotation = 0.0f;
        
        pInfo.shapeInfo2 = &rInfo2;
        
        if( pScene->isGameObjectPlaceable(Vector2f(event.mouseInput.X,event.mouseInput.Y), EOT_PORTAL ) )
        {
            pScene->addGameObject(pScene->createGameObject(pInfo));
        }

    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////STATE FOR ABSORBER RECT CREATION /////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

EditorAbsorberRectShapeCreateState::EditorAbsorberRectShapeCreateState()
{
    
}

void EditorAbsorberRectShapeCreateState::Enter(Game* pOwner)
{
    
}

//this is the states normal update function
void EditorAbsorberRectShapeCreateState::Execute(Game* pOwner)
{
    
}

//this will execute when the state is exited.
void EditorAbsorberRectShapeCreateState::Exit(Game* pOwner)
{
    
}

sbool EditorAbsorberRectShapeCreateState::OnEvent( Event& event)
{
    if(event.mouseInput.Event == spider3d::LEFT_MOUSE_LEFT_UP)
    {
        Scene* pScene = Game::getInstance()->getScene();
        
        ShapeGameObjectInfo sInfo;
        
        RectangleInfo rInfo;
        rInfo.m_IsReflective = false;
        rInfo.m_isRefractive = false;
        rInfo.m_RefractionIndex = 1.0f;
        
        rInfo.width  = BASIC_ELEMENT_SIZE;
        rInfo.height = BASIC_ELEMENT_SIZE;
        rInfo.position.x = event.mouseInput.X;
        rInfo.position.y = event.mouseInput.Y;
        
        sInfo.m_Position = rInfo.position;
        sInfo.m_Rotation = 0.0f;
        
        sInfo.shapeInfo = &rInfo;
        sInfo.eMaterialType = EMT_ABSORBITIVE;
        
        if( pScene->isGameObjectPlaceable(Vector2f(event.mouseInput.X,event.mouseInput.Y), EOT_SHAPE_OBJECT ) )
        {
            pScene->addGameObject(pScene->createGameObject(sInfo));
        }

    }
    return true;
}


//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////STATE FOR SOURCE CREATION /////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

EditorSourceCreateState::EditorSourceCreateState()
{
    
}

void EditorSourceCreateState::Enter(Game* pOwner)
{
    
}

//this is the states normal update function
void EditorSourceCreateState::Execute(Game* pOwner)
{
    
}

//this will execute when the state is exited.
void EditorSourceCreateState::Exit(Game* pOwner)
{
    
}

sbool EditorSourceCreateState::OnEvent( Event& event)
{
    if(event.mouseInput.Event == spider3d::LEFT_MOUSE_LEFT_UP)
    {
        Scene* pScene = Game::getInstance()->getScene();
        
        //Vector2f sourcePos = pScene->getGrid()->getWorldCoordinatesFromGrid(Vector2f(8.5,5));
        Vector2f sourcePos(event.mouseInput.X, event.mouseInput.Y);
        
        SourceGameObjectInfo sourceInfo;
        sourceInfo.color = RAY_COLOR;
        sourceInfo.origin    = sourcePos;
        sourceInfo.direction.x = -1;
        sourceInfo.direction.y = -1;
        
        sourceInfo.m_Position = sourcePos;
        sourceInfo.m_Rotation = 0.0f;
        
        Vector2f gridCentre;
        pScene->getGrid()->snap(sourcePos, gridCentre);
        sourceInfo.closestGridCentre = gridCentre;
        
        if( pScene->isGameObjectPlaceable(Vector2f(event.mouseInput.X,event.mouseInput.Y), EOT_LIGHT_SOURCE ) )
        {
            pScene->addGameObject(pScene->createGameObject(sourceInfo));
        }

    }
    return true;
}


//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////STATE FOR SINK CREATION /////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

EditorSinkCreateState::EditorSinkCreateState()
{
    
}

void EditorSinkCreateState::Enter(Game* pOwner)
{
    
}

//this is the states normal update function
void EditorSinkCreateState::Execute(Game* pOwner)
{
    
}

//this will execute when the state is exited.
void EditorSinkCreateState::Exit(Game* pOwner)
{
    
}

sbool EditorSinkCreateState::OnEvent( Event& event)
{
    if(event.mouseInput.Event == spider3d::LEFT_MOUSE_LEFT_UP)
    {
        Scene* pScene = Game::getInstance()->getScene();
        
        //Vector2f sourcePos = pScene->getGrid()->getWorldCoordinatesFromGrid(Vector2f(8.5,5));
        Vector2f sourcePos(event.mouseInput.X, event.mouseInput.Y);
        
        SinkGameObjectInfo sinkInfo;
        sinkInfo.color = RAY_COLOR;
        
        sinkInfo.m_Position.x = event.mouseInput.X;
        sinkInfo.m_Position.y = event.mouseInput.Y;
        
        sinkInfo.m_Rotation = 0.0f;
        

        Vector2f gridCentre;
        pScene->getGrid()->snap(sinkInfo.m_Position, gridCentre);
        sinkInfo.closestGridCentre = gridCentre;
        
        if( pScene->isGameObjectPlaceable(Vector2f(event.mouseInput.X,event.mouseInput.Y), EOT_SINK ) )
        {
            pScene->addGameObject(pScene->createGameObject(sinkInfo));
        }

    }
    return true;
}


//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////STATE FOR PLACEMENT MARKER CREATION /////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

EditorPlacementMarkerCreateState::EditorPlacementMarkerCreateState()
{
    
}

void EditorPlacementMarkerCreateState::Enter(Game* pOwner)
{
    
}

//this is the states normal update function
void EditorPlacementMarkerCreateState::Execute(Game* pOwner)
{
    
}

//this will execute when the state is exited.
void EditorPlacementMarkerCreateState::Exit(Game* pOwner)
{
    
}

sbool EditorPlacementMarkerCreateState::OnEvent( Event& event)
{
    if(event.mouseInput.Event == spider3d::LEFT_MOUSE_LEFT_UP)
    {
        Scene* pScene = Game::getInstance()->getScene();
        
        //Vector2f sourcePos = pScene->getGrid()->getWorldCoordinatesFromGrid(Vector2f(8.5,5));
        Vector2f sourcePos(event.mouseInput.X, event.mouseInput.Y);
        
        PlacementMarkerGameObjectInfo markerInfo;
        markerInfo.width  = BASIC_ELEMENT_SIZE;
        markerInfo.height = BASIC_ELEMENT_SIZE;
        markerInfo.m_Position.x = event.mouseInput.X;
        markerInfo.m_Position.y = event.mouseInput.Y;
        
        if( pScene->isGameObjectPlaceable(Vector2f(event.mouseInput.X,event.mouseInput.Y), EOT_PLACEMENT_MARKER ) )
        {
            pScene->addGameObject(pScene->createGameObject(markerInfo));
        }
    }
    return true;
}

