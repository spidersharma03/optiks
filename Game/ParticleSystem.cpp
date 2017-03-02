#include "ParticleSystem.h"
#include "ParticleBase.h"
#include "OglRenderer.h"
#include "DTimer.h"
#include "Game.h"
#include "Scene.h"
#include "ShaderProgram.h"
#include <algorithm>

using namespace std;


ParticleSystem::ParticleSystem(unsigned int maxParticles)
    :m_NumActiveParticles(0),m_MaxParticles(maxParticles)
{
    m_ParticlePool.reserve(m_MaxParticles);
    m_bEnable = true;
    m_pTexture = 0;
    
    m_Shape = EPS_POINT;
    
    if( m_Shape == EPS_LINE )
        m_ParticleLinePool.reserve(2*m_MaxParticles);
    if( m_Shape == EPS_QUAD )
    {
        m_pQuads = new Quad[m_MaxParticles];
        
        m_pIndices = (unsigned short*)calloc( sizeof(m_pIndices[0]) * m_MaxParticles * 6, 1 );
        
        for( unsigned int i = 0; i < m_MaxParticles; i++) {
            const unsigned int i6 = i*6;
            const unsigned int i4 = i*4;
            m_pIndices[i6+0] = (unsigned short) i4+0;
            m_pIndices[i6+1] = (unsigned short) i4+1;
            m_pIndices[i6+2] = (unsigned short) i4+2;
            
            m_pIndices[i6+3] = (unsigned short) i4+0;
            m_pIndices[i6+4] = (unsigned short) i4+2;
            m_pIndices[i6+5] = (unsigned short) i4+3;
        }
        float left = 0;
        float bottom = 0;
        float right = 1.0f;
        float top = 1.0f;
        for(unsigned int  i=0; i<m_MaxParticles; i++) {
            
            // bottom-left vertex:
            m_pQuads[i].bl.texCoords.x = left;
            m_pQuads[i].bl.texCoords.y = bottom;
            // bottom-right vertex:
            m_pQuads[i].br.texCoords.x = right;
            m_pQuads[i].br.texCoords.y = bottom;
            // top-left vertex:
            m_pQuads[i].tl.texCoords.x = left;
            m_pQuads[i].tl.texCoords.y = top;
            // top-right vertex:
            m_pQuads[i].tr.texCoords.x = right;
            m_pQuads[i].tr.texCoords.y = top;
        }
    }
}

ParticleSystem::ParticleSystem(unsigned int maxParticles, PARTICLE_SHAPE shape)
    :m_NumActiveParticles(0),m_MaxParticles(maxParticles),m_Shape(shape)
{
    m_ParticlePool.reserve(m_MaxParticles);
    m_bEnable = true;
    m_pTexture = 0;
    
    if( m_Shape == EPS_LINE )
        m_ParticleLinePool.reserve(2*m_MaxParticles);
    if( m_Shape == EPS_QUAD )
    {
        m_pQuads = new Quad[m_MaxParticles];
        
        m_pIndices = (unsigned short*)calloc( sizeof(m_pIndices[0]) * m_MaxParticles * 6, 1 );
        
        for( unsigned int i = 0; i < m_MaxParticles; i++) {
            const unsigned int i6 = i*6;
            const unsigned int i4 = i*4;
            m_pIndices[i6+0] = (unsigned short) i4+0;
            m_pIndices[i6+1] = (unsigned short) i4+1;
            m_pIndices[i6+2] = (unsigned short) i4+2;
            
            m_pIndices[i6+3] = (unsigned short) i4+0;
            m_pIndices[i6+4] = (unsigned short) i4+2;
            m_pIndices[i6+5] = (unsigned short) i4+3;
        }
        float left = 0;
        float bottom = 0;
        float right = 1.0f;
        float top = 1.0f;
        for(unsigned int  i=0; i<m_MaxParticles; i++) {
            
            // bottom-left vertex:
            m_pQuads[i].bl.texCoords.x = left;
            m_pQuads[i].bl.texCoords.y = bottom;
            // bottom-right vertex:
            m_pQuads[i].br.texCoords.x = right;
            m_pQuads[i].br.texCoords.y = bottom;
            // top-left vertex:
            m_pQuads[i].tl.texCoords.x = left;
            m_pQuads[i].tl.texCoords.y = top;
            // top-right vertex:
            m_pQuads[i].tr.texCoords.x = right;
            m_pQuads[i].tr.texCoords.y = top;
        }
    }
}

ParticleSystem::~ParticleSystem()
{
    m_ParticlePool.clear();
    if( m_Shape == EPS_LINE )
        m_ParticleLinePool.clear();
    
    if( m_Shape == EPS_QUAD )
    {
        delete[] m_pQuads;
        delete[] m_pIndices;
    }
}

void ParticleSystem::updateParticleQuads()
{
    for( int i=0; i<m_ParticlePool.size(); i++ )
    {
        Particle& particle = m_ParticlePool[i];
        
        Quad &quad = m_pQuads[i];
        
        quad.bl.colors = particle.m_Color;
        quad.br.colors = particle.m_Color;
        quad.tl.colors = particle.m_Color;
        quad.tr.colors = particle.m_Color;
        
        // vertices
        float size_2 = 0.5f * particle.size;
        
        float x1 = -size_2;
        float y1 = -size_2;
        
        float x2 = size_2;
        float y2 = size_2;
        float x = particle.m_X;
        float y = particle.m_Y;

        Vector2f velocity( particle.m_Vx, particle.m_Vy);
        float velLength = velocity.length();
        velocity /= velLength;
        
        Matrix3f xForm;
        xForm.col1 = Vector3f(velocity.x, velocity.y, 0.0f);
        xForm.col2 = Vector3f(-velocity.y, velocity.x, 0.0f);
        xForm.col3 = Vector3f(x, y, 1.0f);
              
        // bottom-left
        quad.bl.vertices = Vector2f(x1,y1);
        //quad.bl.vertices.y = ay;
        xForm.transformAsPoint(quad.bl.vertices);
        
        // bottom-right vertex:
        quad.br.vertices = Vector2f(x2,y1);
        //quad.br.vertices.y = by;
        xForm.transformAsPoint(quad.br.vertices);

        // top-left vertex:
        quad.tl.vertices = Vector2f(x1, y2);
        //quad.tl.vertices.y = dy;
        xForm.transformAsPoint(quad.tl.vertices);

        // top-right vertex:
        quad.tr.vertices = Vector2f(x2, y2);
        //quad.tr.vertices.y = cy;
        xForm.transformAsPoint(quad.tr.vertices);

    }
}

void  ParticleSystem::setColor( const Color4f& color )
{
    m_Color = color;
    m_ParticleEmitter->setColor(m_Color);
}

void ParticleSystem::render()
{
    if(!m_bEnable)return;
    
    int count = 0;
    count = m_ParticlePool.size()-1;
    
    if(count > 0)
    {        
        if( m_Shape == EPS_POINT )
        {
            Game::getInstance()->getScene()->m_pDefaultColorShaderProgram->OnSet();
            int sceneWidth  = Game::getInstance()->getScene()->getSceneWidth();
            int sceneHeight = Game::getInstance()->getScene()->getSceneHeight();
            
            Matrix4f ortho, identity;
            identity.translate(m_Position);
            ortho.ortho(0, sceneWidth, 0, sceneHeight, -1, 1);
            Game::getInstance()->getRenderer()->setTransform(ortho, PROJECTION);
            Game::getInstance()->getRenderer()->setTransform(identity, WORLD);
            
            Game::getInstance()->getRenderer()->drawVertexPrimitiveList(count, &m_ParticlePool[0].m_X, 2, &m_ParticlePool[0].m_Color.r, 4, 0, 0, sizeof(Particle), EPT_POINT);
            
            Game::getInstance()->getScene()->m_pDefaultShaderProgram->OnSet();
            ortho.ortho(0, sceneWidth, 0, sceneHeight, -1, 1);
            Game::getInstance()->getRenderer()->setTransform(ortho, PROJECTION);
            Game::getInstance()->getRenderer()->setTransform(identity, WORLD);
            
        }
        else if ( m_Shape == EPS_LINE )
        {
            int c = 0;
            for(unsigned p=0; p<m_ParticlePool.size();p++)
            {
                Particle& particle1 = m_ParticlePool[p];
                //particleLinePool.push_back(particle1);
                m_ParticleLinePool[c] = particle1;
                Particle particle2;
                particle2 = particle1;
                
                particle2.m_X = particle1.m_X + particle1.m_Vx * 0.2;
                particle2.m_Y = particle1.m_Y + particle1.m_Vy * 0.2;
                //particleLinePool.push_back(particle2);
                m_ParticleLinePool[c+1] = particle2;
                c+=2;
            }
            Game::getInstance()->getScene()->m_pDefaultColorShaderProgram->OnSet();

            Matrix4f ortho, identity;
            identity.translate(m_Position);
            ortho.ortho(0, 1024, 0, 768, -1, 1);
            Game::getInstance()->getRenderer()->setTransform(ortho, PROJECTION);
            Game::getInstance()->getRenderer()->setTransform(identity, WORLD);
            
            Game::getInstance()->getRenderer()->setTextureObject(0);
            
            Game::getInstance()->getRenderer()->setBlendingEnable(true);
            Game::getInstance()->getRenderer()->setBlendingFactors(EBF_SRC_ALPHA, EBF_ONE_MINUS_SRC_ALPHA);
            
            Game::getInstance()->getRenderer()->drawVertexPrimitiveList(c,&m_ParticleLinePool[0].m_X ,2 , &m_ParticleLinePool[0].m_Color.r, 4, 0, 0, sizeof(Particle), EPT_LINE);
            
            Game::getInstance()->getScene()->m_pDefaultShaderProgram->OnSet();
            ortho.ortho(0, 1024, 0, 768, -1, 1);
            Game::getInstance()->getRenderer()->setTransform(ortho, PROJECTION);
            Game::getInstance()->getRenderer()->setTransform(identity, WORLD);
        }
        else if ( m_Shape == EPS_QUAD  )
        {
            Game::getInstance()->getScene()->m_pDefaultShaderProgram->OnSet();
            Matrix4f ortho, identity;
            identity.translate(m_Position);
            ortho.ortho(0, 1024, 0, 768, -1, 1);
            Game::getInstance()->getRenderer()->setTransform(ortho, PROJECTION);
            Game::getInstance()->getRenderer()->setTransform(identity, WORLD);
            
            Game::getInstance()->getRenderer()->setTextureObject(m_pTexture);
            
            //Game::getInstance()->getRenderer()->setBlendingFactors(EBF_SRC_ALPHA, EBF_ONE_MINUS_SRC_ALPHA);

            Game::getInstance()->getRenderer()->drawIndexVertexPrimitiveList((m_ParticlePool.size()-1)*6, (float*)m_pQuads, &m_pQuads[0].bl.colors,4,(float*)&m_pQuads[0].bl.texCoords, sizeof(Vertex2f) , m_pIndices, EPT_TRIANGLE);
            Game::getInstance()->getRenderer()->setTextureObject(0);
            
            //Game::getInstance()->getRenderer()->setBlendingFactors(EBF_SRC_ALPHA, EBF_ONE);
        }
        //Game::getInstance()->getRenderer()->setBlendingEnable(false);
    }
}

void ParticleSystem::update()
{
    if(!m_bEnable)return;

    double currentTime = DTimer::getTime();
    
    if ( m_ParticleEmitter )
        m_ParticleEmitter->emit(currentTime);

    for ( unsigned pa=0; pa<m_VecParticleAnimators.size(); pa++)
    {
        m_VecParticleAnimators[pa]->animate( currentTime );
    }
    
    if( m_Shape == EPS_QUAD )
        updateParticleQuads();
}

void ParticleSystem::print()
{
}

void ParticleSystem::setParticleEmitter( ParticleEmitter* particleEmitter)
{
    m_ParticleEmitter = particleEmitter;
    m_ParticleEmitter->m_pParticleSystem = this;
    m_ParticleEmitter->m_CurrentTime = m_ParticleEmitter->m_PrevTime = DTimer::getTime();
}

void ParticleSystem::addParticleAnimator(ParticleAnimator* particleAnimator)
{
    m_VecParticleAnimators.push_back(particleAnimator);
    particleAnimator->currTime = particleAnimator->prevTime = DTimer::getTime();
}
