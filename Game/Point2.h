/*
 *  Point2.h
 *  Dirac2D
 *
 *  Created by Prashant on 28/05/12.
 *
 */

#include "definitions.h"

#ifndef _POINT2_H_
#define _POINT2_H_

BEGIN_NAMESPACE_DIRAC2D

template< class T >
class Point2
{
public:
	Point2():x(0),y(0)
	{
	}
	
	Point2( T x, T y):x(x),y(y)
	{
	}
	
	Point2( const Point2<T>& other):x(other.x),y(other.y)
	{
	}
	
	Point2<T> operator-() const
	{
		return Point2<T>(-x, -y);
	}
	
	Point2<T> operator+( const Point2<T>& other ) const
	{
		return Point2<T>(x+other.x, y+other.y);
	}
	
	Point2<T> operator-( const Point2<T>& other ) const
	{
		return Point2<T>(x-other.x, y-other.y);
	}
	
	inline void set( T inx, T iny)
	{
		x = inx; y = iny;
	}
	
	void operator/=( const T d )
	{
		x /= d;
		y /= d;
	}
	
	void operator/=( const Point2<T>& other )
	{
		x /= other.x;
		y /= other.y;
	}
	
	void operator*=( const Point2<T>& other )
	{
		x *= other.x;
		y *= other.y;
	}
	
	void operator*=( const T d )
	{
		x *= d;
		y *= d;
	}
	
	void operator+=( const Point2<T>& other )
	{
		x += other.x;
		y += other.y;
	}
	
	void operator-=( const Point2<T>& other )
	{
		x -= other.x;
		y -= other.y;
	}
	
	bool operator==( const Point2<T>& other )
	{
		return (x==other.x) && (y==other.y);
	}
	
	inline T EuclidiandDistance( Point2<T>& other ) const
	{
		return sqrt( (x-other.x)*(x-other.x) + (y-other.y)*(y-other.y) );
	}
	
	inline T EuclidiandDistanceSquared( Point2<T>& other ) const
	{
		return (x-other.x)*(x-other.x) + (y-other.y)*(y-other.y);
	}
	
	inline T ManhattendDistance( Point2<T>& other ) const
	{
		return fabs(x-other.x) + fabs(y-other.y);
	}
	
public:
	T x,y;
};


typedef Point2<dfloat> Point2f;
typedef Point2<ddouble> Point2d;


END_NAMESPACE_DIRAC2D

#endif