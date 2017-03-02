//
//  PortalGameObject.cpp
//  Optiks
//
//  Created by Prashant on 03/01/14.
//
//

#include "PortalGameObject.h"
#include "Scene.h"
#include "Game.h"
/*
 
        3
    -------------
    |           |
    |           |
1   |           |   0
    |           |
    |           |
    -------------
        2
 
 */

void PortalGameObject::render()
{
    m_pPortalShape1->render();
    m_pPortalShape2->render();
}

void PortalGameObject::renderGlow()
{
}

void PortalGameObject::update(float dt)
{
    rayTraceForGlow();
    
    reset();
}

bool PortalGameObject::intersectRay( Ray& ray, IntersectionInformation& intersectInfo )
{
//    bool bRes = m_pShape2D->intersectRay(ray, intersectInfo);
//    int hitEdge = -1;
//    if( bRes )
//    {
//        hitEdge = ((RectangleShape*)m_pShape2D)->getHitEdge();
//        // Check if this ray from given source is hitting any edge more than once. if yes ignore this hit!!!
//        if( m_NumRaysReflectedFromShapeEdge[ray.m_SourceID][hitEdge] > 1)
//        {
//            bRes = false;
//        }
//    }
    
//    return bRes;
    
    bool bOut = false;
    Intersect_Info hitInfo1, hitInfo2;
    bool bRes1 = m_pPortalShape1->intersectRay(ray, hitInfo1) && hitInfo1.T > 0.0f;
    bool bRes2 = m_pPortalShape2->intersectRay(ray, hitInfo2) && hitInfo2.T > 0.0f;
    
    if( bRes1 && bRes2 )
    {
        bOut = true;
        intersectInfo = hitInfo1.T < hitInfo2.T ? hitInfo1 : hitInfo2;
        // Decide from which Shape the Portal Ray is generated.
        // Ray will be generated from Portal Shape2
        if( hitInfo1.T < hitInfo2.T )
        {
            RectangleShape* pRectShape1 = (RectangleShape*)m_pPortalShape1;
            RectangleShape* pRectShape2 = (RectangleShape*)m_pPortalShape2;

            int hitEdge    = pRectShape1->getHitEdge();
            int portalEdge = m_PortalEdgeMap[hitEdge];
            m_PortalRayOrigin = pRectShape2->getPosition() + m_LocalPortalRayOriginPoints[portalEdge];
            m_PortalRayDirection = ray.direction;
            
            m_FirstHitPortalShapeIndex = 0;
            hitEdge = pRectShape1->getHitEdge();
            // Check if this ray from given source is hitting any edge more than once. if yes ignore this hit!!!
            if( m_NumRaysReflectedFromShape1Edge[ray.m_SourceID][hitEdge] > 2)
            {
                bOut = false;
            }
            m_FirstHitPortalShapeIndex = 0;
        }
        else
        {
            RectangleShape* pRectShape1 = (RectangleShape*)m_pPortalShape1;
            RectangleShape* pRectShape2 = (RectangleShape*)m_pPortalShape2;
            
            int hitEdge    = pRectShape2->getHitEdge();
            int portalEdge = m_PortalEdgeMap[hitEdge];
            m_PortalRayOrigin = pRectShape1->getPosition() + m_LocalPortalRayOriginPoints[portalEdge];
            m_PortalRayDirection = ray.direction;
            
            m_FirstHitPortalShapeIndex = 1;

            hitEdge = pRectShape2->getHitEdge();
            // Check if this ray from given source is hitting any edge more than once. if yes ignore this hit!!!
            if( m_NumRaysReflectedFromShape2Edge[ray.m_SourceID][hitEdge] > 2)
            {
                bOut = false;
            }
        }
    }
    else if ( bRes1 )
    {
        bOut = true;
        intersectInfo = hitInfo1;
        RectangleShape* pRectShape1 = (RectangleShape*)m_pPortalShape1;
        RectangleShape* pRectShape2 = (RectangleShape*)m_pPortalShape2;
        
        int hitEdge    = pRectShape1->getHitEdge();
        int portalEdge = m_PortalEdgeMap[hitEdge];
        m_PortalRayOrigin = pRectShape2->getPosition() + m_LocalPortalRayOriginPoints[portalEdge];
        m_PortalRayDirection = ray.direction;
        
        m_FirstHitPortalShapeIndex = 0;
        hitEdge = pRectShape1->getHitEdge();
        // Check if this ray from given source is hitting any edge more than once. if yes ignore this hit!!!
        if( m_NumRaysReflectedFromShape1Edge[ray.m_SourceID][hitEdge] > 2)
        {
            bOut = false;
        }
    }
    else if ( bRes2 )
    {
        bOut = true;
        intersectInfo = hitInfo2;
        RectangleShape* pRectShape1 = (RectangleShape*)m_pPortalShape1;
        RectangleShape* pRectShape2 = (RectangleShape*)m_pPortalShape2;
        
        int hitEdge    = pRectShape2->getHitEdge();
        int portalEdge = m_PortalEdgeMap[hitEdge];
        m_PortalRayOrigin = pRectShape1->getPosition() + m_LocalPortalRayOriginPoints[portalEdge];
        m_PortalRayDirection = ray.direction;
        
        m_FirstHitPortalShapeIndex = 1;

        hitEdge = pRectShape2->getHitEdge();
        // Check if this ray from given source is hitting any edge more than once. if yes ignore this hit!!!
        if( m_NumRaysReflectedFromShape2Edge[ray.m_SourceID][hitEdge] > 2)
        {
            bOut = false;
        }
    }
    else
    {
    }
    return bOut;
}

Ray PortalGameObject::getPortalRay() const
{
    Ray ray;
    ray.origin = m_PortalRayOrigin;
    ray.direction = m_PortalRayDirection;
    ray.direction.normalize();
    return ray;
}

Vector2f PortalGameObject::getClosestGridCentre() const
{
    if( m_pSelectedShape == m_pPortalShape1 )
        return m_pPortalShape1->getPosition();
    else
        return m_pPortalShape2->getPosition();
}

bool PortalGameObject::pick(const Vector2f& point) const
{
    bool bRes = false;
    if( m_pPortalShape1->pick(point) )
    {
        m_pSelectedShape = m_pPortalShape1;
        bRes = true;
    }
    if( m_pPortalShape2->pick(point) )
    {
        m_pSelectedShape = m_pPortalShape2;
        bRes = true;
    }
    return bRes;
}

void PortalGameObject::save(FILE* pFile)
{
    m_pPortalShape1->save(pFile);
    m_pPortalShape2->save(pFile);
}

void PortalGameObject::load(FILE* pFile)
{
    
}

void PortalGameObject::setTransform(const Matrix3f& transform)
{
    m_Transform = transform;
    m_pSelectedShape->setTransform(transform);
    if( m_pSelectedShape == m_pPortalShape1 )
    {
        m_pGlowShape1->setTransform(transform);
    }
    if( m_pSelectedShape == m_pPortalShape2 )
    {
        m_pGlowShape2->setTransform(transform);
    }
}

void PortalGameObject::setPosition( const Vector2f& position )
{
    m_Position = position;
    m_pSelectedShape->setPosition(position);
    if( m_pSelectedShape == m_pPortalShape1 )
    {
        m_pGlowShape1->setPosition(position);
    }
    if( m_pSelectedShape == m_pPortalShape2 )
    {
        m_pGlowShape2->setPosition(position);
    }
}

void PortalGameObject::setRotation( float rotation )
{
    m_Rotation = rotation;
    m_pSelectedShape->rotate(rotation);
}

void PortalGameObject::reset()
{
    // Assuming that the Shape is Rect
    for( int i=0; i<MAX_SOURCES; i++ )
    {
        for( int j=0; j<4; j++ )
        {
            m_NumRaysReflectedFromShape1Edge[i][j] = 0;
            m_NumRaysReflectedFromShape2Edge[i][j] = 0;
        }
    }
}
// Determine no of hits for edges, and flag the ray to terminate.
void PortalGameObject::increaseRayHitCount(Ray& ray)
{
    IntersectionInformation intersectInfo;
    
    if( m_FirstHitPortalShapeIndex == 0 )
        m_pPortalShape1->intersectRay(ray, intersectInfo);
    else
        m_pPortalShape2->intersectRay(ray, intersectInfo);

    int hitEdge = 0;
    
    if( m_FirstHitPortalShapeIndex == 0 )
        hitEdge = ((RectangleShape*)m_pPortalShape1)->getHitEdge();
    else
        hitEdge = ((RectangleShape*)m_pPortalShape2)->getHitEdge();

    if( m_FirstHitPortalShapeIndex == 0 )
        m_NumRaysReflectedFromShape1Edge[ray.m_SourceID][hitEdge]++;
    else
        m_NumRaysReflectedFromShape2Edge[ray.m_SourceID][hitEdge]++;

    // if the Ray hits more than once for a given edge terminate it.
    int hitCount = 0;
    
    if( m_FirstHitPortalShapeIndex == 0 )
        hitCount = m_NumRaysReflectedFromShape1Edge[ray.m_SourceID][hitEdge];
    else
        hitCount = m_NumRaysReflectedFromShape2Edge[ray.m_SourceID][hitEdge];

    if(  hitCount > 2 )
    {
        ray.m_bUserTerminate = true;
    }
}


PortalGameObject::~PortalGameObject()
{
    Scene::s_pBlockAllocator->Free(m_pPortalShape1, sizeof(RectangleShape));
    Scene::s_pBlockAllocator->Free(m_pPortalShape2, sizeof(RectangleShape));
    Scene::s_pBlockAllocator->Free(m_pGlowShape1, sizeof(Circle));
    Scene::s_pBlockAllocator->Free(m_pGlowShape2, sizeof(Circle));
}

PortalGameObject::PortalGameObject( const PortalGameObjectInfo& pInfo ):GameObject(pInfo)
{
    RectangleInfo* rInfo1 = (RectangleInfo*)pInfo.shapeInfo1;
    m_pPortalShape1 = new(Scene::s_pBlockAllocator->Allocate(sizeof(RectangleShape)))RectangleShape(*rInfo1);
    RectangleInfo* rInfo2 = (RectangleInfo*)pInfo.shapeInfo2;
    m_pPortalShape2 = new(Scene::s_pBlockAllocator->Allocate(sizeof(RectangleShape)))RectangleShape(*rInfo2);
    Matrix3f xForm;
    xForm.translate(pInfo.shapeInfo1->position.x, pInfo.shapeInfo1->position.y);
    m_pPortalShape1->setTransform(xForm);
    m_pPortalShape1->setPosition(Vector2f(xForm.col3.x, xForm.col3.y));
    xForm.translate(pInfo.shapeInfo2->position.x, pInfo.shapeInfo2->position.y);
    m_pPortalShape2->setTransform(xForm);
    m_pPortalShape2->setPosition(Vector2f(xForm.col3.x, xForm.col3.y));

    
    m_Width = rInfo1->width;
    m_Height = rInfo1->height;
    
    m_pTexture = Game::getInstance()->m_pPortalRectShapeTexture;
    m_pNormalMapTexture = Game::getInstance()->m_pPortalRectShapeNormalMapTexture;
    
    m_FirstHitPortalShapeIndex = 0;
    m_pSelectedShape = m_pPortalShape1;
    m_ObjectInfo.m_ObjectTextureID = EOT_PORTAL;

    m_zOrder = 100;
    
    m_PortalEdgeMap[0] = 1;
    m_PortalEdgeMap[1] = 0;
    m_PortalEdgeMap[2] = 3;
    m_PortalEdgeMap[3] = 2;
    
    Vector2f epsilonx(0.01f,0.0f);
    Vector2f epsilony(0.0f,0.01f);
    
    m_LocalPortalRayOriginPoints[0] = Vector2f(m_Width*0.5f, 0.0f) + epsilonx;
    m_LocalPortalRayOriginPoints[1] = Vector2f(-m_Width*0.5f, 0.0f) - epsilonx;
    m_LocalPortalRayOriginPoints[2] = Vector2f(0.0f,-m_Height*0.5f) - epsilony;
    m_LocalPortalRayOriginPoints[3] = Vector2f(0.0f,m_Height*0.5f) + epsilony;
    
    m_NumInterReflections = 20;
    m_GlowShapeRadius = 40.0f;
    
    CircleInfo cInfo;
    cInfo.m_IsReflective = true;
    cInfo.radius = m_GlowShapeRadius;
    m_pGlowShape1 = new(Scene::s_pBlockAllocator->Allocate(sizeof(Circle))) Circle(cInfo);
    m_pGlowShape2 = new(Scene::s_pBlockAllocator->Allocate(sizeof(Circle))) Circle(cInfo);
    m_pGlowShape1->setTransform(xForm);
    m_pGlowShape2->setTransform(xForm);
    reset();
}

void PortalGameObject::renderGlowRays(float width)
{
    for( int i=0; i<m_vecInnerGlowRays.size(); i++)
    {
        m_vecInnerGlowRays[i].setColor(Color4f(0.8f,0.16f,0.0f,1.0f));
        m_vecInnerGlowRays[i].render(width/1.5, Color4f(0.0f,0.4f,1.0f,1.0f));
    }
}

void PortalGameObject::rayTraceForGlow(const Vector2f& centre, Shape2D* pShape)
{
    m_SourceRay.direction = Vector2f(1.0f,0.0f);
    m_SourceRay.direction.normalize();
    
    m_SourceRay.origin = Vector2f( centre - Vector2f(0.0f,m_GlowShapeRadius*0.8));
    Matrix3f xForm, rotXform, T, TInv;
    rotXform.rotate(m_RotAngle);
    T.translate(centre);
    TInv.translate(-centre);
    xForm = T;
    xForm *= rotXform;
    xForm *= TInv;

    Ray startRay = m_SourceRay;

    xForm.transformAsPoint(startRay.origin);
    //xForm.transformAsVector(startRay.direction);
    
    
    m_RotAngle += 0.05f;
    
    Intersect_Info intersectInfo;
    
    for( int i=0; i<m_NumInterReflections; i++)
    {
        pShape->intersectRay(startRay, intersectInfo);
        
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

void PortalGameObject::rayTraceForGlow()
{
    m_vecInnerGlowRays.clear();

    rayTraceForGlow(m_pPortalShape1->getPosition(), m_pGlowShape1);
    rayTraceForGlow(m_pPortalShape2->getPosition(), m_pGlowShape2);
}