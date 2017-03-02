//
//  GUIStageSelectionPanel.cpp
//  Optiks
//
//  Created by Prashant on 08/03/14.
//
//

#include "GUILevelSelectionPanel.h"
#include "Matrix4.h"
#include "Game.h"
#include "Renderer.h"
#include "ParticleSystem.h"
#include "ParticleBase.h"
#include "FontRenderer.h"

USE_NAMESPACE_DIRAC2D

GUILevelSelectionPanel::GUILevelSelectionPanel(si32 width, si32 height):GUIPanel(width, height)
{
    
}

void GUILevelSelectionPanel::update()
{
    GUIPanel::update();
    
    //m_pParticleSystem->update();
}

GUILevelSelectionPanel::GUILevelSelectionPanel(GUIContainer* parent, si32 width, si32 height):GUIPanel(parent, width, height)
{
    for( int i=0; i<NUM_LEVELS_PER_STAGE; i++ )
    {
        m_bRayEnable[i] = false;
    }
    
    initParticleSystem();
}

void GUILevelSelectionPanel::render()
{
    GUIPanel::render();
}


sbool GUILevelSelectionPanel::OnEvent( Event& event)
{
    return GUIPanel::OnEvent(event);
}

void GUILevelSelectionPanel::renderRays( float width )
{
    if( !m_bVisible || !m_bIsClipped )
        return;
    
    Matrix4f xForm, xForm1;
    
   
    float height = 220;

    int rayCount = 0;
    Matrix3f rot;
    float theta = 0.0f;
    float dTheta = 360/NUM_LEVELS_PER_STAGE;
    
    for( int i=0; i<NUM_LEVELS_PER_STAGE; i++ )
    {
        Vector2f endPoint(height, 0.0f);
        rot.rotate(theta);
        rot.transformAsPoint(endPoint);
        Vector2f dir = endPoint;
        dir.normalize();
        Ray ray(Vector2f(0,0), dir);
        if( m_bRayEnable[i] == true )
            ray.setColor(Color4f(1.0f,0.6f,0.0f,1.0f));
        else
            ray.setColor(Color4f(0.5f,0.5f,0.5f,1.0f));

        ray.endPoint = endPoint;
        
        pushRayDataToRayVertexBuffer(width, ray, rayCount);
        rayCount++;
        theta += dTheta;
    }
    
    xForm1.translate(m_GlobalTransform.col3.x + m_Size.x*0.5f - 40, m_GlobalTransform.col3.y + m_Size.y * 0.5f + 340);

    xForm1.m00 = xForm1.m11 = 0.6f;
    FontRenderer::Instance()->SetTransform(xForm1);
    FontRenderer::Instance()->SetColor(Color4f(1.0f,0.7f,0.0f,0.7f));
    FontRenderer::Instance()->DrawText(m_Title);
    
    
    xForm.translate(m_GlobalTransform.col3.x + m_Size.x*0.5f, m_GlobalTransform.col3.y + m_Size.y * 0.5f + 40);
    
    Game::getInstance()->getRenderer()->setTransform(xForm, spider3d::WORLD);
    
    Game::getInstance()->getRenderer()->setTextureObject(Ray::m_pTexture);
    
    Game::getInstance()->getRenderer()->drawVertexPrimitiveList(6*rayCount, m_pAllRayVertices, 2, m_pAllRayColors, 4, m_pAllRayTexCoords, 2, 0, EPT_TRIANGLE);
    
    Game::getInstance()->getRenderer()->setTextureObject(0);
    
    
    
//    m_pParticleSystem->setPosition(Vector2f(m_GlobalTransform.col3.x + m_Size.x*0.5f, m_GlobalTransform.col3.y +m_Size.y*0.5f + 40));
//    m_pParticleSystem->render();
}

void  GUILevelSelectionPanel::setRayAtIndexEnable( int index, bool bEnable)
{
    m_bRayEnable[index] = true;
}

void GUILevelSelectionPanel::pushRayDataToRayVertexBuffer(float rayWidth, Ray& ray, int rayCount)
{
    Vector2f rayVector(ray.origin.x - ray.endPoint.x, ray.origin.y - ray.endPoint.y);
    float rayLength = rayVector.length();
    //rayLength = rayLength > 100.0f ? 100.0f : rayLength;
    
    Matrix3f rot, xForm;
    Vector2f T(ray.origin.x, ray.origin.y);
    xForm.translate(T);
    rot.rotate(ray.m_Angle);
    xForm.multiply(rot);
    
    float dt = 0.0f;
    static float dx = 0.0f;
    dx -= 0.1 * dt;
    
    if( dx > 1.0f )
        dx = 0.0f;
    
    Vector2f v0(0.0f,-rayWidth);
    Vector2f v1(rayLength, -rayWidth);
    Vector2f v2(rayLength, rayWidth);
    Vector2f v3(0.0f, rayWidth);
    
    xForm.transformAsPoint(v0);
    xForm.transformAsPoint(v1);
    xForm.transformAsPoint(v2);
    xForm.transformAsPoint(v3);
    
    m_pAllRayVertices[12*rayCount]   = v0.x;
    m_pAllRayVertices[12*rayCount+1] = v0.y;
    m_pAllRayVertices[12*rayCount+2] = v1.x;
    m_pAllRayVertices[12*rayCount+3] = v1.y;
    m_pAllRayVertices[12*rayCount+4] = v2.x;
    m_pAllRayVertices[12*rayCount+5] = v2.y;
    m_pAllRayVertices[12*rayCount+6] = v0.x;
    m_pAllRayVertices[12*rayCount+7] = v0.y;
    m_pAllRayVertices[12*rayCount+8] = v2.x;
    m_pAllRayVertices[12*rayCount+9] = v2.y;
    m_pAllRayVertices[12*rayCount+10] = v3.x;
    m_pAllRayVertices[12*rayCount+11] = v3.y;
    
    m_pAllRayTexCoords[12*rayCount]   = dx;
    m_pAllRayTexCoords[12*rayCount+1] = 0.0f;
    m_pAllRayTexCoords[12*rayCount+2] = rayLength/1000.0f + dx;
    m_pAllRayTexCoords[12*rayCount+3] = 0.0f;
    m_pAllRayTexCoords[12*rayCount+4] = rayLength/1000.0f + dx;
    m_pAllRayTexCoords[12*rayCount+5] = 1.0f;
    m_pAllRayTexCoords[12*rayCount+6] = dx;
    m_pAllRayTexCoords[12*rayCount+7] = 0.0f;
    m_pAllRayTexCoords[12*rayCount+8] = rayLength/1000.0f + dx;
    m_pAllRayTexCoords[12*rayCount+9] = 1.0f;
    m_pAllRayTexCoords[12*rayCount+10] = dx;
    m_pAllRayTexCoords[12*rayCount+11] = 1.0f;
    
    for( int i=0; i<24; i+=4 )
    {
        m_pAllRayColors[24*rayCount + i]     = ray.m_Color.r;
        m_pAllRayColors[24*rayCount + i + 1] = ray.m_Color.g;
        m_pAllRayColors[24*rayCount + i + 2] = ray.m_Color.b;
        m_pAllRayColors[24*rayCount + i + 3] = ray.m_Color.a;
    }
}

void GUILevelSelectionPanel::initParticleSystem()
{
    return;
    m_pParticleSystem   = new ParticleSystem(300);
    
    ParticleEmitter* pEmitter = new ParticleEmitterPoint();
    pEmitter->setEmissionRate(50);
    pEmitter->setVelocity(Vector2f(100.0f,0.0f));
    pEmitter->setVelocityVariance(Vector2f(10,20));
    pEmitter->setColor(Color4f(1.0f,0.7f,0.0f,1.0f));
    pEmitter->setLifeTime(1.5);
    
    m_pParticleSystem->setParticleEmitter(pEmitter);
    
    m_pParticleAnimator = new ParticleGravityAnimator(m_pParticleSystem,0.0f);
    //pAnimator->setDiffuseFactor(1000);
    m_pParticleSystem->addParticleAnimator(m_pParticleAnimator);
}
