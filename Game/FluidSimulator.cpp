/*
 *  FluidSimulator.cpp
 *  FluidSimulation
 *
 *  Created by Prashant on 08/02/12.
 *  Copyright 2012 Sourcebits Technologies. All rights reserved.
 *
 */

#include <stdlib.h>

#include "FluidSimulator.h"
#include <math.h>
#include <stdio.h>
#include "GameSettings.h"

#define MAX_ITERATIONS 10


RadialFlowField::RadialFlowField(FluidSimulator* pFluidSimulator) : FlowField(pFluidSimulator)
{
}

void RadialFlowField::advance(float dt)
{
	//floatptr vx = m_pFluidSimulator->prev_vx;
	//floatptr vy = m_pFluidSimulator->prev_vy;

	// Radial
	//{
	//		float s = 1.0f;
	//		
	//		prev_vy[ INDEX( sX-1, sY, nX) ] = 0.0f;
	//		vx[ INDEX( sX-1, sY, nX) ] = -s * dt;
	//		
	//		vx[ INDEX( sX, sY-1, nX) ] =  0.0f;
	//		prev_vy[ INDEX( sX, sY-1, nX) ] = -s * dt;
	//		
	//		prev_vy[ INDEX( sX+1, sY, nX) ] = 0.0f;
	//		vx[ INDEX( sX+1, sY, nX) ] = s * dt;
	//		
	//		vx[ INDEX( sX, sY+1, nX) ] = 0.0f;
	//		prev_vy[ INDEX( sX, sY+1, nX) ] = s * dt;
	//		
	//		vx[ INDEX( sX+1, sY+1, nX) ] = s * sqrt2 * dt;
	//		prev_vy[ INDEX( sX+1, sY+1, nX) ] = s * sqrt2 * dt;
	//		
	//		vx[ INDEX( sX-1, sY+1, nX) ] = -s * sqrt2 * dt;
	//		prev_vy[ INDEX( sX-1, sY+1, nX) ] = s * sqrt2 * dt;
	//		
	//		vx[ INDEX( sX-1, sY-1, nX) ] = -s * sqrt2 * dt;
	//		prev_vy[ INDEX( sX-1, sY-1, nX) ] = -s * sqrt2 * dt;
	//		
	//		vx[ INDEX( sX+1, sY-1, nX) ] = s * sqrt2 * dt;
	//		prev_vy[ INDEX( sX+1, sY-1, nX) ] = -s * sqrt2 * dt;
	//		
	//		vx[ INDEX( sX, sY, nX) ] = 0.0f;
	//		prev_vy[ INDEX( sX, sY, nX) ] = 0.0f;
	//		
	//	}
	
}


FluidSimulator::FluidSimulator( int nX, int nY ):nX(nX), nY(nY)
{
	initFluidParticles();
	
	//densityR     = new float[(nX+2)*(nY+2)];
	//densityG     = new float[(nX+2)*(nY+2)];
	//densityB     = new float[(nX+2)*(nY+2)];
	//prevDensityR = new float[(nX+2)*(nY+2)];
	//prevDensityG = new float[(nX+2)*(nY+2)];
	//prevDensityB = new float[(nX+2)*(nY+2)];
	vx          = new float[(nX+2)*(nY+2)];
	prev_vx     = new float[(nX+2)*(nY+2)];
    vy          = new float[(nX+2)*(nY+2)];
    prev_vy     = new float[(nX+2)*(nY+2)];
	omega		= new float[(nX+2)*(nY+2)];
	confinementForceX = new float[(nX+2)*(nY+2)];
	confinementForceY = new float[(nX+2)*(nY+2)];

	sX = sY = 0;
	m_SourceStrengthX = 0.0f;
	m_SourceStrengthY = 0.0f;

	nCells = (nX+2)*(nY+2);
	colorR = (float)rand()/RAND_MAX;
	colorG = (float)rand()/RAND_MAX;
	colorB = (float)rand()/RAND_MAX;
	m_MeanDensity = -0.001f;
	bShowVelocities = 0;

	for( int i=0; i<nCells; i++ )
	{
		vx[i] = vy[i] = prev_vy[i] = prev_vx[i] = 0.0f;
	}
	
	Diffusion = 10.1f;
	Viscocity = 0.00005f;
    
    TEX_RES_X = 1024;
    TEX_RES_Y = 768;
}


void FluidSimulator::initFluidParticles()
{
//	m_MaxParticles = 10000;	
//	m_FluidParticlePool = new Particle[m_MaxParticles];
//	for( int i=0; i< m_MaxParticles; i++ )
//	{
//		Particle* p = m_FluidParticlePool+i;
//		p->x = p->oldx = 2*(float)rand()/RAND_MAX - 1;
//		p->y = p->oldy = 2*(float)rand()/RAND_MAX - 1;
//		
//		p->colorR = (float)rand()/RAND_MAX * 255;
//		p->colorG = (float)rand()/RAND_MAX * 255;
//		p->colorB = (float)rand()/RAND_MAX * 255;
//
//	}
//	
//	movingParticle.x = 0.0f;
//	movingParticle.y = 0.0f;
//	movingParticle.vx = 1.0f;
//	movingParticle.vy = 1.0f;
//	float len = sqrt(movingParticle.vx * movingParticle.vx + movingParticle.vy * movingParticle.vy);
//	if( len != 0.0f )
//	{
//		movingParticle.vx /= len;
//		movingParticle.vy /= len;
//	}
}

#define  WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512

#define INDEX( I, J, N ) ((I) + ((N) + 2)*(J) )

void FluidSimulator::advanceFluidParticles( Particle* pParticles, float dt )
{
	// Update Moving Particle
	//static int counter = 0;
//	float vx = movingParticle.vx;
//	float vy = movingParticle.vy;
//	counter++;
//	if( counter >= 10 )
//	{
//	  counter = 0;	
//	  vx = 2.0f * (float)rand()/RAND_MAX - 1.0f;
//	  vy = 2.0f * (float)rand()/RAND_MAX - 1.0f;
//      float len = sqrt(vx*vx + vy*vy);
//	  if( len != 0.0f )
//	  {
//		vx /= len;
//		vy /= len;
//	  }
//		movingParticle.vx = vx;
//		movingParticle.vy = vy;
//	}
//	
//	float velScale = 0.05f;
//	movingParticle.x += vx * dt * velScale;
//	movingParticle.y += vy * dt * velScale;
//		
//	float x_ = movingParticle.x;
//	float y_ = movingParticle.y;
//	
//	x_ = 0.5f * x_ + 0.5f;
//	y_ = 0.5f * y_ + 0.5f;
//	
//	x_ *= TEX_RES_X;
//	y_ *= TEX_RES_Y;
	
	//addExtImpulse( -vx*2, -vy*2, x_, y_, 10, dt);

	int i0, j0, i1, j1;
	float s0, s1, t0, t1;
	
	for( int i=0; i<MAX_PARTICLES; i++ )
	{
		Particle* p = pParticles + i;
		float x = p->m_X;
		float y = p->m_Y;
		// Find the Interpolated velocity of the fluid at this position.
		//x = (x * 0.5f + 0.5f)*nX;
		//y = nX - (y * 0.5f + 0.5f)*nX;
		x = x/1024 * 64;
        y = nX - y/768 * 64;
        
		if( x < 0.5f )
			x = 0.5f;
		if( x > nX + 0.5f )
			x = nX + 0.5f;
		
		i0 = (int)x; i1 = i0 + 1;
		
		if( y < 0.5f )
			y = 0.5f;
		if( y > nX + 0.5f )
			y = nX + 0.5f;		
		
		j0 = (int)y; j1 = j0 + 1; 
		s1 = x - i0; s0 = 1 - s1; t1 = y - j0; t0 = 1-t1; 
		
		float vx_ = s0 * ( t0 * prev_vx[INDEX(i0,j0,nX)] + t1 * prev_vx[INDEX(i0,j1,nX)] ) 
		+ s1 * ( t0 * prev_vx[INDEX(i1,j0,nX)] + t1 * prev_vx[ INDEX(i1,j1,nX)] );
		
		float vy_ = s0 * ( t0 * prev_vy[INDEX(i0,j0,nX)] + t1 * prev_vy[INDEX(i0,j1,nX)] ) 
		+ s1 * ( t0 * prev_vy[INDEX(i1,j0,nX)] + t1 * prev_vy[ INDEX(i1,j1,nX)] );
		
		p->m_X += vx_  * 100;
		p->m_Y += -vy_  * 100;
	}
}



void FluidSimulator::injectSource(float sourceStrength , int sx, int sy, int radius, float dt)
{
	float s1 = (float)TEX_RES_X/nX;
	float s2 = (float)TEX_RES_Y/nY;
	
	sx/= s1; sy/= s2;
	
	prevDensityR[ INDEX( sx, sy, nX ) ] += sourceStrength*colorR * dt;
	prevDensityG[ INDEX( sx, sy, nX ) ] += sourceStrength*colorG * dt;
	prevDensityB[ INDEX( sx, sy, nX ) ] += sourceStrength*colorB * dt;

	//prev_vy[ INDEX( sx, sy, nX )] = -sourceStrength * dt;
	//prevDensity[ INDEX( sx, sy, nX ) ] = prevDensity[ INDEX( sx, sy, nX ) ] > 255 ? 255 : prevDensity[ INDEX( sx, sy, nX ) ];
}

void FluidSimulator::addExtImpulse(float sourceStrengthX, float  sourceStrengthY, int sx, int sy, int radius, float dt)
{
	float s1 = (float)TEX_RES_X/nX;
	float s2 = (float)TEX_RES_Y/nY;
	
	//sx/= s1; sy/= s2;
	sX = sx; sY = sy;
	m_SourceStrengthX = sourceStrengthX;
	m_SourceStrengthY = sourceStrengthY;

	//float dx = movingParticle.x - sx;
	//float dy = movingParticle.y - dy;
	
	//if( 
	prev_vx[ INDEX( sx, sy, nX )] = sourceStrengthX * dt;	
	prev_vy[ INDEX( sx, sy, nX )] = sourceStrengthY * dt;
	
	
	//prevDensity[ INDEX( sx, sy, nX ) ] = prevDensity[ INDEX( sx, sy, nX ) ] > 255 ? 255 : prevDensity[ INDEX( sx, sy, nX ) ];
}

void FluidSimulator::removeSource(float sourceStrength , int sx, int sy, int radius, float dt)
{
	float s1 = (float)TEX_RES_X/nX;
	float s2 = (float)TEX_RES_Y/nY;
	
	sx/= s1; sy/= s2;
	//prevDensity[ INDEX( sx, sy, nX ) ] -= sourceStrength * dt;
	//prev_vy[ INDEX( sx, sy, nX )] = -sourceStrength * dt;
	//prevDensity[ INDEX( sx, sy, nX ) ] = prevDensity[ INDEX( sx, sy, nX ) ] < 0.0f ? 0.0f : prevDensity[ INDEX( sx, sy, nX ) ];
}

float sqrt2 = 1.0f/sqrt(2.0f);

void FluidSimulator::advance( float dt )
{	
	// Circulation
	//{
//		float s = 1.0f;
//		prev_vy[ INDEX( sX-1, sY, nX) ] = -s * dt;
//		prev_vx[ INDEX( sX-1, sY, nX) ] = 0.0;
//		
//		prev_vx[ INDEX( sX, sY-1, nX) ] = s * dt;
//		prev_vy[ INDEX( sX, sY-1, nX) ] = 0.0;
//
//		prev_vy[ INDEX( sX+1, sY, nX) ] = s * dt;
//		prev_vx[ INDEX( sX+1, sY, nX) ] = 0.0;
//
//		prev_vx[ INDEX( sX, sY+1, nX) ] = -s * dt;
//		prev_vy[ INDEX( sX, sY+1, nX) ] = -0.0;
//
//	}
	// Radial
	//{
//		float s = 1.0f;
//		
//		prev_vy[ INDEX( sX-1, sY, nX) ] = 0.0f;
//		prev_vx[ INDEX( sX-1, sY, nX) ] = -s * dt;
//		
//		prev_vx[ INDEX( sX, sY-1, nX) ] =  0.0f;
//		prev_vy[ INDEX( sX, sY-1, nX) ] = -s * dt;
//		
//		prev_vy[ INDEX( sX+1, sY, nX) ] = 0.0f;
//		prev_vx[ INDEX( sX+1, sY, nX) ] = s * dt;
//		
//		prev_vx[ INDEX( sX, sY+1, nX) ] = 0.0f;
//		prev_vy[ INDEX( sX, sY+1, nX) ] = s * dt;
//		
//		prev_vx[ INDEX( sX+1, sY+1, nX) ] = s * sqrt2 * dt;
//		prev_vy[ INDEX( sX+1, sY+1, nX) ] = s * sqrt2 * dt;
//		
//		prev_vx[ INDEX( sX-1, sY+1, nX) ] = -s * sqrt2 * dt;
//		prev_vy[ INDEX( sX-1, sY+1, nX) ] = s * sqrt2 * dt;
//		
//		prev_vx[ INDEX( sX-1, sY-1, nX) ] = -s * sqrt2 * dt;
//		prev_vy[ INDEX( sX-1, sY-1, nX) ] = -s * sqrt2 * dt;
//		
//		prev_vx[ INDEX( sX+1, sY-1, nX) ] = s * sqrt2 * dt;
//		prev_vy[ INDEX( sX+1, sY-1, nX) ] = -s * sqrt2 * dt;
//		
//		prev_vx[ INDEX( sX, sY, nX) ] = 0.0f;
//		prev_vy[ INDEX( sX, sY, nX) ] = 0.0f;
//		
//	}
	
	advanceVelocity(dt);
}

void FluidSimulator::render()
{
	//glActiveTexture(GL_TEXTURE0);
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, textureObject);
	
//	glEnable(GL_BLEND);
//	glEnable(GL_POINT_SMOOTH);
//	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	
//	glEnableClientState(GL_VERTEX_ARRAY);
//	glEnableClientState(GL_COLOR_ARRAY);
//
//	glPointSize(5.0f);

//	glVertexPointer(2, GL_FLOAT, sizeof(Particle), &m_FluidParticlePool->x );
//	//glVertexPointer(2, GL_FLOAT, 0, lineData );
//
//	glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(Particle), &m_FluidParticlePool->colorR);
//	
//	//glColor3ub(0, 0, 0);
//	glDrawArrays(GL_POINTS, 0, m_MaxParticles);
//	//glColor3ub(255, 255, 255);
//	
//	glDisableClientState(GL_VERTEX_ARRAY);
//	glDisableClientState(GL_COLOR_ARRAY);
}

void FluidSimulator::advanceDensity( float dt )
{
	//diffuseDensity(0,Diffusion, prevDensityR,prevDensityG,prevDensityB, densityR, densityG, densityB, dt);
	diffuse(0,Diffusion, prevDensityR, densityR, dt);
	SWAP_PTR( densityR, prevDensityR );
	advect(0,prevDensityR, densityR, prev_vx, prev_vy, dt);
	SWAP_PTR( densityR, prevDensityR );

	diffuse(0,Diffusion, prevDensityG, densityG, dt);
	SWAP_PTR( densityG, prevDensityG );
	advect(0,prevDensityG, densityG, prev_vx, prev_vy, dt);
	SWAP_PTR( densityG, prevDensityG );

	diffuse(0,Diffusion, prevDensityB, densityB, dt);
	SWAP_PTR( densityB, prevDensityB );
	advect(0,prevDensityB, densityB, prev_vx, prev_vy, dt);
	SWAP_PTR( densityB, prevDensityB );
}

void FluidSimulator::addSources(float* x, float* x0, float dt)
{
	for( int i=0; i<nCells; i++ )
		x0[i] += x[i]*dt;
}

void FluidSimulator::advanceVelocity( float dt )
{
	//addSources(prev_vx, vx, dt);
	//addSources(prev_vy, vy, dt);
	
	//SWAP_PTR( vx, prev_vx );  
	//SWAP_PTR( vy, prev_vy );

	diffuse(1,Viscocity, prev_vx, vx, dt, MAX_ITERATIONS);
	diffuse(2,Viscocity, prev_vy, vy, dt, MAX_ITERATIONS);

	SWAP_PTR( vx, prev_vx );  
	SWAP_PTR( vy, prev_vy );

	advect(1,prev_vx, vx, prev_vx, prev_vy, dt);
	advect(2,prev_vy, vy, prev_vx, prev_vy, dt);

	addExternalForces(vx, vy, dt);
	projectVelocity( vx, vy, prev_vx, prev_vy, dt );
	SWAP_PTR( vx, prev_vx ); 
	SWAP_PTR( vy, prev_vy );
}

void FluidSimulator::addExternalForces(float* vx, float* vy, float dt)
{
	vorticityConfinement( vx, vy, dt );
	
	for( int i=1; i<=nX; i++ )
	{
		for( int j=1; j<=nY; j++ )
		{
			//float rho = ( densityR[ INDEX(i, j, nX) ] + densityG[ INDEX(i, j, nX) ] + densityB[ INDEX(i, j, nX) ] ) * 0.3333;
			//vy[ INDEX(i, j, nX) ] += ( (  rho - m_MeanDensity ) * dt );
			vy[ INDEX(i, j, nX) ] += confinementForceY[ INDEX(i, j, nX) ];
			vx[ INDEX(i, j, nX) ] += confinementForceX[ INDEX(i, j, nX) ];
		}
	}
	setBC(1, vx);
	setBC(2, vy);
}

void FluidSimulator::diffuseDensity( int b, float D, float* inx1, float* inx2, float* inx3, float* outx1, float* outx2, float* outx3, float dt)
{
	float K = D * nX*nX * dt;
	float C = 1.0f/( 1.0f + 4.0f * K );
	// implicit Poisson eqn solution
	for( int iter=0; iter<MAX_ITERATIONS; iter++ )
	{
		for( int i=1; i<=nX; i++ )
		{
			for( int j=1; j<=nY; j++ )
			{
				outx1[ INDEX(i, j, nX) ] = ( inx1[ INDEX(i, j, nX) ] + K * ( outx1[INDEX(i+1, j, nX)] + outx1[INDEX(i-1, j, nX)] + outx1[INDEX(i, j+1, nX)] + outx1[INDEX(i, j-1, nX)]) ) * C;
				outx2[ INDEX(i, j, nX) ] = ( inx2[ INDEX(i, j, nX) ] + K * ( outx2[INDEX(i+1, j, nX)] + outx2[INDEX(i-1, j, nX)] + outx2[INDEX(i, j+1, nX)] + outx2[INDEX(i, j-1, nX)]) ) * C;
				outx3[ INDEX(i, j, nX) ] = ( inx3[ INDEX(i, j, nX) ] + K * ( outx3[INDEX(i+1, j, nX)] + outx3[INDEX(i-1, j, nX)] + outx3[INDEX(i, j+1, nX)] + outx3[INDEX(i, j-1, nX)]) ) * C;
			}
		}
		setBC(b, outx1);
		setBC(b, outx2);
		setBC(b, outx3);
	}
}

void FluidSimulator::diffuse( int b, float D, float* inx, float* outx, float dt, int numIterations )
{
	float K = D * nX*nX * dt;
	float C = 1.0f/( 1.0f + 4.0f * K );
	// implicit Poisson eqn solution
	for( int iter=0; iter<numIterations; iter++ )
	{
		for( int i=1; i<=nX; i++ )
		{
			for( int j=1; j<=nY; j++ )
			{
				outx[ INDEX(i, j, nX) ] = ( inx[ INDEX(i, j, nX) ] + K * ( outx[INDEX(i+1, j, nX)] + outx[INDEX(i-1, j, nX)] + outx[INDEX(i, j+1, nX)] + outx[INDEX(i, j-1, nX)]) ) * C;
			}
		}
		setBC(b, outx);
	}
}

#define CLAMP( X, A, B ) \
X = X < (A) ? (A) : X; \
X = X > (B) ? (B) : X; \

void FluidSimulator::advect(int b, float* inx, float* outx, float* u, float* v, float dt)
{
	float x,y;
	int i0, i1;
	int j0, j1;
	float s1, s0, t1, t0;
	float dt0 = nX * dt;
	for( int i=1; i<=nX; i++ )
	{
		for( int j=1; j<=nY; j++ )
		{
			x = i - u[INDEX(i,j,nX)] * dt0; y = j - v[INDEX(i,j,nX)] * dt0;
			//CLAMP( x, 0.5f, nX + 0.5f )
			if( x < 0.5f )
				x = 0.5f;
			if( x > nX + 0.5f )
				x = nX + 0.5f;
			
			i0 = (int)x; i1 = i0 + 1;
			//CLAMP ( y, 0.5f, nX + 0.5f )
			
			if( y < 0.5f )
				y = 0.5f;
			if( y > nX + 0.5f )
				y = nX + 0.5f;		

			j0 = (int)y; j1 = j0 + 1; 
			s1 = x - i0; s0 = 1 - s1; t1 = y - j0; t0 = 1-t1; 
			
			outx[INDEX(i,j,nX)] = s0 * ( t0 * inx[INDEX(i0,j0,nX)] + t1 * inx[INDEX(i0,j1,nX)] ) 
								+ s1 * ( t0 * inx[INDEX(i1,j0,nX)] + t1 * inx[ INDEX(i1,j1,nX)] );
		}
	}
	setBC(b, outx);
}

// Using Helmholtz decomposition of a vector V = curl A + grad B, the divergent velocity after the advection step is converted into a divergenceless velocity.
// this ensures the continuity condition is satisfied.
// V = curl A + grad B --> div V = laplacian B. hence this Poission can be solved for B.
// Now X = curl B = V - grad B, which gives the desired divergenceless velocity.

void FluidSimulator::projectVelocity(float* vx, float* vy, float* p, float* div, float dt )
{
	// Find Divergence of the velocity output from advection step.
	float dx = 1.0f/nX;
	//dx = dx;
	for( int i=1; i<=nX; i++ )
	{
		for( int j=1; j<=nY; j++ )
		{
			div[INDEX(i,j,nX)] = -( vx[INDEX(i+1,j,nX)] - vx[INDEX(i-1,j,nX)] + vy[INDEX(i,j+1,nX)] - vy[INDEX(i,j-1,nX)] ) * 0.5f * dx;
			p[INDEX(i,j,nX)] = 0.0f;
		}
	}
	setBC(0, div);
	setBC(0, p);
	// Solve Poission equation 
	for( int iter=0; iter<MAX_ITERATIONS; iter++ )
	{
		for( int i=1; i<=nX; i++ )
		{
			for( int j=1; j<=nY; j++ )
			{
				p[ INDEX(i, j, nX) ] = ( div[  INDEX(i, j, nX) ] + p[INDEX(i+1, j, nX)] + p[INDEX(i-1, j, nX)] + p[INDEX(i, j+1, nX)] + p[INDEX(i, j-1, nX)]) * 0.25f;
			}
		}
		setBC(0, p);
	}
	// Subtract grad p from divergent velocity to get divergenceless velocity.
	dx = 1.0f/dx;
	for( int i=1; i<=nX; i++ )
	{
		for( int j=1; j<=nY; j++ )
		{
			vx[INDEX(i,j,nX)] -= ( ( p[INDEX(i+1,j,nX)] - p[INDEX(i-1,j,nX)]) * 0.5f * dx );
			vy[INDEX(i,j,nX)] -= ( ( p[INDEX(i,j+1,nX)] - p[INDEX(i,j-1,nX)]) * 0.5f * dx );
		}
	}
	setBC(1, vx);
	setBC(2, vy);

}

void FluidSimulator::vorticityConfinement(float* u, float* v, float dt)
{
	//return;
	float dx = nX;
	float eps = 0.0f;
	// Find omega = Curl v
	for( int i=1; i<=nX; i++ )
	{
		for( int j=1; j<=nY; j++ )
		{
			omega[INDEX(i,j,nX)] = ( ( v[INDEX(i+1,j,nX)] - v[INDEX(i-1,j,nX)]) - ( u[INDEX(i,j+1,nX)] - u[INDEX(i,j-1,nX)]) ) * 0.5f * dx;
		}
	}
	// Find grad omega
	for( int i=1; i<=nX; i++ )
	{
		for( int j=1; j<=nY; j++ )
		{
			float grad_omega_x = ( omega[INDEX(i+1,j,nX)] - omega[INDEX(i-1,j,nX)]) * 0.5f * dx;
			float grad_omega_y = ( omega[INDEX(i,j+1,nX)] - omega[INDEX(i,j-1,nX)]) * 0.5f * dx;
			float d = sqrt(grad_omega_x*grad_omega_x + grad_omega_y*grad_omega_y);
			if( d > 1e-15 )
			{
				grad_omega_x /= d;
				grad_omega_y /= d;
				confinementForceX[INDEX(i,j,nX)] = grad_omega_x * eps;
				confinementForceY[INDEX(i,j,nX)] = grad_omega_y * eps;
			}
		}
	}
}

void FluidSimulator::setBC(int b, float * x)
{
	for ( int i=1 ; i<=nX ; i++ ) 
	{ 
		x[INDEX(0,i,nX)] = b==1 ? -x[INDEX(1,i,nX)] : x[INDEX(1,i,nX)]; 
		x[INDEX(nX+1,i,nX)] = b==1 ? -x[INDEX(nX,i,nX)] : x[INDEX(nX,i,nX)]; 
		x[INDEX(i,0,nX)] = b==2 ? -x[INDEX(i,1,nX)] : x[INDEX(i,1,nX)];
		x[INDEX(i,nX+1,nX)] = b==2 ? -x[INDEX(i,nX,nX)] : x[INDEX(i,nX,nX)];
	}
	
	x[INDEX(0 ,0,nX )] = 0.5*( x[INDEX(1,0,nX )] + x[INDEX(0 ,1,nX)]); 
	x[INDEX(0,nX+1,nX)]	=	0.5*(x[INDEX(1,nX+1,nX)]+x[INDEX(0,nX,nX )]); 
	x[INDEX(nX+1,0,nX )] = 0.5*(x[INDEX(nX,0,nX )]+x[INDEX(nX+1,1,nX)]); 
	x[INDEX(nX+1,nX+1,nX)] = 0.5*(x[INDEX(nX,nX+1,nX)]+x[INDEX(nX+1,nX,nX)]);
}


#define SWAP( A, B )  \
{                     \
int TEMP = A;     \
A = B;            \
B = TEMP;         \
}                     \

void FluidSimulator:: RasterizeVelocity( const Vector2i& start, const Vector2i& end )
{    
    Vector2i lineBegin = start;//GetGridCoordinate(start + Vector2f(16,16) );
    Vector2i lineEnd   = end;//GetGridCoordinate(end   + Vector2f(16,16) );

    Vector2f direction = Vector2f(lineEnd.x, lineEnd.y) - Vector2f(lineBegin.x, lineBegin.y);
    direction.normalize();
    float velocityMagnitude = 0.1f;

    int dy = end.y - lineBegin.y;
    int dx = end.x - lineBegin.x;

    bool bSteep = abs(dy) > abs(dx);

    if( bSteep )
    {
        // Swap X0,y)
        SWAP( lineBegin.x, lineBegin.y );
        SWAP( lineEnd.x  , lineEnd.y );
        dy = lineEnd.y - lineBegin.y;
        dx = lineEnd.x - lineBegin.x;
    }
    if( lineBegin.x > lineEnd.x )
    {
        SWAP( lineBegin.x, lineEnd.x );
        SWAP( lineBegin.y, lineEnd.y );
        dy *= -1;
        dx *= -1;
    }
    int yStep = 1;
    if( lineBegin.y > lineEnd.y )
    {
        yStep = -1;
    }


    int E = 0;
    int y = lineBegin.y;

    for( int x=lineBegin.x; x<=lineEnd.x; x++)
    {
        //int tileID = x * 64 + y;
        if( bSteep )
        {
            //            vx[INDEX(y, x, nX)] = direction.x * velocityMagnitude;
            //            vy[INDEX(y, x, nX)] = direction.y * velocityMagnitude;
            addExtImpulse(direction.x * velocityMagnitude, direction.y * velocityMagnitude, y, x, 10000, 0.16);
        }
        //  tileID = PositionToIndex( Vector2i(y*32,x*32) );
        else
        {
            //            vx[INDEX(x, y, nX)] = direction.x * velocityMagnitude;
            //            vy[INDEX(x, y, nX)] = direction.y * velocityMagnitude;
            addExtImpulse(direction.x * velocityMagnitude, direction.y * velocityMagnitude, x, y, 10000, 0.16);
            //  tileID = PositionToIndex( Vector2i(x*32,y*32) );
        }
        //printf("\n Tile Pos: %d,%d", x,y);
        //vecTileIDs.push_back(tileID);

        E += dy*yStep;
        //printf("\n 2*E=%d, dx*yStep = %d", E*2,dx*yStep);

        if( (E << 1) >=dx * yStep )
        {
            y+=yStep;
            E -= dx;
        }
    }
}
