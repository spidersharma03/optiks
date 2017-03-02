/*
 *  GameObject.h
 *
 *  Created by Prashant on 30/10/12.
 *
 */


#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include "MathUtil.h"
#include "ISavable.h"
#include "GameSettings.h"

USE_NAMESPACE_DIRAC2D

enum GAME_OBJECT_TYPE { EOT_LIGHT_SOURCE = 1, EOT_SHAPE_OBJECT, EOT_SINK, EOT_PLACEMENT_MARKER, EOT_PORTAL, EOT_DECOR, EOT_NONE };

enum COLOR_CODE { ECC_WHITE, ECC_RED, ECC_GREEN, ECC_BLUE };

enum GAME_OBJECT_SHAPE_TYPE
{
	EGS_RECT, EGS_CIRCLE, EGS_CIRCULAR_ARC, EGS_PARABOLIC_SEGMENT, EGS_LENS, EGS_LINE, EGS_REGULAR_POLY, EGS_COMPOUND
};


struct GameObjectInfo
{
	GameObjectInfo()
	{
        m_ObjectType = EOT_NONE;
        m_Rotation = 0.0f;
        m_ObjectTextureID = -1;
	}
	
    Vector2f m_Position;
    float    m_Rotation;
	GAME_OBJECT_TYPE m_ObjectType;
    int              m_ObjectTextureID;
    int              m_ObjectID;
};

class Texture;

class GameObject : ISavable
{
public:
    virtual GameObject* clone()
    {
        return 0;
    }
    
	inline Matrix3f getTransform() const
	{
		return m_Transform;
	}
	
    virtual void setTransform(const Matrix3f& transform)
	{
        m_Transform = transform;
	}

    virtual void setPosition( const Vector2f& position )
    {
        m_Transform.translate(position);
        m_Position = position;
    }
    
    virtual void setRotation( float rotation )
    {
        m_Rotation = rotation;
    }
    
    virtual Vector2i getGridCoordinate() const;
    
    inline float getRotation() const
    {
        return m_Rotation;
    }
    
	inline Vector2f getPosition() const
	{
		return m_Position;
	}
	
    inline int getZOrder() const
    {
        return m_zOrder;
    }
    
	inline const GameObjectInfo& getGameObjectInfo() const
	{
		return m_ObjectInfo;
	}
    
    void setVisible( bool bVisible )
    {
        m_bVisible = bVisible;
    }
    
    bool getVisible() const
    {
        return m_bVisible;
    }

    COLOR_CODE getColorCode() const 
	{
		return m_ColorCode;
	}
    
    void setTexture( Texture* pTexture )
    {
        m_pTexture = pTexture;
    }
    
    Texture* getTexture() const
    {
        return m_pTexture;
    }
    
    Texture* getNormalMapTexture() const
    {
        return m_pNormalMapTexture;
    }
    
    virtual void render();
    
	virtual void update(float dt){};
    
    virtual bool pick(const Vector2f& point) const
    {
        return false;
    }
    
    virtual ~GameObject()
    {
        m_ObjectCount--;
    }
	
    virtual void setClosestGridCentre(const Vector2f& centre)
    {
        
    }
    
    virtual Vector2f getClosestGridCentre() const
    {
        return m_Position;
    }
    
    int            getID() const
    {
        return m_ID;
    }
    // ISavable virtual methods
    
    virtual void save(FILE* pFile)
    {
        printf("Not yet Implemented\n");
    }

    virtual void load(FILE* pFile)
    {
        printf("Not yet Implemented\n");
    }

protected:
        
    GameObject(const GameObjectInfo& gInfo)
    {
        m_ObjectInfo = gInfo;
        m_Position = gInfo.m_Position;
        m_Rotation = gInfo.m_Rotation;
		m_pTexture = 0;
        m_pNormalMapTexture = 0;
        m_bVisible = true;
		m_ObjectCount++;
        m_zOrder = -1;
        m_ID = m_ObjectCount;
        m_bSetByHint = false;
    }

	Matrix3f m_Transform;
	Vector2f m_Position;
    float    m_Rotation;

	GameObjectInfo m_ObjectInfo;
	static int m_ObjectCount;
    
    COLOR_CODE m_ColorCode;

    bool    m_bVisible;
    Texture *m_pTexture;
    Texture *m_pNormalMapTexture;
    
    int     m_zOrder;
    int     m_ID;
    bool    m_bSetByHint;
};



#endif