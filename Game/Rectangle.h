//
//  Rectangle.h
//  testCpp
//
//  Created by Prashant on 07/11/11.
//  Copyright 2011 Sourcebits Technologies. All rights reserved.
//

#ifndef _RECTANGLE_H_
#define _RECTANGLE_H_

#include <math.h>
#include "Point2.h"

using namespace NAMESPACE_DIRAC2D;

template< class T >
class Rectangle
{
public:
	
	Rectangle();
	
	Rectangle(T startX, T startY, T endX, T endY);
	
	inline void addPoint( T x, T y);
	
	inline bool isPointInside( T x, T y, T error = 0.0f );
	
	void reCalculateRect();
	
	T getArea();
	
	Point2f getEdge(int i );
	
	Point2f getCentre();
	
	bool triRectOverlap(Point2f& p0, Point2f& p1, Point2f& p2);
	
public:
	T startX, startY;
	T endX, endY;
	
	Point2f m_Centre;
	Point2f m_Edges[4];
};

template< class T >
Rectangle<T>::Rectangle():startX(1000.0f), startY(1000.0f),endX(-1000.0f), endY(-1000.0f)
{
}

template< class T >
Rectangle<T>::Rectangle(T startX, T startY, T endX, T endY):startX(startX), startY(startY),endX(endX), endY(endY)
{
}

template< class T >
inline void Rectangle<T>::addPoint( T x, T y)
{
	if (x>endX) endX = x;
	if (y>endY) endY = y;
	
	if (x<startX) startX = x;
	if (y<startY) startY = y;
}

template< class T >
inline bool Rectangle<T>::isPointInside( T x, T y, T errorBounds)
{
	return ( (x >= (startX - errorBounds)) && (x <=  (endX + errorBounds))
			&& ( y >= (startY - errorBounds)) && ( y <=  (endY + errorBounds)) );
}

template< class T >
void Rectangle<T>::reCalculateRect()
{
	Point2f diag;
	
	m_Centre.x = ( endX + startX ) * 0.5f;
	m_Centre.y = ( endY + startY ) * 0.5f;

	diag.x = ( m_Centre.x -  endX );
	diag.y = ( m_Centre.y -  endY );
	
	m_Edges[0].x = m_Centre.x + diag.x; m_Edges[0].y = m_Centre.y + diag.y;
	m_Edges[1].x = m_Centre.x - diag.x; m_Edges[1].y = m_Centre.y + diag.y;
	m_Edges[2].x = m_Centre.x - diag.x; m_Edges[2].y = m_Centre.y - diag.y;
	m_Edges[3].x = m_Centre.x + diag.x; m_Edges[3].y = m_Centre.y - diag.y;
}

template< class T >
T Rectangle<T>::getArea()
{
	return ( endX - startX ) * ( endY - startY );
}

template< class T >
Point2f Rectangle<T>::getEdge(int i )
{
	return m_Edges[i];
}

template< class T >
Point2f Rectangle<T>::getCentre()
{
	return m_Centre;
}

#define PROJECTION_AXIS(  out,  v0,  v1 ) \
{\
out.x = v1.y - v0.y; out.y = v0.x - v1.x; \
float d = out.x * out.x + out.y * out.y; \
if( fabs(d) > 1e-15 ) { \
d = sqrt(d); \
out.x /= d; out.y /= d; } \
}\


#define PROJECT(axis, points, numPoints, minimum , maximum) { \
	float dot = axis.x * points[0].x + axis.y * points[0].y; \
	minimum = maximum = dot; \
	for( int i = 1; i < numPoints; i++ ) \
	{								\
	     dot = axis.x * points[i].x + axis.y * points[i].y; \
		minimum = dot < minimum ? dot : minimum;			\
		maximum = dot > maximum ? dot : maximum;			\
	}												\
}													\


template< class T >
bool Rectangle<T>::triRectOverlap(Point2f& p0, Point2f& p1, Point2f& p2)
{
	// SAT test for triangle Rectangle overlap. we need to test against 5 axis. 2 for Rectangle and 3 for Triangle.
	
	// gather All the Axis
	Point2f axis[5];
	// Rectangle's Seperating axis
	axis[0].x = 0.0f; axis[0].y = 1.0f;
	axis[1].x = 1.0f; axis[1].y = 0.0f;
	// Triangle's Seperating axis
	PROJECTION_AXIS( axis[2], p0, p1 )
	PROJECTION_AXIS( axis[3], p1, p2 )
	PROJECTION_AXIS( axis[4], p2, p0 )

	Point2f rectanglePoints[] = { m_Edges[0], m_Edges[1], m_Edges[2], m_Edges[3] };
	Point2f trianglePoints[] = {p0, p1, p2};
		
	for( int i=0; i<5; i++ )
	{
		Point2f& sat_axis = axis[i];
		
		float min1, min2, max1, max2; //Project both bodies onto the perpendicular
		
		PROJECT( sat_axis, trianglePoints, 3, min1, max1);
		PROJECT( sat_axis, rectanglePoints, 4, min2, max2);

		float distance; //Calculate the distance between the two intervals
		distance = min1 < min2 ? min2-max1 : min1-max2;
		
		if( distance > 0.0f ) //If the intervals don't overlap, return, since there is no collision
			return false;
	}
	
	return true;
}

typedef Rectangle<float> Rectangle2f;
typedef Rectangle<int> Rectangle2i;

#endif
