/*
 *  BlackMatter.cpp
 *  Optiks
 *
 *  Created by Prashant on 15/12/11.
 *  Copyright 2011 Sourcebits Technologies. All rights reserved.
 *
 */

#include "BlackMatter.h"

BlackMatter::BlackMatter(float influenceRadius, float fallOff , bool bAttractive ):m_InfluenceRadius(influenceRadius), m_FallOff(fallOff), m_bAttractive(bAttractive)
{
	float r = m_InfluenceRadius;
	float dr = m_InfluenceRadius/(NUM_CIRCLES+1);
	float refractionIndex = 1.1;
	for( int c=0; c<NUM_CIRCLES; c++ )
	{
		Circle* circle = new Circle(r);
		circle->m_isRefractive = true;
		//circle->m_bRenderable = false;
		circle->m_RefractionIndex = refractionIndex;
		circle->m_IsReflective = false;
		vecCircles.push_back(circle);
		r-= dr;
		refractionIndex = circle->m_RefractionIndex * 1.01;
	}
}