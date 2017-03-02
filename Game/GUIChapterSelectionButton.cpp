//
//  GUIChapterSelectionButton.cpp
//  Optiks
//
//  Created by Prashant on 09/03/14.
//
//

#include "GUIChapterSelectionButton.h"
#include "Shape2D.h"
#include "Scene.h"
#include "Game.h"
#include "Renderer.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "ComponentScaleChangeTask.h"
#include "TaskManager.h"
#include "Renderer.h"
#include "ParticleSystem.h"
#include "ParticleBase.h"
#include "FontRenderer.h"

GUIChapterSelectionButton::GUIChapterSelectionButton(si32 width, si32 height)
:GUIButton(width, height)
{
    
}

GUIChapterSelectionButton::GUIChapterSelectionButton(GUIContainer* parent, si32 width, si32 height)
:GUIButton(parent, width, height)
{
    CircleInfo cInfo;
    cInfo.radius = BASIC_ELEMENT_IMAGE_SIZE*1.2;
    
    m_DisableStateRadius = cInfo.radius;
    m_EnableStateRadius  = cInfo.radius * 1.2f;
    
    m_pShape2D1 = new(Scene::s_pBlockAllocator->Allocate(sizeof(Circle))) Circle(cInfo);
    
    cInfo.radius *= 0.17;
    m_pShape2D2 = new(Scene::s_pBlockAllocator->Allocate(sizeof(Circle))) Circle(cInfo);
    //m_pShape2D->setPosition( Vector2f(m_Position.x, m_Position.y));
    m_pTexture = 0;
    
    m_NumInterReflections = 55;
    
    m_RotAngle = 0.0f;
    
    
    m_pAllRayVertices  = new float[6*2*m_NumInterReflections];
    m_pAllRayColors    = new float[6*4*m_NumInterReflections];
    m_pAllRayTexCoords = new float[6*2*m_NumInterReflections];
    
    m_pParticleSystemOrbit   = new ParticleSystem(1000);
    ParticleEmitter *pEmitterCircle = new ParticleEmitterCircularDisk(width*0.6,width*0.6 + 50);
    pEmitterCircle->setEmissionRate(2000);
    pEmitterCircle->setLifeTime(0.2);
    pEmitterCircle->setSize(2);
    pEmitterCircle->setSizeVariance(1);
    
    m_pParticleSystemOrbit->setParticleEmitter(pEmitterCircle);
    
    m_pParticleAnimatorOrbit = new ParticleOrbitCentreAnimator(m_pParticleSystemOrbit);
    ((ParticleOrbitCentreAnimator*)m_pParticleAnimatorOrbit)->setStrength(100.0);
    m_pParticleSystemOrbit->addParticleAnimator(m_pParticleAnimatorOrbit);
    
    Color4f particleColor = Color4f(1.0f,0.8f,0.0f,1.0f);
    //    particleColor.r += 0.6f;
    //    particleColor.g += 0.6f;
    //    particleColor.b += 0.6f;
    
    m_pParticleSystemOrbit->setColor(particleColor);
    
    setEnable(false);
}

void GUIChapterSelectionButton::render()
{
    if( !m_bIsClipped )
    {
        return;
    }
    
    Matrix4f xForm;
    
    Game::getInstance()->getScene()->m_LensRenderProgram->OnSet();
    
    xForm.translate(m_ClippedBounds.x + m_Size.x*0.5f , m_ClippedBounds.y + m_Size.y*0.5f );
    
    xForm.m00 = m_LocalTransform.col1.x;
    xForm.m11 = m_LocalTransform.col2.y;
    
    Game::getInstance()->getRenderer()->setTransform(xForm, spider3d::WORLD);
    
    Matrix4f orthoMatrix;
    orthoMatrix.ortho(0, Game::getInstance()->getScene()->getSceneWidth(), 0, Game::getInstance()->getScene()->getSceneHeight(), -1, 1);
    
    if( m_ClippedBounds.x > 1024 || m_ClippedBounds.x < 0 || m_ClippedBounds.y > 768 || m_ClippedBounds.y < 0 )
    {
        Game::getInstance()->getScene()->m_pDefaultShaderProgram->OnSet();
        
        Game::getInstance()->getRenderer()->setTransform(orthoMatrix, spider3d::PROJECTION);
        return;
    }
    
    
    Game::getInstance()->getRenderer()->setTransform(orthoMatrix, spider3d::PROJECTION);
    
    Game::getInstance()->getRenderer()->setTextureObject(Game::getInstance()->getScene()->m_FinalCompositeTexture, ETU_0);
    
    Game::getInstance()->getRenderer()->setTextureObject(Game::getInstance()->getScene()->m_RenderTextureGlowPrevious[3], ETU_2);
    
    
    Texture* pTexture;
    if( m_bEnable )
    {
        pTexture = Game::getInstance()->m_pChapterSelectionButtonTexture;
    }
    else
    {
        pTexture = Game::getInstance()->m_pChapterSelectionButtonFixedTexture;
    }
    if( pTexture )
    {
        Game::getInstance()->getRenderer()->setTextureObject(pTexture, ETU_3);
    }
    Texture* pNormalMapTexture = Game::getInstance()->m_pChapterSelectionButtonNormalMapTexture;
    if( pNormalMapTexture )
    {
        Game::getInstance()->getRenderer()->setTextureObject(pNormalMapTexture, ETU_1);
    }
    
    
    float width = m_ClippedBounds.width;
    float height = m_ClippedBounds.height;
    
    float vertices[8];
    vertices[0] = -width*0.5f;
    vertices[1] = -height*0.5f;
    vertices[2] = width*0.5f;
    vertices[3] = -height*0.5f;
    vertices[4] = width*0.5f;
    vertices[5] = height*0.5f;
    vertices[6] = -width*0.5f;
    vertices[7] = height*0.5f;
    
    m_TexCoords[0] = 0.0f;
    m_TexCoords[1] = 0.0f;
    m_TexCoords[2] = 1.0f;
    m_TexCoords[3] = 0.0f;
    m_TexCoords[4] = 1.0f;
    m_TexCoords[5] = 1.0f;
    m_TexCoords[6] = 0.0f;
    m_TexCoords[7] = 1.0f;

    Game::getInstance()->getRenderer()->drawVertexPrimitiveList(4, vertices, 2, m_Colors, 4, m_TexCoords, 2, 0, EPT_TRIANGLE_FAN);
    
    
    Game::getInstance()->getRenderer()->setTextureObject(0, ETU_0);
    Game::getInstance()->getRenderer()->setTextureObject(0, ETU_1);
    Game::getInstance()->getRenderer()->setTextureObject(0, ETU_2);
    Game::getInstance()->getRenderer()->setTextureObject(0, ETU_3);
    
    Game::getInstance()->getScene()->m_pDefaultShaderProgram->OnSet();
    
    Matrix4f m;
    Game::getInstance()->getRenderer()->setTransform(orthoMatrix, spider3d::PROJECTION);
    Game::getInstance()->getRenderer()->setTransform(m, spider3d::WORLD);
    
//    m_pParticleSystemOrbit->setPosition(Vector2f(m_GlobalTransform.col3.x + m_Size.x*0.5f, m_GlobalTransform.col3.y + m_Size.y*0.5f));
//    m_pParticleSystemOrbit->render();
}

void GUIChapterSelectionButton::renderRays(float rayWidth)
{
    if( m_ClippedBounds.x > 1024 || m_ClippedBounds.x < 0 || m_ClippedBounds.y > 768 || m_ClippedBounds.y < 0 )
        return;
    
    int rayCount = 0;
	for( int i=0; i<m_vecInnerGlowRays.size(); i++ )
	{
        pushRayDataToRayVertexBuffer(rayWidth, m_vecInnerGlowRays[i], rayCount);
        rayCount++;
	}
    
    NAMESPACE_DIRAC2D::Matrix4f xForm, xForm1;
    
    xForm.translate(m_GlobalTransform.col3.x + m_Size.x*0.5f, m_GlobalTransform.col3.y + m_Size.y * 0.5f);
    xForm1 = xForm;
    xForm1.m03 += -90.0f;
    xForm1.m13 += 320;
    
    FontRenderer::Instance()->SetTransform(xForm1);
    FontRenderer::Instance()->SetColor(Color4f(1.0f,0.7f,0.0f,0.7f));
    FontRenderer::Instance()->DrawText(m_Title);
    
    Game::getInstance()->getRenderer()->setTextureObject(Ray::m_pTexture);


    Game::getInstance()->getRenderer()->setTransform(xForm, spider3d::WORLD);

    Game::getInstance()->getRenderer()->drawVertexPrimitiveList(6*rayCount, m_pAllRayVertices, 2, m_pAllRayColors, 4, m_pAllRayTexCoords, 2, 0, EPT_TRIANGLE);
    
    
//    m_pParticleSystemOrbit->setPosition(Vector2f(m_GlobalTransform.col3.x + m_Size.x*0.5f, m_GlobalTransform.col3.y + m_Size.y*0.5f));
//    m_pParticleSystemOrbit->render();
}

void GUIChapterSelectionButton::update()
{
    GUIButton::update();
    
    //m_pParticleSystemOrbit->update();
    
    rayTraceForSinkGlow();
}

sbool GUIChapterSelectionButton::OnEvent( Event& event)
{
    bool bRes = GUIButton::OnEvent(event);
    
    if( !bRes )
        return false;

    if( event.eventType == SMOUSE_EVENT )
    {
        if( event.mouseInput.Event == LEFT_MOUSE_PRESSED_DOWN )
        {
            // Create a task to Scale this button up
            ComponentScaleChangeTask* pTask = new ComponentScaleChangeTask(this, 1.0f, 1.05f, 0.3);
            Game::getInstance()->getTaskManager()->Attach(pTask);
        }
        if( event.mouseInput.Event == LEFT_MOUSE_LEFT_UP )
        {
            // Create a task to Scale this button down to original scale
            ComponentScaleChangeTask* pTask = new ComponentScaleChangeTask(this, 1.05f, 1.0f, 0.3);
            Game::getInstance()->getTaskManager()->Attach(pTask);
        }
    }
    return bRes;
}

void GUIChapterSelectionButton::setRayColor( const Color4f& rayColor )
{
    m_SourceRay.setColor(rayColor);
}

void GUIChapterSelectionButton::rayTraceForSinkGlow()
{
    if( m_ClippedBounds.x > 1024 || m_ClippedBounds.x < 0 || m_ClippedBounds.y > 768 || m_ClippedBounds.y < 0 )
        return;
    
    m_SourceRay.direction = Vector2f(0.5f,1.0f);
    m_SourceRay.direction.normalize();
    m_SourceRay.origin = Vector2f(0.0f,m_DisableStateRadius*0.9f);
    Matrix3f xForm, rotXform;
    rotXform.rotate(m_RotAngle);
    xForm = rotXform;

    m_RotAngle += 0.005f;

    xForm.transformAsPoint(m_SourceRay.origin);
    //xForm.transformAsVector(m_SourceRay.direction);

    //    static float radius = m_DisableStateRadius;
//    radius = m_DisableStateRadius*1.4 + 50 * sin(m_RotAngle*100);
//    
//    ((Circle*)m_pShape2D1)->setRadius(radius);
    
    Ray startRay = m_SourceRay;
    
    if( m_bEnable )
    {
        startRay.setColor(Color4f(1.0f,0.5f,0.0f,1.0f));
    }
    else
    {
        startRay.setColor(Color4f(0.5f,0.5f,0.5f,1.0f));
    }
    
    Intersect_Info intersectInfo, intersectInfo1;
    
    m_vecInnerGlowRays.clear();
    
    for( int i=0; i<m_NumInterReflections; i++)
    {
        m_pShape2D1->intersectRay(startRay, intersectInfo);
        m_pShape2D2->intersectRay(startRay, intersectInfo1);
        
        if( intersectInfo.T > intersectInfo1.T && intersectInfo1.T > 0.0)
        {
            intersectInfo = intersectInfo1;
        }
        //intersectInfo = intersectInfo.T < intersectInfo1.T ? intersectInfo : intersectInfo1;
        
        Vector2f ReflectionDir;
        Vector2f V(-intersectInfo.hitPoints[0].x + startRay.origin.x, -intersectInfo.hitPoints[0].y + startRay.origin.y);
        V.normalize();
        float NdotV = intersectInfo.hitNormal[0].dot(V);
        ReflectionDir.x = 2 * NdotV * intersectInfo.hitNormal[0].x - V.x;
        ReflectionDir.y = 2 * NdotV * intersectInfo.hitNormal[0].y - V.y;
        Vector2f rayOrigin(intersectInfo.hitPoints[0].x + ReflectionDir.x * 0.01f, intersectInfo.hitPoints[0].y + ReflectionDir.y * 0.01f);
        Ray reflectedRay(rayOrigin , ReflectionDir);
        reflectedRay.setColor(startRay.getColor());
        
        startRay.endPoint = startRay.origin + startRay.direction * intersectInfo.T;
        m_vecInnerGlowRays.push_back(startRay);
        
        startRay = reflectedRay;
    }
}

void GUIChapterSelectionButton::pushRayDataToRayVertexBuffer(float rayWidth, Ray& ray, int rayCount)
{
    Vector2f rayVector(ray.origin.x - ray.endPoint.x, ray.origin.y - ray.endPoint.y);
    float rayLength = rayVector.length();
    //rayLength = rayLength > 100.0f ? 100.0f : rayLength;
    
    Matrix3f rot, xForm;
    Vector2f T(ray.origin.x, ray.origin.y);
    xForm.translate(T);
    rot.rotate(ray.m_Angle);
    xForm.multiply(rot);
    
    float dt = 0.0001f;
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
    m_pAllRayTexCoords[12*rayCount+2] = rayLength/500.0f + dx;
    m_pAllRayTexCoords[12*rayCount+3] = 0.0f;
    m_pAllRayTexCoords[12*rayCount+4] = rayLength/500.0f + dx;
    m_pAllRayTexCoords[12*rayCount+5] = 1.0f;
    m_pAllRayTexCoords[12*rayCount+6] = dx;
    m_pAllRayTexCoords[12*rayCount+7] = 0.0f;
    m_pAllRayTexCoords[12*rayCount+8] = rayLength/500.0f + dx;
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