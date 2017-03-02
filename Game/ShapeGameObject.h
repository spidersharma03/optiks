//
//  PhysicalGameObject.h
//  Optiks
//
//  Created by Prashant Sharma on 13/10/13.
//
//

#ifndef __Optiks__PhysicalGameObject__
#define __Optiks__PhysicalGameObject__

#include "GameObject.h"
#include "Shape2D.h"
#include "MemoryBlockAllocator.h"
#include "GameSettings.h"

enum E_MATERIAL_TYPE { EMT_REFLECTIVE = 100, EMT_TRANSMISSIVE, EMT_REFRACTIVE, EMT_ABSORBITIVE, EMT_COLOR_FILTER };

struct ShapeGameObjectInfo : public GameObjectInfo
{
    ShapeGameObjectInfo()
    {
        m_ObjectType = EOT_SHAPE_OBJECT;
        bFixed = 0;
    }
    E_MATERIAL_TYPE eMaterialType;
    ShapeInfo *shapeInfo;
    Color4f m_Color;
    int bFixed;
};

class ShapeGameObject : public GameObject
{
public:
    virtual GameObject* clone();

    virtual void setTransform(const Matrix3f& transform);
    
    virtual void setPosition( const Vector2f& position );
    
    virtual void setRotation( float rotation );
    
    void         reset();

    void         increaseRayHitCount(Ray& ray);

    void         setColor( const Color4f& color )
    {
        m_Color = color;
    }
    
    Color4f getColor() const
    {
        return m_Color;
    }
    
    Shape2D* getShape2D() const
    {
        return m_pShape2D;
    }
    
    void setFixed(bool bFixed);
    
    bool isFixed() const
    {
        return m_bFixed;
    }
    
    E_MATERIAL_TYPE getMaterialType() const
    {
        return m_eMaterialType;
    }
    // GameObject virtual methods
    virtual void render();
    virtual void update(float dt);
    
    virtual bool pick(const Vector2f& point) const;
    
    virtual bool intersectRay( Ray& ray, IntersectionInformation& intersectInfo );
    
    virtual void save(FILE* pFile);
    
    virtual void load(FILE* pFile);

    virtual ~ShapeGameObject();
protected:
    
    Shape2D* createShape2D(const ShapeGameObjectInfo& shapeGameObjectInfo);
    
protected:
    
    E_MATERIAL_TYPE m_eMaterialType;
    
    ShapeGameObject(const ShapeGameObjectInfo& shapeGameObjectInfo);
    
    Shape2D *m_pShape2D;
    
    bool    m_bFixed;
    // Number of Rays reflected from the Shapes edges.
    int     m_NumRaysReflectedFromShapeEdge[MAX_SOURCES][4];
    
    Color4f  m_Color;
    
    friend class Scene;
};

#endif /* defined(__Optiks__PhysicalGameObject__) */
