//
//  GUILevelSelectionButton.cpp
//  Optiks
//
//  Created by Prashant on 03/03/14.
//
//

#include "GUILevelSelectionButton.h"
#include "Shape2D.h"
#include "Scene.h"
#include "Game.h"
#include "Renderer.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "ComponentScaleChangeTask.h"
#include "TaskManager.h"
#include "FontRenderer.h"

GUILevelSelectionButton::GUILevelSelectionButton(si32 width, si32 height)
:GUIButton(width, height)
{
    
}

GUILevelSelectionButton::GUILevelSelectionButton(GUIContainer* parent, si32 width, si32 height)
:GUIButton(parent, width, height)
{
    CircleInfo cInfo;
    cInfo.radius = BASIC_ELEMENT_IMAGE_SIZE/7;
    
    m_DisableStateRadius = cInfo.radius;
    m_EnableStateRadius  = cInfo.radius * 1.5f;
    
    m_pShape2D = new(Scene::s_pBlockAllocator->Allocate(sizeof(Circle))) Circle(cInfo);
    //m_pShape2D->setPosition( Vector2f(m_Position.x, m_Position.y));
    m_pTexture = 0;
    
    m_NumInterReflections = 25;
    
    m_RotAngle = 0.0f;
    
    
    m_pAllRayVertices  = new float[6*2*m_NumInterReflections];
    m_pAllRayColors    = new float[6*4*m_NumInterReflections];
    m_pAllRayTexCoords = new float[6*2*m_NumInterReflections];
    
    setEnable(false);
    
    m_RenderState = 0;
}

void GUILevelSelectionButton::setButtonState(int state)
{
    m_RenderState = state;
}

void GUILevelSelectionButton::render()
{
    if( !m_bIsClipped )
    {
        return;
    }
    
    Matrix4f xForm;
    
    //return;
    
    Game::getInstance()->getScene()->m_pDefaultShaderProgram->OnSet();
    
    xForm.translate(m_ClippedBounds.x + m_Size.x*0.5f , m_ClippedBounds.y + m_Size.y*0.5f );
    
    //float tx = m_Size.x * 0.5f;
    //float ty = m_Size.y * 0.5f;
    
    xForm.m00 = m_LocalTransform.col1.x;
    xForm.m11 = m_LocalTransform.col2.y;
    //xForm.m03 = tx*(1.0f - xForm.m00);
    //xForm.m13 = ty*(1.0f - xForm.m11);
    
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
    
    Game::getInstance()->getRenderer()->setTextureObject(Game::getInstance()->m_pReflectiveRectShapeTexture, ETU_0);
    Game::getInstance()->getRenderer()->setBlendingFactors(EBF_SRC_ALPHA, EBF_ONE_MINUS_SRC_ALPHA);
    
    
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
    
    Game::getInstance()->getRenderer()->setBlendingFactors(EBF_SRC_ALPHA, EBF_ONE);
    
    Game::getInstance()->getRenderer()->drawVertexPrimitiveList(4, vertices, 2, m_Colors, 4, m_TexCoords, 2, 0, EPT_TRIANGLE_FAN);
    
    Game::getInstance()->getRenderer()->setTextureObject(Game::getInstance()->m_pTransmissiveRectShapeTexture);
    
    Game::getInstance()->getRenderer()->drawVertexPrimitiveList(4, vertices, 2, m_Colors, 4, m_TexCoords, 2, 0, EPT_TRIANGLE_FAN);
    
    
    Game::getInstance()->getRenderer()->setTextureObject(Game::getInstance()->m_pSettingsButtonTexture);
    Game::getInstance()->getRenderer()->drawVertexPrimitiveList(4, vertices, 2, m_Colors, 4, m_TexCoords, 2, 0, EPT_TRIANGLE_FAN);
    
    //renderRays(30);
    
    //    Game::getInstance()->getRenderer()->setTextureObject(0, ETU_0);
    //    Game::getInstance()->getRenderer()->setTextureObject(0, ETU_1);
    //    Game::getInstance()->getRenderer()->setTextureObject(0, ETU_2);
    //    Game::getInstance()->getRenderer()->setTextureObject(0, ETU_3);
    
    Game::getInstance()->getScene()->m_pDefaultShaderProgram->OnSet();
    
    Matrix4f m;
    Game::getInstance()->getRenderer()->setTransform(orthoMatrix, spider3d::PROJECTION);
    Game::getInstance()->getRenderer()->setTransform(m, spider3d::WORLD);
    
    xForm.m00 = xForm.m11 = 0.4f;
    xForm.translate(m_GlobalTransform.col3.x + m_Size.x*0.35f, m_GlobalTransform.col3.y + m_Size.y*0.7f);
    if( m_Title.size() > 1 )
        xForm.translate(m_GlobalTransform.col3.x + m_Size.x*0.25f, m_GlobalTransform.col3.y + m_Size.y*0.7f);
    FontRenderer::Instance()->SetTransform(xForm);
    FontRenderer::Instance()->SetColor(Color4f(1.0f,0.7f,0.0f,0.7f));
    FontRenderer::Instance()->DrawText(m_Title);
    //renderRays(25, Color4f(0.5f,0.5f,0.5f,0.5f));
}

void GUILevelSelectionButton::renderRays(float rayWidth, const Color4f& addColor)
{
    //return;
    if( m_ClippedBounds.x > 1024 || m_ClippedBounds.x < 0 || m_ClippedBounds.y > 768 || m_ClippedBounds.y < 0 )
        return;
    
    if( m_RenderState == 2 )
        rayWidth *= 3;
    
    int rayCount = 0;
	for( int i=0; i<m_vecInnerGlowRays.size(); i++ )
	{
        pushRayDataToRayVertexBuffer(rayWidth, m_vecInnerGlowRays[i], rayCount, addColor);
        rayCount++;
	}
    
    
    NAMESPACE_DIRAC2D::Matrix4f xForm;
    xForm.m00 = m_LocalTransform.col1.x;
    xForm.m11 = m_LocalTransform.col2.y;
    
    xForm.translate(m_ClippedBounds.x + m_Size.x*0.5f, m_ClippedBounds.y + m_Size.y*0.5f);
    
    Game::getInstance()->getRenderer()->setTransform(xForm, spider3d::WORLD);

    Game::getInstance()->getRenderer()->setTextureObject(Ray::m_pTexture);
    
    Game::getInstance()->getRenderer()->drawVertexPrimitiveList(6*rayCount, m_pAllRayVertices, 2, m_pAllRayColors, 4, m_pAllRayTexCoords, 2, 0, EPT_TRIANGLE);
    
    
}

void GUILevelSelectionButton::update()
{
    GUIButton::update();
    
    rayTraceForSinkGlow();
}

sbool GUILevelSelectionButton::OnEvent( Event& event)
{
    bool bRes = GUIButton::OnEvent(event);
    
    if( !bRes )
        return false;
    
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

void GUILevelSelectionButton::rayTraceForSinkGlow()
{
    if( m_ClippedBounds.x > 1024 || m_ClippedBounds.x < 0 || m_ClippedBounds.y > 768 || m_ClippedBounds.y < 0 )
        return;

    m_SourceRay.direction = Vector2f(1.0f,1.0f);
    m_SourceRay.direction.normalize();
    
    if(m_bEnable)
        m_SourceRay.origin = Vector2f( Vector2f(0.0f,m_EnableStateRadius*0.95f) );
    else
        m_SourceRay.origin = Vector2f( Vector2f(0.0f,m_DisableStateRadius*0.95f) );

    Matrix3f xForm, rotXform;
    rotXform.rotate(m_RotAngle);
    xForm = rotXform;
    
    xForm.transformAsPoint(m_SourceRay.origin);
    xForm.transformAsVector(m_SourceRay.direction);
    
    Ray startRay = m_SourceRay;
    
    m_RotAngle += 1.0f;
    
    Intersect_Info intersectInfo;
    
    m_vecInnerGlowRays.clear();
    
    if( m_bEnable )
    {
        startRay.setColor(Color4f(1.0f,0.5f,0.0f,1.0f));
    }
    else
    {
        startRay.setColor(Color4f(0.5f,0.5f,0.5f,1.0f));
    }
    
    if( m_bEnable )
    {
        ((Circle*)m_pShape2D)->setRadius(m_EnableStateRadius);
    }
    else
    {
        ((Circle*)m_pShape2D)->setRadius(m_DisableStateRadius);
    }
    
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
        reflectedRay.setColor(startRay.getColor());
        
        startRay = reflectedRay;
    }
}

void GUILevelSelectionButton::pushRayDataToRayVertexBuffer(float rayWidth, Ray& ray, int rayCount, const Color4f& addColor)
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
        Color4f finalColor = ray.m_Color + addColor;
        finalColor.r = finalColor.r > 1.0f ? 1.0f : finalColor.r;
        finalColor.g = finalColor.g > 1.0f ? 1.0f : finalColor.g;
        finalColor.b = finalColor.b > 1.0f ? 1.0f : finalColor.b;
        
        m_pAllRayColors[24*rayCount + i]     = finalColor.r;
        m_pAllRayColors[24*rayCount + i + 1] = finalColor.g;
        m_pAllRayColors[24*rayCount + i + 2] = finalColor.b;
        m_pAllRayColors[24*rayCount + i + 3] = ray.m_Color.a;
    }
}