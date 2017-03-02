//
//  EditorObjectCreateState.h
//  Optiks
//
//  Created by Prashant Sharma on 07/11/13.
//
//

#ifndef __Optiks__EditorObjectCreateState__
#define __Optiks__EditorObjectCreateState__

#include <iostream>
#include "IState.h"

class Game;

class EditorObjectCreateState : public IState<Game>
{
public:
    //this will execute when the state is entered
    virtual void            Enter(Game* pOwner);
    
    //this is the states normal update function
    virtual void            Execute(Game* pOwner);
    
    //this will execute when the state is exited.
    virtual void            Exit(Game* pOwner);
    
    virtual sbool           OnEvent( Event& event);
    
protected:
    
    EditorObjectCreateState();    
};

class EditorRectShapeCreateState : public EditorObjectCreateState
{
public:
    //this will execute when the state is entered
    virtual void            Enter(Game* pOwner);
    
    //this is the states normal update function
    virtual void            Execute(Game* pOwner);
    
    //this will execute when the state is exited.
    virtual void            Exit(Game* pOwner);
    
    virtual sbool           OnEvent( Event& event);
    
    static EditorRectShapeCreateState* getInstance()
    {
        if( !s_pEditorRectShapeCreateState )
        {
            s_pEditorRectShapeCreateState = new EditorRectShapeCreateState();
        }
        return s_pEditorRectShapeCreateState;
    }
    
protected:
    
    EditorRectShapeCreateState();
    
    static EditorRectShapeCreateState   *s_pEditorRectShapeCreateState;
};

class EditorAbsorberRectShapeCreateState : public EditorObjectCreateState
{
public:
    //this will execute when the state is entered
    virtual void            Enter(Game* pOwner);
    
    //this is the states normal update function
    virtual void            Execute(Game* pOwner);
    
    //this will execute when the state is exited.
    virtual void            Exit(Game* pOwner);
    
    virtual sbool           OnEvent( Event& event);
    
    static EditorAbsorberRectShapeCreateState* getInstance()
    {
        if( !s_pEditorAbsorberRectShapeCreateState )
        {
            s_pEditorAbsorberRectShapeCreateState = new EditorAbsorberRectShapeCreateState();
        }
        return s_pEditorAbsorberRectShapeCreateState;
    }
    
protected:
    
    EditorAbsorberRectShapeCreateState();
    
    static EditorAbsorberRectShapeCreateState   *s_pEditorAbsorberRectShapeCreateState;
};

class EditorTransmissiveRectShapeCreateState : public EditorObjectCreateState
{
public:
    //this will execute when the state is entered
    virtual void            Enter(Game* pOwner);
    
    //this is the states normal update function
    virtual void            Execute(Game* pOwner);
    
    //this will execute when the state is exited.
    virtual void            Exit(Game* pOwner);
    
    virtual sbool           OnEvent( Event& event);
    
    static EditorTransmissiveRectShapeCreateState* getInstance()
    {
        if( !s_pEditorTransmissiveRectShapeCreateState )
        {
            s_pEditorTransmissiveRectShapeCreateState = new EditorTransmissiveRectShapeCreateState();
        }
        return s_pEditorTransmissiveRectShapeCreateState;
    }
    
protected:
    
    EditorTransmissiveRectShapeCreateState();
    
    static EditorTransmissiveRectShapeCreateState   *s_pEditorTransmissiveRectShapeCreateState;
};

class EditorColorFilterRectShapeCreateState : public EditorObjectCreateState
{
public:
    //this will execute when the state is entered
    virtual void            Enter(Game* pOwner);
    
    //this is the states normal update function
    virtual void            Execute(Game* pOwner);
    
    //this will execute when the state is exited.
    virtual void            Exit(Game* pOwner);
    
    virtual sbool           OnEvent( Event& event);
    
    static EditorColorFilterRectShapeCreateState* getInstance()
    {
        if( !s_pEditorColorFilterRectShapeCreateState )
        {
            s_pEditorColorFilterRectShapeCreateState = new EditorColorFilterRectShapeCreateState();
        }
        return s_pEditorColorFilterRectShapeCreateState;
    }
    
protected:
    
    EditorColorFilterRectShapeCreateState();
    
    static EditorColorFilterRectShapeCreateState   *s_pEditorColorFilterRectShapeCreateState;
};

class EditorRefractiveRectShapeCreateState : public EditorObjectCreateState
{
public:
    //this will execute when the state is entered
    virtual void            Enter(Game* pOwner);
    
    //this is the states normal update function
    virtual void            Execute(Game* pOwner);
    
    //this will execute when the state is exited.
    virtual void            Exit(Game* pOwner);
    
    virtual sbool           OnEvent( Event& event);
    
    static EditorRefractiveRectShapeCreateState* getInstance()
    {
        if( !s_pEditorRefractiveRectShapeCreateState )
        {
            s_pEditorRefractiveRectShapeCreateState = new EditorRefractiveRectShapeCreateState();
        }
        return s_pEditorRefractiveRectShapeCreateState;
    }
    
protected:
    
    EditorRefractiveRectShapeCreateState();
    
    static EditorRefractiveRectShapeCreateState   *s_pEditorRefractiveRectShapeCreateState;
};

class EditorSourceCreateState : public EditorObjectCreateState
{
public:
    //this will execute when the state is entered
    virtual void            Enter(Game* pOwner);
    
    //this is the states normal update function
    virtual void            Execute(Game* pOwner);
    
    //this will execute when the state is exited.
    virtual void            Exit(Game* pOwner);
    
    virtual sbool           OnEvent( Event& event);
    
    static EditorSourceCreateState* getInstance()
    {
        if( !s_pEditorSourceCreateState )
        {
            s_pEditorSourceCreateState = new EditorSourceCreateState();
        }
        return s_pEditorSourceCreateState;
    }
    
protected:
    
    EditorSourceCreateState();
    
    static EditorSourceCreateState   *s_pEditorSourceCreateState;
};

class EditorPortalCreateState : public EditorObjectCreateState
{
public:
    //this will execute when the state is entered
    virtual void            Enter(Game* pOwner);
    
    //this is the states normal update function
    virtual void            Execute(Game* pOwner);
    
    //this will execute when the state is exited.
    virtual void            Exit(Game* pOwner);
    
    virtual sbool           OnEvent( Event& event);
    
    static EditorPortalCreateState* getInstance()
    {
        if( !s_pEditorPortalCreateState )
        {
            s_pEditorPortalCreateState = new EditorPortalCreateState();
        }
        return s_pEditorPortalCreateState;
    }
    
protected:
    
    EditorPortalCreateState();
    
    static EditorPortalCreateState   *s_pEditorPortalCreateState;
};


class EditorSinkCreateState : public EditorObjectCreateState
{
public:
    //this will execute when the state is entered
    virtual void            Enter(Game* pOwner);
    
    //this is the states normal update function
    virtual void            Execute(Game* pOwner);
    
    //this will execute when the state is exited.
    virtual void            Exit(Game* pOwner);
    
    virtual sbool           OnEvent( Event& event);
    
    static EditorSinkCreateState* getInstance()
    {
        if( !s_pEditorSinkCreateState )
        {
            s_pEditorSinkCreateState = new EditorSinkCreateState();
        }
        return s_pEditorSinkCreateState;
    }
    
protected:
    
    EditorSinkCreateState();
    
    static EditorSinkCreateState   *s_pEditorSinkCreateState;
};


class EditorPlacementMarkerCreateState : public EditorObjectCreateState
{
public:
    //this will execute when the state is entered
    virtual void            Enter(Game* pOwner);
    
    //this is the states normal update function
    virtual void            Execute(Game* pOwner);
    
    //this will execute when the state is exited.
    virtual void            Exit(Game* pOwner);
    
    virtual sbool           OnEvent( Event& event);
    
    static EditorPlacementMarkerCreateState* getInstance()
    {
        if( !s_pEditorPlacementMarkerCreateState )
        {
            s_pEditorPlacementMarkerCreateState = new EditorPlacementMarkerCreateState();
        }
        return s_pEditorPlacementMarkerCreateState;
    }
    
protected:
    
    EditorPlacementMarkerCreateState();
    
    static EditorPlacementMarkerCreateState   *s_pEditorPlacementMarkerCreateState;
};
#endif /* defined(__Optiks__EditorObjectCreateState__) */
