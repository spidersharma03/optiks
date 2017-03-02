/*
 *  AABB2.h
 *  Dirac2D
 *
 *  Created by Prashant on 14/05/12.
 *
 */

#include "definitions.h"
#include "Vector2.h"

#ifndef _AABB2_H_
#define _AABB2_H_

BEGIN_NAMESPACE_DIRAC2D

template< class T >
class AABB2
{
public:
	AABB2()
	{
	}
	
	AABB2(const AABB2<T>& other)
	{
		m_LowerBounds = other.m_LowerBounds;
		m_UpperBounds = other.m_UpperBounds;
	}
	
	AABB2(const Vector2<T>& lower, const Vector2<T>& upper)
	{
		m_LowerBounds = lower;
		m_UpperBounds = upper;
	}
	
	AABB2(T lowerX, T lowerY, T upperX, T upperY)
	{
		m_LowerBounds.x = lowerX;
		m_LowerBounds.y = lowerY;

		m_UpperBounds.x = upperX;
		m_UpperBounds.y = upperY;
	}
	
	inline T getArea() const
	{
		return ( m_UpperBounds.x - m_LowerBounds.x ) * ( m_UpperBounds.y - m_LowerBounds.y );
	}
	
	inline T getPerimeter() const
	{
		return T(2.0) * ( ( m_UpperBounds.x - m_LowerBounds.x ) + ( m_UpperBounds.y - m_LowerBounds.y ) );
	}
	
	inline Vector2<T> getExtents() const
	{
		return Vector2<T>( (m_UpperBounds - m_LowerBounds) * (T)0.5);
	}

	inline Vector2<T> getCentre() const
	{
		return Vector2<T>( (m_UpperBounds + m_LowerBounds) * (T)0.5);
	}

	inline dbool contains(const AABB2<T>& other) const
	{
		dbool bRes = true;
		bRes = bRes && m_LowerBounds.x <= other.m_LowerBounds.x;
		bRes = bRes && m_LowerBounds.y <= other.m_LowerBounds.y;
		bRes = bRes && other.m_UpperBounds.x <= m_UpperBounds.x;
		bRes = bRes && other.m_UpperBounds.y <= m_UpperBounds.y;
		return bRes;
	}
	
	inline void combine( const AABB2<T>& other )
	{
		dfloat min_x = 100000.0f;
		dfloat min_y = 100000.0f;
		
		dfloat max_x = -100000.0f;
		dfloat max_y = -100000.0f;
		
		if( min_x > m_LowerBounds.x )
			min_x = m_LowerBounds.x;
		if( min_y > m_LowerBounds.y )
			min_y = m_LowerBounds.y;
			
		if( max_x < m_UpperBounds.x )
			max_x = m_UpperBounds.x;
		if( max_y < m_UpperBounds.y )
			max_y = m_UpperBounds.y;
		
		if( min_x > other.m_LowerBounds.x )
			min_x = other.m_LowerBounds.x;
		if( min_y > other.m_LowerBounds.y )
			min_y = other.m_LowerBounds.y;
		
		if( max_x < other.m_UpperBounds.x )
			max_x = other.m_UpperBounds.x;
		if( max_y < other.m_UpperBounds.y )
			max_y = other.m_UpperBounds.y;
		
		m_LowerBounds.set(min_x, min_y);
		m_UpperBounds.set(max_x, max_y);
	}
	
	inline void combine( const AABB2<T>& one, const AABB2<T>& two )
	{
		dfloat min_x = 100000.0f;
		dfloat min_y = 100000.0f;
		
		dfloat max_x = -100000.0f;
		dfloat max_y = -100000.0f;
		
		if( min_x > one.m_LowerBounds.x )
			min_x = one.m_LowerBounds.x;
		if( min_y > one.m_LowerBounds.y )
			min_y = one.m_LowerBounds.y;
		
		if( max_x < one.m_UpperBounds.x )
			max_x = one.m_UpperBounds.x;
		if( max_y < one.m_UpperBounds.y )
			max_y = one.m_UpperBounds.y;
		
		if( min_x > two.m_LowerBounds.x )
			min_x = two.m_LowerBounds.x;
		if( min_y > two.m_LowerBounds.y )
			min_y = two.m_LowerBounds.y;
		
		if( max_x < two.m_UpperBounds.x )
			max_x = two.m_UpperBounds.x;
		if( max_y < two.m_UpperBounds.y )
			max_y = two.m_UpperBounds.y;
		
		m_LowerBounds.set(min_x, min_y);
		m_UpperBounds.set(max_x, max_y);
	}
	
	inline dbool intersectAABB( const AABB2<T>& other ) const
	{
		Vector2f d1, d2;
		d1 = other.m_LowerBounds - m_UpperBounds;
		d2 = m_LowerBounds - other.m_UpperBounds;
		
		if (d1.x > 0.0f || d1.y > 0.0f)
			return false;
		
		if (d2.x > 0.0f || d2.y > 0.0f)
			return false;
		
		return true;
	}
	
public:
	Vector2<T> m_LowerBounds, m_UpperBounds;
};

typedef AABB2<dfloat> AABB2f;
typedef AABB2<ddouble> AABB2d;
typedef AABB2<dint32> AABB2i;

END_NAMESPACE_DIRAC2D

#endif