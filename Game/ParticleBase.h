#ifndef _PARTICLE_BASE_H
#define _PARTICLE_BASE_H

#include "spiderDataTypes.h"
#include "Color4.h"
#include "Matrix4.h"
#include "Vector2.h"
#include <vector>

USE_NAMESPACE_DIRAC2D

using namespace std;

class ParticleSystem;

		struct Particle
		{
			float m_X, m_Y;
			float m_PrevX, m_PrevY;
			Color4f m_Color;

			float m_Vx, m_Vy;
			float m_Ax, m_Ay;
			float size;

			float m_Mass, m_InvMass;
			float m_Life, m_Decay;
            float m_CurrentLife;
			bool m_isDead;

			float m_Restitution;
			float distFromCameraSq;

			float m_SizeX, m_SizeY;
			float m_FrictionCoefficient;
			void* userData;

			Particle()
			{
				m_Mass = m_InvMass = 0.0f;
				m_X = m_Y = 0.0f;
				m_Vx = m_Vy = 0.0f;
				m_Ax = m_Ay = 0.0f;
				size = 1.0f;
				m_Life = m_Decay = 0.0f;
                m_CurrentLife = 0.0f;
				m_isDead = true;
				distFromCameraSq = 0.0f;
				m_Restitution = 0.99f;
				m_SizeX = m_SizeY = 1.0f;
				m_FrictionCoefficient = 0.0f;
				userData = 0;
			}
			~Particle()
			{
			}

		};

		// Call back function prototype
		typedef void (*PARTICLE_CALL_BACK)(Particle&, void* userData);

		class ParticleEmitter
		{
		public:
			ParticleEmitter()
			{
				m_EmissionRate = 10.0f;
				particlesToSpawn = 0.0f;
				m_LifeTime = 1.0f;
				m_LifeTimeVariance = 0.0f;
				m_Decay = 0.01f;
				m_DecayVariance = 0.0f;
				m_bEnable = true;
				onBirthCallBack = 0;
				onDeathCallBack = 0;
                m_Size = 10.0f;
                m_SizeVariance = 2.0f;
                
                m_bScale = true;
                m_bRotate = true;
			}

            virtual ~ParticleEmitter()
            {
                
            }
            
			virtual void initializeParticle(Particle& p)
			{
				Vector2f point;
				//m_Transform.transformAsPoint(point);

				//p.m_X = point.x;
				//p.m_Y = point.y;

				//p.m_PrevX = point.x;
				//p.m_PrevY = point.y;

				Vector2f velocity(m_Velocity.x + m_VelocityVariance.x*((float)rand()/RAND_MAX - 0.5f),
                                  m_Velocity.y + m_VelocityVariance.y*((float)rand()/RAND_MAX- 0.5f));
				//m_Transform.transformAsVector(velocity);

				p.m_Vx = velocity.x;
				p.m_Vy = velocity.y;
                
                p.size = m_Size + m_SizeVariance * (float)rand()/RAND_MAX;
                
				p.m_Color = m_Color;// + m_ColorVariance.r*(float)rand()/RAND_MAX;
				
				p.m_CurrentLife = p.m_Life =	m_LifeTime + m_LifeTimeVariance*(float)rand()/RAND_MAX;

				p.m_Decay = m_Decay + m_DecayVariance*(float)rand()/RAND_MAX;
				p.m_isDead = false;

				if ( onBirthCallBack )
					onBirthCallBack(p,0);
			}



			virtual unsigned int emit(double currentTime) = 0;

			virtual void modulateParticleProperties(float currentTime)
			{
			}

			virtual void modulateEmissionRate(float currentTime)
			{
			}
			// inline Methods
			inline void setTransform(Matrix4f& transform)
			{
				m_Transform = transform;
			}

			inline Matrix4f& getTransform()
			{
				return m_Transform;
			}
            
            inline void setSize( float size )
            {
                m_Size = size;
            }
            
            inline void setSizeVariance( float sizeVar )
            {
                m_SizeVariance = sizeVar;
            }

            inline void setRotateWithVelocity( bool bRotate )
            {
                m_bRotate = bRotate;
            }
            
            inline void setScaleWithVelocity( bool bScale )
            {
                m_bScale = bScale;
            }
            
			inline void setEnable( bool bEnable )
			{
				m_bEnable = bEnable;
			}

			inline void setEmissionRate(float emissionRate)
			{
				m_EmissionRate = emissionRate;
			}

			inline float getEmissionRate()
			{
				return m_EmissionRate;
			}

			inline void setVelocity(const Vector2f& V)
			{
				m_Velocity = V;
			}

			inline void setVelocityVariance(const Vector2f& var )
			{
				m_VelocityVariance = var;
			}

			inline void setColor( const Color4f& color )
			{
				m_Color = color;
			}

			inline void setColorVariance( const Color4f& var )
			{
				m_ColorVariance = var;
			}

			inline void setLifeTime( float lifeTime)
			{
				m_LifeTime = lifeTime;
			}

			inline void setLifeTimeVariance( float lifeTimeVar)
			{
				m_LifeTimeVariance = lifeTimeVar;
			}

			inline void setDecay( float decay)
			{
				m_Decay = decay;
			}

			inline void setDecayVariance( float decayVar)
			{
				m_DecayVariance = decayVar;
			}

			void registerOnBirthCallBack(PARTICLE_CALL_BACK callback)
			{
				onBirthCallBack = callback;
			}

			void registerOnDeathCallBack(PARTICLE_CALL_BACK callback)
			{
				onDeathCallBack = callback;
			}

			friend class ParticleNode;

		protected:

			inline unsigned int calculateSpawnParticles()
			{
				double diffTime = (m_CurrentTime - m_PrevTime)/ 1000;

				//modulateEmissionRate(m_CurrentTime);

				unsigned int numParticles;

				particlesToSpawn += diffTime * m_EmissionRate;

				if ( particlesToSpawn < 1 )
				{
					numParticles = 0;
				}
				else
				{
					numParticles = (int)particlesToSpawn;
					particlesToSpawn -= numParticles;
				}
                return numParticles;
			}

            ParticleSystem *m_pParticleSystem;
			float m_EmissionRate;
			Matrix4f m_Transform;
			Vector2f m_Velocity;
			Vector2f m_VelocityVariance;
			Color4f m_Color;
			Color4f m_ColorVariance;
			float m_LifeTime;
			float m_LifeTimeVariance;
			float m_Decay;
			float m_DecayVariance;
            float m_Size;
            float m_SizeVariance;
			double m_CurrentTime;
			double m_PrevTime;
			float particlesToSpawn;
			bool m_bEnable;
            bool m_bRotate;
            bool m_bScale;
            
            friend class ParticleSystem;
            
			PARTICLE_CALL_BACK onBirthCallBack;
			PARTICLE_CALL_BACK onDeathCallBack;
		};

		class ParticleEmitterPoint: public ParticleEmitter
		{
		public:
			ParticleEmitterPoint()
			{
			}

			ParticleEmitterPoint(Vector2f& source):m_Source(source)
			{
			}

			virtual unsigned int emit(double currentTime);

			virtual void modulateEmissionRate(float currentTime);

		private:
			Vector2f m_Source;
		};

		class ParticleEmitterLine: public ParticleEmitter
		{
		public:
			ParticleEmitterLine(float length):m_Length(length)
			{
			}

			virtual unsigned int emit(double currentTime);

			virtual void modulateEmissionRate(float currentTime);

		private:
			float m_Length;
		};

        class ParticleEmitterRect: public ParticleEmitter
        {
        public:
            ParticleEmitterRect(float width, float height):m_Width(width), m_Height(height)
            {
            }
            
            virtual unsigned int emit(double currentTime);
            
            virtual void modulateEmissionRate(float currentTime);
            
        private:
            float m_Width, m_Height;
        };

		class ParticleEmitterCylinder: public ParticleEmitter
		{
		public:
			ParticleEmitterCylinder(float length, float radius):m_Length(length),m_Radius1(radius),m_Radius2(radius)
			{
			}

			ParticleEmitterCylinder(float length, float radius1, float radius2):m_Length(length),m_Radius1(radius1),m_Radius2(radius2)
			{
			}

			virtual unsigned int emit(double currentTime);

			virtual void modulateEmissionRate(float currentTime);

		private:
			float m_Length, m_Radius1, m_Radius2;
		};

		class ParticleEmitterCircle: public ParticleEmitter
		{
		public:
			ParticleEmitterCircle(float radius):m_Radius(radius)
			{
			}

			virtual unsigned int emit(double currentTime);

			virtual void modulateEmissionRate(float currentTime);

		private:
			float m_Radius;
		};

        class ParticleEmitterCircularDisk: public ParticleEmitter
        {
        public:
            ParticleEmitterCircularDisk(float minRadius, float maxRadius):m_MinRadius(minRadius), m_MaxRadius(maxRadius)
            {
            }
            
            virtual unsigned int emit(double currentTime);
            
            virtual void modulateEmissionRate(float currentTime);
            
        private:
            float m_MinRadius, m_MaxRadius;
        };

		class ParticleAnimator
		{
		public:
			ParticleAnimator(ParticleSystem* pParticleSystem):m_pParticleSystem(pParticleSystem)
			{
			}

			virtual void animate( double currentTime) = 0;

			friend class ParticleNode;

		protected:
			double currTime, prevTime;
			ParticleSystem* m_pParticleSystem;
            
            friend class ParticleSystem;
		};

		class ParticleGravityAnimator:public ParticleAnimator
		{
		public:

			ParticleGravityAnimator(ParticleSystem* pParticleSystem):ParticleAnimator(pParticleSystem)
			{
				m_Gravity = -10.0f;
			}

			ParticleGravityAnimator(ParticleSystem* pParticleSystem, float gravity):ParticleAnimator(pParticleSystem),m_Gravity(gravity)
			{

			}

			inline void setGravity(float gravity)
			{
				m_Gravity = gravity;
			}

			inline float getGravity()
			{
				return m_Gravity;
			}

			virtual void animate( double currentTime);

		private:
			float m_Gravity;
		};


		class ParticleDiffuseAnimator:public ParticleAnimator
		{
		public:

			ParticleDiffuseAnimator(ParticleSystem* pParticleSystem):ParticleAnimator(pParticleSystem),m_DiffuseFactor(1.0f)
			{
			}

			ParticleDiffuseAnimator(ParticleSystem* pNode, float diffuseFactor):ParticleAnimator(pNode),m_DiffuseFactor(diffuseFactor)
			{
			}

			void setDiffuseFactor(float diffuseFactor)
			{
				m_DiffuseFactor = diffuseFactor;
			}

			float getDiffuseFactor()
			{
				return m_DiffuseFactor;
			}

			virtual void animate( double currentTime );

		private:
			float m_DiffuseFactor;
		};

		class ParticleFlockAnimator:public ParticleAnimator
		{
		public:

			ParticleFlockAnimator(ParticleSystem* pParticleSystem):ParticleAnimator(pParticleSystem)
			{
			}

			virtual void animate( double currentTime );

		}; 

		class ParticleOrbitCentreAnimator:public ParticleAnimator
		{
		public:

			ParticleOrbitCentreAnimator(ParticleSystem* pParticleSystem):ParticleAnimator(pParticleSystem),m_Strength(1.0f)
			{
                m_bReverseAcceleration = false;
			}

			ParticleOrbitCentreAnimator(ParticleSystem* pParticleSystem, float strength):ParticleAnimator(pParticleSystem),m_Strength(strength)
			{
                m_bReverseAcceleration = false;
			}

			inline void setCentre(Vector2f& centre)
			{
				m_Centre = centre;
			}

			inline Vector2f& getCentre()
			{
				return m_Centre;
			}

            void  reverseAcceleration(bool bReverse)
            {
                m_bReverseAcceleration = bReverse;
            }
            
            void setStrength( float strength )
            {
                m_Strength = strength;
            }
            
			virtual void animate( double currentTime );
		private:
			float m_Strength;
            bool  m_bReverseAcceleration;
			Vector2f m_Centre;
		}; 


		class ParticleTargetFollowAnimator:public ParticleAnimator
		{
		public:

			ParticleTargetFollowAnimator(ParticleSystem* pParticleSystem):ParticleAnimator(pParticleSystem),m_Target(0.0f,0.0f)
			{
			}

			ParticleTargetFollowAnimator(ParticleSystem* pParticleSystem, Vector2f& target):ParticleAnimator(pParticleSystem),m_Target(target)
			{
			}

			inline void setTarget(Vector2f& target)
			{
				m_Target = target;
			}

			inline Vector2f& getTarget()
			{
				return m_Target;
			}

			virtual void animate( double currentTime );
		private:
			Vector2f m_Target;
		}; 

#endif