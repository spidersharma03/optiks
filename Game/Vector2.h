/*
 *  Vector2.h
 *  Dirac2D
 *
 *  Created by Prashant on 04/05/12.
 *
 */
#include "definitions.h"
#include "math.h"

#ifndef _VECTOR2_H_
#define _VECTOR2_H_

BEGIN_NAMESPACE_DIRAC2D


template< class T > class Matrix2;
template< class T > class Matrix3;

template< class T >
class Vector2
{
public:
	Vector2():x(0),y(0)
	{
	}
	
	Vector2( T x, T y):x(x),y(y)
	{
	}
	
	Vector2( const Vector2<T>& other):x(other.x),y(other.y)
	{
	}
	
	inline void operator=(const Vector2<T> other)
	{
		x = other.x;
		y = other.y;
	}

	inline T dot( const Vector2<T>& other) const
	{
		return x*other.x + y*other.y;
	}

	inline T cross( const Vector2<T>& other ) const
	{
		return x*other.y - y*other.x;
	}
	
	inline Vector2<T> cross( const T d ) const
	{
		return Vector2<T>(d*y, -d*x);
	}
	
	static inline Vector2<T> cross( const Vector2<T>& v, const T d )
	{
		return Vector2<T>(d * v.y, -d * v.x);
	}
	
	static inline Vector2<T> cross( const T d, const Vector2<T>& v )
	{
		return Vector2<T>(-d * v.y, d * v.x);
	}
	
	static inline T cross( const Vector2<T>& v1, const Vector2<T>& v2 )
	{
		return  v1.x * v2.y - v1.y * v2.x;
	}
	
	inline T distanceSquared( const Vector2<T>& other) const
	{
		T dx = x-other.x;
		T dy = y-other.y;
		return  dx*dx + dy*dy;
	}
	
	inline T distance( const Vector2<T>& other) const
	{
		T dx = x-other.x;
		T dy = y-other.y;
		return  sqrt(dx*dx + dy*dy);
	}
	
	static inline T distanceSquared( const Vector2<T>& v1, const Vector2<T>& v2)
	{
		T dx = v1.x-v2.x;
		T dy = v1.y-v2.y;
		return  dx*dx + dy*dy;
	}
	
	static inline T distance( const Vector2<T>& v1, const Vector2<T>& v2)
	{
		T dx = v1.x-v2.x;
		T dy = v1.y-v2.y;
		return  sqrt(dx*dx + dy*dy);
	}
	
	inline T length() const
	{
		return sqrt(x*x + y*y);
	}
	
	inline T lengthSquared() const
	{
		return (x*x + y*y);
	}
	
	inline void normalize()
	{
		T len = length();
		if( len > EPSILON )
		{
			len = 1.0f/len;
			x *= len;
			y *= len;
		}
	}
	
	inline void setZero()
	{
		x = y = (T)0.0;
	}
	
	inline void set( const T inx, const T iny)
	{
		x = inx; y = iny;
	}
	
	inline void set( const Vector2<T>& other )
	{
		x = other.x; y = other.y;
	}
	
	inline void add( const Vector2<T>& other )
	{
		x += other.x;
		y += other.y;
	}
	
	inline void sub( const Vector2<T>& other )
	{
		x -= other.x;
		y -= other.y;
	}
	
	inline Vector2<T> operator-() const
	{
		return Vector2<T>(-x, -y);
	}
	
	inline Vector2<T> operator+( const Vector2<T>& other ) const
	{
		return Vector2<T>(x+other.x, y+other.y);
	}
	
	inline Vector2<T> operator-( const Vector2<T>& other ) const
	{
		return Vector2<T>(x-other.x, y-other.y);
	}
	
	inline Vector2<T> operator*( const Vector2<T>& other ) const
	{
		return Vector2<T>(x*other.x, y*other.y);
	}
	
	// Right Multiply By a 2x2 Matrix( this is same as out = AT * v )
	inline Vector2<T> operator*( const Matrix2<T>& matrix ) const
	{
		return Vector2<T>(matrix.a11*x + matrix.a21*y, matrix.a12*x + matrix.a22*y);
	}
	
	// Right Multiply By a 3x3 Matrix( this is same as out = AInv * v )
	inline Vector2<T> operator*( const Matrix3<T>& matrix ) const
	{
		dfloat tx  = -matrix.col1.x * matrix.col3.x - matrix.col1.y * matrix.col3.y;
		dfloat ty  = -matrix.col2.x * matrix.col3.x - matrix.col2.y * matrix.col3.y;
		
		return Vector2<T>(matrix.col1.x * x + matrix.col1.y * y + tx , matrix.col2.x * x + matrix.col2.y * y + ty );
	}
	
	// Right Multiply By a 2x2 Matrix( this is same as this = AT * v )
	inline void operator*=( const Matrix2<T>& matrix )
	{
		dfloat tx = matrix.a11 * x + matrix.a21 * y;
		y = matrix.a12 * x + matrix.a22 * y;
		x = tx;
	}
	
	// Right Multiply By a 3x3 Matrix( this is same as this = AInv * v )
	inline void operator*=( const Matrix3<T>& matrix )
	{
		dfloat tx  = -matrix.col1.x * matrix.col3.x - matrix.col1.y * matrix.col3.y;
		dfloat ty  = -matrix.col2.x * matrix.col3.x - matrix.col2.y * matrix.col3.y;
		dfloat temp = matrix.col1.x * x + matrix.col1.y * y + tx;
		y = matrix.col2.x * x + matrix.col2.y * y + ty;
		x = temp;
	}
	
	inline Vector2<T> operator*( const T d ) const
	{
		return Vector2<T>(x*d, y*d);
	}
	
	inline Vector2<T> operator/( const T d ) const
	{
		return Vector2<T>(x/d, y/d);
	}
	
	inline void operator/=( const T d )
	{
		x /= d;
		y /= d;
	}
	
	inline Vector2<T> operator/( const Vector2<T>& other ) const
	{
		return Vector2<T>(x/other.x, y/other.y);
	}
	
	inline void operator/=( const Vector2<T>& other )
	{
		x /= other.x;
		y /= other.y;
	}
	
	inline void operator*=( const Vector2<T>& other )
	{
		x *= other.x;
		y *= other.y;
	}
	
	inline void operator*=( const T d )
	{
		x *= d;
		y *= d;
	}
	
	inline void operator+=( const Vector2<T>& other )
	{
		x += other.x;
		y += other.y;
	}
	
	inline void operator-=( const Vector2<T>& other )
	{
		x -= other.x;
		y -= other.y;
	}
	
	inline bool operator==( const Vector2<T>& other ) const
	{
		return (x==other.x) && (y==other.y);
	}

	inline bool operator<( const Vector2<T>& other ) const
	{
		return (x<other.x) && (y<other.y);
	}
	
public:
	T x, y;
};


typedef Vector2<dfloat> Vector2f;
typedef Vector2<ddouble> Vector2d;
typedef Vector2<dint32> Vector2i;


inline Vector2f operator* ( dfloat a, const Vector2f& v )
{
	return Vector2f(a*v.x, a*v.y);
}

inline Vector2d operator* ( dfloat a, const Vector2d& v )
{
	return Vector2d(a*v.x, a*v.y);
}

END_NAMESPACE_DIRAC2D

#endif