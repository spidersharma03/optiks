#include "ParticleBase.h"
#include "ParticleSystem.h"
#include <vector>
#include <math.h>

using namespace std;

		// This Emitter emits particle from a PointSource
		unsigned int ParticleEmitterPoint::emit(double currentTime)
		{
			if( !m_bEnable ) return 0;

			m_CurrentTime = currentTime;

			unsigned int numParticles = calculateSpawnParticles();
            
			vector<Particle>& particlePool = m_pParticleSystem->m_ParticlePool;

			int nDeadParticles = m_pParticleSystem->m_MaxParticles - particlePool.size();

			numParticles = numParticles > nDeadParticles ? nDeadParticles : numParticles;

			for(unsigned int p=0; p<numParticles; p++)
			{
				Particle particle;

				initializeParticle(particle);

				particlePool.push_back(particle);
			}
            
            
			m_PrevTime = m_CurrentTime;
			return numParticles;
		}


		void ParticleEmitterPoint::modulateEmissionRate(float currentTime)
		{
			return;
			static int i = 0;
			int n1 = 150;
			int n2 = 200;
			i++;

			if ( i > n2 )
				i = 0;

			if ( i > n1 && i < n2 )
			{
				m_EmissionRate = 0.0f;
			}
			else
			{
				m_EmissionRate = 1500.0f;
			}

			//m_EmissionRate = 500 + ( 500.0f * sin(currentTime*0.0001f) );
		}

		// LINE SOURCE

		unsigned int ParticleEmitterLine::emit(double currentTime)
		{
			if( !m_bEnable ) return 0;

			m_CurrentTime = currentTime;

			unsigned int numParticles = calculateSpawnParticles();

			vector<Particle>& particlePool = m_pParticleSystem->m_ParticlePool;

			int nDeadParticles = m_pParticleSystem->m_MaxParticles - particlePool.size();

			numParticles = numParticles > nDeadParticles ? nDeadParticles : numParticles;

			for(unsigned int p=0; p<numParticles; p++)
			{
				Particle particle;

				float y  = (float)rand()/RAND_MAX;

				particle.m_Y = m_Length * y;

				float vx = 2.0f*(float)rand()/RAND_MAX - 1.0f;
				float vy = 0.0f;//(float)rand()/RAND_MAX ;

				particle.m_isDead = false;
				particle.m_CurrentLife = particle.m_Life;
				particle.m_Decay = 0.0002f;
				particle.m_Vx = 0.001*vx;
				particle.m_Vy = 0.001*vy;

				particle.m_X = 0.0f;
				particle.m_Y = 0.0f;

				initializeParticle(particle);

				particlePool.push_back(particle);

			}

			m_PrevTime = m_CurrentTime;
			return numParticles;
		}

		void ParticleEmitterLine::modulateEmissionRate(float currentTime)
		{
			//m_EmissionRate = 1500 + ( 1500.0f * sin(currentTime*0.001) );
		}

		// CYLINDRICAL SOURCE
		// LINE SOURCE

		unsigned int ParticleEmitterCylinder::emit(double currentTime)
		{
			if( !m_bEnable ) return 0; 

			m_CurrentTime = currentTime;

			unsigned int numParticles = calculateSpawnParticles();

			vector<Particle>& particlePool = m_pParticleSystem->m_ParticlePool;

			int nDeadParticles = m_pParticleSystem->m_MaxParticles - particlePool.size();

			numParticles = numParticles > nDeadParticles ? nDeadParticles : numParticles;

			for(unsigned int p=0; p<numParticles; p++)
			{
				Particle particle;

				float y		= (float)rand()/RAND_MAX;
				float theta  = (float)rand()/RAND_MAX * M_PI * 2.0f;


				//float vx = 2.0f*(float)rand()/RAND_MAX - 1.0f;
				//float vy = 0.0f;//(float)rand()/RAND_MAX ;
				//float vz = 2.0f*(float)rand()/RAND_MAX - 1.0f;

				float vx = 2.0f*(float)rand()/RAND_MAX - 1.0f;
				float vy = 0.0f;//(float)rand()/RAND_MAX ;
				float vz = 2.0f*(float)rand()/RAND_MAX - 1.0f;

				particle.m_isDead = false;
				particle.m_CurrentLife = particle.m_Life;
				particle.m_Decay = 0.003f;

				float radius = m_Radius1*(1-y) + y*m_Radius2;

				particle.m_Y = m_Length * y;
				particle.m_X = radius * cos(theta);

				Vector2f R(particle.m_X,0.0);
				Vector2f m_TornadoAxis(0,1);
				//Vector2f V = m_TornadoAxis.cross(R);

//				particle.m_Vx = V.x;
//				particle.m_Vy = 0.0f;
//				particle.m_Vz = V.z;

				initializeParticle(particle);

				particlePool.push_back(particle);
			}

			m_PrevTime = m_CurrentTime;
			return numParticles;
		}

		void ParticleEmitterCylinder::modulateEmissionRate(float currentTime)
		{
			//m_EmissionRate = 1500 + ( 1500.0f * sin(currentTime*0.001) );
		}

		// SPHERICAL SOURCE

		unsigned int ParticleEmitterCircle::emit(double currentTime)
		{
			if( !m_bEnable ) return 0;

			m_CurrentTime = currentTime;

			unsigned int numParticles = calculateSpawnParticles();

			vector<Particle>& particlePool = m_pParticleSystem->m_ParticlePool;

			int nDeadParticles = m_pParticleSystem->m_MaxParticles - particlePool.size();

			numParticles = numParticles > nDeadParticles ? nDeadParticles : numParticles;

			for(unsigned int p=0; p<numParticles; p++)
			{
				Particle particle;

				// Choose Random Spawn location
				float phi    = (float)rand()/RAND_MAX * M_PI * 2.0f;
                float radius = (float)rand()/RAND_MAX * m_Radius + 10;
                
				particle.m_isDead = false;
				particle.m_CurrentLife = particle.m_Life;

				particle.m_PrevY = particle.m_Y = radius * cos(phi);
				particle.m_PrevX = particle.m_X = radius * sin(phi);

                initializeParticle(particle);

				Vector2f R(particle.m_X ,particle.m_Y );
				
                Vector2f V(R.y, -R.x);
                V *= 1.0;
                
				particle.m_Vx = V.x;
				particle.m_Vy = V.y;
//				particle.m_Vz = V.z;


				particlePool.push_back(particle);
			}

			m_PrevTime = m_CurrentTime;
			return numParticles;
		}

		void ParticleEmitterCircle::modulateEmissionRate(float currentTime)
		{
			//m_EmissionRate = 1500 + ( 1500.0f * sin(currentTime*0.001) );
		}


        unsigned int ParticleEmitterRect::emit(double currentTime)
        {
            if( !m_bEnable ) return 0;
            
            m_CurrentTime = currentTime;
            
            unsigned int numParticles = calculateSpawnParticles();
            
            vector<Particle>& particlePool = m_pParticleSystem->m_ParticlePool;
            
            int nDeadParticles = m_pParticleSystem->m_MaxParticles - particlePool.size();
            
            numParticles = numParticles > nDeadParticles ? nDeadParticles : numParticles;
            
            for(unsigned int p=0; p<numParticles; p++)
            {
                Particle particle;
                
                // Choose Random Spawn location
                float x    = (float)rand()/RAND_MAX * m_Width;
                float y    = (float)rand()/RAND_MAX * m_Height;
                
                particle.m_isDead = false;
                particle.m_CurrentLife = particle.m_Life;
                
                particle.m_PrevY = particle.m_Y = y;
                particle.m_PrevX = particle.m_X = x;
                
                Vector2f R(particle.m_X ,particle.m_Y );
                
                //				particle.m_Vz = V.z;
                
                initializeParticle(particle);
                
                particlePool.push_back(particle);
            }
            
            m_PrevTime = m_CurrentTime;
            return numParticles;
        }

        void ParticleEmitterRect::modulateEmissionRate(float currentTime)
        {
            //m_EmissionRate = 1500 + ( 1500.0f * sin(currentTime*0.001) );
        }

        unsigned int ParticleEmitterCircularDisk::emit(double currentTime)
        {
            if( !m_bEnable ) return 0;
            
            m_CurrentTime = currentTime;
            
            unsigned int numParticles = calculateSpawnParticles();
            
            vector<Particle>& particlePool = m_pParticleSystem->m_ParticlePool;
            
            int nDeadParticles = m_pParticleSystem->m_MaxParticles - particlePool.size();
            
            numParticles = numParticles > nDeadParticles ? nDeadParticles : numParticles;
            
            for(unsigned int p=0; p<numParticles; p++)
            {
                Particle particle;
                
                // Choose Random Spawn location
                float phi    = (float)rand()/RAND_MAX * M_PI * 2.0f;
                float radius = (float)rand()/RAND_MAX * (m_MaxRadius-m_MinRadius) + m_MinRadius;
                
                particle.m_isDead = false;
                particle.m_CurrentLife = particle.m_Life;
                
                particle.m_PrevY = particle.m_Y = radius * cos(phi);
                particle.m_PrevX = particle.m_X = radius * sin(phi);
                
                Vector2f R(particle.m_X ,particle.m_Y );
                
                Vector2f V(R.y, -R.x);
                
                //				particle.m_Vz = V.z;
                initializeParticle(particle);
                
                particle.m_Vx = V.x;
                particle.m_Vy = V.y;
                
                particlePool.push_back(particle);
            }
            
            m_PrevTime = m_CurrentTime;
            return numParticles;
        }

        void ParticleEmitterCircularDisk::modulateEmissionRate(float currentTime)
        {
            //m_EmissionRate = 1500 + ( 1500.0f * sin(currentTime*0.001) );
        }