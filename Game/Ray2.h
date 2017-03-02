/*
 *  Ray2.h
 *  Dirac2D
 *
 *  Created by Prashant on 18/06/12.
 *
 */

#include "definitions.h"
#include "Vector2.h"
#include "Matrix2.h"
#include "Matrix3.h"
#include "AABB2.h" 
//#include <stdio.h>

#ifndef _RAY2_H_
#define _RAY2_H_

BEGIN_NAMESPACE_DIRAC2D

struct RayIntersectionInfo
{
	dfloat m_HitT;
	Vector2f m_HitNormal;
	Vector2f m_HitPoint;
};

template< class T >
class Ray2
{
public:
	Ray2()
	{
	}
	
	Ray2(const Ray2<T>& other)
	{
		m_Origin = other.m_Origin;
		m_Direction = other.m_Direction;
		m_InvDirection = Vector2<T>(1.0/m_Direction.x, 1.0/m_Direction.y);
	}
	
	Ray2(Vector2<T>& origin, Vector2<T>& direction)
	{
		m_Origin = origin;
		m_Direction = direction;
		m_InvDirection = Vector2<T>(1.0/m_Direction.x, 1.0/m_Direction.y);
	}
	
	inline void set(Ray2<T>& other)
	{
		m_Origin = other.m_Origin;
		m_Direction = other.m_Direction;
		m_InvDirection = Vector2<T>(1.0/m_Direction.x, 1.0/m_Direction.y);
	}

	inline void set(Vector2<T>& origin, Vector2<T>& direction)
	{
		m_Origin = origin;
		m_Direction = direction;
		m_InvDirection = Vector2<T>(1.0/m_Direction.x, 1.0/m_Direction.y);
	}
	
#define RAYAABB_EPSILON 1e-06
	// Clip Plane Based Implementation
	inline dbool intersectAABB(const AABB2<T>& aabb, float* tmin, float* tmax, int* bRayInside) const
	{
		dbool bInside = true;
		Vector2<T>& MinB = aabb.m_LowerBounds;
		Vector2<T>& MaxB = aabb.m_UpperBounds;
		dfloat MaxTx, MaxTy;
		dfloat MinTx, MinTy;
		
		
		MaxTx = MaxTy = -10000000.0f;
		MinTx = MinTy =  10000000.0f;
		
		// X DIRECTION
		if( m_Origin.x < MinB.x )
		{
			bInside		= false;
			if(m_Direction.x)	
			{
				MinTx = (MaxB.x - m_Origin.x) * m_InvDirection.x;
				MaxTx = (MinB.x - m_Origin.x) * m_InvDirection.x;
			}
		}
		else if( m_Origin.x > MaxB.x )
		{
			bInside		= false;
			if(m_Direction.x)
			{
				MaxTx = (MaxB.x - m_Origin.x) * m_InvDirection.x;
				MinTx = (MinB.x - m_Origin.x) * m_InvDirection.x;
			}
		}
		
		// Y DIRECTION
		if(m_Origin.y < MinB.y)
		{
			bInside		= false;
			if(m_Direction.y)	
			{
				MaxTy = (MinB.y - m_Origin.y) * m_InvDirection.y;
				MinTy = (MaxB.y - m_Origin.y) * m_InvDirection.y;
			}
		}
		else if(m_Origin.y > MaxB.y)
		{
			bInside		= false;
			// Calculate T distances to candidate planes
			if(m_Direction.y)
			{
				MaxTy = (MaxB.y - m_Origin.y) * m_InvDirection.y;
				MinTy = (MinB.y - m_Origin.y) * m_InvDirection.y;
			}
		}
		
		// Get largest of the maxT's for final choice of intersection
		dfloat Largest = MaxTx;
		if(MaxTy > Largest)	Largest = MaxTy;
		
		dfloat Smallest = MinTx;
		if(MinTy < Smallest)	Smallest = MinTy;
		
		*tmin = Largest;
		*tmax = Smallest;
		
		// Ray origin inside bounding box
		if(bInside)
		{
			*bRayInside = 1;
			// X DIRECTION
			if(m_Direction.x > 0.0f)	
			{
				MinTx = (MaxB.x - m_Origin.x) * m_InvDirection.x;
				MaxTx = (MinB.x - m_Origin.x) * m_InvDirection.x;
			}
			else
			{
				MaxTx = (MaxB.x - m_Origin.x) * m_InvDirection.x;
				MinTx = (MinB.x - m_Origin.x) * m_InvDirection.x;
			}
			
			// Y DIRECTION
			if(m_Direction.y > 0.0f)	
			{
				MaxTy = (MinB.y - m_Origin.y) * m_InvDirection.y;
				MinTy = (MaxB.y - m_Origin.y) * m_InvDirection.y;
			}
			else
			{
				MaxTy = (MaxB.y - m_Origin.y) * m_InvDirection.y;
				MinTy = (MinB.y - m_Origin.y) * m_InvDirection.y;
			}
			
			Largest = MaxTx;
			if(MaxTy > Largest)	Largest = MaxTy;
			
			Smallest = MinTx;
			if(MinTy < Smallest)	Smallest = MinTy;
			
			*tmin = Largest;
			*tmax = Smallest;
			return true;
		}
		
		*bRayInside = 0;
				
		float outx = m_Origin.x + Largest * m_Direction.x;
		if( (outx < MinB.x - RAYAABB_EPSILON) || (outx > MaxB.x + RAYAABB_EPSILON) )	return false;
		
		float outy = m_Origin.y + Largest * m_Direction.y;
		if( (outy < MinB.y - RAYAABB_EPSILON) || (outy > MaxB.y + RAYAABB_EPSILON) )	return false;
		
		return true;	// ray hits box
	}
	
public:
	Vector2<T> m_Origin;
	Vector2<T> m_Direction;
private:
	Vector2<T> m_InvDirection;
};


template< class T >
class RaySegment
{
public:
	RaySegment()
	{
	}
	
	RaySegment(const RaySegment<T>& other)
	{
		m_Start = other.m_Start;
		m_End = other.m_End;
	}
	
	RaySegment(const Vector2<T>& start, const Vector2<T>& end)
	{
		m_Start = start;
		m_End = end;
	}
	
	inline void set(const Ray2<T>& other)
	{
		m_Start = other.m_Start;
		m_End = other.m_End;
	}
	
	inline void set(const Vector2<T>& start, const Vector2<T>& end)
	{
		m_Start = start;
		m_End = end;
	}
	
/*
  This uses the SAT for the intersection check between Ray Segment and AABB.
  the implementation is done as described in gino's book "Collision Detecton in
   Interactive 3D Environments". page 79-80
*/
	inline dbool intersectAABB(const AABB2<T>& aabb) const
	{
		AABB2<T> raySegAABB;
		T minx = m_Start.x < m_End.x ? m_Start.x : m_End.x;
		T miny = m_Start.y < m_End.y ? m_Start.y : m_End.y;
		T maxx = m_Start.x > m_End.x ? m_Start.x : m_End.x;
		T maxy = m_Start.y > m_End.y ? m_Start.y : m_End.y;

		raySegAABB.m_LowerBounds.set(minx, miny);
		raySegAABB.m_UpperBounds.set(maxx, maxy);
		
		// This is SAT test for AABB's seperating Axis list( X and Y)
		if( !raySegAABB.intersectAABB( aabb ) )
			return false;

		// This is SAT test for the RaySegments seperating Axis
		Vector2f satAxis(m_Start.y - m_End.y, m_End.x - m_Start.x);
		Vector2f absSatAxis =  Vector2f( fabs(satAxis.x), fabs(satAxis.y ) );

		//dfloat a = fabs( satAxis.dot( raySegAABB.m_LowerBounds - aabb.getCentre() ) );
		dfloat b = absSatAxis.dot(aabb.getExtents());
		Vector2f centre = aabb.getCentre();
		
		// Projection interval of the aabb.
		dfloat dot = satAxis.dot(centre);
		dfloat aabbMin = dot - b;
		dfloat aabbMax = dot + b;
		
		dfloat a3 = satAxis.dot(m_Start);
		
		//printf("Interval = %f  %f  %f\n", aabbMin, aabbMax, a3);
		return (a3 < aabbMax && a3 > aabbMin);
	}
	
	/*
	 Clip Plane Based Implementation
	 */
	inline dbool intersectAABB(const AABB2<T>& aabb, dfloat& tmin, dfloat& tmax, dchar& bRayIndside) const
	{
		dbool bInside = true;
		const Vector2<T>& MinB = aabb.m_LowerBounds;
		const Vector2<T>& MaxB = aabb.m_UpperBounds;
		dfloat MaxTx, MaxTy;
		dfloat MinTx, MinTy;
		
		MaxTx = MaxTy = -10000000.0f;
		MinTx = MinTy =  10000000.0f;
		
		Vector2f rayDirection = m_End - m_Start;
		rayDirection.normalize();
		
		// X DIRECTION
		if( m_Start.x < MinB.x )
		{
			bInside		= false;
			if(rayDirection.x)	
			{
				MinTx = (MaxB.x - m_Start.x) / rayDirection.x;
				MaxTx = (MinB.x - m_Start.x) / rayDirection.x;
			}
		}
		else if( m_Start.x > MaxB.x )
		{
			bInside		= false;
			if(rayDirection.x)
			{
				MaxTx = (MaxB.x - m_Start.x) / rayDirection.x;
				MinTx = (MinB.x - m_Start.x) / rayDirection.x;
			}
		}
		
		// Y DIRECTION
		if(m_Start.y < MinB.y)
		{
			bInside		= false;
			if(rayDirection.y)	
			{
				MaxTy = (MinB.y - m_Start.y) / rayDirection.y;
				MinTy = (MaxB.y - m_Start.y) / rayDirection.y;
			}
		}
		else if(m_Start.y > MaxB.y)
		{
			bInside		= false;
			// Calculate T distances to candidate planes
			if(rayDirection.y)
			{
				MaxTy = (MaxB.y - m_Start.y) / rayDirection.y;
				MinTy = (MinB.y - m_Start.y) / rayDirection.y;
			}
		}
		
		// Get largest of the maxT's for final choice of intersection
		dfloat Largest = MaxTx;
		if(MaxTy > Largest)	Largest = MaxTy;
		
		dfloat Smallest = MinTx;
		if(MinTy < Smallest)	Smallest = MinTy;
		
		tmin = Largest;
		tmax = Smallest;
		
		if( tmax < tmin )
			return false;

		// Ray origin inside bounding box
		if(bInside)
		{
			bRayIndside = 1;
			// X DIRECTION
			if(rayDirection.x > 0.0f)	
			{
				MinTx = (MaxB.x - m_Start.x) / rayDirection.x;
				MaxTx = (MinB.x - m_Start.x) / rayDirection.x;
			}
			else
			{
				MaxTx = (MaxB.x - m_Start.x) / rayDirection.x;
				MinTx = (MinB.x - m_Start.x) / rayDirection.x;
			}
			
			// Y DIRECTION
			if(rayDirection.y > 0.0f)	
			{
				MaxTy = (MinB.y - m_Start.y) / rayDirection.y;
				MinTy = (MaxB.y - m_Start.y) / rayDirection.y;
			}
			else
			{
				MaxTy = (MaxB.y - m_Start.y) / rayDirection.y;
				MinTy = (MinB.y - m_Start.y) / rayDirection.y;
			}
			
			Largest = MaxTx;
			if(MaxTy > Largest)	Largest = MaxTy;
			
			Smallest = MinTx;
			if(MinTy < Smallest)	Smallest = MinTy;
			
			tmin = Largest;
			tmax = Smallest;
			return true;
		}
		
		bRayIndside = 0;
		
		float outx = m_Start.x + Largest * rayDirection.x;
		if( (outx < MinB.x - RAYAABB_EPSILON) || (outx > MaxB.x + RAYAABB_EPSILON) )	return false;
		
		float outy = m_Start.y + Largest * rayDirection.y;
		if( (outy < MinB.y - RAYAABB_EPSILON) || (outy > MaxB.y + RAYAABB_EPSILON) )	return false;
		
		return true;	// ray hits box
	}

public:
	Vector2<T> m_Start;
	Vector2<T> m_End;
};

typedef Ray2<dfloat> Ray2f;
typedef Ray2<ddouble> Ray2d;

typedef RaySegment<dfloat> RaySegment2f;
typedef RaySegment<ddouble> RaySegment2d;

END_NAMESPACE_DIRAC2D

#endif