//
//  Particle.h
//  Optiks
//
//  Created by Prashant on 01/03/14.
//
//

#ifndef Optiks_Particle_h
#define Optiks_Particle_h

#include "Vector2.h"
#include "Color4.h"

USE_NAMESPACE_DIRAC2D

struct Particle
{
	Particle()
	{
		x = y = oldx = oldy = vx = vy = 0.0f;
        colorR = colorG = colorB = 1.0f;
        colorA = 0.5f;
        x = (float)rand()/RAND_MAX * 1024;
        y = (float)rand()/RAND_MAX * 768;
        age = 0.0f;
	}
	
	float x, y;
	float oldx, oldy;
	float vx, vy;
    float colorR, colorG, colorB, colorA;
    float age;
};

struct RigidParticle : public Particle
{
	RigidParticle()
	{
		m_Omega = 0.0f;
		m_Angle = 0.0f;
	}
	float m_Omega;
	float m_Angle;
};


#endif
