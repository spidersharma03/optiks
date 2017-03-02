#include "Scene.h"
#include "Shape2D.h"
#include <math.h>
#include <vector>
#include "Grid.h"
#include "Source.h"
#include <sys/stat.h>
#include "FBO.h"
#include "SourceCodeShader.h"
#include "GLSLShaderProgram.h"
#include "GLSL2ShaderProgram.h"
#include "Texture2D.h"
#include "FluidSimulator.h"
#include "TextureMaker.h"
#include "TextRenderer.h"
#include "GameObject.h"
#include "ShapeGameObject.h"
#include "SourceGameObject.h"
#include "SinkGameObject.h"
#include "PlacementMarkerGameObject.h"
#include "GameMenuBackgroundDecorObject.h"
#include "PortalGameObject.h"
#include <assert.h>
#include "OglRenderer.h"
#include "Game.h"
#include "FileUtil.h"
#include "GameLevelSelectionState.h"
#include "GameChapterSelectionState.h"
#include "IGlowing.h"
#include "TaskManager.h"

using namespace spider3d::render;

memory::MemoryBlockAllocator* Scene::s_pBlockAllocator = NULL;

using namespace std;


static float Time = 0.0f;

Scene::Scene(int nX, int nY, int screenScale)
{
    m_ScreenScaleFactor = screenScale;
	traceDepth = 1;
    m_HintID = -1;
    m_SelectedHintID = -1;
	m_RefractionIndex = 1.0f;
	currentSelectedObjectIndex = 0;
	m_pGrid = 0;
	m_PickedObject = 0;
    m_TextureSizeX = nX;
    m_TextureSizeY = nY;
    m_SceneWidth   = nX;
    m_SceneHeight  = nY;
    m_bEnableDebugGrid = true;
    m_pDraggedObject = NULL;
    m_bReadyForScreenShot = false;
    
    m_Fbo = new FrameBufferObjectTexture_2D( nX,  nY );
    m_RenderTextureCurrent = new Texture2D( nX, nY, 0);
    m_LineGlowTexture = new Texture2D(nX/2,nY/2,0);
    m_FinalCompositeTexture = new Texture2D(nX, nY, 0);
    m_pFinalTexture = new Texture2D(nX, nY,0);
    m_pFinalTempTexture = new Texture2D(nX, nY,0);
    m_pGuiBackGroundTexture = new Texture2D(nX/4,nY/4,0);
    m_pGuiBackGroundTempTexture = new Texture2D(nX/4,nY/4,0);
    
    int waveGeneratorSize = 80;
    Image* pImage = new Image(waveGeneratorSize,waveGeneratorSize*(float)nY/nX,EIF_RGBA_FLOAT32);
    m_pFloatTextureWaveGenerator = new Texture2D(waveGeneratorSize,waveGeneratorSize*(float)nY/nX,pImage);
    
    m_ReflectionTextureScale = 1.0f;
    m_ReflectionTexture = new Texture2D(nX/m_ReflectionTextureScale,nY/m_ReflectionTextureScale,0);

    m_AbsolutePath = "/Users/prashantsharma/Downloads/Backup_MacBookPro/Optiks/";
    
    initLensShader();
    
    m_GlowScaleDownFactor = 4.0f;
    int width = nX/2;
    int height = nY/2;
    for( int i=0; i<NUM_TEXTURES; i++ )
    {
        m_RenderTextureGlowCurrent[i] = new Texture2D(width,height,0);
        m_RenderTextureGlowPrevious[i] = new Texture2D(width,height,0);
        
        width /= 2;
        height /= 2;
    }
    
    m_AverageLightingFactor = 0.0f;
    //m_Fbo->attachTexture(m_RenderTextureGlowCurrent);
    
    s_pBlockAllocator = new memory::MemoryBlockAllocator();
    
    m_currentSelectedGameObject = NULL;
    m_PickedGameObject = NULL;
    
    m_NumSources = 0;
    m_NumShapeGameObjects = 0;
    m_NumSinkGameObjects = 0;
    m_NumPlacementMarkerGameObjects = 0;
    
    createPrototypeObjects();
    
    
    m_pAllRayVertices  = new float[6*2*MAX_RAY_COUNT];
    m_pAllRayColors    = new float[6*4*MAX_RAY_COUNT];
    m_pAllRayTexCoords = new float[6*2*MAX_RAY_COUNT];
    
    GameMenuBackgroundDecorObjectInfo decorInfo;
    decorInfo.numInterReflections = 40;

    decorInfo.color = Color4f(0.5f,0.21f,0.0f,1.0f);
    decorInfo.m_Position = Vector2f(512,384);
    decorInfo.radius = BASIC_ELEMENT_IMAGE_SIZE * 2.0;
    

//    m_vecGameMenuBackgroundDecorObjects.push_back(m_pGameMenuBackgroundDecorObject1);
//    m_vecGameMenuBackgroundDecorObjects.push_back(m_pGameMenuBackgroundDecorObject2);
    //m_vecGameMenuBackgroundDecorObjects.push_back(m_pGameMenuBackgroundDecorObject3);
    //m_vecGameMenuBackgroundDecorObjects.push_back(m_pGameMenuBackgroundDecorObject4);
    //orthoMatrix.ortho(0, m_SceneWidth, 0, m_SceneHeight, -1, 1);
    
    //m_pFluidSimulator = new FluidSimulator(64,64);
    //m_pFluidSimulator->TEX_RES_X = 1024.0f;
    //m_pFluidSimulator->TEX_RES_Y = 768.0f;
    
    m_pFadeSceneTask = new FadeSceneTask(m_SceneWidth, m_SceneHeight, 1.0f,1.0f,1.0f);
    Game::getInstance()->getTaskManagerBelowGUI()->Attach(m_pFadeSceneTask);
}

void Scene::fadeIn()
{
    m_pFadeSceneTask->reset();
    m_pFadeSceneTask->setStartTransparency(1.0f);
    m_pFadeSceneTask->setEndTransparency(0.3f);
}

void Scene::fadeOut()
{
    m_pFadeSceneTask->reset();
    m_pFadeSceneTask->setStartTransparency(0.3f);
    m_pFadeSceneTask->setEndTransparency(1.0f);
}

string  Scene::getFullPath(const string& fileName) const
{
    string retPath;
#ifdef  IOS
    FileUtil fu;
    retPath = fu.getAbsolutePath(fileName);
#else
    retPath = m_AbsolutePath + fileName;
#endif
    return retPath;
}

void Scene::changeSize( int width, int height )
{
    m_SceneWidth  = width;
    m_SceneHeight = height;
}

GameObject* Scene::createGameObject(const GameObjectInfo& gameObjectInfo)
{
    GameObject* pGameObject = NULL;
    
    switch (gameObjectInfo.m_ObjectType) {
        // Shape Game Object
        case EOT_SHAPE_OBJECT:
        {
            ShapeGameObjectInfo& sInfo = (ShapeGameObjectInfo&)gameObjectInfo;
            pGameObject = new(s_pBlockAllocator->Allocate(sizeof(ShapeGameObject)))
            ShapeGameObject(sInfo);
            break;
        }
        case EOT_PORTAL:
        {
            PortalGameObjectInfo& pInfo = (PortalGameObjectInfo&)gameObjectInfo;
            pGameObject = new(s_pBlockAllocator->Allocate(sizeof(PortalGameObject)))
            PortalGameObject(pInfo);
            break;
        }
        // Light Source
        case EOT_LIGHT_SOURCE:
        {
            SourceGameObjectInfo& sInfo = (SourceGameObjectInfo&)gameObjectInfo;
            pGameObject = new(s_pBlockAllocator->Allocate(sizeof(SourceGameObject)))
            SourceGameObject(sInfo);
            break;
        }
        // Light Sink/Reactor
        case EOT_SINK:
        {
            SinkGameObjectInfo& sInfo = (SinkGameObjectInfo&)gameObjectInfo;
            pGameObject = new(s_pBlockAllocator->Allocate(sizeof(SinkGameObject)))
            SinkGameObject(sInfo);
            break;
        }
        case EOT_PLACEMENT_MARKER:
        {
            PlacementMarkerGameObjectInfo& vInfo = (PlacementMarkerGameObjectInfo&)gameObjectInfo;
            pGameObject = new(s_pBlockAllocator->Allocate(sizeof(PlacementMarkerGameObject)))
            PlacementMarkerGameObject(vInfo);
            break;
        }
        case EOT_DECOR:
        {
            GameMenuBackgroundDecorObjectInfo& dInfo = (GameMenuBackgroundDecorObjectInfo&)gameObjectInfo;
            pGameObject = new(s_pBlockAllocator->Allocate(sizeof(GameMenuBackgroundDecorObject)))
            GameMenuBackgroundDecorObject(dInfo);
            break;
        }
        default:
            break;
    }
    return pGameObject;
}

void Scene::removeGameObject(GameObject* pGameObject)
{
    for( int i=0; i<m_vecGameObjects.size(); i++ )
    {
        if( pGameObject == m_vecGameObjects[i])
        {
            m_vecGameObjects.erase(m_vecGameObjects.begin()+i);
        }
    }
    for( int i=0; i<m_vecSinkObjects.size(); i++ )
    {
        if( pGameObject == m_vecSinkObjects[i])
        {
            m_vecSinkObjects.erase(m_vecSinkObjects.begin()+i);
        }
    }
    for( int i=0; i<m_vecSourceObjects.size(); i++ )
    {
        if( pGameObject == m_vecSourceObjects[i])
        {
            m_vecSourceObjects.erase(m_vecSourceObjects.begin()+i);
        }
    }
    for( int i=0; i<m_vecPlacementMarkerGameObjects.size(); i++ )
    {
        if( pGameObject == m_vecPlacementMarkerGameObjects[i])
        {
            m_vecPlacementMarkerGameObjects.erase(m_vecPlacementMarkerGameObjects.begin()+i);
        }
    }
    for( int i=0; i<m_vecPortalObjects.size(); i++ )
    {
        if( pGameObject == m_vecPortalObjects[i])
        {
            m_vecPortalObjects.erase(m_vecPortalObjects.begin()+i);
        }
    }
}

void Scene::destroyGameObject(GameObject* pGameObject)
{
    const GameObjectInfo& gInfo = pGameObject->getGameObjectInfo();
        
    switch ( gInfo.m_ObjectType ) {
        // Shape Game Object
        case EOT_SHAPE_OBJECT:
        {
            ShapeGameObject* pShapeGameObject =(ShapeGameObject*)pGameObject;
            Vector2f v = pGameObject->getClosestGridCentre();
            m_pGrid->resetOccupancy(v.x, v.y);
            pShapeGameObject->~ShapeGameObject();
            if( m_NumShapeGameObjects > 0 )
                m_NumShapeGameObjects--;
            s_pBlockAllocator->Free(pGameObject, sizeof(ShapeGameObject));
            break;
        }
        case EOT_LIGHT_SOURCE:
        {
            SourceGameObject* pSourceGameObject =(SourceGameObject*)pGameObject;
            Vector2f v = pGameObject->getClosestGridCentre();
            m_pGrid->resetOccupancy(v.x, v.y);
            pSourceGameObject->~SourceGameObject();
            if( m_NumSources > 0 )
                m_NumSources--;
            s_pBlockAllocator->Free(pGameObject, sizeof(SourceGameObject));
            break;
        }
        case EOT_SINK:
        {
            SinkGameObject* pSinkGameObject =(SinkGameObject*)pGameObject;
            Vector2f v = pGameObject->getClosestGridCentre();
            m_pGrid->resetOccupancy(v.x, v.y);
            pSinkGameObject->~SinkGameObject();
            if( m_NumSinkGameObjects > 0 )
                m_NumSinkGameObjects--;
            s_pBlockAllocator->Free(pGameObject, sizeof(SinkGameObject));
            break;
        }
        case EOT_PLACEMENT_MARKER:
        {
            PlacementMarkerGameObject* pPlacementMarkerGameObject =(PlacementMarkerGameObject*)pGameObject;
            Vector2f v = pGameObject->getClosestGridCentre();
            m_pGrid->resetPlacementMark(v.x, v.y);
            pPlacementMarkerGameObject->~PlacementMarkerGameObject();
            if( m_NumPlacementMarkerGameObjects > 0 )
                m_NumPlacementMarkerGameObjects--;
            s_pBlockAllocator->Free(pGameObject, sizeof(PlacementMarkerGameObject));
            break;
        }
        case EOT_PORTAL:
        {
            PortalGameObject* pPortalGameObject =(PortalGameObject*)pGameObject;
            Vector2f posPortal1 = pPortalGameObject->getPortalShape1()->getPosition();
            Vector2f posPortal2 = pPortalGameObject->getPortalShape2()->getPosition();
            m_pGrid->resetOccupancy(posPortal1.x, posPortal1.y);
            m_pGrid->resetOccupancy(posPortal2.x, posPortal2.y);
            pPortalGameObject->~PortalGameObject();
            if( m_NumPortalGameObjects > 0 )
                m_NumPortalGameObjects--;
            s_pBlockAllocator->Free(pGameObject, sizeof(PortalGameObject));
            break;
        }
        default:
            break;
    }
    removeGameObject(pGameObject);
}


void Scene::startRayTracing()
{
	
}

bool Scene::removeSelectedObject()
{
	if( m_currentSelectedObject == 0 )
		return false;
	
	int index = -1;
	for( int i=0; i<objects.size(); i++ )
	{
		if( m_currentSelectedObject == objects[i] )
		{
			index = i;
			break;
		}
	}
	if( index != -1 )
	{
		// clear the grid occupancy
		if( m_pGrid  && m_currentSelectedObject)
		{
			//Matrix3f m = m_currentSelectedObject->getTransform();
			Vector2f v = m_currentSelectedObject->getPosition();
			m_pGrid->resetOccupancy(v.x, v.y);
		}
		objects.erase(objects.begin() + index);
	}
	return index != -1;
}

bool Scene::getFreePlacementmarkerPosition(Vector2f& freePosition)
{
    bool bFound = false;
    Vector2f inPoint;
    for( int i=0; i<m_vecPlacementMarkerGameObjects.size(); i++ )
    {
        PlacementMarkerGameObject* pMarkerObject = m_vecPlacementMarkerGameObjects[i];
        Vector2f position = pMarkerObject->getPosition();
        
        if( !m_pGrid->isOccupied(position.x, position.y, 0) )
        {
            inPoint = position;
            bFound = true;
            break;
        }
    }
    m_pGrid->snap(inPoint, freePosition);
    return bFound;
}

void Scene::reset()
{
	for( int o=0; o<m_vecGameObjects.size(); o++)
    {
        if( m_vecGameObjects[o]->getGameObjectInfo().m_ObjectType == EOT_SHAPE_OBJECT )
        {
            ShapeGameObject* pShapeGameObject = (ShapeGameObject*)m_vecGameObjects[o];
            pShapeGameObject->getShape2D()->m_bRayEntering = false;
        }
        if( m_vecGameObjects[o]->getGameObjectInfo().m_ObjectType == EOT_PORTAL )
        {
            PortalGameObject* pPortalGameObject = (PortalGameObject*)m_vecGameObjects[o];
            pPortalGameObject->getPortalShape1()->m_bRayEntering = false;
            pPortalGameObject->getPortalShape2()->m_bRayEntering = false;
        }
    }
	vecRays.clear();
    
	traceDepth = 100;
    m_AverageLightingFactor = 0.0f;
}

void Scene::clearScene()
{
    reset();
    
    m_pGrid->resetGrid();
    
    for( int i=0; i<m_vecGameObjects.size(); i++ )
    while(m_vecGameObjects.size() > 0 )
    {
        destroyGameObject(m_vecGameObjects[i]);
    }

    m_vecGameObjects.clear();
    m_vecSourceObjects.clear();
    m_vecPlacementMarkerGameObjects.clear();
    m_vecSinkObjects.clear();
    m_vecPortalObjects.clear();
    
    m_NumShapeGameObjects = 0;
    m_NumSources = 0;
    m_NumSinkGameObjects = 0;
    m_NumPlacementMarkerGameObjects = 0;
    m_NumPortalGameObjects = 0;
    m_HintID = -1;
}

void Scene::update()
{
    for( int i=0; i<m_vecSinkObjects.size(); i++ )
    {
        m_vecSinkObjects[i]->setEnable(false);
    }
    for( int i=0; i<m_vecGameObjects.size(); i++ )
    {
        m_vecGameObjects[i]->update(0.0f);
    }
    for( int i=0; i<m_vecSourceObjects.size(); i++ )
    {
        Ray ray = m_vecSourceObjects[i]->getRay();
        
        rayTrace(ray);
    }
    // Check if the Sinks are intersecting.
    for( int i=0; i<m_vecSinkObjects.size(); i++ )
    {
        m_vecSinkObjects[i]->checkForRayColorMatch();
    }
    
    //rasterizeVelocity();
    
    //m_pFluidSimulator->advance(0.16);

    //m_pFluidSimulator->advanceFluidParticles(m_Particles, 0.16);
}

void Scene::rayTrace(Ray& ray)
{
	traceDepth--;
	
	if( traceDepth <= 0 )
		return;
	
	if( ray.m_Power < 0.1f )
		return;
	
    GameObject* pClosestGameObject = 0;
    
	Shape2D* closest_object = 0;
	Intersect_Info hitInfo;
	Intersect_Info besthitInfo;
	
	besthitInfo.T = 1e20f;
	hitInfo.T = 1e20f;
    
	for ( unsigned i = 0 ; i < m_vecGameObjects.size(); i++ )
	{
        GameObject* pGameObject = m_vecGameObjects[i];
        
        if( pGameObject->getGameObjectInfo().m_ObjectType == EOT_LIGHT_SOURCE ||
            pGameObject->getGameObjectInfo().m_ObjectType == EOT_PLACEMENT_MARKER )
            continue;
        
        ShapeGameObject* pShapeGameObject = (ShapeGameObject*)pGameObject;
        
        Shape2D* pShape2D = pShapeGameObject->getShape2D();
        
		bool ret = pShapeGameObject->intersectRay(ray , hitInfo);
        
		if ( !ret )
		{
			continue;
		}
		if( hitInfo.T < 1e-15 ) // Ignore if the Point of Intersecion is self
		{
			continue;
		}
		
		if ( hitInfo.T < besthitInfo.T )
		{
			closest_object = pShape2D;
            pClosestGameObject = pShapeGameObject;
			besthitInfo = hitInfo;
		}
	}
	//
	if ( closest_object )
	{
		ray.endPoint = ray.origin + ray.direction * besthitInfo.T;// besthitInfo.hitPoints[0].x;
     
        // Push Back the Ray
        if( !ray.m_bUserTerminate )
            vecRays.push_back(ray);
        
        if( pClosestGameObject->getGameObjectInfo().m_ObjectType == EOT_SHAPE_OBJECT )
        {
            ShapeGameObject* pShapeGameObject = ((ShapeGameObject*)pClosestGameObject);
            if( !ray.m_bInside)
                pShapeGameObject->increaseRayHitCount(ray);
        }
        // Portal object
        if ( pClosestGameObject->getGameObjectInfo().m_ObjectType == EOT_PORTAL )
        {
            PortalGameObject* pPortalGameObject = (PortalGameObject*)pClosestGameObject;
            Ray portalRay = pPortalGameObject->getPortalRay();
            portalRay.setColor(ray.getColor());
            portalRay.m_SourceID = ray.m_SourceID;
            
            //if( !ray.m_bInside)
                pPortalGameObject->increaseRayHitCount(ray);
            
            if( !ray.m_bUserTerminate )
                rayTrace(portalRay);
        }
		// Create Refraction Ray
        if( closest_object->m_isRefractive  )
		{
			if( closest_object->m_isSolid )
			{
				closest_object->m_bRayEntering = !closest_object->m_bRayEntering;
				
				float refrativeIndexRatio;
				float rImedium1, rImedium2;
                
				rImedium1 = ray.m_CurrentShape == 0 ? m_RefractionIndex : ray.m_CurrentShape->m_RefractionIndex;
				rImedium2 = closest_object->m_RefractionIndex;
                
                //if( closest_object->m_bRayEntering )
                if( !ray.m_bInside )
                {
                    refrativeIndexRatio = m_RefractionIndex/closest_object->m_RefractionIndex;
                }
                else
                {
                    refrativeIndexRatio = m_RefractionIndex/closest_object->m_RefractionIndex;
                    refrativeIndexRatio = 1.0f/refrativeIndexRatio;
                }
				
				Vector2f normal = besthitInfo.hitNormal[0];
				// Flip Normal if the Ray is inside.
				if( ray.m_bInside )
                //if( !closest_object->m_bRayEntering )
                {
					normal.x *= -1.0f;
					normal.y *= -1.0f;
				}
				
				Vector2f V(-besthitInfo.hitPoints[0] + ray.origin);
				V.normalize();
                
				float cstheta1 = normal.dot(V);
				float sn2theta1 = 1.0f - cstheta1 * cstheta1;
				float k = sqrt( 1.0f - refrativeIndexRatio * refrativeIndexRatio * sn2theta1);
				
				Vector2f RefractionDir;
				RefractionDir = normal * (refrativeIndexRatio * cstheta1 - k) - V * refrativeIndexRatio;
				RefractionDir.normalize();
				
				Vector2f rayOrigin(besthitInfo.hitPoints[0] + RefractionDir * 0.01f);
				Ray refractedRay(rayOrigin , RefractionDir);
				
				refractedRay.m_bInside = ray.m_bInside ? false : true;
				
				refractedRay.m_CurrentShape = refractedRay.m_SecondShape = closest_object;
                refractedRay.m_SourceID = ray.m_SourceID;
                refractedRay.setColor(ray.getColor());
                
                // Change the Color of the ray, if it is a filter.
                if( pClosestGameObject->getGameObjectInfo().m_ObjectType == EOT_SHAPE_OBJECT )
                {
                    ShapeGameObject* pShapeGameObject = (ShapeGameObject*)pClosestGameObject;
                    if( pShapeGameObject->getMaterialType() == EMT_COLOR_FILTER )
                    {
                        Color4f filterColor = pShapeGameObject->getColor();
                        Color4f filterRayColor;
                        // if the ray color is same as filter color, let it pass through.
                        if( fabs(filterColor.r - ray.m_Color.r) < 0.01f &&
                           fabs(filterColor.g - ray.m_Color.g) < 0.01f &&
                           fabs(filterColor.b - ray.m_Color.b) < 0.01f )
                        {
                            filterRayColor = ray.m_Color;
                        }
                        // Find out the pass through color, by subtracting filter color from ray color.
                        else
                        {
                            Color4f complColor = Color4f(1.0f-filterColor.r, 1.0f-filterColor.g, 1.0f-filterColor.b, filterColor.a);
                            filterRayColor = ray.m_Color - complColor;
                            filterRayColor.a = 1.0f;
                            
                            filterRayColor.r = filterRayColor.r < 0.0f ? 0.0f : filterRayColor.r;
                            filterRayColor.g = filterRayColor.g < 0.0f ? 0.0f : filterRayColor.g;
                            filterRayColor.b = filterRayColor.b < 0.0f ? 0.0f : filterRayColor.b;
                        }
                        // No Ray is spawned if the filter absorbs it all!!!
                        if( filterRayColor.r == 0.0f && filterRayColor.g == 0.0f && filterRayColor.b == 0.0f )
                        {
                            ray.m_bUserTerminate = true;
                        }
                        else
                        {
                            refractedRay.setColor(filterRayColor);
                        }
                    }
                }
                
                if( !ray.m_bUserTerminate )
                    rayTrace(refractedRay);
			}
		}
        // Create Reflection Ray
        if( closest_object->m_IsReflective && !ray.m_bInside )
		{
			Vector2f ReflectionDir;
			Vector2f V(-besthitInfo.hitPoints[0] + ray.origin);
			V.normalize();
			float NdotV = besthitInfo.hitNormal[0].dot(V);
			ReflectionDir = 2 * NdotV * besthitInfo.hitNormal[0] - V;
			Vector2f rayOrigin(besthitInfo.hitPoints[0] + ReflectionDir * 0.01);
			Ray reflectedRay(rayOrigin , ReflectionDir);
			
            reflectedRay.m_CurrentShape = closest_object;
			reflectedRay.m_SourceID = ray.m_SourceID;
            reflectedRay.setColor(ray.getColor());
            
            if( !ray.m_bUserTerminate )
                rayTrace(reflectedRay);
		}
	}
	else{
		ray.endPoint.x = ray.origin.x + ray.direction.x * 1e5;
		ray.endPoint.y = ray.origin.y + ray.direction.y * 1e5;
        // Push Back the Ray
        if( !ray.m_bUserTerminate )
            vecRays.push_back(ray);
	}
    // Check if this Ray intersects with any Sink
    checkIntersectionWithSinkObjects(ray, besthitInfo.T);
}

void Scene::checkIntersectionWithSinkObjects(Ray& ray, float bestHit)
{
    for( int i=0; i<m_vecSinkObjects.size(); i++ )
    {
        SinkGameObject* pSinkGameObject = m_vecSinkObjects[i];
        Intersect_Info hitInfo;
        bool bRes = pSinkGameObject->getShape2D()->intersectRay(ray, hitInfo);
        if( bRes && hitInfo.T > 0.0f &&  hitInfo.T < bestHit)
        {
            pSinkGameObject->pushRayForSinkEnableCheck(ray);
        }
    }
}


bool Scene::areAllSinkIntersecting()
{
    int numIntersectedSinkObjects = 0;
    for( int i=0; i<m_vecSinkObjects.size(); i++ )
    {
        SinkGameObject* pSinkGameObject = m_vecSinkObjects[i];
        if( pSinkGameObject->isEnable() )
        {
            numIntersectedSinkObjects++;
        }
    }
    m_bAllSinkIntersecting = (numIntersectedSinkObjects == m_vecSinkObjects.size());
    return m_bAllSinkIntersecting;
}

GameObject* Scene::pickGameObject(const Vector2f& point )
{
    GameObject* pPickedGameObject = NULL;
    bool bRes = false;
    
	for( int i=0; i<m_vecGameObjects.size(); i++ )
    {
        bRes = m_vecGameObjects[i]->pick(point);
        if( bRes )
        {
            pPickedGameObject = m_vecGameObjects[i];
        }
    }
    m_currentSelectedGameObject = pPickedGameObject;
    
    return pPickedGameObject;
}

PlacementMarkerGameObject* Scene::pickMarkerObject(const Vector2f& point )
{
    PlacementMarkerGameObject* pPickedMarkerObject = NULL;
    bool bRes = false;
    
	for( int i=0; i<m_vecPlacementMarkerGameObjects.size(); i++ )
    {
        bRes = m_vecPlacementMarkerGameObjects[i]->pick(point);
        if( bRes )
        {
            pPickedMarkerObject = m_vecPlacementMarkerGameObjects[i];
            break;
        }
    }
    return pPickedMarkerObject;
}

bool Scene::isGameObjectPlaceable(const Vector2f& position, GAME_OBJECT_TYPE objectType)
{
    bool bResult = false;

    if( objectType == EOT_PLACEMENT_MARKER )
    {
        if( !m_pGrid->isPlacementMarked(position.x, position.y, 0) )
        {
            bResult = true;
        }
    }
    else if ( objectType == EOT_SHAPE_OBJECT )
    {
        if( m_pGrid->isPlacementMarked(position.x, position.y, 0) && !m_pGrid->isOccupied(position.x, position.y, 0) )
            bResult = true;
    }
    else if ( objectType == EOT_PORTAL )
    {
        // Portal Object requires more careful treatment
        if( m_pGrid->isPlacementMarked(position.x, position.y, 0) && !m_pGrid->isOccupied(position.x, position.y, 0) )
            bResult = true;
    }
    else if ( objectType == EOT_SINK || objectType == EOT_LIGHT_SOURCE )
    {
        bResult = true;
    }
    return bResult;
}

bool Scene::addGameObject(GameObject* pGameObject)
{
    bool bResult = false;
    // Placement Marker for placing Shapes, Sinks etc.
    if( pGameObject->getGameObjectInfo().m_ObjectType == EOT_PLACEMENT_MARKER )
    {
        //Matrix3f m = pGameObject->getTransform();
		Vector2f v = pGameObject->getPosition();//( m.col3.x, m.col3.y);
		Vector2f vout;
		
		// If grid is not marked for placement.
		//if( !m_pGrid->isPlacementMarked(v.x, v.y, 0) )
		{
            m_vecGameObjects.insert(m_vecGameObjects.begin(), (PlacementMarkerGameObject*)pGameObject);
            //m_vecGameObjects.push_back((PlacementMarkerGameObject*)pGameObject);
			m_vecPlacementMarkerGameObjects.push_back((PlacementMarkerGameObject*)pGameObject);
            m_NumPlacementMarkerGameObjects++;
            
            m_pGrid->setPlacementMarked(v.x, v.y, 0);
            
            m_pGrid->snap(v,vout);
            
            Vector2f t;
            Matrix3f xForm;
            xForm.setIdentity();
            t.x = vout.x;
            t.y = vout.y;
            xForm.translate(t.x, t.y);
            pGameObject->setPosition(t);
            bResult = true;
		}
    }
	if( pGameObject->getGameObjectInfo().m_ObjectType == EOT_LIGHT_SOURCE )
    {
        SourceGameObject* pSourceGameObject = (SourceGameObject*)pGameObject;
		Vector2f v = pSourceGameObject->getPosition();//( m.col3.x, m.col3.y);
        Vector2f vout;
        
        m_pGrid->snapSource(v,vout);

        Vector2f t;
        Matrix3f xForm;
        xForm.setIdentity();
        t.x = vout.x;
        t.y = vout.y;
        xForm.translate(t.x, t.y);
        pGameObject->setPosition(t);
        
        m_vecGameObjects.push_back(pGameObject);
        m_vecSourceObjects.push_back((SourceGameObject*)pGameObject);
        m_NumSources++;
        bResult = true;
    }
	if( pGameObject->getGameObjectInfo().m_ObjectType == EOT_SHAPE_OBJECT )
	{
		Vector2f v = pGameObject->getPosition();//( m.col3.x, m.col3.y);
		Vector2f vout;
		
		// If Placement Marker available and grid is not occupied.
        //if( m_pGrid->isPlacementMarked(v.x, v.y, 0) && !m_pGrid->isOccupied(v.x, v.y, 0) )
		{
            GameObject* pPickedGameObject = pickGameObject(v);
            if( pPickedGameObject&& pPickedGameObject->getGameObjectInfo().m_ObjectType != EOT_PLACEMENT_MARKER )
            {
                assert(0);
            }
			m_vecGameObjects.push_back(pGameObject);
            m_NumShapeGameObjects++;
            
            m_pGrid->setOccupied(v.x, v.y, 0);

            m_pGrid->snap(v,vout);
            
            Vector2f t;
            Matrix3f xForm;
            xForm.setIdentity();
            t.x = vout.x;
            t.y = vout.y;
            xForm.translate(t.x, t.y);
            pGameObject->setPosition(t);
            bResult = true;
		}
	}
    if( pGameObject->getGameObjectInfo().m_ObjectType == EOT_SINK )
	{
		SinkGameObject* pSinkGameObject = (SinkGameObject*)pGameObject;
        Vector2f v = pSinkGameObject->getPosition();//( m.col3.x, m.col3.y);
        Vector2f vout;

        m_pGrid->snapSource(v,vout);
        
        Vector2f t;
        Matrix3f xForm;
        xForm.setIdentity();
        t.x = vout.x;
        t.y = vout.y;
        xForm.translate(t.x, t.y);
        pGameObject->setPosition(t);

        m_vecSinkObjects.push_back((SinkGameObject*)pGameObject);
        m_vecGameObjects.push_back(pGameObject);
        m_NumSinkGameObjects++;
        bResult = true;
	}
    if( pGameObject->getGameObjectInfo().m_ObjectType == EOT_PORTAL )
    {
        Vector2f v = pGameObject->getPosition();//( m.col3.x, m.col3.y);
		Vector2f vout;
		
		// If grid is not marked for placement.
		//if( m_pGrid->isPlacementMarked(v.x, v.y, 0) && !m_pGrid->isOccupied(v.x, v.y, 0) )
		{
            GameObject* pPickedGameObject = pickGameObject(v);
            if( pPickedGameObject->getGameObjectInfo().m_ObjectType != EOT_PLACEMENT_MARKER )
            {
                assert(0);
            }            
			m_vecGameObjects.push_back(pGameObject);
            m_vecPortalObjects.push_back((PortalGameObject*)pGameObject);
            m_NumPortalGameObjects++;
            
            m_pGrid->setOccupied(v.x, v.y, 0);
            
            m_pGrid->snap(v,vout);
            
            // Get Free Marker in the Scene
            Vector2f freePosition;
            bool bRes = false;
            
            PortalGameObject* pPortalGameObject = (PortalGameObject*)pGameObject;
            
            // Check if the Portal Shapes are coinciding( when created from editor )
            if( pPortalGameObject->getPortalShape1()->getPosition() == pPortalGameObject->getPortalShape2()->getPosition() )
            {
                bRes = getFreePlacementmarkerPosition(freePosition);
            }
            // its loaded from file
            else
            {
                Vector2f portalShape2Position = pPortalGameObject->getPortalShape2()->getPosition();
                m_pGrid->setOccupied(portalShape2Position.x, portalShape2Position.y, 0);
            }
            
            // if it was created from editor, find a free marker position
            if( bRes )
            {
                pPortalGameObject->getPortalShape2()->setPosition(freePosition);
                m_pGrid->setOccupied(freePosition.x, freePosition.y, 0);
            }
            else
            {
                bResult = false;
            }
            
            Vector2f t;
            Matrix3f xForm;
            xForm.setIdentity();
            t.x = vout.x;
            t.y = vout.y;
            xForm.translate(t.x, t.y);
            pGameObject->setPosition(t);
            bResult = true;
		}
    }
    m_currentSelectedGameObject = pGameObject;
	
	return bResult;
}


void Scene::cycleObject()
{
	if( currentSelectedObjectIndex > objects.size()-1 )
	{
		currentSelectedObjectIndex = 0;
		m_currentSelectedObject = objects[currentSelectedObjectIndex];
	}
	
	currentSelectedObjectIndex++;
	m_currentSelectedObject = objects[currentSelectedObjectIndex-1];
}

void Scene::setDepth(unsigned traceDepth)
{
	this->traceDepth = traceDepth;
}

void Scene::clearRays()
{
	vecRays.clear();
}

void Scene::DrawQuad(float w, float h, float scaleX, float scaleY)
{
    Matrix4f xForm;
    Game::getInstance()->getRenderer()->setTransform(xForm, spider3d::WORLD);
    Matrix4f orthoMatrix;
    orthoMatrix.ortho(0, m_SceneWidth, 0, m_SceneHeight, -1, 1);
    Game::getInstance()->getRenderer()->setTransform(orthoMatrix, spider3d::PROJECTION);
        
    float vertices[8];
    vertices[0] = w;
    vertices[1] = h;
    vertices[2] = 0;
    vertices[3] = h;
    vertices[4] = 0;
    vertices[5] = 0;
    vertices[6] = w;
    vertices[7] = 0;

    float texCoords[8];
    texCoords[0] = scaleX;
    texCoords[1] = scaleY;
    texCoords[2] = 0.0f;
    texCoords[3] = scaleY;
    texCoords[4] = 0.0f;
    texCoords[5] = 0.0f;
    texCoords[6] = scaleX;
    texCoords[7] = 0.0f;

    float colors[16];
    for(int i=0; i<16; i+=4 )
    {
        colors[i] = 1.0f;
        colors[i+1] = 1.0f;
        colors[i+2] = 1.0f;
        colors[i+3] = 1.0f;
    }
    
    Game::getInstance()->getRenderer()->drawVertexPrimitiveList(4, vertices, 2, colors, 4, texCoords, 2, 0, EPT_TRIANGLE_FAN);

}

void Scene::initLensShader()
{
    string defaultVertexColorShadername = "DefaultVertexColorShaderGLSL.vs";
    string defaultFragmentColorShadername = "DefaultFragmentColorShaderGLSL.fs";
    string defaultVertexShadername = "DefaultVertexShaderGLSL.vs";
    string defaultFragmentShadername = "DefaultFragmentShaderGLSL.fs";
    string blurShaderName = "BlurShaderglsl.fs";
    string lensShaderName = "LensShaderglsl.fs";
    string reflectionMapShaderName = "ReflectionMapShaderglsl.fs";
    string glowCompositeShaderName = "GlowCompositeShaderglsl.fs";
    string lineGlowShaderName = "LineGlowShaderglsl.fs";
    string waveGeneratorFragmentShaderName = "WaveGeneratorShaderglsl.fs";
    
    if( Game::getInstance()->getRenderer()->getRendererType() == spider3d::ERT_OPENGL2 )
    {
#ifdef IOS
        defaultVertexColorShadername = "DefaultVertexColorShaderGLSLes2.vs";
        defaultFragmentColorShadername = "DefaultFragmentColorShaderGLSLes2.fs";
        defaultVertexShadername = "DefaultVertexShaderGLSLes2.vs";
        blurShaderName = "BlurShaderglsles2.fs";
        lensShaderName = "LensShaderglsles2.fs";
        reflectionMapShaderName = "ReflectionMapShaderglsles2.fs";
        glowCompositeShaderName = "GlowCompositeShaderglsles2.fs";
        lineGlowShaderName = "LineGlowShaderglsles2.fs";
        defaultFragmentShadername = "DefaultFragmentShaderGLSLes2.fs";
        waveGeneratorFragmentShaderName = "WaveGeneratorShaderglsles2.fs";
#else
        defaultVertexColorShadername = "DefaultVertexColorShaderGLSL2.vs";
        defaultFragmentColorShadername = "DefaultFragmentColorShaderGLSL2.fs";
        defaultVertexShadername = "DefaultVertexShaderGLSL2.vs";
        blurShaderName = "BlurShaderglsl2.fs";
        lensShaderName = "LensShaderglsl2.fs";
        reflectionMapShaderName = "ReflectionMapShaderglsl2.fs";
        glowCompositeShaderName = "GlowCompositeShaderglsl2.fs";
        lineGlowShaderName = "LineGlowShaderglsl2.fs";
        defaultFragmentShadername = "DefaultFragmentShaderGLSL2.fs";
        waveGeneratorFragmentShaderName = "WaveGeneratorShaderglsl2.fs";
#endif
    }
    
    SourceCodeShader* shaderDefaultVS = new SourceCodeShader(EST_VERTEX, ESL_GLSL,getFullPath(defaultVertexShadername).c_str() );
	
	SourceCodeShader* shaderDefaultFS = new SourceCodeShader(EST_FRAGMENT, ESL_GLSL, getFullPath(defaultFragmentShadername).c_str() );
    
	m_pDefaultShaderProgram = Game::getInstance()->getRenderer()->createShaderProgram();
	m_pDefaultShaderProgram->setShaders(shaderDefaultVS, shaderDefaultFS);

    
    ShaderUniform* su = new ShaderUniform();
	su->m_VarType = ESVT_SINGLE_INT;
	su->m_Name = (char*)"TextureSampler";
	su->m_Value = new float;
	*su->m_Value = 0;
	m_pDefaultShaderProgram->addShaderUniform(su);
    
    SourceCodeShader* shaderDefaultColorVS = new SourceCodeShader(EST_VERTEX, ESL_GLSL,getFullPath(defaultVertexColorShadername).c_str() );
	
	SourceCodeShader* shaderDefaultColorFS = new SourceCodeShader(EST_FRAGMENT, ESL_GLSL, getFullPath(defaultFragmentColorShadername).c_str() );
    
	m_pDefaultColorShaderProgram = Game::getInstance()->getRenderer()->createShaderProgram();
	m_pDefaultColorShaderProgram->setShaders(shaderDefaultColorVS, shaderDefaultColorFS);
    
    SourceCodeShader* shaderVS = new SourceCodeShader(EST_VERTEX, ESL_GLSL,getFullPath(defaultVertexShadername).c_str() );
	
	SourceCodeShader* shaderFS = new SourceCodeShader(EST_FRAGMENT, ESL_GLSL, getFullPath(glowCompositeShaderName).c_str() );
    
	m_RenderProgram = Game::getInstance()->getRenderer()->createShaderProgram();
	m_RenderProgram->setShaders(shaderVS, shaderFS);
    
    ShaderUniform* su0 = new ShaderUniform();
	su0->m_VarType = ESVT_SINGLE_INT;
	su0->m_Name = (char*)"TextureSampler";
	su0->m_Value = new float;
	*su0->m_Value = 0;
	m_RenderProgram->addShaderUniform(su0);
    
    ShaderUniform* suGMap = new ShaderUniform();
	suGMap->m_VarType = ESVT_SINGLE_INT;
	suGMap->m_Name = (char*)"GlowSampler";
	suGMap->m_Value = new float;
	*suGMap->m_Value = 1;
	m_RenderProgram->addShaderUniform(suGMap);
    
    ShaderUniform* suSmokeMap = new ShaderUniform();
	suSmokeMap->m_VarType = ESVT_SINGLE_INT;
	suSmokeMap->m_Name = (char*)"SmokeSampler";
	suSmokeMap->m_Value = new float;
	*suSmokeMap->m_Value = 2;
	m_RenderProgram->addShaderUniform(suSmokeMap);
    
    ShaderUniform* suTexCoordSampler = new ShaderUniform();
	suTexCoordSampler->m_VarType = ESVT_SINGLE_INT;
	suTexCoordSampler->m_Name = (char*)"TexCoordSampler";
	suTexCoordSampler->m_Value = new float;
	*suTexCoordSampler->m_Value = 3;
	m_RenderProgram->addShaderUniform(suTexCoordSampler);
    
    
    ShaderUniform* suAvgLight = new ShaderUniform();
	suAvgLight->m_VarType = ESVT_SINGLE_FLOAT;
	suAvgLight->m_Name = (char*)"AvgLighting";
	suAvgLight->m_Value = new float;
	*suAvgLight->m_Value = 1;
	m_RenderProgram->addShaderUniform(suAvgLight);
    
    ShaderUniform* suTime = new ShaderUniform();
	suTime->m_VarType = ESVT_SINGLE_FLOAT;
	suTime->m_Name = (char*)"time";
	suTime->m_Value = new float;
	*suTime->m_Value = 0.0f;
	m_RenderProgram->addShaderUniform(suTime);
    
    ShaderUniform* suWaveCentre = new ShaderUniform();
	suWaveCentre->m_VarType = ESVT_VEC2;
	suWaveCentre->m_Name = (char*)"waveCentre";
	suWaveCentre->m_Value = new float[2];
	suWaveCentre->m_Value[0] = 0.5f;
	suWaveCentre->m_Value[1] = 0.5f;
	m_RenderProgram->addShaderUniform(suWaveCentre);
    
	SourceCodeShader* shaderLensVS = new SourceCodeShader(EST_VERTEX, ESL_GLSL,getFullPath(defaultVertexShadername).c_str() );
    
    SourceCodeShader* shaderLensFS = new SourceCodeShader(EST_FRAGMENT, ESL_GLSL, getFullPath(lensShaderName).c_str() );

	m_LensRenderProgram = Game::getInstance()->getRenderer()->createShaderProgram();
	m_LensRenderProgram->setShaders(shaderLensVS, shaderLensFS);
    
    ShaderUniform* su1 = new ShaderUniform();
	su1->m_VarType = ESVT_SINGLE_INT;
	su1->m_Name = (char*)"TextureSampler";
	su1->m_Value = new float;
	*su1->m_Value = 0;
	m_LensRenderProgram->addShaderUniform(su1);
    
    ShaderUniform* suWidth = new ShaderUniform();
	suWidth->m_VarType = ESVT_SINGLE_FLOAT;
	suWidth->m_Name = (char*)"w";
	suWidth->m_Value = new float;
    
	*suWidth->m_Value = m_ScreenScaleFactor*m_SceneWidth;
    
	m_LensRenderProgram->addShaderUniform(suWidth);
    
    ShaderUniform* suHeight = new ShaderUniform();
	suHeight->m_VarType = ESVT_SINGLE_FLOAT;
	suHeight->m_Name = (char*)"h";
	suHeight->m_Value = new float;

	*suHeight->m_Value = m_ScreenScaleFactor*m_SceneHeight;

    
    m_LensRenderProgram->addShaderUniform(suHeight);
    
    ShaderUniform* suNMap = new ShaderUniform();
	suNMap->m_VarType = ESVT_SINGLE_INT;
	suNMap->m_Name = (char*)"NormalMap";
	suNMap->m_Value = new float;
	*suNMap->m_Value = 1;
	m_LensRenderProgram->addShaderUniform(suNMap);
    
    ShaderUniform* suRMap = new ShaderUniform();
	suRMap->m_VarType = ESVT_SINGLE_INT;
	suRMap->m_Name = (char*)"ReflectionMap";
	suRMap->m_Value = new float;
	*suRMap->m_Value = 2;
	m_LensRenderProgram->addShaderUniform(suRMap);
    
    ShaderUniform* suSEMap = new ShaderUniform();
	suSEMap->m_VarType = ESVT_SINGLE_INT;
	suSEMap->m_Name = (char*)"SuperEllipseMap";
	suSEMap->m_Value = new float;
	*suSEMap->m_Value = 3;
	m_LensRenderProgram->addShaderUniform(suSEMap);
    
    ShaderUniform* su2 = new ShaderUniform();
	su2->m_VarType = ESVT_SINGLE_FLOAT;
	su2->m_Name = (char*)"CentreX";
	su2->m_Value = new float;
	*su2->m_Value = 0.0f;
	m_LensRenderProgram->addShaderUniform(su2);
    
    ShaderUniform* su3 = new ShaderUniform();
	su3->m_VarType = ESVT_SINGLE_FLOAT;
	su3->m_Name = (char*)"CentreY";
	su3->m_Value = new float;
	*su3->m_Value = 0.0f;
	m_LensRenderProgram->addShaderUniform(su3);
    
    {
        SourceCodeShader* shaderReflectionVS = new SourceCodeShader(EST_VERTEX, ESL_GLSL,getFullPath(defaultVertexShadername).c_str() );

        SourceCodeShader* shaderReflectionFS = new SourceCodeShader(EST_FRAGMENT, ESL_GLSL, getFullPath(reflectionMapShaderName).c_str() );
        
        
        m_ReflectionMapProgram = Game::getInstance()->getRenderer()->createShaderProgram();
        m_ReflectionMapProgram->setShaders(shaderReflectionVS, shaderReflectionFS);
        
        ShaderUniform* su1 = new ShaderUniform();
        su1->m_VarType = ESVT_SINGLE_INT;
        su1->m_Name = (char*)"TextureSampler";
        su1->m_Value = new float;
        *su1->m_Value = 0;
        m_ReflectionMapProgram->addShaderUniform(su1);
        
        ShaderUniform* suWidth = new ShaderUniform();
        suWidth->m_VarType = ESVT_SINGLE_FLOAT;
        suWidth->m_Name = (char*)"w";
        suWidth->m_Value = new float;
        *suWidth->m_Value = m_SceneWidth;
        m_ReflectionMapProgram->addShaderUniform(suWidth);
        
        ShaderUniform* suHeight = new ShaderUniform();
        suHeight->m_VarType = ESVT_SINGLE_FLOAT;
        suHeight->m_Name = (char*)"h";
        suHeight->m_Value = new float;
        *suHeight->m_Value = m_SceneHeight;
        m_ReflectionMapProgram->addShaderUniform(suHeight);
    }
    
    SourceCodeShader* shaderBlurVS = new SourceCodeShader(EST_VERTEX, ESL_GLSL,getFullPath(defaultVertexShadername).c_str() );
	
    SourceCodeShader* shaderBlurFS = new SourceCodeShader(EST_FRAGMENT, ESL_GLSL, getFullPath(blurShaderName).c_str() );

	m_BlurProgram = Game::getInstance()->getRenderer()->createShaderProgram();
	m_BlurProgram->setShaders(shaderBlurVS, shaderBlurFS);
    
	ShaderUniform* su_0 = new ShaderUniform();
	su_0->m_VarType = ESVT_SINGLE_INT;
	su_0->m_Name = (char*)"TextureSampler";
	su_0->m_Value = new float;
	*su_0->m_Value = 0;
	m_BlurProgram->addShaderUniform(su_0);
    
    ShaderUniform* su_1 = new ShaderUniform();
	su_1->m_VarType = ESVT_SINGLE_FLOAT;
	su_1->m_Name = (char*)"nx";
	su_1->m_Value = new float;
	//*su1->m_Value = 512.0/LEVEL;
	m_BlurProgram->addShaderUniform(su_1);
    
    ShaderUniform* su_2 = new ShaderUniform();
	su_2->m_VarType = ESVT_SINGLE_FLOAT;
	su_2->m_Name = (char*)"ny";
	su_2->m_Value = new float;
	//*su1->m_Value = 512.0/LEVEL;
	m_BlurProgram->addShaderUniform(su_2);
    
    {
    
    SourceCodeShader* shaderLineGlowVS = new SourceCodeShader(EST_VERTEX, ESL_GLSL,getFullPath(defaultVertexShadername).c_str() );
	
    SourceCodeShader* shaderLineGlowFS = new SourceCodeShader(EST_FRAGMENT, ESL_GLSL, getFullPath(lineGlowShaderName).c_str() );

	m_LineGlowProgram = Game::getInstance()->getRenderer()->createShaderProgram();
	m_LineGlowProgram->setShaders(shaderLineGlowVS, shaderLineGlowFS);
        
	ShaderUniform* su0 = new ShaderUniform();
	su0->m_VarType = ESVT_SINGLE_INT;
	su0->m_Name = (char*)"TextureSampler1";
	su0->m_Value = new float;
	*su0->m_Value = 0;
	m_LineGlowProgram->addShaderUniform(su0);
    
    ShaderUniform* su1 = new ShaderUniform();
	su1->m_VarType = ESVT_SINGLE_INT;
	su1->m_Name = (char*)"TextureSampler2";
	su1->m_Value = new float;
	*su1->m_Value = 1;
	m_LineGlowProgram->addShaderUniform(su1);
    
    ShaderUniform* su2 = new ShaderUniform();
	su2->m_VarType = ESVT_SINGLE_INT;
	su2->m_Name = (char*)"TextureSampler3";
	su2->m_Value = new float;
	*su2->m_Value = 2;
	m_LineGlowProgram->addShaderUniform(su2);
    
    ShaderUniform* su3 = new ShaderUniform();
	su3->m_VarType = ESVT_SINGLE_INT;
	su3->m_Name = (char*)"TextureSampler4";
	su3->m_Value = new float;
	*su3->m_Value = 3;
	m_LineGlowProgram->addShaderUniform(su3);
    
    ShaderUniform* su4 = new ShaderUniform();
	su4->m_VarType = ESVT_SINGLE_INT;
	su4->m_Name = (char*)"TextureSampler5";
	su4->m_Value = new float;
	*su4->m_Value = 4;
	m_LineGlowProgram->addShaderUniform(su4);
    
    ShaderUniform* su5 = new ShaderUniform();
	su5->m_VarType = ESVT_SINGLE_INT;
	su5->m_Name = (char*)"TextureSampler6";
	su5->m_Value = new float;
	*su5->m_Value = 5;
	m_LineGlowProgram->addShaderUniform(su5);
    
    ShaderUniform* su6 = new ShaderUniform();
	su6->m_VarType = ESVT_SINGLE_INT;
	su6->m_Name = (char*)"TextureSampler7";
	su6->m_Value = new float;
	*su6->m_Value = 6;
	m_LineGlowProgram->addShaderUniform(su6);
    
    ShaderUniform* su7 = new ShaderUniform();
	su7->m_VarType = ESVT_SINGLE_INT;
	su7->m_Name = (char*)"TextureSampler8";
	su7->m_Value = new float;
	*su7->m_Value = 7;
	m_LineGlowProgram->addShaderUniform(su7);
    }
    
    {
        SourceCodeShader* shaderLineGlowVS = new SourceCodeShader(EST_VERTEX, ESL_GLSL,getFullPath(defaultVertexShadername).c_str() );
        
        SourceCodeShader* shaderLineGlowFS = new SourceCodeShader(EST_FRAGMENT, ESL_GLSL, getFullPath(waveGeneratorFragmentShaderName).c_str() );
        
        m_pWaveGeneratorProgram = Game::getInstance()->getRenderer()->createShaderProgram();
        m_pWaveGeneratorProgram->setShaders(shaderLineGlowVS, shaderLineGlowFS);
        
        ShaderUniform* suTime = new ShaderUniform();
        suTime->m_VarType = ESVT_SINGLE_FLOAT;
        suTime->m_Name = (char*)"time";
        suTime->m_Value = new float;
        *suTime->m_Value = 0.0f;
        m_pWaveGeneratorProgram->addShaderUniform(suTime);
        
        ShaderUniform* suWaveCentre = new ShaderUniform();
        suWaveCentre->m_VarType = ESVT_VEC2;
        suWaveCentre->m_Name = (char*)"waveCentre";
        suWaveCentre->m_Value = new float[2];
        suWaveCentre->m_Value[0] = 0.5f;
        suWaveCentre->m_Value[1] = 0.5f;
        m_pWaveGeneratorProgram->addShaderUniform(suWaveCentre);
    }
}

#define BLUR_PASSES 6

void Scene::blurTexture(int texID, float nx, float ny)
{
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //return;
    glViewport(0, 0, nx, ny);
    ShaderUniform* suNx = m_BlurProgram->getShaderUniformByName("nx");
	if( suNx )
    {
		*suNx->m_Value = nx;
    }
    ShaderUniform* suNy = m_BlurProgram->getShaderUniformByName("ny");
	if( suNy )
    {
		*suNy->m_Value = ny;
    }
    m_BlurProgram->OnSet();
	//glActiveTexture(GL_TEXTURE0);
	//glEnable(GL_TEXTURE_2D);
	for( int i=0; i<BLUR_PASSES; i++ )
	{
        Game::getInstance()->getRenderer()->setTextureObject(m_RenderTextureGlowPrevious[texID]);
		//glBindTexture(GL_TEXTURE_2D, *m_RenderTextureGlowPrevious[texID]->getTextureName());
		m_Fbo->attachTexture(m_RenderTextureGlowCurrent[texID]);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0, 0, 0, 0);
		DrawQuad(m_SceneWidth,m_SceneHeight);
        
		SWAP_TEXTURES(m_RenderTextureGlowPrevious[texID], m_RenderTextureGlowCurrent[texID]);
	}
	m_BlurProgram->UnSet();
    //glViewport(0, 0, m_TextureSizeX, m_TextureSizeY);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Scene::blurTexture(Texture2D* pTexture, Texture2D* pTempTexture, int blurPasses)
{
    int resolutionX = pTexture->getWidth();
    int resolutionY = pTexture->getHeight();
    
    glViewport(0,0,resolutionX,resolutionY);
    
    ShaderUniform* suNx = m_BlurProgram->getShaderUniformByName("nx");
    if( suNx )
    {
        *suNx->m_Value = resolutionX;
    }
    ShaderUniform* suNy = m_BlurProgram->getShaderUniformByName("ny");
    if( suNy )
    {
        *suNy->m_Value = resolutionY;
    }
    m_BlurProgram->OnSet();
    
	for( int i=0; i<blurPasses; i++ )
	{
        Game::getInstance()->getRenderer()->setTextureObject(pTexture);
		m_Fbo->attachTexture(pTempTexture);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0, 0, 0, 0);
        
		DrawQuad(m_SceneWidth,m_SceneHeight);

		SWAP_TEXTURES(pTexture, pTempTexture);
	}
    //Game::getInstance()->getRenderer()->setTextureObject(0);
    
    m_BlurProgram->UnSet();
}

void Scene::glowPass()
{
    glViewport(0, 0, m_TextureSizeX/2, m_TextureSizeY/2);
    
    // 2. Glow Pass
    m_Fbo->attachTexture(m_LineGlowTexture);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);
    E_TEXTURE_UNIT_STATE texUnit = ETU_0;
    for( int i=0; i<NUM_TEXTURES; i++ )
    {
        Game::getInstance()->getRenderer()->setTextureObject(m_RenderTextureGlowCurrent[i], texUnit);
        texUnit = E_TEXTURE_UNIT_STATE(texUnit + 1);
    }
    m_LineGlowProgram->OnSet();
    DrawQuad(m_SceneWidth,m_SceneHeight);
    texUnit = ETU_0;
    for( int i=0; i<NUM_TEXTURES; i++ )
    {
        Game::getInstance()->getRenderer()->setTextureObject(0, texUnit);
        texUnit = E_TEXTURE_UNIT_STATE(texUnit + 1);
    }
    m_LineGlowProgram->UnSet();
}

bool TransparencySortPredicate( GameObject* pObject1,  GameObject* pObject2)
{
    float d1 = pObject1->getZOrder();
    float d2 = pObject2->getZOrder();
    return d1 < d2;
}

void Scene::DrawQuadWithTexture( Texture* pTexture )
{    
    m_Fbo->unSet();
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT );
    {
        glViewport(0, 0, m_SceneWidth, m_SceneHeight);

        Game::getInstance()->getRenderer()->setTextureObject(pTexture);

        m_pDefaultShaderProgram->OnSet();

        DrawQuad(m_SceneWidth, m_SceneHeight);
    }
}

void Scene::renderGlowingRays()
{
    m_pDefaultShaderProgram->OnSet();
    
    Matrix4f orthoMatrix;
    orthoMatrix.ortho(0, m_SceneWidth, 0, m_SceneHeight, -1, 1);
    Game::getInstance()->getRenderer()->setTransform(orthoMatrix, spider3d::PROJECTION);
    
    Matrix4f xForm;
    Game::getInstance()->getRenderer()->setTransform(xForm, spider3d::WORLD);
    
    // Render All the rays for glow in FBO
    int width = m_TextureSizeX/2;
    int height = m_TextureSizeY/2;
    
    m_Fbo->onSet();
    
    // Draw all the rays in the texture
    glViewport(0, 0, width, height);
    m_Fbo->attachTexture(m_RenderTextureGlowCurrent[0]);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    renderAllRays(50.0f);
    
    width /= 2;
    height /= 2;
    
    // Draw smaller textures for glow pass
    for( int i=1; i<NUM_TEXTURES; i++ )
    {
        glViewport(0, 0, width, height);
        m_Fbo->attachTexture(m_RenderTextureGlowCurrent[i]);
        Game::getInstance()->getRenderer()->setTextureObject(m_RenderTextureGlowCurrent[i-1]);
        //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        DrawQuad(m_SceneWidth, m_SceneHeight);
        
        width /= 2;
        height /= 2;
    }
    
    m_pDefaultShaderProgram->UnSet();
    
    for( int i=0; i<NUM_TEXTURES; i++ )
    {
        SWAP_TEXTURES(m_RenderTextureGlowPrevious[i], m_RenderTextureGlowCurrent[i]);
    }
}

void Scene::blurTexturePass()
{
    // Blur all Textures
    for( int i=0; i<NUM_TEXTURES; i++ )
    {
        // Bigger Textures require less no of blur passes.
        int passes[] = {0,0,2,4,4,4};
        blurTexture(m_RenderTextureGlowPrevious[i],m_RenderTextureGlowCurrent[i], passes[i]);
    }
}

void Scene::reflectionDrawPass()
{
    // Draw objects in reflection map with appropriate scaling
    glViewport(0, 0, m_TextureSizeX/m_ReflectionTextureScale, m_TextureSizeY/m_ReflectionTextureScale);
    m_Fbo->attachTexture(m_ReflectionTexture);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    m_pDefaultShaderProgram->OnSet();
    
    renderAllRays(20.0);
    
    m_pDefaultShaderProgram->UnSet();
    
    m_ReflectionMapProgram->OnSet();
    
    Matrix4f orthoMatrix;
    orthoMatrix.ortho(0, m_SceneWidth, 0, m_SceneHeight, -1, 1);
    Game::getInstance()->getRenderer()->setTransform(orthoMatrix, spider3d::PROJECTION);
    
    for( int i=0; i<m_vecGameObjects.size(); i++ )
    {
        if( m_vecGameObjects[i]->getGameObjectInfo().m_ObjectType != EOT_PLACEMENT_MARKER )
        {
            Texture* pTexture = m_vecGameObjects[i]->getTexture();
            if( pTexture )
            {
                Game::getInstance()->getRenderer()->setTextureObject(pTexture);
            }
            if( m_vecGameObjects[i]->getGameObjectInfo().m_ObjectType != EOT_PLACEMENT_MARKER)
            m_vecGameObjects[i]->render();
        }
    }
    
    Game::getInstance()->getRenderer()->setTextureObject(0);
    
    m_ReflectionMapProgram->UnSet();
}

void Scene::drawBackgroundAndMarkersPass()
{
    glViewport(0, 0, m_TextureSizeX, m_TextureSizeY);
    
    m_Fbo->attachTexture(m_RenderTextureCurrent);
    
    m_pDefaultShaderProgram->OnSet();
    
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);
    //glClearColor(0, 0, 0, 0);
    
    // Draw Into FBO the background.
//#ifdef IOS
//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D, textureID);
//#else
    Game::getInstance()->getRenderer()->setTextureObject(m_BackGroundTexture);
//#endif
    DrawQuad(m_SceneWidth, m_SceneHeight,1.0f,1.0f/1.5);
    //glDisable(GL_TEXTURE_2D);
    Game::getInstance()->getRenderer()->setTextureObject(0);
    
    if( m_bEnableDebugGrid )
    m_pGrid->render();
    
    Matrix4f orthoMatrix;
    orthoMatrix.ortho(0, m_SceneWidth, 0, m_SceneHeight, -1, 1);
    Game::getInstance()->getRenderer()->setTransform(orthoMatrix, spider3d::PROJECTION);
    
    for( int i=0; i<m_vecPlacementMarkerGameObjects.size(); i++ )
    {
        m_vecPlacementMarkerGameObjects[i]->render();
    }
    
    
    // Draw Into the FBO all the rays
	renderAllRays(10.0);
    
    m_pDefaultShaderProgram->UnSet();
}

void Scene::waterRipplePass()
{
    // Back to normal backbuffer
    float dt = 0.005f;
    
    Time += dt;
    if( Time > 2.0f * M_PI )
    Time = 2.0f * M_PI - Time;
    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    //         WATER SHADER PASS
    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    
    ShaderUniform* suTime = m_pWaveGeneratorProgram->getShaderUniformByName("time");
    if( suTime )
    {
        *suTime->m_Value = Time;
    }
    
    m_pWaveGeneratorProgram->OnSet();
    m_Fbo->attachTexture(m_pFloatTextureWaveGenerator);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    DrawQuad(m_pFloatTextureWaveGenerator->getWidth(), m_pFloatTextureWaveGenerator->getHeight());
    
    m_pWaveGeneratorProgram->UnSet();
}

void Scene::finalCompositePass()
{
    glViewport(0, 0, m_SceneWidth, m_SceneHeight);
    
    m_Fbo->attachTexture(m_FinalCompositeTexture);
    
    ShaderUniform* suAvgLight = m_RenderProgram->getShaderUniformByName("AvgLighting");
    if( suAvgLight )
    {
        *suAvgLight->m_Value = m_AverageLightingFactor;
    }
    
    m_RenderProgram->OnSet();
    
    Game::getInstance()->getRenderer()->setTextureObject(m_RenderTextureCurrent, ETU_0);
    Game::getInstance()->getRenderer()->setTextureObject(m_LineGlowTexture, ETU_1);
    Game::getInstance()->getRenderer()->setTextureObject(m_ReflectionTexture, ETU_2);
    Game::getInstance()->getRenderer()->setTextureObject(m_pFloatTextureWaveGenerator, ETU_3);
    
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    
    DrawQuad(m_SceneWidth,m_SceneHeight);
    
    Game::getInstance()->getRenderer()->setTextureObject(0, ETU_0);
    Game::getInstance()->getRenderer()->setTextureObject(0, ETU_1);
    Game::getInstance()->getRenderer()->setTextureObject(0, ETU_2);
    Game::getInstance()->getRenderer()->setTextureObject(0, ETU_3);
    
    m_RenderProgram->UnSet();
    
    m_Fbo->unSet();
}

void Scene::drawObjectsPass()
{
    Game::getInstance()->getRenderer()->setBlendingFactors(spider3d::EBF_SRC_ALPHA, spider3d::EBF_ONE_MINUS_SRC_ALPHA);

    Matrix4f orthoMatrix;
    orthoMatrix.ortho(0, m_SceneWidth, 0, m_SceneHeight, -1, 1);
    
    // DRAW THE OBJECTS
    m_LensRenderProgram->OnSet();
    
    Game::getInstance()->getRenderer()->setTextureObject(m_FinalCompositeTexture, ETU_0);
    
    Game::getInstance()->getRenderer()->setTextureObject(m_RenderTextureGlowPrevious[3], ETU_2);
    
    for( int i=0; i<m_vecGameObjects.size(); i++ )
    {
        m_LensRenderProgram->OnSet();
        
        if(m_vecGameObjects[i]->getGameObjectInfo().m_ObjectType != EOT_PLACEMENT_MARKER &&
           m_vecGameObjects[i]->getGameObjectInfo().m_ObjectType != EOT_SINK )
        {
            Texture* pTexture = m_vecGameObjects[i]->getTexture();
            
            if( pTexture )
            {
                Game::getInstance()->getRenderer()->setTextureObject(pTexture, ETU_3);
            }
            Texture* pNormalMapTexture = m_vecGameObjects[i]->getNormalMapTexture();
            if( pNormalMapTexture )
            {
                Game::getInstance()->getRenderer()->setTextureObject(pNormalMapTexture, ETU_1);
            }
            
            Game::getInstance()->getRenderer()->setTransform(orthoMatrix, spider3d::PROJECTION);
            
            m_vecGameObjects[i]->render();
        }
    }
    
    m_LensRenderProgram->UnSet();
    
    Game::getInstance()->getRenderer()->setTextureObject(0, ETU_0);
    Game::getInstance()->getRenderer()->setTextureObject(0, ETU_1);
    Game::getInstance()->getRenderer()->setTextureObject(0, ETU_2);
    Game::getInstance()->getRenderer()->setTextureObject(0, ETU_3);
    
    
    m_pDefaultShaderProgram->OnSet();
    
    Game::getInstance()->getRenderer()->setTransform(orthoMatrix, spider3d::PROJECTION);
    
    Game::getInstance()->getRenderer()->setBlendingFactors(spider3d::EBF_SRC_ALPHA, spider3d::EBF_ONE);

    
    // Render Dragged Object on top
    if( m_pDraggedObject )
    {
        GAME_OBJECT_TYPE objectType = m_pDraggedObject->getGameObjectInfo().m_ObjectType;
        
        if( objectType == EOT_SHAPE_OBJECT )
        {
            Texture* pTexture = m_pDraggedObject->getTexture();
            if( pTexture )
            {
                Game::getInstance()->getRenderer()->setTextureObject(pTexture);
            }
            m_pDraggedObject->render();
        }
        else if( objectType == EOT_PORTAL )
        {
            Texture* pTexture = m_pDraggedObject->getTexture();
            if( pTexture )
            {
                Game::getInstance()->getRenderer()->setTextureObject(pTexture);
            }
            PortalGameObject* pPortalGameObject = (PortalGameObject*)m_pDraggedObject;
            pPortalGameObject->getSelectedShape()->render();
        }
        else if ( objectType == EOT_SINK )
        {
            SinkGameObject* pSinkGameObject = (SinkGameObject*)m_pDraggedObject;
            pSinkGameObject->rayTraceForSinkGlow();
            pSinkGameObject->renderGlowRays(20.0f);
        }
        else if ( objectType == EOT_LIGHT_SOURCE )
        {
            SourceGameObject* pSourceGameObject = (SourceGameObject*)m_pDraggedObject;
            pSourceGameObject->rayTraceGlowRays();
            pSourceGameObject->renderGlowRays(20.0f);
        }
    }
    Game::getInstance()->getRenderer()->setTextureObject(0);
    
}

void Scene::renderGuiScene()
{
    return;
    
    Game::getInstance()->getRenderer()->setBlendingEnable(true);
    Game::getInstance()->getRenderer()->setBlendingFactors(spider3d::EBF_SRC_ALPHA, spider3d::EBF_ONE);

    glViewport(0, 0, m_pGuiBackGroundTexture->getWidth(), m_pGuiBackGroundTexture->getHeight() );
    
    m_Fbo->onSet();
    m_Fbo->attachTexture(m_pGuiBackGroundTexture);
    m_pDefaultShaderProgram->OnSet();

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
   
    int rayCount = 0;
    
    if( rayCount > 0 )
    {
        Matrix4f xForm;
        Game::getInstance()->getRenderer()->setTransform(xForm, spider3d::WORLD);
        Matrix4f orthoMatrix;
        orthoMatrix.ortho(0, m_SceneWidth, 0, m_SceneHeight, -1, 1);
        Game::getInstance()->getRenderer()->setTransform(orthoMatrix, spider3d::PROJECTION);
        
        Game::getInstance()->getRenderer()->setBlendingFactors(spider3d::EBF_SRC_ALPHA, spider3d::EBF_ONE);
        Game::getInstance()->getRenderer()->setTextureObject(Ray::m_pTexture);

        
        Game::getInstance()->getRenderer()->drawVertexPrimitiveList(6*rayCount, m_pAllRayVertices, 2, m_pAllRayColors, 4, m_pAllRayTexCoords, 2, 0, EPT_TRIANGLE);
    }
    m_pDefaultShaderProgram->UnSet();
    
    // Blur the Texture
    blurTexture(m_pGuiBackGroundTexture, m_pGuiBackGroundTempTexture, 6);
    
    
    m_Fbo->unSet();

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    m_pDefaultShaderProgram->OnSet();
    // Draw the Blurred Texture
    glViewport(0, 0, m_SceneWidth, m_SceneHeight);
    Matrix4f orthoMatrix;
    orthoMatrix.ortho(0, m_SceneWidth, 0, m_SceneHeight, -1, 1);
    Game::getInstance()->getRenderer()->setTransform(orthoMatrix, spider3d::PROJECTION);
    
    Game::getInstance()->getRenderer()->setTextureObject(m_pGuiBackGroundTexture);
    DrawQuad(m_SceneWidth, m_SceneHeight);
    Game::getInstance()->getRenderer()->setTextureObject(NULL);
    
    m_pDefaultShaderProgram->UnSet();
    
}

void Scene::renderParticles()
{
    
}

void Scene::cullRays()
{
    return;
    
    int size = vecRays.size();
    for( int i=0; i<size; i++ )
    {
        Ray& ray = vecRays[i];
        float rayLenSq = ray.origin.distanceSquared(ray.endPoint);
        if( rayLenSq < 3.0f )
        {
            vecRays.erase(vecRays.begin()+i);
        }
    }
}

void Scene::renderScene()
{
    //glViewport(0, 0, m_SceneWidth, m_SceneHeight);
    
    Game::getInstance()->getRenderer()->setBlendingEnable(true);
    Game::getInstance()->getRenderer()->setBlendingFactors(spider3d::EBF_SRC_ALPHA, spider3d::EBF_ONE);

    // sort all the GameObjects for rendering
    std::sort(m_vecGameObjects.begin(), m_vecGameObjects.end(), TransparencySortPredicate);
    
    cullRays();
    // PASS 1 :: Render Glowing Objects, and draw into smaller Textures for glow pass
    renderGlowingRays();

    // PASS 2  :: Blur All Textures before the glow pass
    blurTexturePass();

    // PASS 3  :: Make Glow Texture
    glowPass();
    
    // PASS 4  :: Draw objects in the reflection map
    reflectionDrawPass();

    // PASS 5  :: Draw Background and Placement markers in this pass
    drawBackgroundAndMarkersPass();
    
    // PASS 6  :: Create Ripple effect in a much smaller texture for better performance
    waterRipplePass();
    
    // PASS 7  :: Final composite pass
    finalCompositePass();
    
    // PASS 8  :: Draw the quad with final composite Texture
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT );
    
    Game::getInstance()->getRenderer()->setTextureObject(m_FinalCompositeTexture);

    m_pDefaultShaderProgram->OnSet();

    glViewport(0, 0, m_SceneWidth*m_ScreenScaleFactor, m_SceneHeight*m_ScreenScaleFactor);
    
    DrawQuad(m_SceneWidth,m_SceneHeight);
   
    Game::getInstance()->getRenderer()->setTextureObject(0);
    
    renderAllRays(10.0, Color4f(0.6f,0.6f,0.6f,1.0f));

    m_pDefaultShaderProgram->UnSet();

    // PASS 9  :: Draw Objects on top
    drawObjectsPass();
    
    if( m_bReadyForScreenShot )
    {
        string path = "/Users/prashantsharma/Downloads/Backup_MacBookPro/Optiks/";
        
        Image* pImage = new Image(1024, 732, EIF_RGBA);
        takeScreenShot(pImage->getData());
        Image* pScaledImage = pImage->getScaledImage(1024/5, 732/5);
        delete pImage;
        string saveImageName = path  + "ScreenShot.png";
        
        pScaledImage->save(saveImageName.c_str());
        delete pScaledImage;
        m_bReadyForScreenShot = false;
    }
    //blurTexture(m_pFinalTexture, m_pFinalTempTexture, 3);
    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    //         PASS 7 === DRAW THE FBO TEXTURE
    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    m_Fbo->unSet();
    
//    m_pDefaultShaderProgram->OnSet();
//    glClearColor(0, 0, 0, 0);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    
//    Game::getInstance()->getRenderer()->setTextureObject(m_pFinalTexture);
//    
//    DrawQuad(m_SceneWidth,m_SceneHeight);
//
//    Game::getInstance()->getRenderer()->setTextureObject(0);
//    
//    m_pDefaultShaderProgram->UnSet();
}

void Scene::pushRayDataToRayVertexBuffer(float rayWidth, Ray& ray, int rayCount, bool bMoveTexCoords, const Color4f& addColor)
{
    //if( rayCount == 0 )
      //  return;
    
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
    
    dx -= RAY_SPEED * dt;
    
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
    
    if( bMoveTexCoords )
    {
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
    }
    else
    {
        m_pAllRayTexCoords[12*rayCount]   = 0.0f;
        m_pAllRayTexCoords[12*rayCount+1] = 0.0f;
        m_pAllRayTexCoords[12*rayCount+2] = rayLength/250.0f;
        m_pAllRayTexCoords[12*rayCount+3] = 0.0f;
        m_pAllRayTexCoords[12*rayCount+4] = rayLength/250.0f;
        m_pAllRayTexCoords[12*rayCount+5] = 1.0f;
        m_pAllRayTexCoords[12*rayCount+6] = 0.0f;
        m_pAllRayTexCoords[12*rayCount+7] = 0.0f;
        m_pAllRayTexCoords[12*rayCount+8] = rayLength/250.0f;
        m_pAllRayTexCoords[12*rayCount+9] = 1.0f;
        m_pAllRayTexCoords[12*rayCount+10] = 0.0f;
        m_pAllRayTexCoords[12*rayCount+11] = 1.0f;
    }
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

void Scene::rasterizeVelocity()
{
    for( int i=0; i<vecRays.size(); i++ )
	{
        Ray& ray = vecRays[i];
        
		Vector2i start, end;
        
        Vector2f origin = ray.origin;
        origin.y = m_SceneHeight - origin.y;
        Vector2f endPoint = ray.endPoint;
        endPoint.y = m_SceneHeight - endPoint.y;
        
        endPoint.x = endPoint.x < 0.0f ? 0.0f : endPoint.x;
        endPoint.y = endPoint.y < 0.0f ? 0.0f : endPoint.y;
        endPoint.x = endPoint.x > m_SceneWidth ? m_SceneWidth : endPoint.x;
        endPoint.y = endPoint.y > m_SceneHeight ? m_SceneHeight : endPoint.y;

        start.x = origin.x /m_SceneWidth * 64;
        start.y = origin.y /m_SceneHeight * 64;
        end.x = endPoint.x /m_SceneWidth * 64;
        end.y = endPoint.y /m_SceneHeight * 64;
        m_pFluidSimulator->RasterizeVelocity(start, end);
	}
//    vector<Ray>& rays = m_vecSinkObjects[0]->getInnerGlowRays();
    
//    for( int i=0; i<rays.size(); i++ )
//    {
//        Ray& ray = rays[i];
//        
//		Vector2i start, end;
//        
//        Vector2f origin = ray.origin;
//        origin.y = m_SceneHeight - origin.y;
//        Vector2f endPoint = ray.endPoint;
//        endPoint.y = m_SceneHeight - endPoint.y;
//        
//        endPoint.x = endPoint.x < 0.0f ? 0.0f : endPoint.x;
//        endPoint.y = endPoint.y < 0.0f ? 0.0f : endPoint.y;
//        endPoint.x = endPoint.x > m_SceneWidth ? m_SceneWidth : endPoint.x;
//        endPoint.y = endPoint.y > m_SceneHeight ? m_SceneHeight : endPoint.y;
//        
//        start.x = origin.x /m_SceneWidth * 64;
//        start.y = origin.y /m_SceneHeight * 64;
//        end.x = endPoint.x /m_SceneWidth * 64;
//        end.y = endPoint.y /m_SceneHeight * 64;
//        m_pFluidSimulator->RasterizeVelocity(start, end);
//    }
}

void Scene::renderAllRays(float rayWidth, const Color4f& addColor)
{
    int rayCount = 0;
	for( int i=0; i<vecRays.size(); i++ )
	{
        pushRayDataToRayVertexBuffer(rayWidth, vecRays[i], rayCount, true, addColor);
        rayCount++;
	}
    for( int i=0; i<m_vecSinkObjects.size(); i++ )
    {
        m_vecSinkObjects[i]->renderGlowRays(rayWidth);
        m_vecSinkObjects[i]->render();
    }
    for( int i=0; i<m_vecSinkObjects.size(); i++ )
    {
        vector<Ray>& rays = m_vecSinkObjects[i]->getInnerGlowRays();
        float modifier = m_vecSinkObjects[i]->getCurrentRayWidthMultFactor();
        for( int j=0; j<rays.size(); j++ )
        {
            pushRayDataToRayVertexBuffer(rayWidth*modifier, rays[j], rayCount, true, addColor);
            rayCount++;
        }
    }
    
    for( int i=0; i<m_vecSourceObjects.size(); i++ )
    {
        vector<Ray>& rays = m_vecSourceObjects[i]->getInnerGlowRays();
        for( int j=0; j<rays.size(); j++ )
        {
            pushRayDataToRayVertexBuffer(rayWidth, rays[j], rayCount, true, addColor);
            rayCount++;
        }
    }
    
    for( int i=0; i<m_vecPortalObjects.size(); i++ )
    {
        vector<Ray>& rays = m_vecPortalObjects[i]->getInnerGlowRays();
        for( int j=0; j<rays.size(); j++ )
        {
            pushRayDataToRayVertexBuffer(rayWidth/1.5f, rays[j], rayCount, true, addColor);
            rayCount++;
        }
    }
    
    for( int i=0; i<m_vecGlowing.size(); i++ )
    {
        vector<Ray>& rays = m_vecGlowing[i]->getGlowingRays();
        for( int j=0; j<rays.size(); j++ )
        {
            pushRayDataToRayVertexBuffer(rayWidth/0.8f, rays[j], rayCount, false, addColor);
            rayCount++;
        }
    }

    Matrix4f xForm;
    Game::getInstance()->getRenderer()->setTransform(xForm, spider3d::WORLD);
    Matrix4f orthoMatrix;
    orthoMatrix.ortho(0, m_SceneWidth, 0, m_SceneHeight, -1, 1);
    Game::getInstance()->getRenderer()->setTransform(orthoMatrix, spider3d::PROJECTION);
    
    Game::getInstance()->getRenderer()->setTextureObject(Ray::m_pTexture);
    
    Game::getInstance()->getRenderer()->drawVertexPrimitiveList(6*rayCount, m_pAllRayVertices, 2, m_pAllRayColors, 4, m_pAllRayTexCoords, 2, 0, EPT_TRIANGLE);

    Game::getInstance()->getRenderer()->setTextureObject(0);
    
    m_pDefaultShaderProgram->OnSet();
}

bool Scene::getHintAt( const Vector2f& position, int& hintID )
{
    hintID = -1;
    for( int i=0; i<m_HintMap.size(); i++ )
    {
        Vector2f hintPosition = (m_HintMap[i].objectPosition + Vector2f(0.5f,0.5f)) * BASIC_ELEMENT_IMAGE_SIZE;
        if( position.x == hintPosition.x && position.y == hintPosition.y )
        {
            hintID = i;
            return true;
        }
    }
    return false;
}

GameObject* Scene::mousePressed(const Event& event)
{
    GameObject* pPickedGameObject = NULL;
    
    int x = event.mouseInput.X;
    int y = event.mouseInput.Y;
    
	if( event.mouseInput.Event == spider3d::LEFT_MOUSE_PRESSED_DOWN )
	{
        m_PickedGameObject = pickGameObject(Vector2f(x,y));
        //m_PickedGameObject = m_currentSelectedGameObject;
		if( m_PickedGameObject )
		{
            setDraggedObject();
            
			Vector2f v = m_PickedGameObject->getPosition();
			m_PickPointOffset.x = x - v.x;
			m_PickPointOffset.y = y - v.y;
			m_PickedObjectCentre.x = v.x;
			m_PickedObjectCentre.y = v.y;
            
            if( m_pDraggedObject )
                m_pDraggedObject->setPosition(v);
			
            if( m_pGrid )
			{
                Vector2f centre = m_PickedGameObject->getClosestGridCentre();
                GAME_OBJECT_TYPE objectType = m_PickedGameObject->getGameObjectInfo().m_ObjectType;
                if( objectType != EOT_SINK && objectType != EOT_LIGHT_SOURCE )
                    m_pGrid->resetOccupancy(centre.x,centre.y);
			}
            
            getHintAt(m_PickedGameObject->getPosition(), m_SelectedHintID );
            
		}
        pPickedGameObject = m_PickedGameObject;
	}
	if( event.mouseInput.Event == spider3d::LEFT_MOUSE_LEFT_UP )
	{
        if( m_PickedGameObject )
        {
            GAME_OBJECT_TYPE objectType = m_PickedGameObject->getGameObjectInfo().m_ObjectType;
            if(  objectType != EOT_PLACEMENT_MARKER )
            {
                m_PickPointOffset.x = m_PickPointOffset.y = 0.0f;
                Vector2f vOut;
                GAME_OBJECT_TYPE objectType = m_PickedGameObject->getGameObjectInfo().m_ObjectType;
                
                if( objectType == EOT_LIGHT_SOURCE || objectType == EOT_SINK )
                {
                    Vector2f centre;
                    m_pGrid->snap(Vector2f(x,y), centre);
                    m_PickedGameObject->setClosestGridCentre(centre);
                    m_pGrid->snapSource(Vector2f(x,y),vOut);
                    m_PickedGameObject->setPosition(vOut);
                }
                else
                {
                    // If grid is placeable.
                    vOut = findClosestPlacementMarkerPosition(Vector2f(x,y));

                    bool bPlaceable = m_pGrid->isPlacementMarked(vOut.x, vOut.y, 0) && !m_pGrid->isOccupied(vOut.x,vOut.y,0);

                    if( bPlaceable )
                    {
                        if( objectType != EOT_LIGHT_SOURCE && objectType != EOT_SINK )
                        {
                            m_PickedGameObject->setClosestGridCentre(m_PickedGameObject->getPosition());
                            //m_pGrid->snap(Vector2f(x,y),vOut);
                        }
                        Matrix3f xForm;
                        xForm.translate(vOut.x, vOut.y);
                        
                        if( objectType != EOT_LIGHT_SOURCE && objectType != EOT_SINK )
                            m_pGrid->setOccupied(vOut.x, vOut.y, 0);
                        
                        m_PickedGameObject->setPosition(vOut);
                        
                        // Flag the Hint as Unused if there was an Object moved which used the Hint earlier
                        
                        if( m_SelectedHintID > -1 && (m_PickedObjectCentre.x != vOut.x || m_PickedObjectCentre.y != vOut.y ))
                        {
                            //printf("UnConsumed Hint ID = %d\n", m_SelectedHintID );
                            m_HintMap[m_SelectedHintID].bIsAvailable = true;
                        }
                    }
                    else
                    {
                        Matrix3f xForm;
                        xForm.translate(m_PickedObjectCentre.x, m_PickedObjectCentre.y);
                        m_PickedGameObject->setPosition(m_PickedObjectCentre);
                        
                        if( objectType != EOT_LIGHT_SOURCE && objectType != EOT_SINK )
                            m_pGrid->setOccupied(m_PickedObjectCentre.x, m_PickedObjectCentre.y, 0);
                    }
                }
            }
        }
        unSetDraggedObject();
	}
    
    return pPickedGameObject;
}

Vector2f Scene::findClosestPlacementMarkerPosition( const Vector2f& position )
{
    Vector2f outPosition;
    float minDist = 1000000.0f;
    int closestMarkerIndex = -1;
    for( int i=0; i<m_vecPlacementMarkerGameObjects.size(); i++ )
    {
        PlacementMarkerGameObject* pMarker = m_vecPlacementMarkerGameObjects[i];
        Vector2f markerPosition = pMarker->getPosition();// - Vector2f(pMarker->getWidth(), pMarker->getHeight());
        float d2 = markerPosition.distanceSquared(position);
        if( d2 < minDist )
        {
            minDist = d2;
            closestMarkerIndex = i;
        }
    }
    PlacementMarkerGameObject *pClosestMarker = m_vecPlacementMarkerGameObjects[closestMarkerIndex];
    outPosition = pClosestMarker->getPosition();
    return outPosition;
}

void Scene::mouseMoved(const Event& event)
{
    int x = event.mouseInput.X;
    int y = event.mouseInput.Y;
    
	if( event.mouseInput.Event == spider3d::SMOUSE_DRAGGED )
	{
		if( m_PickedGameObject  && m_PickedGameObject->getGameObjectInfo().m_ObjectType != EOT_PLACEMENT_MARKER)
		{
			Vector2f t(x - m_PickPointOffset.x, y - m_PickPointOffset.y);
            
            if( m_pDraggedObject )
                m_pDraggedObject->setPosition(t);
		}
	}
}

sbool Scene::OnEvent( Event& event)
{
    return true;
}

void Scene::appendSolutionData(const char* fileName) const
{
    string path = "/Users/prashantsharma/Downloads/Backup_MacBookPro/Optiks/";
    path += fileName;
    FILE* pFile = fopen(path.c_str(), "a");
	if( pFile )
    {
        fprintf(pFile, "\n\n-------LEVEL_SOLUTION_INFO::-------\n\n");
        // Save Shape Objects positions
        fprintf(pFile, "NUM_SHAPE_OBJECTS::\n");
        int numShapeGameObjects = 0;
        for( int i=0; i<m_vecGameObjects.size(); i++ )
            if( m_vecGameObjects[i]->getGameObjectInfo().m_ObjectType == EOT_SHAPE_OBJECT )
                numShapeGameObjects++;
        
        //fprintf(pFile, "%d\n", numShapeGameObjects);
		for( int i=0; i<1; i++ )
		{
            
            if( m_currentSelectedGameObject && m_currentSelectedGameObject->getGameObjectInfo().m_ObjectType == EOT_SHAPE_OBJECT )
            {
                fprintf(pFile, "SHAPE_OBJECT_ID::\n");
                fprintf(pFile, "%d\n", m_currentSelectedGameObject->getID());
                fprintf(pFile, "SHAPE_OBJECT_POSITION::\n");
                Vector2f position = m_currentSelectedGameObject->getPosition();
                fprintf(pFile, "%d %d\n", (int)(position.x/BASIC_ELEMENT_IMAGE_SIZE), (int)(position.y/BASIC_ELEMENT_IMAGE_SIZE));
            }
		}
    }
    fclose(pFile);
}

void Scene::save(const char* fileName) const
{
    string path = "/Users/prashantsharma/Downloads/Backup_MacBookPro/Optiks/";
    path += fileName;
    
	FILE* pFile = fopen(path.c_str(), "w");
	if( pFile )
	{
        // Save Placement Markers
        fprintf(pFile, "-------PLACEMENT_MARKER_INFO::-------\n\n");
        fprintf(pFile, "NUM_PLACEMENT_MARKERS::\n");
        fprintf(pFile, "%d\n", (int)m_vecPlacementMarkerGameObjects.size());
        assert(m_NumPlacementMarkerGameObjects>0);
        for( int i=0; i<m_vecPlacementMarkerGameObjects.size(); i++ )
		{
			m_vecPlacementMarkerGameObjects[i]->save(pFile);
		}
        // Save Sources
        fprintf(pFile, "-------SOURCE_INFO::-------\n\n");
        fprintf(pFile, "NUM_SOURCES::\n");
        fprintf(pFile, "%d\n", (int)m_vecSourceObjects.size());
        assert(m_NumSources>0);
        for( int i=0; i<m_vecSourceObjects.size(); i++ )
		{
			m_vecSourceObjects[i]->save(pFile);
		}
	    // Save Shape Objects
        fprintf(pFile, "-------GAME_SHAPE__OBJECTS_INFO::-------\n\n");
        fprintf(pFile, "NUM_SHAPE_OBJECTS::\n");
        int numShapeGameObjects = 0;
        for( int i=0; i<m_vecGameObjects.size(); i++ )
            if( m_vecGameObjects[i]->getGameObjectInfo().m_ObjectType == EOT_SHAPE_OBJECT )
                numShapeGameObjects++;
        
        fprintf(pFile, "%d\n", numShapeGameObjects);
		for( int i=0; i<m_vecGameObjects.size(); i++ )
		{
            if( m_vecGameObjects[i]->getGameObjectInfo().m_ObjectType == EOT_SHAPE_OBJECT )
            {
                m_vecGameObjects[i]->save(pFile);
            }
		}
        // Save Portal Objects
        fprintf(pFile, "-------GAME_PORTAL__OBJECTS_INFO::-------\n\n");
        fprintf(pFile, "NUM_PORTAL_OBJECTS::\n");
        fprintf(pFile, "%d\n", (int)m_vecPortalObjects.size());
		for( int i=0; i<m_vecPortalObjects.size(); i++ )
		{
            m_vecPortalObjects[i]->save(pFile);
		}
        // Save Sink Objects
        fprintf(pFile, "-------GAME_SINK__OBJECTS_INFO::-------\n\n");
        fprintf(pFile, "NUM_SINK_OBJECTS::\n");
        fprintf(pFile, "%d\n", (int)m_vecSinkObjects.size());
        for( int i=0; i<m_vecSinkObjects.size(); i++ )
		{
            m_vecSinkObjects[i]->save(pFile);
		}
	}
	fclose(pFile);
}

void Scene::loadAndSaveAllLevels()
{
    char levelFilename[32];
    for( int chapter = 0; chapter < NUM_CHAPTERS; chapter ++ )
    {
        for( int stage=0; stage<NUM_STAGES; stage++ )
        {
            for( int level=0; level<NUM_LEVELS_PER_STAGE; level++ )
            {
                load(chapter+1, stage+1, level+1);
                sprintf(levelFilename,"ch%dstg%dlvl%d", chapter+1, stage+1, level+1);
                save(levelFilename);
            }
        }
    }
}

void Scene::load(int chapter, int stage, int level)
{
    GameLevelSelectionState::getInstance()->setLevelTo(level);
    GameLevelSelectionState::getInstance()->setStageTo(stage);
    GameChapterSelectionState::getInstance()->setChapterTo(chapter);
    
    char levelFilename[32];
    sprintf(levelFilename,"ch%dstg%dlvl%d",chapter, stage, level);
    
#ifdef IOS
    FileUtil fu;
    load( fu.getAbsolutePath(levelFilename).c_str() );
#else
    load(levelFilename);
#endif
}

void Scene::load(const char* fileName)
{
    Game::getInstance()->setLevelCompleteTaskFired(false);
    // Reset old scene
    clearScene();
    
    // Load new Scene
    string path = "/Users/prashantsharma/Downloads/Backup_MacBookPro/Optiks/";

#ifdef IOS
    path = fileName;
#else
    path += fileName;
#endif
    
	FILE* pFile = fopen(path.c_str(), "r");
    
    char dummyBuffer[100];
	if(!pFile)
    {
        printf("ERROR::Could Not Load the File\n");
        assert(pFile);
    }
    
    Vector2f sceneOffset;
    
    if( pFile )
	{
        // Load Placement Marker Objects
        int numPlacementMarkerObjects;
        fscanf(pFile, "%s\n\n", dummyBuffer);
        fscanf(pFile, "%s\n", dummyBuffer);
        fscanf(pFile, "%d\n", &numPlacementMarkerObjects);
        assert(numPlacementMarkerObjects>0);
        for( int i=0; i<numPlacementMarkerObjects; i++)
        {
            PlacementMarkerGameObjectInfo pInfo;
            loadPlacementMarkerGameObjectInfo(pFile, pInfo);
            addGameObject( createGameObject(pInfo) );
        }
        // Find Scene Offset
        sceneOffset = findSceneOffset();
        
        // Load Sources
        int numSources;
        fscanf(pFile, "%s\n\n", dummyBuffer);
        fscanf(pFile, "%s\n", dummyBuffer);
        fscanf(pFile, "%d\n", &numSources);
        assert(numSources>0);
        for( int i=0; i<numSources; i++)
        {
            SourceGameObjectInfo sInfo;
            loadSourceGameObjectInfo(pFile, sInfo);
            addGameObject( createGameObject(sInfo) );
        }        
        // Load Game Objects
        int numShapeObjects;
        fscanf(pFile, "%s\n\n", dummyBuffer);
        fscanf(pFile, "%s\n", dummyBuffer);
        fscanf(pFile, "%d\n", &numShapeObjects);
        //assert(numShapeObjects>0);
        for( int i=0; i<numShapeObjects; i++)
        {
            ShapeGameObjectInfo sInfo;
            loadShapeGameObjectInfo(pFile, sInfo);
            if( !sInfo.shapeInfo->m_IsReflective )
            {
                sInfo.eMaterialType = EMT_ABSORBITIVE;
            }
            else
            {
                sInfo.eMaterialType = EMT_REFLECTIVE;
            }
            if( sInfo.shapeInfo->m_isRefractive )
            {
                sInfo.eMaterialType = EMT_TRANSMISSIVE;
                if( sInfo.shapeInfo->m_RefractionIndex > 1.0 )
                {
                    sInfo.eMaterialType = EMT_REFRACTIVE;
                }
            }
            if( !sInfo.shapeInfo->m_IsReflective && sInfo.shapeInfo->m_isRefractive )
            {
                sInfo.eMaterialType = EMT_COLOR_FILTER;
            }
            addGameObject( createGameObject(sInfo) );
            delete sInfo.shapeInfo;
        }
        // Load Portal Objects
        int numPortalObjects;
        fscanf(pFile, "%s\n\n", dummyBuffer);
        fscanf(pFile, "%s\n", dummyBuffer);
        fscanf(pFile, "%d\n", &numPortalObjects);
        for( int i=0; i<numPortalObjects; i++)
        {
            PortalGameObjectInfo pInfo;
            loadPortalGameObjectInfo(pFile, pInfo);
            addGameObject( createGameObject(pInfo) );
            delete pInfo.shapeInfo1;
            delete pInfo.shapeInfo2;
        }
        // Load Sink Objects
        int numSinkObjects;
        fscanf(pFile, "%s\n\n", dummyBuffer);
        fscanf(pFile, "%s\n", dummyBuffer);
        fscanf(pFile, "%d\n", &numSinkObjects);
        //assert(numSinkObjects>0);
        for( int i=0; i<numSinkObjects; i++)
        {
            SinkGameObjectInfo sInfo;
            sInfo.m_ObjectType = EOT_SINK;
            loadSinkGameObjectInfo(pFile, sInfo);
            addGameObject( createGameObject(sInfo) );
        }
	}
    //readHints(pFile);
    fclose(pFile);
    
    for( int i=0; i<m_vecGameObjects.size(); i++ )
    {
        if( m_vecGameObjects[i]->getGameObjectInfo().m_ObjectType == EOT_SHAPE_OBJECT )
        {
            int id = m_vecGameObjects[i]->getGameObjectInfo().m_ObjectID;
            m_ObjectMap[id] = m_vecGameObjects[i];
        }
    }
    
    // Offset the grid.
    m_pGrid->setOrigin(sceneOffset);
    

    adjustObjectsPositions(sceneOffset);
}

void Scene::applyHint()
{
    // Get First free hint and apply it.
    bool bHintFound = false;
    int hintID = -1;
    HintInfo hintInfo;
    for( int i=0; i<m_HintMap.size(); i++ )
    {
        if( m_HintMap[i].bIsAvailable )
        {
            hintInfo = m_HintMap[i];
            bHintFound = true;
            hintID = i;
            break;
        }
    }
    // Apply the Hint
    GameObject* pGameObject = getFirstFreeObjectOfMaterialType(hintInfo.objectID);
    if( pGameObject )
    {
        Vector2f hintPosition = ( hintInfo.objectPosition + Vector2f(0.5f, 0.5f) ) * BASIC_ELEMENT_IMAGE_SIZE;
        
        GameObject* pPickedGameObject = pickGameObject(hintPosition);
        
        // If there is already a Shape Object at Hint Position, Swap
        if( pPickedGameObject->getGameObjectInfo().m_ObjectType == EOT_SHAPE_OBJECT )
        {
            pPickedGameObject->setPosition(pGameObject->getPosition());
            pGameObject->setPosition(hintPosition);
            m_HintMap[hintID].bIsAvailable = false;
        }
        else
        {
            Event event;
            event.eventType = SMOUSE_EVENT;
            event.mouseInput.Event = LEFT_MOUSE_PRESSED_DOWN;
            event.mouseInput.X = pGameObject->getPosition().x;
            event.mouseInput.Y = pGameObject->getPosition().y;
            mousePressed(event);
            event.mouseInput.X = hintPosition.x;
            event.mouseInput.Y = hintPosition.y;
            event.mouseInput.Event = LEFT_MOUSE_LEFT_UP;
            mousePressed(event);
            // Hint is Consumed
            m_HintMap[hintID].bIsAvailable = false;
            //printf("Consumed Hint ID = %d\n", hintID );
        }
    }
    // This will happen, if no free object was found. ie. already object is correctly located.
    else
    {
        m_HintMap[hintID].bIsAvailable = false;
    }
}

bool Scene::isObjectOccupyingHintPosition( const Vector2f& position, int materialType )
{
    bool bRes = false;
    
    for( int i=0; i<m_HintMap.size(); i++ )
    {
        Vector2f hintPosition = (m_HintMap[i].objectPosition + Vector2f(0.5f,0.5f) ) * BASIC_ELEMENT_IMAGE_SIZE;
        if( (position.x == hintPosition.x && position.y == hintPosition.y) && materialType == m_HintMap[i].objectID )
        {
            bRes = true;
            break;
        }
    }
    return bRes;
}

GameObject* Scene::getFirstFreeObjectOfMaterialType(int materialType)
{
    GameObject *pFreeObject = NULL;
    
    for( int i=0; i<m_vecGameObjects.size(); i++ )
    {
        GAME_OBJECT_TYPE _objectType = m_vecGameObjects[i]->getGameObjectInfo().m_ObjectType;
        if( _objectType == EOT_SHAPE_OBJECT )
        {
            ShapeGameObject* pShapeObject = (ShapeGameObject*)m_vecGameObjects[i];
            
            if( materialType == pShapeObject->getMaterialType() && !isObjectOccupyingHintPosition(m_vecGameObjects[i]->getPosition(), materialType) )
            {
                pFreeObject = m_vecGameObjects[i];
                break;
            }
        }
    }
    return pFreeObject;
}

void Scene::adjustObjectsPositions(const Vector2f& offset)
{
    // Adjust position of all objects so that the Scene is at the Centre of Screen.
    for( int i=0; i<m_vecGameObjects.size(); i++ )
    {
        GameObject* pGameObject = m_vecGameObjects[i];
        Vector2f currentPosition = pGameObject->getPosition();
        if( pGameObject->getGameObjectInfo().m_ObjectType == EOT_PORTAL )
        {
            PortalGameObject* pPortal = (PortalGameObject*)m_vecGameObjects[i];
            pPortal->getPortalShape1()->setPosition(pPortal->getPortalShape1()->getPosition()+offset);
            pPortal->getPortalShape2()->setPosition(pPortal->getPortalShape2()->getPosition()+offset);
            pPortal->getGlowShape1()->setPosition(pPortal->getPortalShape1()->getPosition());
            pPortal->getGlowShape2()->setPosition(pPortal->getPortalShape2()->getPosition());
        }
        else
        {
            pGameObject->setPosition(currentPosition + offset);
        }
    }
}

Vector2f Scene::findSceneOffset()
{
    m_BoundingBox.m_LowerBounds = Vector2f(10000.0f,10000.0f);
    m_BoundingBox.m_UpperBounds = Vector2f(-10000.0f,-10000.0f);
    
    for( int i=0; i<m_vecPlacementMarkerGameObjects.size(); i++ )
    {
        PlacementMarkerGameObject* pMarkerObject = m_vecPlacementMarkerGameObjects[i];
        Vector2f position = pMarkerObject->getPosition();
        float w = pMarkerObject->getWidth();
        float h = pMarkerObject->getHeight();
        AABB2f aabb;
        aabb.m_LowerBounds = position - Vector2f(w*0.5f, h*0.5f);
        aabb.m_UpperBounds = position + Vector2f(w*0.5f, h*0.5f);
        m_BoundingBox.combine(aabb);
    }
    
    Vector2f sceneBoundingBoxCentre = m_BoundingBox.getCentre();
    Vector2f sceneCentre = Vector2f(m_SceneWidth*0.5f, m_SceneHeight*0.5f);
    Vector2f sceneCentreOffset = sceneCentre - sceneBoundingBoxCentre;
    
    return sceneCentreOffset;
}

const void Scene::loadSinkGameObjectInfo( FILE* pFile, SinkGameObjectInfo& sInfo ) const
{
    char dummyBuffer[100];
    
    fscanf(pFile, "%s\n", (char*)&dummyBuffer);
    int r,g,b;
    fscanf(pFile, "%d %d %d\n", &r,&g,&b);
    sInfo.color.r = (float)r/255;
    sInfo.color.g = (float)g/255;
    sInfo.color.b = (float)b/255;

    fscanf(pFile, "%s\n", (char*)&dummyBuffer);
    fscanf(pFile, "%f  %f\n", &sInfo.m_Position.x, &sInfo.m_Position.y);
    sInfo.m_Position = sInfo.m_Position * BASIC_ELEMENT_IMAGE_SIZE * 0.5f;

    fscanf(pFile, "%s\n", (char*)&dummyBuffer);
    fscanf(pFile, "%f  %f\n", &sInfo.closestGridCentre.x, &sInfo.closestGridCentre.y);
}

const void Scene::loadSourceGameObjectInfo( FILE* pFile, SourceGameObjectInfo& sInfo ) const
{
    char dummyBuffer[100];
    
    sInfo.m_ObjectType = EOT_LIGHT_SOURCE;
    fscanf(pFile, "%s\n", (char*)&dummyBuffer);
    int r,g,b;
    fscanf(pFile, "%d %d %d\n", &r,&g,&b);
    sInfo.color.r = (float)r/255;
    sInfo.color.g = (float)g/255;
    sInfo.color.b = (float)b/255;
    
    fscanf(pFile, "%s\n", (char*)&dummyBuffer);
    fscanf(pFile, "%f  %f\n", &sInfo.origin.x, &sInfo.origin.y);
    sInfo.origin = sInfo.origin * BASIC_ELEMENT_IMAGE_SIZE * 0.5f;
    
    fscanf(pFile, "%s\n", (char*)&dummyBuffer);
    fscanf(pFile, "%f  %f\n", &sInfo.direction.x, &sInfo.direction.y);
    fscanf(pFile, "%s\n", (char*)&dummyBuffer);
    fscanf(pFile, "%f  %f\n", &sInfo.closestGridCentre.x, &sInfo.closestGridCentre.y);
    sInfo.m_Position = sInfo.origin;
}

const void Scene::loadPlacementMarkerGameObjectInfo( FILE* pFile, PlacementMarkerGameObjectInfo& pInfo ) const
{
    char dummyBuffer[100];
    
    fscanf(pFile, "%s\n", (char*)&dummyBuffer);
    fscanf(pFile, "%f  %f\n", &pInfo.m_Position.x, &pInfo.m_Position.y);
    // Positions are saved in grid coordinates, so convert to world
    pInfo.m_Position = ( pInfo.m_Position + Vector2f(0.5f, 0.5f) ) * BASIC_ELEMENT_IMAGE_SIZE;

    fscanf(pFile, "%s\n", (char*)&dummyBuffer);
    fscanf(pFile, "%f  %f\n", &pInfo.width, &pInfo.height);
    pInfo.width = BASIC_ELEMENT_SIZE;
    pInfo.height = BASIC_ELEMENT_SIZE;
}

const void Scene::loadPortalGameObjectInfo( FILE* pFile, PortalGameObjectInfo& pInfo ) const
{
    char dummyBuffer[100];
    
    SHAPE_TYPE shapeType = EST_NONE;
    fscanf(pFile, "%s\n", (char*)&dummyBuffer);
	fscanf(pFile, "%d\n", &shapeType);
    
    RectangleInfo* rInfo1 = new RectangleInfo();
    pInfo.shapeInfo1 = rInfo1;
    loadCommonShapeInfo(pFile, pInfo.shapeInfo1);
    fscanf(pFile, "%s\n", (char*)&dummyBuffer);
    fscanf(pFile, "%f %f\n", &rInfo1->width, &rInfo1->height);
    rInfo1->width  = BASIC_ELEMENT_SIZE;
    rInfo1->height = BASIC_ELEMENT_SIZE;
    
    RectangleInfo* rInfo2 = new RectangleInfo();
    pInfo.shapeInfo2 = rInfo2;
    fscanf(pFile, "%s\n", (char*)&dummyBuffer);
	fscanf(pFile, "%d\n", &shapeType);
    loadCommonShapeInfo(pFile, pInfo.shapeInfo2);
    fscanf(pFile, "%s\n", (char*)&dummyBuffer);
    fscanf(pFile, "%f %f\n", &rInfo2->width, &rInfo2->height);
    rInfo2->width  = BASIC_ELEMENT_SIZE;
    rInfo2->height = BASIC_ELEMENT_SIZE;
    
    pInfo.m_Position = pInfo.shapeInfo1->position;
}

const void Scene::loadShapeGameObjectInfo( FILE* pFile, ShapeGameObjectInfo& sInfo ) const
{
    char dummyBuffer[100];
    
    SHAPE_TYPE shapeType = EST_NONE;
    fscanf(pFile, "%s\n\n", dummyBuffer);
    fscanf(pFile, "%d\n", &sInfo.bFixed);
    fscanf(pFile, "%s\n", dummyBuffer);
    fscanf(pFile, "%d\n", &sInfo.m_ObjectID);
    fscanf(pFile, "%s\n", dummyBuffer);
    int r,g,b;
    fscanf(pFile, "%d %d %d\n", &r,&g,&b);
    sInfo.m_Color.r = (float)r/255;
    sInfo.m_Color.g = (float)g/255;
    sInfo.m_Color.b = (float)b/255;
    fscanf(pFile, "%s\n", (char*)&dummyBuffer);
	fscanf(pFile, "%d\n", &shapeType);
    
    switch (shapeType)
    {
        case EST_RECT:
        {
            RectangleInfo* rInfo = new RectangleInfo();
            sInfo.shapeInfo = rInfo;
            loadCommonShapeInfo(pFile, sInfo.shapeInfo);
            fscanf(pFile, "%s\n", (char*)&dummyBuffer);
            fscanf(pFile, "%f %f\n", &rInfo->width, &rInfo->height);
            rInfo->width  = BASIC_ELEMENT_SIZE;
            rInfo->height = BASIC_ELEMENT_SIZE;
            break;
        }
        case EST_CIRCLE:
        {
            CircleInfo* cInfo = new CircleInfo();
            sInfo.shapeInfo = cInfo;
            loadCommonShapeInfo(pFile, sInfo.shapeInfo);
            fscanf(pFile, "%s\n", (char*)&dummyBuffer);
            fscanf(pFile, "%f\n", &cInfo->radius);
            //cInfo->radius = 1280.0f/15.0f/2.0f;
            //assert(0);
            break;
        }
        case EST_CIRCULAR_ARC:
        {
            CircularArcInfo* cInfo = new CircularArcInfo();
            sInfo.shapeInfo = cInfo;
            loadCommonShapeInfo(pFile, sInfo.shapeInfo);
            fscanf(pFile, "%s\n", (char*)&dummyBuffer);
            fscanf(pFile, "%f %f\n", &cInfo->radius, &cInfo->angle);
            break;
        }
        case EST_LENS:
        {
            assert(0);
            break;
        }
        case EST_LINE:
        {
            assert(0);
            break;
        }
        case EST_PARABOLIC_SEGMENT:
        {
            assert(0);
            break;
        }
        case EST_REGULAR_POLY:
        {
            assert(0);;
            break;
        }
        case EST_NONE:
        {
            assert(0);
            break;
        }
        default:
            break;
    }
    sInfo.m_Position = sInfo.shapeInfo->position;
    sInfo.m_Rotation = sInfo.shapeInfo->rotation;
}

const void Scene::loadCommonShapeInfo( FILE* pFile, ShapeInfo *sInfo ) const
{
    char dummyBuffer[100];

    fscanf(pFile, "%s\n", (char*)&dummyBuffer);
	fscanf(pFile, "%f %f\n", &sInfo->position.x, &sInfo->position.y);
    // Positions are saved in grid coordinates, so convert to world
    sInfo->position = ( sInfo->position + Vector2f(0.5f, 0.5f) ) * BASIC_ELEMENT_IMAGE_SIZE;

    fscanf(pFile, "%s\n", (char*)&dummyBuffer);
	fscanf(pFile, "%f\n", &sInfo->rotation);
	fscanf(pFile, "%s\n", (char*)&dummyBuffer);
	fscanf(pFile, "%d  %d\n", (int*)&sInfo->m_IsReflective, (int*)&sInfo->m_isRefractive);
	fscanf(pFile, "%s\n", (char*)&dummyBuffer);
    fscanf(pFile, "%f\n", &sInfo->m_RefractionIndex);
}

// Rotates the picked/selected object by theta degrees
void Scene::rotateSelectedObject( GameObject *pObject, float theta)
{
    if( pObject )
    {
        float rotation = pObject->getRotation();
        float angle = theta + rotation;
        angle = angle > 360.0f ? 360.0f - angle : angle;
        pObject->setRotation(angle);
    }
}

void Scene::setDebugGridEnable(bool bEnable)
{
    m_bEnableDebugGrid = bEnable;
}


void Scene::enableMarkerObjects()
{
    for( int i=0; i<m_vecPlacementMarkerGameObjects.size(); i++ )
    {
        m_vecPlacementMarkerGameObjects[i]->setVisible(true);
    }
}

void Scene::setDraggedObject()
{
    if( m_currentSelectedGameObject )
    {
        map<int,GameObject*>::iterator it;
        it = m_mapPrototypeObjects.find(m_currentSelectedGameObject->getGameObjectInfo().m_ObjectTextureID);
        if( it != m_mapPrototypeObjects.end())
        {
            m_pDraggedObject = it->second;
        }
    }
}

void Scene::unSetDraggedObject()
{
    m_pDraggedObject = NULL;
}

void Scene::createPrototypeObjects()
{
    // Opaque RectangleShape
    ShapeGameObjectInfo sInfo;
    RectangleInfo rInfo;
    rInfo.m_IsReflective = true;
    rInfo.m_isRefractive = false;
    rInfo.m_RefractionIndex = 1.0f;
    
    rInfo.width  = BASIC_ELEMENT_SIZE;
    rInfo.height = BASIC_ELEMENT_SIZE;
    
    sInfo.m_Position = rInfo.position;
    sInfo.m_Rotation = 0.0f;
    
    sInfo.shapeInfo = &rInfo;
    sInfo.eMaterialType = EMT_REFLECTIVE;
    m_mapPrototypeObjects[EOT_SHAPE_OBJECT + EMT_REFLECTIVE] = createGameObject(sInfo);
    sInfo.eMaterialType = EMT_TRANSMISSIVE;
    m_mapPrototypeObjects[EOT_SHAPE_OBJECT + EMT_TRANSMISSIVE] = createGameObject(sInfo);
    sInfo.eMaterialType = EMT_REFRACTIVE;
    m_mapPrototypeObjects[EOT_SHAPE_OBJECT + EMT_REFRACTIVE] = createGameObject(sInfo);
    sInfo.eMaterialType = EMT_ABSORBITIVE;
    m_mapPrototypeObjects[EOT_SHAPE_OBJECT + EMT_ABSORBITIVE] = createGameObject(sInfo);
    sInfo.eMaterialType = EMT_COLOR_FILTER;
    m_mapPrototypeObjects[EOT_SHAPE_OBJECT + EMT_COLOR_FILTER] = createGameObject(sInfo);
    
    // Sink
    SinkGameObjectInfo sinkInfo;
    sinkInfo.m_Rotation = 0.0f;
    
    m_mapPrototypeObjects[EOT_SINK] = createGameObject(sinkInfo);
    
    // Source
    SourceGameObjectInfo sourceInfo;
    sourceInfo.direction.x = -1;
    sourceInfo.direction.y = -1;
    
    sourceInfo.m_Rotation = 0.0f;
    m_mapPrototypeObjects[EOT_LIGHT_SOURCE] = createGameObject(sourceInfo);
    
    // Portal
    PortalGameObjectInfo pInfo;
    
    RectangleInfo rInfo1;
    rInfo1.m_IsReflective = false;
    rInfo1.m_isRefractive = false;
    rInfo1.m_RefractionIndex = 1.0f;
    
    rInfo1.width  = BASIC_ELEMENT_SIZE;
    rInfo1.height = BASIC_ELEMENT_SIZE;
    
    pInfo.m_Position = rInfo1.position;
    pInfo.m_Rotation = 0.0f;
    
    pInfo.shapeInfo1 = &rInfo1;
    
    RectangleInfo rInfo2;
    rInfo2.m_IsReflective = false;
    rInfo2.m_isRefractive = false;
    rInfo2.m_RefractionIndex = 1.0f;
    
    rInfo2.width  = BASIC_ELEMENT_SIZE;
    rInfo2.height = BASIC_ELEMENT_SIZE;
    
    pInfo.m_Position = rInfo2.position;
    pInfo.m_Rotation = 0.0f;
    
    pInfo.shapeInfo2 = &rInfo2;
    m_mapPrototypeObjects[EOT_PORTAL] = createGameObject(pInfo);
}

void Scene::takeScreenShot(void* pixels) const
{
    Game::getInstance()->getRenderer()->readPixels(AABB2i(Vector2i(0,0), Vector2i(1024,768)), pixels);
}

void Scene::addGlowingObjects( IGlowing* pGlowing )
{
    m_vecGlowing.push_back(pGlowing);
}

bool Scene::removeGlowingObjects( IGlowing* pGlowing )
{
    int index = -1;
    for( int i=0; i< m_vecGlowing.size(); i++ )
    {
        if( m_vecGlowing[i] == pGlowing )
        {
            index = i;
            break;
        }
    }
    
    if( index != -1 )
        m_vecGlowing.erase(m_vecGlowing.begin() + index );
    
    return index != -1;
}

void Scene::readHints(FILE* pFile)
{
    char dummyBuffer[100];
    int id;
    Vector2f position;
    
    fscanf(pFile, "%s\n\n", dummyBuffer);
    fscanf(pFile, "%s\n", dummyBuffer);
    int numShapeObjects;
    fscanf(pFile, "%d\n", &numShapeObjects);
    
    for( int i=0; i<numShapeObjects; i++ )
    {
        fscanf(pFile, "%s\n", dummyBuffer);
        fscanf(pFile, "%d\n", &id);

        fscanf(pFile, "%s\n", dummyBuffer);
        fscanf(pFile, "%f %f\n", &position.x, &position.y);
        HintInfo hintInfo;
        hintInfo.objectID = id;
        hintInfo.objectPosition = position;
        m_HintMap[i] = hintInfo;
    }
}