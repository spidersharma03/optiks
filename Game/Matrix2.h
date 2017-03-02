/*
 *  Matrix2.h
 *  Dirac2D
 *
 *  Created by Prashant on 04/05/12.
 *
 */

#include "Vector2.h"

#ifndef _MATRIX2_H_
#define _MATRIX2_H_

BEGIN_NAMESPACE_DIRAC2D


template< class T >
class Matrix2
{
public:
	Matrix2()
	{
		a11 = 1; a12 = 0;
		a21 = 0; a22 = 1;
	}
	
	Matrix2(T a, T b, T c, T d)
	{
		a11 = a; a12 = c;
		a21 = b; a22 = d;
	}
	
	Matrix2(Vector2<T> c1, Vector2<T> c2 )
	{
		a11 = c1.x; a12 = c2.x;
		a21 = c1.y; a22 = c2.y;
	}
	
    void setZero()
    {
        a11 = a12 = a21 = a22 = (T)0;
    }
    
	inline Vector2<T> operator* ( const Vector2<T>& v ) const
	{
		return Vector2<T>(a11*v.x + a12*v.y, a21*v.x + a22*v.y);
	}
	
	inline Matrix2<T> operator* ( Matrix2<T>& other ) const
	{
		return Matrix2<T>( a11 * other*a11 + a12 * other.a21,
						   a21 * other.a11 + a22 * other.a21,
						   a11 * other.a12 + a12 * other.a22,
						   a21 * other.a12 + a22 * other.a22 );
	}
	
	inline void operator*= ( Matrix2<T>& other )
	{
		dfloat a = a11 * other.a11 + a12 * other.a21;
		dfloat b = a21 * other.a11 + a22 * other.a21;
		dfloat c = a11 * other.a12 + a12 * other.a22;
		dfloat d = a21 * other.a12 + a22 * other.a22;
		
		a11 = a;
		a21 = b;
		a12 = c;
		a22 = d;
	}

	inline void invert()
	{
		T det = a11 * a22 - a21 * a12;
		if( fabs(det) > EPSILON )
		{
			det = 1.0f/det;
		
			dfloat a11_ = a22 * det; dfloat a12_ = -a12 * det;
			dfloat a21_ = -a21 * det; dfloat a22_ = a11 * det;
			a11 = a11_; a12 = a12_;
			a21 = a21_; a22 = a22_;
		}
	}
	
	inline Matrix2<T> getInverse() const
	{
		Matrix2<T> outMatrix;
		T det = a11 * a22 - a21 * a12;
		if( fabs(det) > EPSILON )
		{
			det = 1.0f/det;
			
			outMatrix.a11 = a22 * det; outMatrix.a12 = -a12 * det;
			outMatrix.a21 = -a21 * det; outMatrix.a22 = a11 * det;
		}
		return outMatrix;
	}
	
	inline T determinant() const
	{
		return a11 * a22 - a21 * a12;
	}
	
	inline T& operator[] ( dint32 index ) const
	{
		return (&a11)[index];
	}
	
	// Solve a System like A * x = rhs. returns result in x.
	void solve( const Vector2<T>& rhs, Vector2<T>& x ) const
	{
		T det = a11 * a22 - a21 * a12;
		if( fabs(det) > EPSILON )
		{
			det = 1.0f/det;
			
			x.x = det * ( a22 * rhs.x - a12 * rhs.y );
			x.y = det * ( -a21 * rhs.x + a11 * rhs.y );
		}
	}
	
	Vector2<T> solve( const Vector2<T>& rhs ) const
	{
		Vector2<T> solution;
		T det = a11 * a22 - a21 * a12;
		if( fabs(det) > EPSILON )
		{
			det = 1.0f/det;
			
			solution.x = det * ( a22 * rhs.x - a12 * rhs.y );
			solution.y = det * ( -a21 * rhs.x + a11 * rhs.y );
		}
		return solution;
	}
	
public:	
	
	T a11, a21, a12, a22;
};

typedef Matrix2<dfloat> Matrix2f;
typedef Matrix2<dfloat> Matrix2d;

END_NAMESPACE_DIRAC2D

#endif