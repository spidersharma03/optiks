/*
 *  FluidSimulator.h
 *  FluidSimulation
 *
 *  Created by Prashant on 08/02/12.
 *  Copyright 2012 Sourcebits Technologies. All rights reserved.
 *
 */

#ifndef _FLUID_SIMULATOR_H_
#define _FLUID_SIMULATOR_H_

#include "ParticleBase.h"
/*
   Simulates 2D Fluid based upon Navier-Stokes eqn. solution domain is a rectangular grid of dimensions nX and nY.
 */

typedef float* floatptr;

#define SWAP_PTR( x, y ) { \
float* temp = y; \
y = x; \
x = temp; } \


class FluidSimulator;

class FlowField
{
public:
	FlowField(FluidSimulator* pFluidSimulator)
	{
		m_pFluidSimulator = pFluidSimulator;
	}
	
	virtual void advance(float dt) = 0;
	
protected:
	int ix, iy;  // Flow Centre
	FluidSimulator* m_pFluidSimulator;
};

class RadialFlowField : public FlowField
{
public:
	RadialFlowField(FluidSimulator* pFluidSimulator);
	
	virtual void advance(float dt);
};

class FluidSimulator
{
public:
	
	FluidSimulator( int nX, int nY );
	
    void advanceFluidParticles( Particle* pParticles, float dt );

	void addExtImpulse(float sourceStrengthX, float  sourceStrengthY, int sx, int sy, int radius, float dt);

	void injectSource(float sourceStrength , int sx, int sy, int radius, float dt);
	
	void removeSource(float sourceStrength , int sx, int sy, int radius, float dt);

	void advance( float dt );
		
    void render();
	
	void setColor( float r, float g, float b )
	{
		colorR = r;
		colorG = g;
		colorB = b;
	}
	
	void setSize(int w, int h)
	{
	}
	
	unsigned int getTextureObject()
	{
		return textureObject;
	}
	
	float  TEX_RES_X, TEX_RES_Y;

    void RasterizeVelocity( const Vector2i& start, const Vector2i& end );

private:
	
	void initFluidParticles();
	
	void diffuse( int b, float D, float* inx, float* outx, float dt, int numIterations = 1);

	void diffuseDensity( int b, float D, float* inx1, float* inx2, float* inx3, float* outx1, float* outx2, float* outx3, float dt);

	void advect(int b, float* inx, float* outx, float* u, float* v, float dt);
	
	void advanceDensity( float dt );

	void advanceVelocity( float dt );
	
	void advanceFluidParticles( float dt );

	void projectVelocity(float* vx, float* vy, float* p, float* div, float dt );
	
	void setBC(int b, float * x);
	
	void addExternalForces(float* vx, float* vy, float dt);

	void vorticityConfinement(float* u, float* v, float dt);
	
	void addSources(float* x, float* x0, float dt);
	
	friend class RadialFlowField;
private:
	unsigned int textureObject;
	int nCells, nX, nY;
	float Diffusion, Viscocity;
	floatptr densityR, prevDensityR;
	floatptr densityG, prevDensityG;
	floatptr densityB, prevDensityB;
	floatptr vx, prev_vx, vy, prev_vy;
	floatptr omega;
	floatptr confinementForceX, confinementForceY;
	unsigned char* textureData;
	float m_MeanDensity;
	float colorR, colorG, colorB;
	bool bShowVelocities;

	int sX, sY;
	float m_SourceStrengthX, m_SourceStrengthY;
};

#endif