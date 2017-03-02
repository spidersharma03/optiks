/*
 *  BlackMatter.h
 *  Optiks
 *
 *  Created by Prashant on 15/12/11.
 *  Copyright 2011 Sourcebits Technologies. All rights reserved.
 *
 */

#ifndef _BLACK_MATTER_H_
#define _BLACK_MATTER_H_

#include "IRenderable.h"
#include "Shape2D.h"
#include <vector>

using namespace std;

class BlackMatter
{
public:
	BlackMatter(float influenceRadius, float fallOff = 1.0f , bool bAttractive = true );
	
	vector<Circle*>& getAllCircles()
	{
		return vecCircles;
	}
	
	void setTransform( const Matrix3f& transform )
	{
		m_Transform = transform;
		for( int c=0; c<vecCircles.size(); c++ )
		{
			vecCircles[c]->setTransform(m_Transform);
		}
	}
	
private:
	float m_InfluenceRadius;
	float m_FallOff;
	bool m_bAttractive;
	Matrix3f m_Transform;
	vector<Circle*> vecCircles;
	static const int NUM_CIRCLES = 30;
};

#endif