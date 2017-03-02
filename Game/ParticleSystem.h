#ifndef _PARTICLE_NODE_H
#define _PARTICLE_NODE_H

#include <stack>
#include <vector>
#include "Vector2.h"
#include "Color4.h"

USE_NAMESPACE_DIRAC2D

using namespace std;

class Particle;
class ParticleEmitter;
class ParticleEmitterPoint;
class ParticleEmitterLine;
class ParticleEmitterCylinder;
class ParticleEmitterSphere;
class ParticleEmitterRect;
class ParticleAnimator;
class ParticleGravityAnimator;
class ParticleTornadoAnimator;
class ParticleDiffuseAnimator;
class ParticleFlockAnimator;
class ParticleOrbitAnimator;
class ParticleTargetFollowAnimator;

typedef struct Vertex2f
{
	//! vertices (3F)
	Vector2f		vertices;			// 8 bytes
    //	char __padding__[4];
    
	//! colors (4B)
	Color4f		colors;				// 16 bytes
    
	// tex coords (2F)
	Vector2f			texCoords;			// 8 byts
} Vertex2f;

typedef struct Quad
{
	//! bottom left
	Vertex2f	bl;
	//! bottom right
	Vertex2f	br;
	//! top right
	Vertex2f	tr;
	//! top left
	Vertex2f	tl;
} Quad;

class Texture;

class ParticleSystem
{
public:

    static const enum PARTICLE_SHAPE{ EPS_POINT, EPS_LINE, EPS_QUAD};

    ParticleSystem(unsigned int maxParticles);

    ParticleSystem(unsigned int maxParticles, PARTICLE_SHAPE shape);

    ~ParticleSystem();
    
    virtual void render();  	

    virtual void update();

    virtual void print();

    void setParticleEmitter( ParticleEmitter* particleEmitter);
    
    void addParticleAnimator(ParticleAnimator* particleAnimator);
    
    void setTexture( Texture* pTexture )
    {
        m_pTexture = pTexture;
    }
    
    Texture* getTexture() const
    {
        return m_pTexture;
    }
    
    inline void setEnable(bool bEnable)
    {
        m_bEnable = bEnable;
    }

    void        setPosition( const Vector2f& position )
    {
        m_Position = position;
    }
    
    void        setColor( const Color4f& color );

    Color4f    getColor() const
    {
        return m_Color;
    }

    inline ParticleEmitter* getParticleEmitter()
    {
        return m_ParticleEmitter;
    }

    void setParticleShape(PARTICLE_SHAPE shape)
    {
        m_Shape = shape;
    }

    PARTICLE_SHAPE getParticleShape()
    {
        return m_Shape;
    }

    friend class ParticleEmitterPoint;
    friend class ParticleEmitterLine;
    friend class ParticleEmitterCircularDisk;
    friend class ParticleEmitterCylinder;
    friend class ParticleEmitterCircle;
    friend class ParticleEmitterRect;
    friend class ParticleAnimator;
    friend class ParticleGravityAnimator;
    friend class ParticleTornadoAnimator;
    friend class ParticleDiffuseAnimator; 
    friend class ParticleFlockAnimator;
    friend class ParticleOrbitCentreAnimator;
    friend class ParticleTargetFollowAnimator;
private:

    ParticleEmitter* m_ParticleEmitter;
    vector<ParticleAnimator*> m_VecParticleAnimators;
    vector<Particle> m_ParticlePool;
    vector<Particle> m_ParticleLinePool;
    unsigned int m_MaxParticles;
    unsigned int m_NumActiveParticles;
    PARTICLE_SHAPE m_Shape;
    bool m_bEnable;
    Vector2f m_Position;
    Color4f  m_Color;
    
    Texture *m_pTexture;
    
    Quad   *m_pQuads;
    unsigned short *m_pIndices;
private:
    void   updateParticleQuads();
    
};

#endif