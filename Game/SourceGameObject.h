//
//  SourceGameObject.h
//  Optiks
//
//  Created by Prashant Sharma on 15/10/13.
//
//

#ifndef __Optiks__SourceGameObject__
#define __Optiks__SourceGameObject__


#include "GameObject.h"
#include "Ray.h"
#include <vector>

using namespace std;

struct SourceGameObjectInfo : public GameObjectInfo
{
    SourceGameObjectInfo()
    {
        m_ObjectType = EOT_LIGHT_SOURCE;
    }
    Vector2f origin;
    Vector2f direction;
    Vector2f closestGridCentre;
    Color4f color;
};

class SourceGameObject : public GameObject
{
public:
    Ray getRay() const
	{
		return m_Ray;
	}
	
	void                    setOrigin(const Vector2f& origin);
	virtual void            setClosestGridCentre(const Vector2f& centre);
	virtual Vector2f        getClosestGridCentre() const;
	
	void                    setDirection(const Vector2f& direction);
    
    virtual void            setTransform(const Matrix3f& transform);
	
    virtual void            setPosition( const Vector2f& position );
    virtual void            setRotation( float rotation );
    
    void                    setColor(const Color4f& color);
    Color4f                 getColor() const;
    
    // GameObject virtual methods
    virtual void            render();
    void                    update(float dt);

    void                    renderGlowRays(float width = 1.0f);
    
    void                    rayTraceGlowRays();
    virtual bool            pick(const Vector2f& point) const;

    virtual void            save(FILE* pFile);
    virtual void            load(FILE* pFile);

    virtual Vector2i        getGridCoordinate() const;

    int                     getSourceID() const;
    
    vector<Ray>&            getInnerGlowRays()
    {
        return m_vecInnerGlowRays;
    }
protected:

    SourceGameObject(const SourceGameObjectInfo& sourceGameObjectInfo);
    virtual ~SourceGameObject();
    
    friend class Scene;
    
    Ray                     m_Ray;
    float                   m_PickingRadius;
    Vector2f                m_ClosestGridCentre;
    Shape2D*                m_pShape2D;
    Ray                     m_SourceRay;
    vector<Ray>             m_vecInnerGlowRays;
    int                     m_NumInterReflections;
    
    static int              s_SourceCount;
    int                     m_SourceID;
    Color4f                 m_Color;
};


#endif /* defined(__Optiks__SourceGameObject__) */
