/*
 *  Source.cpp
 *  Optiks
 *
 *  Created by Prashant on 18/02/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "Source.h"
#include<GLUT/glut.h>

Source::Source(const Vector2f& origin, const Vector2f& direction)
{
	m_Ray.origin.x = origin.x;
	m_Ray.origin.y = origin.y;
	m_Ray.direction.x = direction.x;
	m_Ray.direction.y = direction.y;
	//m_Ray.direction.normalize();
}

void Source::setOrigin(const Vector2f& origin)
{
	m_Ray.origin.x = origin.x;
	m_Ray.origin.y = origin.y;
}

void Source::setDirection(const Vector2f& direction)
{
	m_Ray.direction.x = direction.x;
	m_Ray.direction.y = direction.y;
	//m_Ray.direction.normalize();
}

void Source::render(float size)
{
	glPointSize(size);
	glBegin(GL_POINTS);
    //glColor3b(255, 255, 155);
	glVertex2d(m_Ray.origin.x, m_Ray.origin.y);
	glEnd();
	//glPointSize(1.0f);
}