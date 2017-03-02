/*
 *  GLRenderer.h
 *  Dirac2D
 *
 *  Created by Prashant on 03/05/12.
 *
 */

#include "Renderer.h"

#ifndef _GL_RENDERER_H_
#define _GL_RENDERER_H_

#ifdef WIN32
  #include <GL/glut.h>
#else
  #include <OpenGL/gl.h>
#endif

BEGIN_NAMESPACE_DIRAC2D

class GLRenderer : public Renderer
{
public:
	GLRenderer(PhysicalWorld* world):Renderer(world)
	{
	}
	
	virtual void setPointSize(dfloat size);

	virtual void setLineWidth(dfloat width);

	virtual void setColor( dchar r, dchar g, dchar b, dchar a );
	
	virtual void setColor( dchar r, dchar g, dchar b );
	
	virtual void drawShape(const CollisionShape*);
	
	virtual void drawCircle( dfloat cx, dfloat cy, dfloat radius );
	
	virtual void drawBox( dfloat width, dfloat height);
	
	virtual void drawPolygon( const Vector2f* vertices, dint32 numVertices );
	
	virtual void drawLine( const Vector2f& begin, const Vector2f& end);
	
	virtual void drawPoint( const Vector2f& point );
	
	virtual void drawAABB( const AABB2f& aabb );
	
	virtual void setTransform( const Matrix3f& xform);

	virtual void drawCapsule( dfloat cx, dfloat cy, dfloat width, dfloat height);

protected:
	dfloat data[16];
};

END_NAMESPACE_DIRAC2D

#endif