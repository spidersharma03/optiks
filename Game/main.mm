
#include <math.h>
#include "MathUtil.h"

#include "Scene.h"
#include "ShapeGameObject.h"
#include "SourceGameObject.h"
#include "Shape2D.h"
#include "Ray.h"
#include <GLUT/GLUT.h>
#include <stdio.h>
#include <vector>
#include "DTimer.h"
#include "Game.h"
#include "Renderer.h"
#include "GameMenuState.h"
#include "GameEditorState.h"
#include "GamePlayState.h"
#include "FileLoader.h"

#include "TextureMaker.h"

#include "BlackMatter.h"

#include "Grid.h"
#include "Source.h"

#include "GUI.h"
#include <dirent.h>
#include <iostream>
#include "SourceCodeShader.h"
#include "ShaderProgram.h"

#include "GameChapterSelectionState.h"

using namespace std;
using namespace spider3d::gui;


float width,height;
int specialKey;


int vp[4];
float fov;

float dt = 0.08f;

int windowWidth = 1024;
int windowHeight = 768;

GUIManager* pGUIManager = new GUIManager(windowWidth, windowHeight);


static bool bOnce = false;

Scene *scene;

void changeSize(int w, int h) 
{
	
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	windowWidth = w;
	windowHeight = h;
	
	if(h == 0)
		h = 1;
	
	// we're storing these values for latter use in the 
	// mouse motion functions
	width = w;
	height = h;
	float ratio = 1.0* w / h;
	
	
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);
	
	// Set the correct perspective.
	//gluPerspective(45,ratio,10.0f,100.0f);
	float left, right, top, bottom;

	fov = M_PI/4;
	top =   0.01f * tan(fov/2);
	bottom = -top;
	left = -ratio * top;
	right = -left;
	vp[0] = 0;
	vp[1] = 0;
	vp[2] = width;
	vp[3] = height;
		
    Game::getInstance()->getScene()->changeSize(windowWidth, 768);
}


DTimer timer1;

void initScene()
{    
    scene = new Scene(windowWidth, windowHeight);
    RectGrid *rectGrid = new RectGrid(15,15,BASIC_ELEMENT_IMAGE_SIZE);
    //RectGrid *rectGrid = new RectGrid(15,15,(float)windowWidth/15);
	scene->setGrid(rectGrid);

    Game::getInstance()->setScene(scene);
    
    ShaderProgram *pDefaultShaderProgram;
    
    if( Game::getInstance()->getRenderer()->getRendererType() == ERT_OPENGL )
    {
        SourceCodeShader* shaderDefaultVS = new SourceCodeShader(EST_VERTEX, ESL_GLSL,"/Users/prashantsharma/Downloads/Backup_MacBookPro/Optiks/DefaultVertexShaderGLSL.vs" );
        
        SourceCodeShader* shaderDefaultFS = new SourceCodeShader(EST_FRAGMENT, ESL_GLSL, "/Users/prashantsharma/Downloads/Backup_MacBookPro/Optiks/DefaultFragmentShaderGLSL.fs" );
        
        pDefaultShaderProgram = Game::getInstance()->getRenderer()->createShaderProgram();
        pDefaultShaderProgram->setShaders(shaderDefaultVS, shaderDefaultFS);
    }
    else
    {
        SourceCodeShader* shaderDefaultVS = new SourceCodeShader(EST_VERTEX, ESL_GLSL,"/Users/prashantsharma/Downloads/Backup_MacBookPro/Optiks/DefaultVertexShaderGLSL2.vs" );
        
        SourceCodeShader* shaderDefaultFS = new SourceCodeShader(EST_FRAGMENT, ESL_GLSL, "/Users/prashantsharma/Downloads/Backup_MacBookPro/Optiks/DefaultFragmentShaderGLSL2.fs" );
        
        pDefaultShaderProgram = Game::getInstance()->getRenderer()->createShaderProgram();
        pDefaultShaderProgram->setShaders(shaderDefaultVS, shaderDefaultFS);
    }
    ShaderUniform* su = new ShaderUniform();
    su->m_VarType = ESVT_SINGLE_INT;
    su->m_Name = (char*)"TextureSampler";
    su->m_Value = new float;
    *su->m_Value = 0;
    pDefaultShaderProgram->addShaderUniform(su);
    pGUIManager->m_pDefaultShaderProgram = pDefaultShaderProgram;
    
    Game::getInstance()->setGuiManager(pGUIManager);
    Game::getInstance()->changeState(GameChapterSelectionState::getInstance());
    //Game::getInstance()->changeState(GamePlayState::getInstance());
    
    Texture2D* pTexture = (Texture2D*)TextureLoader::Instance()->getTexture("/Users/prashantsharma/Downloads/Backup_MacBookPro/Optiks/Background_GreySlate.png");
    
	//GLuint textureBack =  SZGLLoadTexture( @"/Users/prashant/Desktop/Optiks/Background_GreySlate.png");
	scene->m_BackGroundTexture = pTexture;
    
    Color4f color;
	color.r = 0.99f;
	color.g = 0.6f;
	color.b = 0.6f;
    
    
	Texture* pRayTexture = TextureMaker::createLaserBeamTexture1( 128, 32, color, 50, gaussianLaser );
	
	Ray::m_pTexture = pRayTexture;

    //scene->loadAndSaveAllLevels();
}

bool operation = false;

void renderScene(void) 
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT );
   	
    Game::getInstance()->update(dt);

	glutSwapBuffers();
}

void processMouse(int button, int state, int x, int y) 
{	
    Event event;
    event.eventType = spider3d::SMOUSE_EVENT;
    
    event.mouseInput.X = x;
    event.mouseInput.Y = windowHeight - y;
    
	if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
        event.mouseInput.Event = spider3d::LEFT_MOUSE_PRESSED_DOWN;
	}
	
	if ( button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
        event.mouseInput.Event = spider3d::LEFT_MOUSE_LEFT_UP;
    }
	
	specialKey = glutGetModifiers();
	// if both a mouse button, and the ALT key, are pressed  then
	if ((state == GLUT_DOWN) && (specialKey == GLUT_ACTIVE_ALT)) {
	}
    
    Game::getInstance()->OnEvent(event);
}

void processMouseActiveMotion(int x, int y)
{
	Event event;
    event.eventType = spider3d::SMOUSE_EVENT;
    event.mouseInput.Event = spider3d::SMOUSE_DRAGGED;
    event.mouseInput.X = x;
    event.mouseInput.Y = windowHeight - y;

    Game::getInstance()->OnEvent(event);
}

void processMousePassiveMotion(int x, int y) {
	
	// User must press the SHIFT key to change the 
	// rotation in the X axis
	if (specialKey != GLUT_ACTIVE_SHIFT) {
		
		
	}
}

void processMouseEntry(int state) 
{
	
}


void processSpecialKeys( int key, int x, int y)
{
    Event event;
    event.eventType = spider3d::SKEY_EVENT;
    event.keyInput.Char = key;
    event.keyInput.PressedDown = true;
    specialKey = glutGetModifiers();
    if( specialKey == GLUT_ACTIVE_SHIFT )
    {
        event.keyInput.Shift = true;
    }
    if( specialKey == GLUT_ACTIVE_CTRL )
    {
        event.keyInput.Control = true;
    }
    if( specialKey == GLUT_ACTIVE_ALT )
    {
        event.keyInput.Alt = true;
    }
    if( specialKey == GLUT_KEY_LEFT )
    {
        event.keyInput.Key = KEY_LEFT;
    }
    if( specialKey == GLUT_KEY_RIGHT )
    {
        event.keyInput.Key = KEY_RIGHT;
    }
}

void processNormalKeys(unsigned char key, int x, int y) {
	bOnce = true;
    
    Event event;
    event.mouseInput.X = x;
    event.mouseInput.Y = windowHeight - y;
    
    event.eventType = spider3d::SKEY_EVENT;
    event.keyInput.Char = key;
    event.keyInput.PressedDown = true;
    if( key == 13 )
    {
        event.keyInput.Key = KEY_RETURN;
    }
    specialKey = glutGetModifiers();
    if( key == 127 )
    {
        event.keyInput.Key = KEY_DELETE;
    }
    if( specialKey == GLUT_ACTIVE_SHIFT )
    {
        event.keyInput.Shift = true;
    }
    if( specialKey == GLUT_ACTIVE_CTRL )
    {
        event.keyInput.Control = true;
    }
    if( specialKey == GLUT_ACTIVE_ALT )
    {
        event.keyInput.Alt = true;
    }
    Game::getInstance()->OnEvent(event);
}

void processUpKey(unsigned char key, int x, int y)
{
	bOnce = false;
    Event event;
    event.eventType = spider3d::SKEY_EVENT;
    event.keyInput.Char = key;
    event.keyInput.PressedDown = false;
    if( specialKey == GLUT_ACTIVE_SHIFT )
    {
        event.keyInput.Shift = true;
    }
    if( specialKey == GLUT_ACTIVE_CTRL )
    {
        event.keyInput.Control = true;
    }
    if( specialKey == GLUT_ACTIVE_ALT )
    {
        event.keyInput.Alt = true;
    }
    Game::getInstance()->OnEvent(event);
}



int main(int argc, char **argv)
{
//    DIR *dir = opendir("/Users/prashantsharma/Desktop/Optiks");
//    struct dirent *ent;
//    
//    if(dir)
//    {
//        while((ent = readdir(dir)) != NULL)
//        {
//            cout << (ent->d_name) << endl;
//        }
//    }
//    else
//    {
//        cout << "Error opening directory" << endl;
//    }
//    
//    while ((ent = readdir (dir)) != NULL) {
//        printf ("%s\n", ent->d_name);
//    }
//    closedir (dir);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(windowWidth,windowHeight);
	glutCreateWindow("SnowMen@Lighthouse3D");
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(processNormalKeys);
	glutKeyboardUpFunc(processUpKey);
	//glutIgnoreKeyRepeat(1);
	glutMouseFunc(processMouse);
	glutMotionFunc(processMouseActiveMotion);
	glutPassiveMotionFunc(processMousePassiveMotion);
	glutEntryFunc(processMouseEntry);
    glutSpecialFunc(processSpecialKeys);
	//glutFullScreen();
	
	initScene();
	
	glutMainLoop();
	
	return 0;
}

//
//#define SWAP( A, B )  \
//{                     \
//int TEMP = A;     \
//A = B;            \
//B = TEMP;         \
//}                     \
//
//void FluidSimulator:: RasterizeVelocity( const Vector2i& start, const Vector2i& end )
//{
//    // return;
//    
//    Vector2i lineBegin = start;//GetGridCoordinate(start + Vector2f(16,16) );
//    Vector2i lineEnd   = end;//GetGridCoordinate(end   + Vector2f(16,16) );
//    
//    Vector2f direction = Vector2f(lineEnd.x, lineEnd.y) - Vector2f(lineBegin.x, lineBegin.y);
//    direction.normalize();
//    float velocityMagnitude = 100.0f;
//    
//    int dy = end.y - lineBegin.y;
//    int dx = end.x - lineBegin.x;
//    
//    bool bSteep = abs(dy) > abs(dx);
//    
//    if( bSteep )
//    {
//        // Swap X0,y)
//        SWAP( lineBegin.x, lineBegin.y );
//        SWAP( lineEnd.x  , lineEnd.y );
//        dy = lineEnd.y - lineBegin.y;
//        dx = lineEnd.x - lineBegin.x;
//    }
//    if( lineBegin.x > lineEnd.x )
//    {
//        SWAP( lineBegin.x, lineEnd.x );
//        SWAP( lineBegin.y, lineEnd.y );
//        dy *= -1;
//        dx *= -1;
//    }
//    int yStep = 1;
//    if( lineBegin.y > lineEnd.y )
//    {
//        yStep = -1;
//    }
//    
//    
//    int E = 0;
//    int y = lineBegin.y;
//    
//    for( int x=lineBegin.x; x<=lineEnd.x; x++)
//    {
//        //int tileID = x * 64 + y;
//        if( bSteep )
//        {
//            //            vx[INDEX(y, x, nX)] = direction.x * velocityMagnitude;
//            //            vy[INDEX(y, x, nX)] = direction.y * velocityMagnitude;
//            addExtImpulse(direction.x * velocityMagnitude, direction.y * velocityMagnitude, y, x, 10000, 0.016);
//        }
//        //  tileID = PositionToIndex( Vector2i(y*32,x*32) );
//        else
//        {
//            //            vx[INDEX(x, y, nX)] = direction.x * velocityMagnitude;
//            //            vy[INDEX(x, y, nX)] = direction.y * velocityMagnitude;
//            addExtImpulse(direction.x * velocityMagnitude, direction.y * velocityMagnitude, x, y, 10000, 0.016);
//            //  tileID = PositionToIndex( Vector2i(x*32,y*32) );
//        }
//        //printf("\n Tile Pos: %d,%d", x,y);
//        //vecTileIDs.push_back(tileID);
//        
//        E += dy*yStep;
//        //printf("\n 2*E=%d, dx*yStep = %d", E*2,dx*yStep);
//        
//        if( (E << 1) >=dx * yStep )
//        {
//            y+=yStep;
//            E -= dx;
//        }
//    }
//}


//void FluidSimulator::syncParticles(Particle* pParticles )
//{
//    int sceneWidth = 1024;
//    int sceneHeight = 768;
//    float x,y;
//	int nbrx0, nbrx1;
//	int nbry0, nbry1;
//	float ds1, ds_1, dt1, dt_1;
//    float dt = 0.016f;
//    
//    Vector2f samplePosition;
//    
//    for( int i=0; i<MAX_PARTICLES; i++ )
//    {
//        Particle& particle = pParticles[i];
//        // sample velocity at particle's position
//        samplePosition = particle.position;
//        samplePosition.x /= sceneWidth;
//        samplePosition.y /= sceneHeight;
//        samplePosition *= nX;
//        //int ix = samplePosition.x;
//        //int jy = samplePosition.y;
//        
//        x = samplePosition.x;// - vx[INDEX(ix,jy,nX)] * dt;
//        y = samplePosition.y;// - vy[INDEX(ix,jy,nX)] * dt;
//        //CLAMP( x, 0.5f, nX + 0.5f )
//        x = x < 0.5f ? 0.5f : x;
//        x = x > (nX + 0.5f) ? nX : x;
//        
//        nbrx0 = (int)x; nbrx1 = nbrx0 + 1;
//        //CLAMP ( y, 0.5f, nX + 0.5f )
//        
//        y = y < 0.5f ? 0.5f : y;
//        y = y > (nX + 0.5f) ? nX : y;
//        
//        nbry0 = (int)y; nbry1 = nbry0 + 1;
//        ds1 = x - nbrx0; ds_1 = 1 - ds1; dt1 = y - nbry0; dt_1 = 1-dt1;
//        
//        
//        particle.velocity.x = ds_1 * ( dt_1 * vx[INDEX(nbrx0,nbry0,nX)] + dt1 * vx[INDEX(nbrx0,nbry1,nX)] )
//        + ds1 * ( dt_1 * vx[INDEX(nbrx1,nbry0,nX)] + dt1 * vx[ INDEX(nbrx1,nbry1,nX)] );
//        
//        particle.velocity.y = ds_1 * ( dt_1 * vy[INDEX(nbrx0,nbry0,nX)] + dt1 * vy[INDEX(nbrx0,nbry1,nX)] )
//        + ds1 * ( dt_1 * vy[INDEX(nbrx1,nbry0,nX)] + dt1 * vy[ INDEX(nbrx1,nbry1,nX)] );
//        
//        particle.position += particle.velocity * dt;
//    }
//}



