#ifndef _SCENE_H
#define _SCENE_H

#include <vector>
#include <string>
#include <map>
#include "Ray.h"
#include "Matrix4.h"
#include "GameObject.h"
#include "MemoryBlockAllocator.h"
//#include "Shapes.h"
#include "ParticleBase.h"
#include "EventListener.h"
#include "FadeSceneTask.h"

/*
 
 To play with the performance Vs quality, a few things can be done.
 1. Num of Textures used for producing glow can be increased/reduced.
 2. Num of blur passes can be increased/reduced.( right now there is a nice balance here by doing less no of blur passes on high res textures and high no of blur passes on low res textures.
 3. Make the Glow Texture Resolution less.
 4. Remove Wave Shader effect. 
 */

class Shape2D;
class Grid;
class Source;
class FrameBufferObjectTexture_2D;
class Texture2D;
class ShaderProgram;
class FluidSimulator;
class TextRenderer;
class SourceGameObject;
class SinkGameObject;
class PlacementMarkerGameObject;
class PortalGameObject;
class GameMenuBackgroundDecorObject;
class IGlowing;

using namespace spider3d;
using namespace std;

struct SourceGameObjectInfo;
struct SinkGameObjectInfo;
struct ShapeGameObjectInfo;
struct PlacementMarkerGameObjectInfo;
struct PortalGameObjectInfo;
struct ShapeInfo;
struct GameMenuBackgroundDecorObjectInfo;

enum OPERATION_MODE { EOM_SELECT, EOM_EDIT, EOM_MOVE, EOM_NONE };

#define SWAP_TEXTURES( x, y ) { \
Texture2D* temp = y; \
y = x; \
x = temp; } \

#define NUM_TEXTURES 6

struct HintInfo
{
    HintInfo()
    {
        bIsAvailable = true;
    }
    
    int objectID;
    Vector2f objectPosition;
    bool bIsAvailable;
};

class Scene : public EventListener
{    
private:
    vector<Shape2D*>                        objects;
    vector<GameObject*>                     m_vecGameObjects;
	vector<SourceGameObject*>               m_vecSourceObjects;
    vector<PortalGameObject*>               m_vecPortalObjects;
    vector<PlacementMarkerGameObject*>      m_vecPlacementMarkerGameObjects;
    vector<SinkGameObject*>                 m_vecSinkObjects;
    vector<IGlowing*>                       m_vecGlowing;
    
	map<int,GameObject*>                    m_mapPrototypeObjects;
    
    vector<Ray>                             vecRays;
	unsigned                                numObjects;
    int                                     traceDepth;
    
    FadeSceneTask                           *m_pFadeSceneTask;
    
    int                                     m_ScreenScaleFactor;
    
    int                                     m_HintID;
    int                                     m_SelectedHintID;
public:
	Scene(int , int, int screenScale = 1);
    
    bool                    isGameObjectPlaceable(const Vector2f& position, GAME_OBJECT_TYPE objectType);
    GameObject*             createGameObject(const GameObjectInfo& gameObjectInfo);
    void                    destroyGameObject(GameObject* pGameObject);
    bool                    addGameObject(GameObject* pGameObject);
	void                    removeGameObject(GameObject* pGameObject);
	bool                    removeSelectedObject();
	Shape2D*                pickObject(const Vector2f& p );
    GameObject*             pickGameObject(const Vector2f& p );
    PlacementMarkerGameObject* pickMarkerObject(const Vector2f& point );
    void                    setDebugGridEnable(bool);
    void                    enableMarkerObjects();
    bool                    getFreePlacementmarkerPosition(Vector2f& freePosition);

    void                    fadeIn();
    void                    fadeOut();
    
    bool                    getHintAt( const Vector2f& position, int& hintID );

    void                    setScreenScaleFactor(int screenScaleFactor )
    {
        m_ScreenScaleFactor = screenScaleFactor;
    }
    
    int                    getScreenScaleFactor() const
    {
        return m_ScreenScaleFactor;
    }
    
	void                    setGrid( Grid* pGrid )
                            {
                                m_pGrid = pGrid;
                            }
    
    vector<SinkGameObject*>& getAllSinkObjects()
                            {
                                return m_vecSinkObjects;
                            }


    void                    addSourceGameObject( SourceGameObject* pSource )
                            {
                            }
	
    int                     getSceneWidth() const
                            {  return m_SceneWidth;
                            }
    
    GameObject*             getCurrentlySelectedObject() const
                            {
                                return m_currentSelectedGameObject;
                            }
    
    int                     getSceneHeight() const
                            {
                                return m_SceneHeight;
                            }
    
    void                    changeSize( int width, int height );
    
    Grid*                   getGrid() const
                            {
                                return m_pGrid;
                            }
    
    void                    addGlowingObjects( IGlowing* pGlowing );
    bool                    removeGlowingObjects( IGlowing* pGlowing );
    
    void                    DrawQuadWithTexture( Texture* pTexture );

    void                    initLensShader();
	void                    save(const char* fileName) const;
    void                    appendSolutionData(const char* fileName) const;
    Vector2f                findSceneOffset();
    void                    adjustObjectsPositions(const Vector2f& offset);

    void                    load(const char* fileName);
    void                    load(int chapter, int stage, int level);

	Shape2D*                getObject(int i);
	void                    cycleObject();
	void                    keyProcessor(unsigned char key, int x, int y);
    GameObject*             mousePressed(const Event& event);
    void                    mouseMoved(const Event& event);
	void                    startRayTracing();
	void                    rayTrace(Ray& ray);
	void                    setDepth(unsigned traceDepth);
	void                    reset();
    void                    clearScene();
	void                    clearRays();
	void                    renderScene();
    void                    renderGuiScene();
    void                    takeScreenShot(void* pixels) const;

	void                    renderAllRays(float rayWidth = 10.0f, const Color4f& addColor = Color4f(0.0f,0.0f,0.0f,1.0f));
	void                    update();
    void                    blurTexture(int texID, float nx, float ny);
    void                    blurTexture(Texture2D* pTexture, Texture2D* pTempTexture, int blurPasses);

    void                    glowPass();
    virtual sbool           OnEvent( Event& event);
    bool                    areAllSinkIntersecting();
    
    void                    rotateSelectedObject(GameObject *pObject, float theta);
    void                    setDraggedObject();
    void                    unSetDraggedObject();
    
    
    float                   m_RefractionIndex;
	int                     currentSelectedObjectIndex;
	Shape2D                 *m_currentSelectedObject;
    GameObject              *m_currentSelectedGameObject;
    GameObject              *m_pDraggedObject;
    
	Grid                    *m_pGrid;
	
	int                     m_Button,  m_State;
	
	Shape2D                 *m_PickedObject;
	GameObject              *m_PickedGameObject;

    Vector2f                m_PickPointOffset;
	Vector2f                m_PickedObjectCentre;
    
    FrameBufferObjectTexture_2D* m_Fbo;
    
    Texture2D               *m_pGuiBackGroundTexture;
    Texture2D               *m_pGuiBackGroundTempTexture;
    
    Texture2D               *m_pFloatTextureWaveGenerator;
    Texture2D               *m_RenderTextureCurrent;
    Texture2D               *m_ReflectionTexture;
    float                   m_ReflectionTextureScale;

    Texture2D               *m_RenderTextureGlowCurrent[10];
    Texture2D               *m_RenderTextureGlowPrevious[10];
    Texture2D               *m_LineGlowTexture;
    Texture2D               *m_FinalCompositeTexture;
    Texture2D               *m_pFinalTexture;
    Texture2D               *m_pFinalTempTexture;
    
    float                   m_GlowScaleDownFactor;
    
    int                     m_TextureSizeX;
    int                     m_TextureSizeY;
    int                     m_SceneWidth, m_SceneHeight;

    ShaderProgram           *m_RenderProgram;
    
    ShaderProgram           *m_LensRenderProgram;
    ShaderProgram           *m_BlurProgram;
    ShaderProgram           *m_LineGlowProgram;
    ShaderProgram           *m_ReflectionMapProgram;
    ShaderProgram           *m_pWaveGeneratorProgram;
    
    ShaderProgram           *m_pDefaultShaderProgram;
    ShaderProgram           *m_pDefaultColorShaderProgram;
    
    FluidSimulator          *m_pFluidSimulator;
    
    float                   m_AverageLightingFactor;
    
    Texture*                m_BackGroundTexture;
    unsigned int            textureID;
    unsigned int            m_FontTextureID;

    int                     m_ReflectionMap;
    
    Texture                 *m_pSuperEllipseTexture;
    Texture                 *m_pNormalMapTexture;
    
    TextRenderer            *m_pTextRenderer;
    
    int                     m_NumSources;
    int                     m_NumShapeGameObjects;
    int                     m_NumSinkGameObjects;
    int                     m_NumPlacementMarkerGameObjects;
    int                     m_NumPortalGameObjects;
    
    bool                    m_bEnableDebugGrid;
    
    bool                    m_bAllSinkIntersecting;
    
    float                   *m_pAllRayVertices;
    float                   *m_pAllRayColors;
    float                   *m_pAllRayTexCoords;
    
    string                  m_AbsolutePath;
    
    void                    setEnableForScreenShot(bool bEnable )
    {
        m_bReadyForScreenShot = bEnable;
    }
    
    void                    rasterizeVelocity();
    void                    renderParticles();

    bool                    m_bReadyForScreenShot;

private:
    
    Vector2f   findClosestPlacementMarkerPosition( const Vector2f& position );

    void       cullRays();

    void       DrawQuad(float w, float h, float scaleX = 1.0f, float scaleY = 1.0f);
    // used for rendering objects when dragged
    void       createPrototypeObjects();
    
    const void loadSourceGameObjectInfo( FILE* pFile, SourceGameObjectInfo& sInfo ) const;

    const void loadSinkGameObjectInfo( FILE* pFile, SinkGameObjectInfo& sInfo ) const;

    const void loadShapeGameObjectInfo ( FILE* pFile, ShapeGameObjectInfo& sInfo ) const;

    const void loadCommonShapeInfo( FILE* pFile, ShapeInfo* sInfo ) const;

    const void loadPlacementMarkerGameObjectInfo( FILE* pFile, PlacementMarkerGameObjectInfo& sInfo ) const;
    
    const void loadPortalGameObjectInfo( FILE* pFile, PortalGameObjectInfo& sInfo ) const;

    void       checkIntersectionWithSinkObjects(Ray& ray, float bestHit);

    void       pushRayDataToRayVertexBuffer(float rayWidth, Ray& ray, int rayCount, bool bMoveTexCoords = true, const Color4f& addColor = Color4f(0.0f,0.0f,0.0f,1.0f));

    string     getFullPath(const string& fileName) const;
    
    void       renderGlowingRays();
    void       blurTexturePass();
    void       reflectionDrawPass();
    void       drawBackgroundAndMarkersPass();
    void       waterRipplePass();
    void       finalCompositePass();
    void       drawObjectsPass();

    void       readHints(FILE* pFile);
    
    //Matrix4f orthoMatrix;
    friend class GameLevelSelectionHUDController;
    friend class GameChapterSelectionHUDController;
    
public:
    void       loadAndSaveAllLevels();
    void       applyHint();
    GameObject* getFirstFreeObjectOfMaterialType(int);
    bool       isObjectOccupyingHintPosition( const Vector2f& position, int materialType );

    static memory::MemoryBlockAllocator *s_pBlockAllocator;
    
private:
    AABB2f      m_BoundingBox;
    map<int, HintInfo> m_HintMap;  // ObjectId and Solution Position
    map<int, GameObject*> m_ObjectMap;
};
#endif
