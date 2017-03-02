/*
 *  Matrix4.h
 *  Dirac2D
 *
 *  Created by Prashant on 04/05/12.
 *
 */

#include "Vector2.h"
#include "Vector3.h"
#include <math.h>

#ifndef _MATRIX4_H_
#define _MATRIX4_H_

BEGIN_NAMESPACE_DIRAC2D

template< class T >
class Matrix4
{
public:
	Matrix4()
	{
		setIdentity();
	}
	
	
	inline void rotate(T angle)
	{
		T cs = cos(angle); T sn = sin(angle);
	}
	
	inline void translate( const Vector2<T>& translation )
	{
        m03 = translation.x;
        m13 = translation.y;
	}
	
	inline void translate( T tx, T ty )
	{
        m03 = tx;
        m13 = ty;
	}
	
	inline Vector2<T> operator*( const Vector2<T>& v ) const
	{
		return Vector2<T>();
	}
	
	inline Vector3<T> operator*( const Vector3<T>& v ) const
	{
		return Vector3<T>();
	}
	
	inline void transformAsPoint(Vector2<T>& v) const
	{
		
	}
	
	inline void transformAsVector(Vector2<T>& v) const
	{
		
	}
	
	inline Matrix2<T> getRotationMatrix() const
	{
	}
	
	inline Matrix2<T> getRotationMatrixTransposed()
	{
	}
	
	inline void setIdentity()
	{
		m00 = 1.0; m01 = 0.0; m02 = 0.0; m03 = 0.0;
        m10 = 0.0; m11 = 1.0; m12 = 0.0; m13 = 0.0;
        m20 = 0.0; m21 = 0.0; m22 = 1.0; m23 = 0.0;
        m30 = 0.0; m31 = 0.0; m32 = 0.0; m33 = 1.0;;
	}
    
    void getPointer( T* data ) const
    {        
        data[0]  = m00 ; data[1]  = m10; data[2]  = m20; data[3]  = m30;
        data[4]  = m01 ; data[5]  = m11; data[6]  = m21; data[7]  = m31;
        data[8]  = m02 ; data[9]  = m12; data[10] = m22; data[11] = m32;
        data[12] = m03 ; data[13] = m13; data[14] = m23; data[15] = m33;        
    }
	
    
    void set( const Matrix3<T>& m )
    {
        m00 = m.col1.x; m01 = m.col2.x; m02 = 0.0f; m03 = m.col3.x;
        m10 = m.col1.y; m11 = m.col2.y; m12 = 0.0f; m13 = m.col3.y;
        m20 = 0.0f;     m21 = 0.0f;     m22 = 0.0f; m23 = 0.0f;
        m30 = 0.0f;     m31 = 0.0f;     m32 = 0.0f; m33 = 1.0f;
    }
		
	inline void getInverse(Matrix4<T>& other) const
	{
		
	}
    
	inline Matrix4<T> getInverse() const
	{
		Matrix4<T> outMatrix;
		
		return outMatrix;
	}
	
	inline T determinant() const
	{
		// Volume of a Parallelepid with col1, col2 and col3 as axis.
	}
	
	// Solve a System like A * x = rhs. returns result in x.
	void solve( Vector3<T>& rhs, Vector3<T>& x )
	{		
	}
	
	Vector3<T> solve( Vector3<T>& rhs )
	{
		Vector3<T> solution;
		return solution;
	}
    
    void lookAt(const Vector3<T>& eye , const Vector3<T>& target , const Vector3<T>& up )
    {
        setIdentity();
        Vector3<T> dir , newUp;
        
        dir = target - eye;
        dir.normalize();
        
        Vector3<T> n;
        n = dir.cross(up);
        n.normalize();
        
        newUp = n.cross(dir);
        
        T eyeX = n.dot(eye);
        T eyeY = newUp.dot(eye);
        T eyeZ = dir.dot(eye);
        
        m00 = n.x  ;  m01 = n.y  ;  m02 = n.z  ;  m03 = -eyeX;
        m10 = newUp.x ;  m11 = newUp.y ;  m12 = newUp.z ;  m13 = -eyeY;
        m20 = -dir.x;  m21 = -dir.y;  m22 = -dir.z;  m23 = eyeZ;
    }
    
    
    void lookAt(T eyeX , T eyeY , T eyeZ
                          , T targetX , T targetY , T targetZ , T upX , T upY , T upZ )
    {
        setIdentity();
        Vector3<T> dir , newUp , eye(eyeX , eyeY, eyeZ) , target(targetX , targetY , targetZ);
        Vector3<T> up(upX, upY, upZ);
        dir = target - eye;
        dir.normalize();
        
        Vector3<T> n;
        n  = dir.cross(up);
        n.normalize();
        
        newUp = n.cross(dir);
        
        m00 = n.x  ;  m01 = n.y  ;  m02 = n.z  ;  m03 = n.dot(eye);
        m10 = newUp.x ;  m11 = newUp.y ;  m12 = newUp.z ;  m13 = newUp.dot(eye);
        m20 = -dir.x;  m21 = -dir.y;  m22 = -dir.z;  m23 = dir.dot(eye);
    }
    
    void frustum(T left , T right , T bottom , T top , T zNear , T zFar )
    {
        setIdentity();
        
        m00 = (2*zNear/(right-left));
        m01 = 0;
        m02 = (right + left )/(right - left );
        m03 = 0;
        
        m10 = 0;
        m11 = (2*zNear/(top-bottom));
        m12 = (top + bottom)/(top - bottom);
        m13 = 0;
        
        m20 = 0;
        m21 = 0;
        m22 = -(zFar + zNear)/(zFar-zNear);
        m23 = -2*zFar*zNear/(zFar-zNear);
        
        m30 = 0;
        m31 = 0;
        m32 = -1;
        m33 = 0;
    }
    
    void ortho(T left , T right , T bottom , T top , T zNear , T zFar)
    {
        setIdentity();
        
        m00 = (2/(right-left));
        m01 = 0;
        m02 = 0;
        m03 = -(right + left )/(right - left );
        
        m10 = 0;
        m11 = (2/(top-bottom));
        m12 = 0;
        m13 = -(top + bottom)/(top - bottom);
        
        m20 = 0;
        m21 = 0;
        m22 = -2/(zFar-zNear);
        m23 = -(zFar + zNear)/(zFar-zNear);
        
        m30 = 0;
        m31 = 0;
        m32 = 0;
        m33 = 1.0f;
    }
    
    void perspective(T fov , T aspectRatio , T zNear , T zFar)
    {
        setIdentity();
        
        T volumeHeight = 2 * zNear * tan(fov/2);
        T volumeWidth  = volumeHeight * aspectRatio;
        
        m00 = 2*zNear/volumeWidth;
        m01 = 0;
        m02 = 0;
        m03 = 0;
        
        m10 = 0;
        m11 = 2*zNear/volumeHeight;
        m12 = 0;
        m13 = 0;
        
        m20 = 0;
        m21 = 0;
        m22 = -(zFar + zNear)/(zFar-zNear);
        m23 = -2*zFar*zNear/(zFar-zNear);
        
        m30 = 0;
        m31 = 0;
        m32 = -1;
        m33 = 0;
    }
	
public:
	//Vector3<T> col1, col2, col3;
    T m00, m01, m02, m03;
    T m10, m11, m12, m13;
    T m20, m21, m22, m23;
    T m30, m31, m32, m33;    
};

typedef Matrix4<dfloat> Matrix4f;
typedef Matrix4<dfloat> Matrix4d;


END_NAMESPACE_DIRAC2D

#endif