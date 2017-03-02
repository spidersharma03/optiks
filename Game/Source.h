/*
 *  Source.h
 *  Optiks
 *
 *
 */

#ifndef _SOURCE_H_
#define _SOURCE_H_

#include "Ray.h"
#include "IRenderable.h"


class Source
{
public:
	Source(const Vector2f& origin, const Vector2f& direction);
	
	virtual void render(float size = 1.0f);
	
	Ray getRay()
	{
		return m_Ray;
	}
	
	void setOrigin(const Vector2f& origin);
	
	void setDirection(const Vector2f& direction);

private:
	Ray m_Ray;
};

#endif