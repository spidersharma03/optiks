//
//  GUIReplayButton.cpp
//  Optiks
//
//  Created by Prashant on 11/04/14.
//
//

#include "GUIReplayButton.h"
#include "Scene.h"
#include "Game.h"
#include "Renderer.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "ComponentScaleChangeTask.h"
#include "TaskManager.h"

GUIReplayButton::GUIReplayButton(si32 width, si32 height)
:GUIButton(width, height)
{
    
}

GUIReplayButton::GUIReplayButton(GUIContainer* parent, si32 width, si32 height)
:GUIButton(parent, width, height)
{
    m_pTexture = 0;
    
    m_RotAngle = 0.0f;
    
    m_pAllRayVertices  = new float[6*2*3];
    m_pAllRayColors    = new float[6*4*3];
    m_pAllRayTexCoords = new float[6*2*3];
}

void GUIReplayButton::render()
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
    
    
    Game::getInstance()->getRenderer()->setTextureObject(Game::getInstance()->m_pReplayButtonTexture);
    Game::getInstance()->getRenderer()->drawVertexPrimitiveList(4, vertices, 2, m_Colors, 4, m_TexCoords, 2, 0, EPT_TRIANGLE_FAN);
    
    renderRays(30);
    
    //    Game::getInstance()->getRenderer()->setTextureObject(0, ETU_0);
    //    Game::getInstance()->getRenderer()->setTextureObject(0, ETU_1);
    //    Game::getInstance()->getRenderer()->setTextureObject(0, ETU_2);
    //    Game::getInstance()->getRenderer()->setTextureObject(0, ETU_3);
    
    Game::getInstance()->getScene()->m_pDefaultShaderProgram->OnSet();
    
    Matrix4f m;
    Game::getInstance()->getRenderer()->setTransform(orthoMatrix, spider3d::PROJECTION);
    Game::getInstance()->getRenderer()->setTransform(m, spider3d::WORLD);
}

void GUIReplayButton::renderRays(float rayWidth)
{
    int rayCount = 0;
	for( int i=0; i<m_vecInnerGlowRays.size(); i++ )
	{
        pushRayDataToRayVertexBuffer(rayWidth, m_vecInnerGlowRays[i], rayCount);
        rayCount++;
	}
    
    
    NAMESPACE_DIRAC2D::Matrix4f xForm;
    
    xForm.translate(m_GlobalTransform.col3.x + m_Size.x*0.5f, m_GlobalTransform.col3.y + m_Size.y*0.5f);
    
    xForm.m00 = m_LocalTransform.col1.x;
    xForm.m11 = m_LocalTransform.col2.y;
    
    Game::getInstance()->getRenderer()->setTransform(xForm, spider3d::WORLD);
    
    Game::getInstance()->getRenderer()->setTextureObject(Ray::m_pTexture);
    
    Game::getInstance()->getRenderer()->drawVertexPrimitiveList(6*rayCount, m_pAllRayVertices, 2, m_pAllRayColors, 4, m_pAllRayTexCoords, 2, 0, EPT_TRIANGLE);
}

void GUIReplayButton::update()
{
    GUIButton::update();
    
    rayTraceForSinkGlow();
}

sbool GUIReplayButton::OnEvent( Event& event)
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

void GUIReplayButton::rayTraceForSinkGlow()
{
    if( !m_bIsClipped )
    {
        return;
    }
    
    float h = m_Size.y*0.4;
    
    Vector2f p0( 0.0f, -h*0.5f);
    Vector2f p1( h*0.45f, 0.0f);
    Vector2f p2( -h*0.45f, 0.0f);
    
    p0.x += m_Size.x*0.25f;
    p0.y += m_Size.y*0.15f;

    p1.x += m_Size.x*0.25f;
    p1.y += m_Size.y*0.15f;

    p2.x += m_Size.x*0.25f;
    p2.y += m_Size.y*0.15f;

    Vector2f d0 = p1-p0;
    d0.normalize();
    Vector2f d1 = p2-p1;
    d1.normalize();
    Vector2f d2 = p0-p2;
    d2.normalize();
    
    Ray ray1( p0, d0);
    ray1.endPoint = p1;
    
    Ray ray2( p1, d1);
    ray2.endPoint = p2;
    
    Ray ray3( p2, d2);
    ray3.endPoint = p0;
    
    m_vecInnerGlowRays.clear();
    
    m_vecInnerGlowRays.push_back(ray1);
    m_vecInnerGlowRays.push_back(ray2);
    m_vecInnerGlowRays.push_back(ray3);
}

void GUIReplayButton::pushRayDataToRayVertexBuffer(float rayWidth, Ray& ray, int rayCount)
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
        m_pAllRayColors[24*rayCount + i + 3] = (1.0f - m_Transparency);
    }
}