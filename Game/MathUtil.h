/*
 *  MathUtil.h
 *  Dirac2D
 *
 *  Created by Prashant on 23/04/12.
 *
 */

#include "definitions.h"

#include "Point2.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix2.h"
#include "Matrix3.h"
//#include "Matrix4.h"
#include "AABB2.h"
#include "Ray2.h"
#include "Color4.h"

#include <stdlib.h>

#ifndef _MATH_UTIL_H_
#define _MATH_UTIL_H_

BEGIN_NAMESPACE_DIRAC2D

#define MAX_POINTS 15



template< typename T >
inline void SWAP( T& a, T& b )
{
	T temp = a;
	a = b;
	b = temp;
}

template< typename T >
inline T CLAMP( T a, T lo, T hi )
{
	if( a < lo )
	{
		a = lo;
		return a;
	}
	if( a > hi )
		a = hi;
	return a;
}

// Creates a random number between low and high limits
template< typename T >
T RANDOM_NUMBER( T low, T high )
{
	return low + (T)rand()/RAND_MAX * ( high - low );
}
// Finds the Closest points of two line segments.
template<class T>
static void findClosestPoints(Vector2<T>& p0, Vector2<T>& p1, Vector2<T>& p2, Vector2<T>& p3, Vector2<T>& outPoint0, Vector2<T>& outPoint1)
{
	T dx1 = p1.x - p0.x;
	T dx2 = p3.x - p2.x;
	T dy1 = p1.y - p0.y;
	T dy2 = p3.y - p2.y;
	
	T A = dx1 * dx1 + dx2 * dx2;
	T B = - dx1 * dx2 - dy1 * dy2;
	T C = p2.x * dx1 + p2.y * dy1 - p0.x * dx1 - p0.y * dy1;
	T D = B;
	T E = dy1 * dy1 + dy2 * dy2;
	T F = p0.x * dx2 + p0.y * dy2 - p2.x * dx2 - p2.y * dy2;
	
	T Denom = D * B - A * E;
	T u1 = 0.0f;
	T u2 = 0.0f;
	if( fabs(Denom) > EPSILON )
	{
		Denom = 1.0f/Denom;
		u2 = ( D * C - A * F ) * Denom;
		u1 = ( B * F - E * C ) * Denom;
	}
	if( u1 < 0.0f ) u1 = 0.0f;
	if( u1 > 1.0f ) u1 = 1.0f;
	if( u2 < 0.0f ) u2 = 0.0f;
	if( u2 > 1.0f ) u2 = 1.0f;

	outPoint0 = p0 + (p1-p0) * u1;
	outPoint1 = p2 + (p3-p2) * u2;
}

// Finds the Closest point on the Line Segment p0-p1 from an Input Point.
template<class T>
static inline void findClosestPoint(const Vector2<T>& p0, const Vector2<T>& p1, const Vector2<T>& point, Vector2<T>& outPoint)
{
	T dx = p1.x - p0.x;
	T dy = p1.y - p0.y;
	T Denom = dx*dx + dy*dy;
	
	T u = 0.0f;
	
	if( Denom*Denom > EPSILON*EPSILON )
	{
		u = (dx * point.x + dy * point.y - dx * p0.x - dy * p0.y)/Denom;
	}
	if( u < 0.0f ) u = 0.0f;
	if( u > 1.0f ) u = 1.0f;
	
	outPoint = p0 + (p1-p0) * u;
}

// Finds the Barycentric Coordinate of the Closest Point on the Line Segment p0-p1 from an Input Point.
template<class T>
static inline void findBaryCentricCoordinate(const Vector2<T>& p0, const Vector2<T>& p1, const Vector2<T>& point, T& u)
{
	T dx = p1.x - p0.x;
	T dy = p1.y - p0.y;
	T Denom = dx*dx + dy*dy;
		
	if( Denom*Denom > EPSILON*EPSILON )
	{
		u = (dx * point.x + dy * point.y - dx * p0.x - dy * p0.y)/Denom;
	}
}

template<class T>
static void tridia_sl(T *lower, T *main, T *upper, T *rhs, T *sol, dint32 n) 
{
	solver(lower, upper, main, rhs, sol, n);
}

template<class T>
static void tridia_sl(T *rhs, T* sol, dint32 n) 
{
	/*T lower[n];
	T upper[n];
	T main[n];
	
	for ( dint32 i = 0 ; i < n ; i ++)
	{
		lower[i] = 1.0f;
		upper[i] = 1.0f;
	}
	for ( dint32 i = 0 ; i < n ; i ++)
	{
		main[i] = 4.0f;
	}
	
	solver(lower, upper, main, rhs, sol, n);*/
}

template<class T>
static void solver(T* l, T* u, T* d, T* r, T* y, dint32 n) 
{
	//gauss_elimination:do i=1,n-1
	for (dint32 i = 0; i < n - 2; i++) {
		u[i] = u[i] / d[i];
		r[i] = r[i] / d[i];
		d[i] = d[i] / d[i];
		r[i + 1] = r[i] * l[i] - r[i + 1];
		d[i + 1] = l[i] * u[i] - d[i + 1];
		u[i + 1] = -u[i + 1];
		l[i] = l[i] * d[i] - l[i];
	}
	y[n - 1] = r[n - 1] / d[n - 1];
	//back_substitution:do i=n-1,1,-1
	for (dint32 i = n - 2; i >= 0; i--) {
		y[i] = (r[i] - u[i] * y[i + 1]) / d[i];
	}
	
}

//template<class T>
//static void findSplineCoeff( CubicSpline* splines , T* D , T* y, dint32 n)
//{
//	for ( dint32 i = 0 ; i < n ; i ++)
//	{
//		splines[i].a = y[i];
//		splines[i].b = D[i];
//		splines[i].c = 3 * ( y[i+1] - y[i] ) - 2 * D[i] - D[i+1];
//		splines[i].d = 2 * ( y[i] - y[i+1] ) + D[i] + D[i+1] ;
//	}
//}
//
//public static float interpolate(CubicSpline spline , float t ){
//	if ( t > 1 || t < 0)return 0;
//	float y;
//	y = (int) (spline.a + spline.b * t + spline.c * t * t + spline.d * t * t * t);
//	return y;
//}

END_NAMESPACE_DIRAC2D

#endif