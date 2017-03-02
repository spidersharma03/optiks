//
//  GUIStageSelectionButton.cpp
//  Optiks
//
//  Created by Prashant on 04/03/14.
//
//

#include "GUIStageSelectionButton.h"
#include "Shape2D.h"
#include "Scene.h"
#include "Game.h"
#include "Renderer.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "GUIScrollView.h"

GUIStageSelectionButton::GUIStageSelectionButton(si32 width, si32 height)
:GUIButton(width, height)
{
    
}

GUIStageSelectionButton::GUIStageSelectionButton(GUIContainer* parent, si32 width, si32 height)
:GUIButton(parent, width, height)
{
    CircleInfo cInfo;
    cInfo.radius = BASIC_ELEMENT_IMAGE_SIZE/4;
    m_pShape2D = new(Scene::s_pBlockAllocator->Allocate(sizeof(Circle))) Circle(cInfo);
    //m_pShape2D->setPosition( Vector2f(m_Position.x, m_Position.y));
    m_pTexture = 0;
    
    m_NumInterReflections = 25;
    
    m_RotAngle = 0.0f;
    
    m_DisableStateRadius = cInfo.radius;
    m_EnableStateRadius  = cInfo.radius;
    
    m_pAllRayVertices  = new float[6*2*m_NumInterReflections];
    m_pAllRayColors    = new float[6*4*m_NumInterReflections];
    m_pAllRayTexCoords = new float[6*2*m_NumInterReflections];
    
    m_RenderState = 0;
    setEnable(false);
}

void GUIStageSelectionButton::setButtonState(int state)
{
    m_RenderState = state;
}

void GUIStageSelectionButton::render()
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
    
    //renderRays(25, Color4f(0.5f,0.5f,0.5f,0.5f));
}

void GUIStageSelectionButton::renderRays(float rayWidth)
{
    return;
    
    if( m_ClippedBounds.x > 1024 || m_ClippedBounds.x < 0 || m_ClippedBounds.y > 768 || m_ClippedBounds.y < 0 )
        return;
    
    if( m_RenderState == 2 )
    {
        rayWidth *= 3;
    }
    
    int rayCount = 0;
	for( int i=0; i<m_vecInnerGlowRays.size(); i++ )
	{
        pushRayDataToRayVertexBuffer(rayWidth, m_vecInnerGlowRays[i], rayCount);
        rayCount++;
	}
    NAMESPACE_DIRAC2D::Matrix4f xForm;
    
    xForm.translate(m_ClippedBounds.x + m_Size.x*0.5f, m_ClippedBounds.y + m_Size.y*0.5f);
    
    xForm.m00 = xForm.m11 = m_LocalTransform.col1.x;
    
    Game::getInstance()->getRenderer()->setTransform(xForm, spider3d::WORLD);
    
    Game::getInstance()->getRenderer()->setTextureObject(Ray::m_pTexture);
    
    Game::getInstance()->getRenderer()->drawVertexPrimitiveList(6*rayCount, m_pAllRayVertices, 2, m_pAllRayColors, 4, m_pAllRayTexCoords, 2, 0, EPT_TRIANGLE);
    
    Game::getInstance()->getRenderer()->setTextureObject(0);
}

void GUIStageSelectionButton::update()
{
    GUIButton::update();
    
    GUIScrollView *pParent = (GUIScrollView*)m_Parent;
    
    float parentX = pParent->getTransform().col3.x;
    float parentSizeX = pParent->getSize().x;
    float parentCentreX = parentX + parentSizeX * 0.5f;
    
    float virtualDisplacement = m_GlobalTransform.col3.x + m_Size.x*0.5f - parentCentreX;
    
    float scaleFactor = 1.0f - fabsf(virtualDisplacement*2/pParent->getSize().x);
    
    scaleFactor = scaleFactor > 1.0f ? 1.0f : scaleFactor;
    scaleFactor = scaleFactor < 0.4f ? 0.4f : scaleFactor;
    
    m_LocalTransform.col1.x = m_LocalTransform.col2.y = scaleFactor;
    
    rayTraceForSinkGlow();
}

sbool GUIStageSelectionButton::OnEvent( Event& event )
{
    return GUIButton::OnEvent(event);
}

void  GUIStageSelectionButton::rayTraceForSinkGlow()
{
    m_SourceRay.direction = Vector2f(1.0f,1.0f);
    m_SourceRay.direction.normalize();
    m_SourceRay.origin = Vector2f( Vector2f(0.0f,m_DisableStateRadius*0.9f) );
    Matrix3f xForm, rotXform, T, TInv;
    rotXform.rotate(m_RotAngle);
    Vector2f position;//( m_Position.x, m_Position.y );
    T.translate(position);
    TInv.translate(-position);
    xForm = T;
    xForm *= rotXform;
    xForm *= TInv;
    
    xForm.transformAsPoint(m_SourceRay.origin);
    xForm.transformAsVector(m_SourceRay.direction);
    
    Ray startRay = m_SourceRay;
    startRay.setColor(Color4f(1.0f,1.0f,0.0f,1.0f));
    
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
        reflectedRay.setColor(startRay.getColor());
        
        startRay.endPoint = startRay.origin + startRay.direction * intersectInfo.T;
        m_vecInnerGlowRays.push_back(startRay);
        
        startRay = reflectedRay;
    }
}

void GUIStageSelectionButton::pushRayDataToRayVertexBuffer(float rayWidth, Ray& ray, int rayCount)
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
        m_pAllRayColors[24*rayCount + i + 3] = ray.m_Color.a;
    }
    
}