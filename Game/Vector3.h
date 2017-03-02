/*
 *  Vector3.h
 *  Dirac2D
 *
 *  Created by Prashant on 04/05/12.
 *
 */
#include "definitions.h"

#ifndef _VECTOR3_H_
#define _VECTOR3_H_

BEGIN_NAMESPACE_DIRAC2D

template< class T >
class Vector3
{
public:
	Vector3():x(0),y(0),z(0)
	{
	}
	
	Vector3( T x, T y, T z):x(x),y(y),z(z)
	{
	}
	
	Vector3(const Vector2<T>& vec2, T z):x(vec2.x),y(vec2.y),z(z)
	{
	}
	
	Vector3( T x, const Vector2<T>& vec2):x(x),y(vec2.x),z(vec2.y)
	{
	}
	
	Vector3( const Vector3<T>& other):x(other.x),y(other.y),z(other.z)
	{
	}
	
	Vector3( const Vector2<T>& other):x(other.x),y(other.y)
	{
	}
	
	inline T dot( const Vector3<T>& other) const
	{
		return x*other.x + y*other.y + z*other.z;
	}
	
	inline Vector3<T> cross( const Vector3<T>& other) const
	{
		return Vector3<T>(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
	}
	
	inline T length() const
	{
		return sqrt(x*x + y*y + z*z);
	}
	
	inline T lengthSquared() const
	{
		return (x*x + y*y + z*z);
	}
	
	inline void setZero()
	{
		x = y = z = (T)0.0;
	}
	
	void normalize()
	{
		T len = length();
		if( len > EPSILON )
		{
			len = 1.0f/len;
			x *= len;
			y *= len;
			z *= len;
		}
	}
	
	void add( const Vector3<T>& other )
	{
		x += other.x;
		y += other.y;
		z += other.z;
	}
	
	void sub( const Vector3<T>& other )
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
	}
	
	inline Vector3<T> operator-() const
	{
		return Vector3<T>(-x, -y, -z);
	}
	
	inline Vector3<T> operator+( const Vector3<T>& other ) const
	{
		return Vector3<T>(x+other.x, y+other.y, z+other.z);
	}
	
	inline Vector3<T> operator-( const Vector3<T>& other ) const
	{
		return Vector3<T>(x-other.x, y-other.y, z-other.z);
	}
	
	void operator/=( const T d )
	{
		x /= d;
		y /= d;
		z /= d;
	}
	
	void operator/=( const Vector3<T>& other )
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
	}
	
	void operator*=( const Vector3<T>& other )
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
	}
	
    Vector3<T> operator*( const T c )
	{
        Vector3<T> outVec;
		outVec.x *= c;
		outVec.y *= c;
		outVec.z *= c;
        return outVec;
	}

	void operator*=( const T d )
	{
		x *= d;
		y *= d;
		z *= d;
	}
	
	void operator+=( const Vector3& other )
	{
		x += other.x;
		y += other.y;
		z += other.z;
	}
	
	void operator-=( const Vector3& other )
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
	}
	
	bool operator==( const Vector3& other )
	{
		return (x==other.x) && (y==other.y) && (z==other.z);
	}
	
public:
	T x, y, z;
};

typedef Vector3<dfloat> Vector3f;
typedef Vector3<ddouble> Vector3d;

END_NAMESPACE_DIRAC2D

#endif
