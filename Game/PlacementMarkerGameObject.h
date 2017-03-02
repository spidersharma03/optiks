//
//  PlacementMarkerGameObject.h
//  Optiks
//
//  Created by Prashant Sharma on 11/11/13.
//
//

#ifndef __Optiks__PlacementMarkerGameObject__
#define __Optiks__PlacementMarkerGameObject__

#include <iostream>
#include "GameObject.h"

struct PlacementMarkerGameObjectInfo : public GameObjectInfo
{
    PlacementMarkerGameObjectInfo()
    {
        m_ObjectType = EOT_PLACEMENT_MARKER;
        width        = 0.0f;
        height       = 0.0f;
    }
    float width;
    float height;
};

class PlacementMarkerGameObject : public GameObject
{
public:
        
    // GameObject virtual methods
    virtual void render();

    virtual void update(float dt);
    
    virtual bool pick(const Vector2f& point) const;
    
    virtual void save(FILE* pFile);
    
    virtual void load(FILE* pFile);
        
    virtual ~PlacementMarkerGameObject();

    float   getWidth() const
    {
        return m_Width;
    }
    
    float   getHeight() const
    {
        return m_Height;
    }
    
protected:
    
    PlacementMarkerGameObject( const PlacementMarkerGameObjectInfo& vInfo );
    
    friend class Scene;
    
    GameObject          *m_pGameObject;
protected:
    float               m_Width;
    float               m_Height;    
};

#endif /* defined(__Optiks__PlacementMarkerGameObject__) */
