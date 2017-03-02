//
//  Shape2D.h
//  Optiks
//
//  Created by Prashant on 06/09/11.
//  Copyright 2011 Sourcebits Technologies. All rights reserved.
//

#ifndef _SHAPE_2D_H_
#define _SHAPE_2D_H_

#include "IRenderable.h"
#include "ISavable.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix3.h"

#include <stdio.h>

USE_NAMESPACE_DIRAC2D

enum SHAPE_TYPE 
{
	EST_RECT, EST_CIRCLE, EST_CIRCULAR_ARC, EST_PARABOLIC_SEGMENT, EST_LENS, EST_LINE, EST_REGULAR_POLY,
    EST_NONE
};


class Ray;
struct IntersectionInformation;

struct ShapeInfo
{
    ShapeInfo()
    {
        rotation = 0.0f;
        shapeType = EST_NONE;
        m_IsReflective = true;
        m_isRefractive = false;
        m_isSolid      = false;
        m_RefractionIndex = 1.0f;
    }
    
    SHAPE_TYPE shapeType;
    Vector2f position;
    float rotation;
    
    bool m_isSolid;
    bool m_IsReflective;
    bool m_isRefractive;
    float m_RefractionIndex;
};



class Shape2D: public IRenderable, public ISavable
{
public:
	Shape2D();
	
    Shape2D( const ShapeInfo& shapeInfo);
    
	virtual bool intersectRay( Ray& ray, IntersectionInformation& intersectInfo ) = 0;
	
	virtual bool pick(const Vector2f& p ) = 0;
	
	virtual void save(FILE* pFile);
	
    virtual void load(FILE* pFile);

    virtual void render();

	void setTransform( const Matrix3f& transform )
	{
		m_Transform = transform;
		m_Transform.getInverse(m_InvTransform);
	}
	
	Matrix3f& getTransform()
	{
		return m_Transform;
	}
	
    void setPosition( const Vector2f& position )
    {
        m_Position = position;
        m_Transform.col3.x = m_Position.x;
        m_Transform.col3.y = m_Position.y;
        m_Transform.getInverse(m_InvTransform);
    }
    
    Vector2f getPosition() const
    {
        return m_Position;
    }
    
    float getRotation() const
    {
        return m_Rotation;
    }
    
	void translate(float x, float y)
	{
		Matrix3f T;
		m_Translation.x = T.col3.x = x;
		m_Translation.y = T.col3.y = y;
		m_Transform.multiply(T);
		m_Transform.getInverse(m_InvTransform);
	}

    void rotate( float angle)
    {
        m_Rotation = angle;
        m_Transform.rotate(angle);
        m_Transform.getInverse(m_InvTransform);
    }
    
    void scale( float sx, float sy)
    {
        m_Scale.x = sx;
        m_Scale.y = sy;
        m_Transform.col1.x *= sx;
        m_Transform.col2.y *= sy;
        m_Transform.getInverse(m_InvTransform);
    }
    
	void rotateAboutPoint( float angle, const Vector3f& point)
	{
		Matrix3f R, T, Tinv;
		T.col3.x = m_Translation.x + point.x;
		T.col3.y = m_Translation.y + point.y;
		Tinv.col3.x = -m_Translation.x + point.x;
		Tinv.col3.y = -m_Translation.y + point.y;
		R.rotate(angle);
		R.multiply(Tinv);
		T.multiply(R);
		m_Transform.multiply(T);
		m_Transform.getInverse(m_InvTransform);
	}
	
	bool m_bRenderable;
	bool m_bRayEntering;
	bool m_IsReflective;
	bool m_isRefractive;
	float m_RefractionIndex;
	bool m_Pickable;
	bool m_isSolid;
	
	Vector3f getReflectionFactor() const
	{
		return m_ReflectionFactor;
	}
	
	Vector3f getTransmissionFactor() const
	{
		return m_TransmissionFactor;
	}
	
	void setReflectionFactor( const Vector3f& reflectionFactor )
	{
		m_ReflectionFactor = reflectionFactor;
		assertMaterialProperties();
	}
	
	void setTransmissionFactor( const Vector3f& transmissionFactor )
	{
		m_TransmissionFactor = transmissionFactor;
		assertMaterialProperties();
	}
	
	
	SHAPE_TYPE getShapeType()
	{
		return m_ShapeType;
	}
				   
protected:
	Matrix3f m_Transform, m_InvTransform;
    Vector2f m_Position;
	float m_Area;
    float m_Rotation;
    Vector2f m_Scale;
	float matrixData[16];
	Vector3f m_Translation;
	Vector3f m_ReflectionFactor;
	Vector3f m_TransmissionFactor;
	SHAPE_TYPE m_ShapeType;
	
private:
	
	void assertMaterialProperties()
	{
		m_ReflectionFactor.x = m_ReflectionFactor.x < 0.0f ? 0.0f : m_ReflectionFactor.x;
		m_ReflectionFactor.y = m_ReflectionFactor.y < 0.0f ? 0.0f : m_ReflectionFactor.y;
		m_ReflectionFactor.z = m_ReflectionFactor.z < 0.0f ? 0.0f : m_ReflectionFactor.z;

		m_TransmissionFactor.x = m_TransmissionFactor.x < 0.0f ? 0.0f : m_TransmissionFactor.x;
		m_TransmissionFactor.y = m_TransmissionFactor.y < 0.0f ? 0.0f : m_TransmissionFactor.y;
		m_TransmissionFactor.z = m_TransmissionFactor.z < 0.0f ? 0.0f : m_TransmissionFactor.z;
		
		if( m_ReflectionFactor.x + m_TransmissionFactor.x > 1.0f )
		{
			m_ReflectionFactor.x = m_ReflectionFactor.x/(m_ReflectionFactor.x + m_TransmissionFactor.x);
			m_TransmissionFactor.x = m_TransmissionFactor.x/(m_ReflectionFactor.x + m_TransmissionFactor.x);
		}
		if( m_ReflectionFactor.y + m_TransmissionFactor.y > 1.0f )
		{
			m_ReflectionFactor.y = m_ReflectionFactor.y/(m_ReflectionFactor.y + m_TransmissionFactor.y);
			m_TransmissionFactor.y = m_TransmissionFactor.y/(m_ReflectionFactor.y + m_TransmissionFactor.y);
		}
		if( m_ReflectionFactor.z + m_TransmissionFactor.z > 1.0f )
		{
			m_ReflectionFactor.z = m_ReflectionFactor.z/(m_ReflectionFactor.z + m_TransmissionFactor.z);
			m_TransmissionFactor.z = m_TransmissionFactor.z/(m_ReflectionFactor.z + m_TransmissionFactor.z);
		}
	}
};

struct CircleInfo : public ShapeInfo
{
    CircleInfo()
    {
        shapeType = EST_CIRCLE;
        radius = 0.0f;
    }
    
    float radius;
};

class Circle : public Shape2D
{
public:
	Circle( float radius );

    Circle( const CircleInfo& circleInfo );

    void         setRadius(float);
    
    float        getRadius() const;
    
	virtual void render();
	
	virtual bool intersectRay( Ray& ray , IntersectionInformation& intersectInfo);

	virtual bool pick(const Vector2f& p );

	virtual void save(FILE* pFile);

    virtual void load(FILE* pFile);

private:
	float m_Radius;
};


struct CircularArcInfo : public ShapeInfo
{
    CircularArcInfo()
    {
        shapeType = EST_CIRCULAR_ARC;
        radius = 0.0f;
        angle  = 0.0f;
    }
    
    float radius;
    float angle;
};

class CircularArc : public Shape2D
{
public:
	CircularArc( float radius, float angle );

    CircularArc( const CircularArcInfo& circularArcInfo );

	virtual void                render();
	virtual bool                intersectRay( Ray& ray , IntersectionInformation& intersectInfo);
	virtual bool                pick(const Vector2f& p );
	virtual void                save(FILE* pFile);
    virtual void                load(FILE* pFile);
    void                        setRayExitEnable( bool bEnable );// if true then the ray which exits from
                                                                 // the Circular Arc wi
    
    int                         m_NumInsideHits;
private:
	float                       m_Radius;
    float                       m_Angle;
};

struct ParabolicSegmentInfo : public ShapeInfo
{
    ParabolicSegmentInfo()
    {
        shapeType = EST_PARABOLIC_SEGMENT;
        height = 0.0f;
        A = 0.0f;
    }
    float height;
	float A;
};

class ParabolicSegment : public Shape2D
{
public:
	ParabolicSegment( float height, float k = 1.0f );

    ParabolicSegment( const ParabolicSegmentInfo& parabolicSegmentInfo);

	virtual void render();
	
	virtual bool intersectRay( Ray& ray , IntersectionInformation& intersectInfo);
	
	virtual bool pick(const Vector2f& p );
	
	virtual void save(FILE* pFile);

    virtual void load(FILE* pFile);

private:
	float m_Height;
	float m_A;
};


struct LineSegmentInfo : public ShapeInfo
{
    LineSegmentInfo()
    {
        shapeType = EST_LINE;
        width = 0.0f;
    }
    
    float width;
};

class LineSegment : public Shape2D
{
public:
	LineSegment( float width );

    LineSegment( const LineSegmentInfo& );

	virtual void render();
	
	virtual bool intersectRay( Ray& ray , IntersectionInformation& intersectInfo);
	
	virtual bool pick(const Vector2f& p );
	
	virtual void save(FILE* pFile);

    virtual void load(FILE* pFile);

private:
	float m_Width;
};


struct RegularPolygonInfo : public ShapeInfo
{
    RegularPolygonInfo()
    {
        shapeType = EST_REGULAR_POLY;
        size = 0.0f;
    }
	float size;
};

class RegularPolygon : public Shape2D
{
public:
	RegularPolygon( float size );

    RegularPolygon( const RegularPolygonInfo& );

	virtual void render();
	
	virtual bool intersectRay( Ray& ray , IntersectionInformation& intersectInfo);
	
	virtual bool pick(const Vector2f& p );

	virtual void save(FILE* pFile);

    virtual void load(FILE* pFile);

private:
	float m_Size;
};


struct RectangleInfo : public ShapeInfo
{
    RectangleInfo()
    {
        shapeType = EST_RECT;
        width = 0.0f;
        height = 0.0f;
        m_isSolid = true;
    }
	float width;
    float height;
};

class RectangleShape : public Shape2D
{
public:
	RectangleShape( float width, float height );

    RectangleShape( const RectangleInfo& );

	virtual void render();
	
	virtual bool intersectRay( Ray& ray , IntersectionInformation& intersectInfo);
	
	virtual bool pick(const Vector2f& p );

	virtual void save(FILE* pFile);

    virtual void load(FILE* pFile);

    float getWidth()
    {
        return m_Width;
    }
    
    float getHeight()
    {
        return m_Height;
    }
    
    int getHitEdge() const
    {
        return m_EdgeHit;
    }
    
private:
    int   m_EdgeHit;
	float m_Width, m_Height;
};

struct SphericalLensInfo : public ShapeInfo
{
	float radius;
    float height;    
};

class SphericalLens : public Shape2D
{
public:
	SphericalLens( float height, float radius );

    SphericalLens( const SphericalLensInfo& );

	virtual void render();
	
	virtual bool intersectRay( Ray& ray , IntersectionInformation& intersectInfo);
	
	virtual bool pick(const Vector2f& p );

	virtual void save(FILE* pFile);

    virtual void load(FILE* pFile);

private:
	
	bool intersectSphericalFace( Ray& ray , IntersectionInformation& intersectInfo, int face);
	
	float m_Radius, m_Height;
	float m_Width;
	float Angle;
	
	Vector2f m_CoC1, m_CoC2;
};

#endif
