//
//  SinkGameObject.cpp
//  Optiks
//
//  Created by Prashant Sharma on 08/11/13.
//
//

#include "SinkGameObject.h"
#include "Shape2D.h"
#include "Scene.h"
#include "Game.h"
#include "Grid.h"
#include "SinkObjectGlowInterpolatorTask.h"
#include "TaskManager.h"
#include "ParticleSystem.h"
#include "ParticleBase.h"
#include "ShaderProgram.h"
#include "Renderer.h"

SinkGameObject::SinkGameObject(const SinkGameObjectInfo& sinkGameObjectInfo): GameObject(sinkGameObjectInfo)
{    
    m_ClosestGridCentre = sinkGameObjectInfo.closestGridCentre;
    m_Color             = sinkGameObjectInfo.color;
    m_Color.r = m_Color.r > 1.0f ? 1.0f : m_Color.r;
    m_Color.g = m_Color.g > 1.0f ? 1.0f : m_Color.g;
    m_Color.b = m_Color.b > 1.0f ? 1.0f : m_Color.b;
    m_Color.a = m_Color.a > 1.0f ? 1.0f : m_Color.a;
    
    m_pParticleSystem   = new ParticleSystem(400);
    
    m_pEmitter = new ParticleEmitterCircle(12);
    m_pEmitter->setEmissionRate(10);
    m_pEmitter->setLifeTime(3);
    //pPointEmitter->setVelocity(Vector2f(100.0f,100.0f));
    
    m_pParticleSystem->setParticleEmitter(m_pEmitter);
    
    m_pAnimator = new ParticleOrbitCentreAnimator(m_pParticleSystem);
    //pAnimator->setDiffuseFactor(1000);
    m_pParticleSystem->addParticleAnimator(m_pAnimator);
    
    m_pParticleSystem->setPosition(m_Position);
    Color4f particleColor = sinkGameObjectInfo.color;
    particleColor.r += 0.6f;
    particleColor.g += 0.6f;
    particleColor.b += 0.6f;

    m_pParticleSystem->setColor(particleColor);
    
    
    m_ObjectInfo.m_ObjectTextureID = EOT_SINK;

    CircleInfo cInfo;
    cInfo.radius = BASIC_ELEMENT_IMAGE_SIZE/4;
    m_pShape2D = new(Scene::s_pBlockAllocator->Allocate(sizeof(Circle))) Circle(cInfo);
    m_pShape2D->setPosition(m_Position);
    m_pTexture = 0;
    
    m_NumInterReflections = 25;
    
    m_zOrder = 10;
    m_RotAngle = 0.0f;
    interPolateRadiusTime = 0.0f;
    m_CurrentRadius = m_Radius = cInfo.radius;
    
    m_DisableStateRayColor = m_Color;//.set(1.0f,0.5f,0.2f,1.0f);
    m_DisableStateRayColor.set(m_Color.r + 0.05f, m_Color.g + 0.05f, m_Color.b + 0.05f,1.0f);

    m_EnableStateRayColor.set(m_Color.r + 0.2f, m_Color.g + 0.2f, m_Color.b + 0.2f,1.0f);
    m_EnableStateRayColor.r = m_EnableStateRayColor.r > 1.0f ? 1.0f : m_EnableStateRayColor.r;
    m_EnableStateRayColor.g = m_EnableStateRayColor.g > 1.0f ? 1.0f : m_EnableStateRayColor.g;
    m_EnableStateRayColor.b = m_EnableStateRayColor.g > 1.0f ? 1.0f : m_EnableStateRayColor.b;
    
    
    m_CurrentRayColor = m_DisableStateRayColor;
    
    m_DisableStateRayWidthMultFactor = 0.9f;
    m_EnableStateRayWidthMultFactor  = 1.0f;
    m_CurrentRayWidthMultFactor = m_DisableStateRayWidthMultFactor;
    m_bFireSinkGlowEnableTask = false;
    m_bFireSinkGlowDisableTask = false;
    
    m_EnableStateRadius = m_Radius * 1.2f;
    m_DisableStateRadius = m_Radius * 0.5f;
    
    m_PickingRadius = 10.0f;
}

SinkGameObject::~SinkGameObject()
{
    delete m_pParticleSystem;
    delete m_pAnimator;
    delete m_pEmitter;
}

void SinkGameObject::setTransform(const Matrix3f& transform)
{
    m_Transform = transform;
    m_pShape2D->setTransform(transform);
    m_pParticleSystem->setPosition(Vector2f(transform.col3.x, transform.col3.y));
}

void SinkGameObject::setPosition( const Vector2f& position )
{
    m_Position = position;
    m_pParticleSystem->setPosition(m_Position);
    m_pShape2D->setPosition(position);
}

void SinkGameObject::setRotation( float rotation )
{
    m_Rotation = rotation;
    m_pShape2D->rotate(rotation);
}

void SinkGameObject::render()
{
    Matrix4f xForm;
    xForm.translate(Vector2f(m_Position.x-25,m_Position.y-25));
    Game::getInstance()->getRenderer()->setTransform(xForm , WORLD);
    Game::getInstance()->getRenderer()->setTextureObject(Game::getInstance()->m_pSinkHaloTexture );
    Game::getInstance()->getRenderer()->DrawQuad(50, 50, m_Color);
    
    m_pParticleSystem->render();
}

void SinkGameObject::update(float dt)
{
    //if( m_bEnable )
    {
        m_vecSinkIntersectionRays.clear();
        
        rayTraceForSinkGlow();
        
        m_pParticleSystem->update();
    }
}

bool SinkGameObject::pick(const Vector2f& point) const
{
    bool bRes = m_Position.distanceSquared(point) < m_PickingRadius*m_PickingRadius;
    return bRes;
}

bool SinkGameObject::intersectRay( Ray& ray, IntersectionInformation& intersectInfo )
{
    return false;
}

void SinkGameObject::save(FILE* pFile)
{
    Circle* pCircle = (Circle*)m_pShape2D;
    pCircle->setRadius(m_Radius);
    fprintf(pFile, "%s\n", "SINK_COLOR::");
    fprintf(pFile, "%d %d %d\n",(int)(m_Color.r*255), (int)(m_Color.g*255), (int)(m_Color.b*255) );
    fprintf(pFile, "%s\n", "SINK_POSITION::");
    Vector2i gridPosition = getGridCoordinate();
    fprintf(pFile, "%d  %d\n", gridPosition.x, gridPosition.y);
    
    fprintf(pFile, "%s\n", "CLOSEST_GRID_CENTRE");
    fprintf(pFile, "%f  %f\n", m_ClosestGridCentre.x, m_ClosestGridCentre.y);
}

void SinkGameObject::load(FILE* pFile)
{
    
}

void SinkGameObject::setEnable(bool bEnable)
{
    m_bEnable = bEnable;
}

void SinkGameObject::pushRayForSinkEnableCheck(Ray& ray)
{
    bool bColorMatch = false;
    // if there is a ray with the same color, ignore this ray.
    for( int i=0; i<m_vecSinkIntersectionRays.size(); i++ )
    {
        Color4f rayColor = m_vecSinkIntersectionRays[i].getColor();
        if( fabs(rayColor.r - ray.m_Color.r) < 0.1f &&
           fabs(rayColor.g - ray.m_Color.g) < 0.1f &&
           fabs(rayColor.b - ray.m_Color.b) < 0.1f )
        {
            bColorMatch = true;
            break;
        }
    }
    
    if( !bColorMatch )
        m_vecSinkIntersectionRays.push_back(ray);
}

void SinkGameObject::checkForRayColorMatch()
{
    Color4f finalColor(0.0f,0.0f,0.0f,0.0f);
    for( int i=0;  i < m_vecSinkIntersectionRays.size(); i++ )
    {
        finalColor += m_vecSinkIntersectionRays[i].getColor();
    }
    finalColor.r = finalColor.r > 1.0f ? 1.0f : finalColor.r;
    finalColor.g = finalColor.g > 1.0f ? 1.0f : finalColor.g;
    finalColor.b = finalColor.b > 1.0f ? 1.0f : finalColor.b;
    finalColor.a = finalColor.a > 1.0f ? 1.0f : finalColor.a;
    
    if( fabs(finalColor.r - m_Color.r) < 0.1f &&
       fabs(finalColor.g - m_Color.g) < 0.1f &&
       fabs(finalColor.b - m_Color.b) < 0.1f )
        m_bEnable = true;
}

bool SinkGameObject::isEnable() const
{
    return m_bEnable;
}

void SinkGameObject::renderGlowRays(float width)
{
    if( m_bEnable)
    {
        m_pAnimator->setStrength(10.0f);
        m_pAnimator->reverseAcceleration(true);
        
#ifdef IOS
        m_pParticleSystem->getParticleEmitter()->setEmissionRate(200);
        m_pParticleSystem->getParticleEmitter()->setLifeTime(0.3);
#else
        m_pParticleSystem->getParticleEmitter()->setEmissionRate(200);
        m_pParticleSystem->getParticleEmitter()->setLifeTime(1.0);
#endif
        //interpolateRadius();

        // if enabled create a Task which interpolates ray width and color.
        if( !m_bFireSinkGlowEnableTask)
        {
            SinkObjectGlowInterpolatorTask *pTask = new SinkObjectGlowInterpolatorTask(this, m_DisableStateRayColor,m_EnableStateRayColor, m_DisableStateRayWidthMultFactor, m_EnableStateRayWidthMultFactor, m_DisableStateRadius, m_EnableStateRadius, 0.5f);
            Game::getInstance()->getTaskManager()->Attach(pTask);
            m_bFireSinkGlowEnableTask = true;
            m_bFireSinkGlowDisableTask = false;
        }
    }
    else
    {
        m_pAnimator->reverseAcceleration(false);
        m_pAnimator->setStrength(1.0f);
        m_pParticleSystem->getParticleEmitter()->setEmissionRate(10);
        m_pParticleSystem->getParticleEmitter()->setLifeTime(3);
        // if enabled create a Task which interpolates ray width and color.
        if( !m_bFireSinkGlowDisableTask )
        {
            SinkObjectGlowInterpolatorTask *pTask = new SinkObjectGlowInterpolatorTask(this, m_EnableStateRayColor,m_DisableStateRayColor, m_EnableStateRayWidthMultFactor, m_DisableStateRayWidthMultFactor, m_EnableStateRadius, m_DisableStateRadius, 0.5f);
            Game::getInstance()->getTaskManager()->Attach(pTask);
            m_bFireSinkGlowDisableTask = true;
            m_bFireSinkGlowEnableTask = false;
        }
    }

    width *= m_CurrentRayWidthMultFactor;
    
    for( int i=0; i<m_vecInnerGlowRays.size(); i++)
    {
        Circle* pCircle = (Circle*)m_pShape2D;
        pCircle->setRadius(m_CurrentRadius);
        m_vecInnerGlowRays[i].setColor(m_CurrentRayColor);
        //m_vecInnerGlowRays[i].render(width, m_CurrentRayColor);
    }
}

void SinkGameObject::rayTraceForSinkGlow()
{
    m_SourceRay.direction = Vector2f(0.9f,1.0f);
    m_SourceRay.direction.normalize();
   
    m_SourceRay.origin = Vector2f(getPosition() - Vector2f(0.0f,m_DisableStateRadius*0.9f));

    Matrix3f xForm, rotXform, T, TInv;
    rotXform.rotate(m_RotAngle);
    T.translate(getPosition());
    TInv.translate(-getPosition());
    xForm = T;
    xForm *= rotXform;
    xForm *= TInv;

    xForm.transformAsPoint(m_SourceRay.origin);
    xForm.transformAsVector(m_SourceRay.direction);
    
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

void SinkGameObject::interpolateRadius()
{
    m_CurrentRadius = m_Radius + m_Radius*0.51*sin(interPolateRadiusTime);
    interPolateRadiusTime += 0.01f;
}

void SinkGameObject::setClosestGridCentre(const Vector2f& centre)
{
    m_ClosestGridCentre = centre;
}

Vector2f SinkGameObject::getClosestGridCentre() const
{
    return m_ClosestGridCentre;
}

void SinkGameObject::setColor( const Color4f& color )
{
    m_Color = color;
    m_DisableStateRayColor = m_Color;
    m_EnableStateRayColor.set(m_Color.r + 0.2f, m_Color.g + 0.2f, m_Color.b + 0.2f,1.0f);
    m_EnableStateRayColor.r = m_EnableStateRayColor.r > 1.0f ? 1.0f : m_EnableStateRayColor.r;
    m_EnableStateRayColor.g = m_EnableStateRayColor.g > 1.0f ? 1.0f : m_EnableStateRayColor.g;
    m_EnableStateRayColor.b = m_EnableStateRayColor.g > 1.0f ? 1.0f : m_EnableStateRayColor.b;
    
    if( m_bEnable )
    {
        m_CurrentRayColor = m_EnableStateRayColor;
        m_pParticleSystem->setColor(m_CurrentRayColor);
    }
    else
    {
        m_CurrentRayColor = m_DisableStateRayColor;
        m_pParticleSystem->setColor(m_CurrentRayColor);
    }
}

Color4f SinkGameObject::getColor() const
{
    return m_Color;
}

// Source Grid Coordinates are given at half intervals.
Vector2i SinkGameObject::getGridCoordinate() const
{
    Grid* pGrid = Game::getInstance()->getScene()->getGrid();
    float halfCellSize = pGrid->getCellSize() * 0.5f;
    Vector2i gridCoord( nearbyintf(m_Position.x/halfCellSize), nearbyintf(m_Position.y/halfCellSize) );
    return gridCoord;
}
