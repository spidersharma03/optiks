//
//  GUIGameThemeButton.cpp
//  Optiks
//
//  Created by Prashant on 28/03/14.
//
//

#include "GUIGameThemeButton.h"
#include "Scene.h"
#include "Game.h"
#include "Renderer.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "ComponentScaleChangeTask.h"
#include "TaskManager.h"
#include "ParticleSystem.h"
#include "ParticleBase.h"
#include "Shape2D.h"

GUIGameThemeButton::GUIGameThemeButton(si32 width, si32 height)
:GUIButton(width, height)
{
    
}

GUIGameThemeButton::GUIGameThemeButton(GUIContainer* parent, si32 width, si32 height)
:GUIButton(parent, width, height)
{
    m_pTexture = 0;
    
    m_RotAngle = 0.0f;
    
    
    m_pParticleSystem   = new ParticleSystem(500);
    m_pParticleSystem->setEnable(false);

    m_pEmitter = new ParticleEmitterCircularDisk(width*0.7,width*0.7 + 20);
    m_pEmitter->setEmissionRate(500);
    m_pEmitter->setLifeTime(1.3);
    
    m_pParticleSystem->setParticleEmitter(m_pEmitter);
    
    m_pAnimator = new ParticleOrbitCentreAnimator(m_pParticleSystem);
    ((ParticleOrbitCentreAnimator*)m_pAnimator)->setStrength(10.0);
    m_pParticleSystem->addParticleAnimator(m_pAnimator);
    
    Color4f particleColor = Color4f(1.0f,0.8f,0.0f,1.0f);
//    particleColor.r += 0.6f;
//    particleColor.g += 0.6f;
//    particleColor.b += 0.6f;
    
    m_pParticleSystem->setColor(particleColor);
    
    m_NumInterReflections = 30;
    
    m_pAllRayVertices  = new float[2*m_NumInterReflections*2*3];
    m_pAllRayColors    = new float[2*m_NumInterReflections*4*3];
    m_pAllRayTexCoords = new float[2*m_NumInterReflections*2*3];
    
    CircleInfo cInfo;
    m_DisableStateRadius = cInfo.radius = BASIC_ELEMENT_IMAGE_SIZE/2;
    m_pShape2D = new(Scene::s_pBlockAllocator->Allocate(sizeof(Circle))) Circle(cInfo);
    m_pShape2D->setPosition(Vector2f(getPosition().x, getPosition().y));
}

void GUIGameThemeButton::render()
{
    if( !m_bIsClipped )
    {
        return;
    }
    
    
    Matrix4f xForm;
    
    xForm.translate(m_ClippedBounds.x + m_Size.x*0.5f , m_ClippedBounds.y + m_Size.y*0.5f );
    
    xForm.m00 = m_LocalTransform.col1.x;
    xForm.m11 = m_LocalTransform.col2.y;
    
    Game::getInstance()->getRenderer()->setTransform(xForm, spider3d::WORLD);
    
    Matrix4f orthoMatrix;
    orthoMatrix.ortho(0, Game::getInstance()->getScene()->getSceneWidth(), 0, Game::getInstance()->getScene()->getSceneHeight(), -1, 1);
    
    Game::getInstance()->getRenderer()->setBlendingFactors(EBF_SRC_ALPHA, EBF_ONE_MINUS_SRC_ALPHA);
    
    if( m_ClippedBounds.x > 1024 || m_ClippedBounds.x < 0 || m_ClippedBounds.y > 768 || m_ClippedBounds.y < 0 )
    {
        Game::getInstance()->getScene()->m_pDefaultShaderProgram->OnSet();
        
        Game::getInstance()->getRenderer()->setTransform(orthoMatrix, spider3d::PROJECTION);
        return;
    }
    
    Game::getInstance()->getRenderer()->setTransform(orthoMatrix, spider3d::PROJECTION);
    
    float width = m_ClippedBounds.width;
    float height = m_ClippedBounds.height;
    
    float vertices[8];
    vertices[0] = -width*0.8f;
    vertices[1] = -height*0.8f;
    vertices[2] = width*0.8f;
    vertices[3] = -height*0.8f;
    vertices[4] = width*0.8f;
    vertices[5] = height*0.8f;
    vertices[6] = -width*0.8f;
    vertices[7] = height*0.8f;
    
    // Darw Halo
    Game::getInstance()->getRenderer()->setTextureObject(Game::getInstance()->m_pGuiHaloTexture );
    Game::getInstance()->getRenderer()->drawVertexPrimitiveList(4, vertices, 2, m_Colors, 4, m_TexCoords, 2, 0, EPT_TRIANGLE_FAN);
    
    vertices[0] = -width*0.5f;
    vertices[1] = -height*0.5f;
    vertices[2] = width*0.5f;
    vertices[3] = -height*0.5f;
    vertices[4] = width*0.5f;
    vertices[5] = height*0.5f;
    vertices[6] = -width*0.5f;
    vertices[7] = height*0.5f;
    
    //Game::getInstance()->getRenderer()->setTextureObject(Game::getInstance()->m_pReflectiveRectShapeTexture);
    
    Game::getInstance()->getRenderer()->drawVertexPrimitiveList(4, vertices, 2, m_Colors, 4, m_TexCoords, 2, 0, EPT_TRIANGLE_FAN);
    
    Game::getInstance()->getRenderer()->setBlendingFactors(EBF_SRC_ALPHA, EBF_ONE);
    
    renderRays(40);
    
    Game::getInstance()->getRenderer()->setTextureObject(Game::getInstance()->m_pChapterSelectionButtonTexture);
    
    Game::getInstance()->getRenderer()->drawVertexPrimitiveList(4, vertices, 2, m_Colors, 4, m_TexCoords, 2, 0, EPT_TRIANGLE_FAN);
    
    Color4f color = m_pParticleSystem->getColor();
    color.a = 1.0f - m_Transparency;
    m_pParticleSystem->setColor(color);
    
    if( m_Transparency < 0.99f )
    {
        m_pParticleSystem->setEnable(true);
        m_pParticleSystem->render();
    }
    Matrix4f m;
    Game::getInstance()->getRenderer()->setTransform(orthoMatrix, spider3d::PROJECTION);
    Game::getInstance()->getRenderer()->setTransform(m, spider3d::WORLD);
    
}

void GUIGameThemeButton::renderRays(float rayWidth)
{
    int rayCount = 0;
    rayWidth *= 0.8f;
	for( int i=0; i<m_vecInnerGlowRays.size(); i++ )
	{
        pushRayDataToRayVertexBuffer(rayWidth, m_vecInnerGlowRays[i], rayCount);
        rayCount++;
	}
    
    
    NAMESPACE_DIRAC2D::Matrix4f xForm;
    
    xForm.translate(m_GlobalTransform.col3.x + m_Size.x*0.5f, m_GlobalTransform.col3.y + m_Size.y*0.5f);
    
    Game::getInstance()->getRenderer()->setTransform(xForm, spider3d::WORLD);
    
    Game::getInstance()->getRenderer()->setTextureObject(Ray::m_pTexture);
    
    Game::getInstance()->getRenderer()->drawVertexPrimitiveList(6*rayCount, m_pAllRayVertices, 2, m_pAllRayColors, 4, m_pAllRayTexCoords, 2, 0, EPT_TRIANGLE);
}

void GUIGameThemeButton::update()
{
    GUIButton::update();
    
    rayTraceForSinkGlow();
    
    m_pParticleSystem->update();
    
    m_pParticleSystem->setPosition( Vector2f(m_GlobalTransform.col3.x + m_Size.x*0.5f, m_GlobalTransform.col3.y + m_Size.y*0.5f));
}

sbool GUIGameThemeButton::OnEvent( Event& event)
{
    bool bRes = GUIButton::OnEvent(event);
    if( event.eventType == SMOUSE_EVENT )
    {
        if( event.mouseInput.Event == LEFT_MOUSE_PRESSED_DOWN )
        {
            // Create a task to Scale this button up
            ComponentScaleChangeTask* pTask = new ComponentScaleChangeTask(this, 1.0f, 1.2f, 0.3);
            Game::getInstance()->getTaskManager()->Attach(pTask);
        }
        if( event.mouseInput.Event == LEFT_MOUSE_LEFT_UP )
        {
            // Create a task to Scale this button down to original scale
            ComponentScaleChangeTask* pTask = new ComponentScaleChangeTask(this, 1.2f, 1.0f, 0.3);
            Game::getInstance()->getTaskManager()->Attach(pTask);
        }
    }
    return bRes;
}

void GUIGameThemeButton::rayTraceForSinkGlow()
{
    if( !m_bIsClipped )
    {
        return;
    }
    
    m_SourceRay.direction = Vector2f(0.8f,1.0f);
    m_SourceRay.direction.normalize();
    //Vector2f position = Vector2f( m_LocalTransform.col3.x, m_LocalTransform.col3.y);
    Vector2f position = Vector2f();//(m_GlobalTransform.col3.x + m_Size.x*0.5f, m_GlobalTransform.col3.y + m_Size.y*0.5f);
    m_SourceRay.origin = Vector2f( position - Vector2f(0.0f,m_DisableStateRadius*0.9f));
    Matrix3f rotXform;
    rotXform.rotate(m_RotAngle);
    
    rotXform.transformAsPoint(m_SourceRay.origin);
    rotXform.transformAsVector(m_SourceRay.direction);
    m_pShape2D->setPosition(Vector2f());

    Ray startRay = m_SourceRay;
    
    m_RotAngle += 1.0f;
    
    Intersect_Info intersectInfo;
    
    m_vecInnerGlowRays.clear();
    
    for( int i=0; i<m_NumInterReflections; i++)
    {
        m_pShape2D->intersectRay(startRay, intersectInfo);
        
        Vector2f ReflectionDir;
        Vector2f V(-intersectInfo.hitPoints[0].x + startRay.origin.x, -intersectInfo.hitPoints[0].y + startRay.origin.y);
        V.normalize();
        float NdotV = intersectInfo.hitNormal[0].dot(V);
        ReflectionDir.x = 2 * NdotV * intersectInfo.hitNormal[0].x - V.x;
        ReflectionDir.y = 2 * NdotV * intersectInfo.hitNormal[0].y - V.y;
        Vector2f rayOrigin(intersectInfo.hitPoints[0].x + ReflectionDir.x * 0.01f, intersectInfo.hitPoints[0].y + ReflectionDir.y * 0.01f);
        Ray reflectedRay(rayOrigin , ReflectionDir);
        
        startRay.endPoint = startRay.origin + startRay.direction * intersectInfo.T;
        m_vecInnerGlowRays.push_back(startRay);
        
        startRay = reflectedRay;
    }
}

void GUIGameThemeButton::pushRayDataToRayVertexBuffer(float rayWidth, Ray& ray, int rayCount)
{
    Vector2f rayVector(ray.origin.x - ray.endPoint.x, ray.origin.y - ray.endPoint.y);
    float rayLength = rayVector.length();
    //rayLength = rayLength > 100.0f ? 100.0f : rayLength;
    
    Matrix3f rot, xForm;
    Vector2f T(ray.origin.x, ray.origin.y);
    xForm.translate(T);
    rot.rotate(ray.m_Angle);
    xForm.multiply(rot);
    
    float dt = 0.00001f;
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
        m_pAllRayColors[24*rayCount + i + 3] = (1.0f-m_Transparency);
    }
}