/*
 *  Matrix3.h
 *  Dirac2D
 *
 *  Created by Prashant on 04/05/12.
 *
 */

#include "Vector2.h"
#include "Vector3.h"
#include <math.h>

#ifndef _MATRIX3_H_
#define _MATRIX3_H_

BEGIN_NAMESPACE_DIRAC2D

template< class T >
class Matrix3
{
public:
	Matrix3()
	{
		setIdentity();
	}
	
	Matrix3(const Matrix3<T>& other)
	{
		col1 = other.col1;
		col2 = other.col2;
		col3 = other.col3;
	}
	
	inline void rotate(const T angle)
	{
        float angleInRad = angle * M_PI/180.0f;
		T cs = cos(angleInRad); T sn = sin(angleInRad);
		col1.x = cs; col2.x = -sn;
		col1.y = sn; col2.y = cs;
	}
	
	inline void translate( const Vector2<T>& translation )
	{
		col3.x = translation.x;
		col3.y = translation.y;
	}
	
	inline void translate( const T tx, const T ty )
	{
		col3.x = tx;
		col3.y = ty;
	}
	
	inline Vector2<T> operator*( const Vector2<T>& v ) const
	{
		return Vector2<T>( col1.x*v.x + col2.x*v.y + col3.x,  col1.y*v.x + col2.y*v.y + col3.y);
	}
	
	inline Vector3<T> operator*( const Vector3<T>& v ) const
	{
		return Vector3<T>( col1.x*v.x + col2.x*v.y + col3.x*v.z,
						   col1.y*v.x + col2.y*v.y + col3.y*v.z, 
						   col1.z*v.x + col2.z*v.y + col3.z*v.z);
	}
	
	inline void transformAsPoint(Vector2<T>& v) const
	{
		dfloat x = col1.x*v.x + col2.x*v.y + col3.x;
		dfloat y = col1.y*v.x + col2.y*v.y + col3.y;
		v.x = x;
		v.y = y;
	}
	
	inline void transformAsVector(Vector2<T>& v) const
	{
		dfloat x = col1.x*v.x + col2.x*v.y;
		dfloat y = col1.y*v.x + col2.y*v.y;
		v.x = x;
		v.y = y;
	}
	
	inline Matrix2<T> getRotationMatrix() const
	{
		return Matrix2<T>(col1.x, col1.y, col2.x, col2.y);
	}
	
	inline Matrix2<T> getRotationMatrixTransposed() const
	{
		return Matrix2<T>(col1.x, col2.x, col1.y, col2.y);
	}
	
	inline void setIdentity()
	{
		col1.x = 1.0f; col2.x = 0.0f; col3.x = 0.0f;
		col1.y = 0.0f; col2.y = 1.0f; col3.y = 0.0f;
		col1.z = 0.0f; col2.z = 0.0f; col3.z = 1.0f;
	}
	
	void getPointer( T* data ) const
	{
		data[0] = col1.x; data[1] = col1.y; data[2] = 0.0;    data[3] = 0.0;
		data[4] = col2.x; data[5] = col2.y; data[6] = 0.0;    data[7] = 0.0;
		data[8] = 0.0;    data[9] = 0.0;    data[10] = 1.0;    data[11] = 0.0;
		data[12] = col3.x; data[13] = col3.y; data[14] = 0.0;    data[15] = 1.0;
	}
	
	Matrix3<T> operator*( Matrix3<T>& other ) const
	{
		Matrix3<T> outMatrix;
		outMatrix.col1.x = col1.x * other.col1.x + col2.x * other.col1.y + col3.x * other.col1.z;
		outMatrix.col2.x = col1.x * other.col2.x + col2.x * other.col2.y + col3.x * other.col2.z;
		outMatrix.col3.x = col1.x * other.col3.x + col2.x * other.col3.y + col3.x * other.col3.z;
		
		outMatrix.col1.y = col1.y * other.col1.x + col2.y * other.col1.y + col3.y * other.col1.z;
		outMatrix.col2.y = col1.y * other.col2.x + col2.y * other.col2.y + col3.y * other.col2.z;
		outMatrix.col3.y = col1.y * other.col3.x + col2.y * other.col3.y + col3.y * other.col3.z;
		
		outMatrix.col1.z = col1.z * other.col1.x + col2.z * other.col1.y + col3.z * other.col1.z;
		outMatrix.col2.z = col1.z * other.col2.x + col2.z * other.col2.y + col3.z * other.col2.z;
		outMatrix.col3.z = col1.z * other.col3.x + col2.z * other.col3.y + col3.z * other.col3.z;
		
		return outMatrix;
	}
	
	void operator*=( Matrix3<T>& other )
	{
		Matrix3<T> outMatrix;
		outMatrix.col1.x = col1.x * other.col1.x + col2.x * other.col1.y + col3.x * other.col1.z;
		outMatrix.col2.x = col1.x * other.col2.x + col2.x * other.col2.y + col3.x * other.col2.z;
		outMatrix.col3.x = col1.x * other.col3.x + col2.x * other.col3.y + col3.x * other.col3.z;
		
		outMatrix.col1.y = col1.y * other.col1.x + col2.y * other.col1.y + col3.y * other.col1.z;
		outMatrix.col2.y = col1.y * other.col2.x + col2.y * other.col2.y + col3.y * other.col2.z;
		outMatrix.col3.y = col1.y * other.col3.x + col2.y * other.col3.y + col3.y * other.col3.z;
		
		outMatrix.col1.z = col1.z * other.col1.x + col2.z * other.col1.y + col3.z * other.col1.z;
		outMatrix.col2.z = col1.z * other.col2.x + col2.z * other.col2.y + col3.z * other.col2.z;
		outMatrix.col3.z = col1.z * other.col3.x + col2.z * other.col3.y + col3.z * other.col3.z;

		*this = outMatrix;
	}
	
	void multiply( const Matrix3<T>& m1, const Matrix3<T>& m2 )
	{
		col1.x = m1.col1.x * m2.col1.x + m1.col2.x * m2.col1.y + m1.col3.x * m2.col1.z;
		col2.x = m1.col1.x * m2.col2.x + m1.col2.x * m2.col2.y + m1.col3.x * m2.col2.z;
		col3.x = m1.col1.x * m2.col3.x + m1.col2.x * m2.col3.y + m1.col3.x * m2.col3.z;
		
		col1.y = m1.col1.y * m2.col1.x + m1.col2.y * m2.col1.y + m1.col3.y * m2.col1.z;
		col2.y = m1.col1.y * m2.col2.x + m1.col2.y * m2.col2.y + m1.col3.y * m2.col2.z;
		col3.y = m1.col1.y * m2.col3.x + m1.col2.y * m2.col3.y + m1.col3.y * m2.col3.z;
		
		col1.z = m1.col1.z * m2.col1.x + m1.col2.z * m2.col1.y + m1.col3.z * m2.col1.z;
		col2.z = m1.col1.z * m2.col2.x + m1.col2.z * m2.col2.y + m1.col3.z * m2.col2.z;
		col3.z = m1.col1.z * m2.col3.x + m1.col2.z * m2.col3.y + m1.col3.z * m2.col3.z;
	}
    
    void multiply( const Matrix3<T>& m2 )
	{
        Matrix3<T> resultantMatrix;
		resultantMatrix.col1.x = col1.x * m2.col1.x + col2.x * m2.col1.y + col3.x * m2.col1.z;
		resultantMatrix.col2.x = col1.x * m2.col2.x + col2.x * m2.col2.y + col3.x * m2.col2.z;
		resultantMatrix.col3.x = col1.x * m2.col3.x + col2.x * m2.col3.y + col3.x * m2.col3.z;
		
		resultantMatrix.col1.y = col1.y * m2.col1.x + col2.y * m2.col1.y + col3.y * m2.col1.z;
		resultantMatrix.col2.y = col1.y * m2.col2.x + col2.y * m2.col2.y + col3.y * m2.col2.z;
		resultantMatrix.col3.y = col1.y * m2.col3.x + col2.y * m2.col3.y + col3.y * m2.col3.z;
		
		resultantMatrix.col1.z = col1.z * m2.col1.x + col2.z * m2.col1.y + col3.z * m2.col1.z;
		resultantMatrix.col2.z = col1.z * m2.col2.x + col2.z * m2.col2.y + col3.z * m2.col2.z;
		resultantMatrix.col3.z = col1.z * m2.col3.x + col2.z * m2.col3.y + col3.z * m2.col3.z;
        *this = resultantMatrix;
	}
	
	inline void invert()
	{
		dfloat det = ( col1.x * ( col2.y * col3.z  - col2.z * col3.y ) 
					  - col2.x * ( col1.y * col3.z - col1.z * col3.y ) 
					  + col3.x * ( col1.y * col2.z - col1.z * col2.y ) );
		
		if ( fabs(det) < EPSILON ) 
			return;
		
		det = 1.0f / det;
		
		dfloat m00 = ( col2.y * col3.z  - col2.z * col3.y ) * det;
		dfloat m10 = -( col1.y * col3.z  - col1.z * col3.y ) * det;
		dfloat m20 = ( col1.y * col2.z  - col1.z * col2.y ) * det;
		
		dfloat m01 = -( col2.x * col3.z  - col2.z * col3.x ) * det;
		dfloat m11 = ( col1.x * col3.z  - col3.x * col1.z ) * det;
		dfloat m21 = -( col1.x * col2.z  - col1.z * col2.x ) * det;
		
		dfloat m02 = ( col2.x * col3.y  - col2.y * col3.x ) * det;
		dfloat m12 = -( col1.x * col3.y  - col1.y * col3.x ) * det;
		dfloat m22 = ( col1.x * col2.y  - col1.y * col2.x ) * det;
		
		col1.x = m00; col2.x = m01; col3.x = m02;		
		col1.y = m10; col2.y = m11; col3.y = m12;		
		col1.z = m20; col2.z = m21; col3.z = m22;		
	}
	
	
	inline void getInverse(Matrix3<T>& other) const
	{
		dfloat det = ( col1.x * ( col2.y * col3.z  - col2.z * col3.y ) 
					  - col2.x * ( col1.y * col3.z - col1.z * col3.y ) 
					  + col3.x * ( col1.y * col2.z - col1.z * col2.y ) );
		
		if ( fabs(det) < EPSILON ) 
			return;
		
		det = 1.0f / det;
		
		other.col1.x = ( col2.y * col3.z  - col2.z * col3.y ) * det;
		other.col1.y = -( col1.y * col3.z  - col1.z * col3.y ) * det;
		other.col1.z = ( col1.y * col2.z  - col1.z * col2.y ) * det;
		
		other.col2.x = -( col2.x * col3.z  - col2.z * col3.x ) * det;
		other.col2.y = ( col1.x * col3.z  - col3.x * col1.z ) * det;
		other.col2.z = -( col1.x * col2.z  - col1.z * col2.x ) * det;
		
		other.col3.x = ( col2.x * col3.y  - col2.y * col3.x ) * det;
		other.col3.y = -( col1.x * col3.y  - col1.y * col3.x ) * det;
		other.col3.z = ( col1.x * col2.y  - col1.y * col2.x ) * det;
	}
		
	inline Matrix3<T> getInverse() const
	{
		Matrix3<T> outMatrix;
		
		dfloat det = ( col1.x * ( col2.y * col3.z  - col2.z * col3.y ) 
					  - col2.x * ( col1.y * col3.z - col1.z * col3.y ) 
					  + col3.x * ( col1.y * col2.z - col1.z * col2.y ) );
		
		if ( fabs(det) < EPSILON ) 
			return outMatrix;
		
		det = 1.0f / det;
		
		outMatrix.col1.x = ( col2.y * col3.z  - col2.z * col3.y ) * det;
		outMatrix.col1.y = -( col1.y * col3.z  - col1.z * col3.y ) * det;
		outMatrix.col1.z = ( col1.y * col2.z  - col1.z * col2.y ) * det;
		
		outMatrix.col2.x = -( col2.x * col3.z  - col2.z * col3.x ) * det;
		outMatrix.col2.y = ( col1.x * col3.z  - col3.x * col1.z ) * det;
		outMatrix.col2.z = -( col1.x * col2.z  - col1.z * col2.x ) * det;
		
		outMatrix.col3.x = ( col2.x * col3.y  - col2.y * col3.x ) * det;
		outMatrix.col3.y = -( col1.x * col3.y  - col1.y * col3.x ) * det;
		outMatrix.col3.z = ( col1.x * col2.y  - col1.y * col2.x ) * det;
		
		return outMatrix;
	}
	
	inline T determinant() const
	{
		// Volume of a Parallelepid with col1, col2 and col3 as axis.
		return col1.dot( col2.cross(col3) );
	}
	
	// Solve a System like A * x = rhs. returns result in x.
	void solve( const Vector3<T>& rhs, Vector3<T>& x ) const
	{		
		T det = col1.dot( col2.cross(col3) );
		if (det != 0.0f)
		{
			det = 1.0f / det;
		}
		// Cramer's Rule solution
		x.x = det * rhs.dot(col2.cross(col3));
		x.y = det * col1.dot(rhs.cross(col3));
		x.z = det * col1.dot(col2.cross(rhs));
	}
	
	Vector3<T> solve( const Vector3<T>& rhs ) const
	{
		Vector3<T> solution;
		
		T det = col1.dot( col2.cross(col3) );
		if (det != 0.0f)
		{
			det = 1.0f / det;
		}
		// Cramer's Rule solution
		solution.x = det * rhs.dot(col2.cross(col3));
		solution.y = det * col1.dot(rhs.cross(col3));
		solution.z = det * col1.dot(col2.cross(rhs));
		return solution;
	}
	
	// Solve a System like A * x = rhs. returns result in x.
	void solve22( const Vector2<T>& rhs, Vector2<T>& x ) const
	{		
		T det = col1.x * col2.y - col1.y * col2.x;
		if( det != 0.0 )
		{
			det = 1.0f/det;
			
			x.x = det * ( col2.y * rhs.x - col2.x * rhs.y );
			x.y = det * ( -col1.y * rhs.x + col1.x * rhs.y );
		}
	}
	
	Vector2<T> solve22( const Vector2<T>& rhs ) const
	{
		Vector2<T> solution;
		
		T det = col1.x * col2.y - col1.y * col2.x;
		if( det != 0.0 )
		{
			det = 1.0f/det;
			
			solution.x = det * ( col2.y * rhs.x - col2.x * rhs.y );
			solution.y = det * ( -col1.y * rhs.x + col1.x * rhs.y );
		}
		return solution;
	}
	
	
public:
	Vector3<T> col1, col2, col3;
};

typedef Matrix3<dfloat> Matrix3f;
typedef Matrix3<dfloat> Matrix3d;


END_NAMESPACE_DIRAC2D

#endif