#include "ParticleBase.h"
#include "ParticleSystem.h"
#include <math.h>


		void ParticleGravityAnimator::animate( double currentTime )
		{
			double dt = (currentTime - prevTime)/1000.0f;
			m_pParticleSystem->m_NumActiveParticles = 0;
			vector<Particle>& vecParticlePool = m_pParticleSystem->m_ParticlePool;
			int n = vecParticlePool.size()-1;

			if ( n < 0 ) return;

			for( unsigned int i=0; i<(vecParticlePool.size()-1); i++ )
			{
				Particle& p = vecParticlePool[i];
				p.m_CurrentLife -= p.m_Decay;

                float t = p.m_CurrentLife/p.m_Life;
                
                t = t < 0.5f ? 2*t : 2*(1-t);
                p.m_Color.a = t;
                
				if ( p.m_CurrentLife <= 1e-10 && !p.m_isDead )
				{
					p.m_isDead = true;
				}
				
				if ( p.m_isDead )
				{
					// Copy Last Particle and Replace this Particle, which is dead.
					vecParticlePool[i] = vecParticlePool[vecParticlePool.size()-1];
					// Remove the last particle, which is copied to the above location.
					vecParticlePool.pop_back();
					continue;
				}
				p.m_Vx += 0.0f * dt;
				p.m_Vy += m_Gravity * dt;

				p.m_PrevX = p.m_X;
				p.m_PrevY = p.m_Y;

				p.m_X += p.m_Vx * dt;
				p.m_Y += p.m_Vy * dt;
				
				
				m_pParticleSystem->m_NumActiveParticles++;
			}
			prevTime = currentTime;
		}


		void ParticleDiffuseAnimator::animate( double currentTime )
		{
			double dt = (currentTime - prevTime)/1000.0f;
			m_pParticleSystem->m_NumActiveParticles = 0;
			vector<Particle>& vecParticlePool = m_pParticleSystem->m_ParticlePool;
			int n = vecParticlePool.size()-1;
			//printf("%d\n",vecParticlePool.size());
			if ( n < 0 )return;

			for( int i=0; i<(vecParticlePool.size()-1); i++ )
			{
				Particle& p = vecParticlePool[i];
				p.m_CurrentLife -= p.m_Decay;

                float t = p.m_CurrentLife/p.m_Life;
                
                t = t < 0.5f ? 2*t : 2*(1-t);
                p.m_Color.a = t;
                
				if ( p.m_CurrentLife <= 1e-10 && !p.m_isDead )
				{
					p.m_isDead = true;
				}
				
				if ( p.m_isDead )
				{
					// Copy Last Particle and Replace this Particle, which is dead.
					vecParticlePool[i] = vecParticlePool[vecParticlePool.size()-1];
					// Remove the last particle, which is copied to the above location.
					vecParticlePool.pop_back();
					continue;
				}
				// Random Force Generation
				float fx = (float)rand()/RAND_MAX - 0.5f;
				float fy = (float)rand()/RAND_MAX - 0.5f;
				
				p.m_Vx += m_DiffuseFactor*fx * dt;
				p.m_Vy += m_DiffuseFactor*fy * dt;

				p.m_X += p.m_Vx * dt;
				p.m_Y += p.m_Vy * dt;
			
				m_pParticleSystem->m_NumActiveParticles++;
			}
			prevTime = currentTime;
		}

		void ParticleFlockAnimator::animate( double currentTime )
		{
			float dt = (currentTime - prevTime)/1000.0f;
			m_pParticleSystem->m_NumActiveParticles = 0;
			vector<Particle>& vecParticlePool = m_pParticleSystem->m_ParticlePool;

			for( unsigned int i=0; i<(vecParticlePool.size()-1); i++ )
			{
				Particle& p = vecParticlePool[i];
				p.m_CurrentLife -= p.m_Decay;

				if ( p.m_CurrentLife <= 1e-10 && !p.m_isDead )
				{
					p.m_isDead = true;
				}
				
				if ( p.m_isDead )
				{
					// Copy Last Particle and Replace this Particle, which is dead.
					vecParticlePool[i] = vecParticlePool[vecParticlePool.size()-1];
					// Remove the last particle, which is copied to the above location.
					vecParticlePool.pop_back();
					continue;
				}
				// Random Force Generation
				float fx = (float)rand()/RAND_MAX - 0.5f;
				float fy = (float)rand()/RAND_MAX - 0.5f;
				
                p.m_PrevX = p.m_X;
				p.m_PrevY = p.m_Y;
                
				p.m_Vx += fx * dt;
				p.m_Vy += fy * dt;

				p.m_X += p.m_Vx * dt;
				p.m_Y += p.m_Vy * dt;
				
                m_pParticleSystem->m_NumActiveParticles++;
			}
			prevTime = currentTime;
		}

		void ParticleOrbitCentreAnimator::animate( double currentTime )
		{
			float dt = (currentTime - prevTime)/1000.0f;
			m_pParticleSystem->m_NumActiveParticles = 0;
			vector<Particle>& vecParticlePool = m_pParticleSystem->m_ParticlePool;
			int n = vecParticlePool.size()-1;
			
			if ( n < 0 ) return;
			for( unsigned int i=0; i<(vecParticlePool.size()-1); i++ )
			{
				Particle& p = vecParticlePool[i];
				p.m_CurrentLife -= p.m_Decay;
                
                p.m_Color.a = p.m_CurrentLife/p.m_Life;
                
				if ( p.m_CurrentLife <= 1e-10 && !p.m_isDead )
				{
					p.m_isDead = true;
				}
				if ( p.m_isDead )
				{
					// Copy Last Particle and Replace this Particle, which is dead.
					vecParticlePool[i] = vecParticlePool[vecParticlePool.size()-1];
					// Remove the last particle, which is copied to the above location.
					vecParticlePool.pop_back();
					continue;
				}
				// Find radius vector from the Centre of Orbit
				Vector2f R(p.m_X,p.m_Y);
				//Vector2f V(p.m_Vx,p.m_Vy,p.m_Vz);
				//Vector2f V = m_TornadoAxis.cross(R);
				//Vector2f m_TornadoAxis = R.cross(V);
				//float Rsq = R.length();
				R.normalize();
                R *= m_Strength;
				//Vector2f A = m_TornadoAxis.cross(V);
				Vector2f A(-R.x, -R.y);
                if( m_bReverseAcceleration )
                    A *= -1.0f;
                
				A *= ( 1e1);

                p.m_PrevX = p.m_X;
                p.m_PrevY = p.m_Y;
                
				p.m_Vx += A.x * dt;
				p.m_Vy += A.y * dt;

				p.m_X += p.m_Vx * dt;
				p.m_Y += p.m_Vy * dt;
				
				//p.m_R = p.m_Life * 255;
				//p.m_G = 127;
				//p.m_B = (1-p.m_Life) * 255;

				m_pParticleSystem->m_NumActiveParticles++;
			}
			prevTime = currentTime;
		}

		void ParticleTargetFollowAnimator::animate( double currentTime )
		{
			float dt = (currentTime - prevTime)/1000.0f;
			m_pParticleSystem->m_NumActiveParticles = 0;
			vector<Particle>& vecParticlePool = m_pParticleSystem->m_ParticlePool;
			int n = vecParticlePool.size()-1;
			
			if ( n < 0 ) return;

			for( unsigned int i=0; i<(vecParticlePool.size()-1); i++ )
			{
				Particle& p = vecParticlePool[i];
				p.m_CurrentLife -= p.m_Decay;

				if ( p.m_CurrentLife <= 1e-10 && !p.m_isDead )
				{
					p.m_CurrentLife = 0.0f;
					p.m_isDead = true;
				}
				
				if ( p.m_isDead )
				{
					// Copy Last Particle and Replace this Particle, which is dead.
					vecParticlePool[i] = vecParticlePool[vecParticlePool.size()-1];
					// Remove the last particle, which is copied to the above location.
					vecParticlePool.pop_back();
					continue;
				}
				// Move target Vector 
				m_Target.x = 2.0f * cos(currentTime*0.001f);

				// Find vector from the target
				Vector2f T(m_Target.x - p.m_X, m_Target.y - p.m_Y);
				float dist = T.length();
				T.normalize();
				float maxSpeed = 10.0f;
				Vector2f V(-p.m_Vx + maxSpeed*T.x, -p.m_Vy + maxSpeed*T.y);
				// Find distance between target and particle
				float R = 1.0f;
				dist = dist < R ? R : dist;
				dist -= R;
				
				//dist = dist > 1.0f ? 1.0f : dist;
				V *= (dist);
				//Vector2f D(-p.m_Vx,-p.m_Vy,-p.m_Vz);
				//D.scale(0.1f);
				//V.add(D);

				/*V.x = V.x > 1000.0f ? 1000.0f : V.x;
				V.y = V.y > 1000.0f ? 1000.0f : V.y;
				V.z = V.z > 1000.0f ? 1000.0f : V.z;*/
				//maxV = maxV < V.length() ? V.length() : maxV;

				p.m_Vx += V.x*dt;// > 10.0f ? V.x : V.x;
				p.m_Vy += V.y*dt;// > 10.0f ? V.y : V.y;

				p.m_X += p.m_Vx * dt;
				p.m_Y += p.m_Vy * dt;
				
                m_pParticleSystem->m_NumActiveParticles++;
			}
			//printf("Max Velocity=%f\n", maxV);
			prevTime = currentTime;
		}
