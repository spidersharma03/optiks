/*
 *  Grid.cpp
 *  Optiks
 *
 *
 */

#include "Grid.h"
#include "Shape2D.h"
#include <stdio.h>
#include "Game.h"
#include "OglRenderer.h"

using namespace spider3d::render;

void Grid::resetOccupancy( int x, int y )
{
    int sx = (x-m_Origin.x)/m_CellSize;
    int sy = (y-m_Origin.y)/m_CellSize;
    
    m_Occupancy[INDEX(sx,sy,nx)] = 0;
}

void Grid::resetPlacementMark( int x, int y )
{
    int sx = x/m_CellSize;
    int sy = y/m_CellSize;
    
    m_PlacementMarkerArray[INDEX(sx,sy,nx)] = 0;
}

RectGrid::RectGrid( int nx, int ny, int cellWidth )  : Grid(nx, ny, cellWidth)
{
	m_ScreenWidth  = 1280;
	m_ScreenHeight = 800;
}

void RectGrid::render()
{
    return;
    
    NAMESPACE_DIRAC2D::Matrix4f xForm;
    Game::getInstance()->getRenderer()->setTransform(xForm, spider3d::WORLD);

	float x = 0.0f; 
	float y = 0.0f; 
		
    Vector2f lineVertices[200];
    
    int count = 0;
	for( int i=0; i<nx; i++ )
	{
		lineVertices[count].x = x;
        lineVertices[count++].y = 0;
        lineVertices[count].x = x;
        lineVertices[count++].y = m_ScreenHeight;
		x += m_CellSize;
	}
	
	for( int i=0; i<ny; i++ )
	{
		lineVertices[count].x = 0;
        lineVertices[count++].y = y;
        lineVertices[count].x = m_ScreenWidth;
        lineVertices[count++].y = y;
        y += m_CellSize;
	}
    Game::getInstance()->getRenderer()->drawVertexPrimitiveList(nx*4, lineVertices, 2, 0, 0, 0, 0, 0, spider3d::EPT_LINE);
}

bool RectGrid::snap(const Vector2f& inPoint, Vector2f& outPoint)
{
    Vector2f point = inPoint - m_Origin;
    
	int sx = point.x/m_CellSize;
	int sy = point.y/m_CellSize;
	
	outPoint.x = (sx+0.5)*m_CellSize + m_Origin.x;
	outPoint.y = (sy+0.5)*m_CellSize + m_Origin.y;
	
	return true;
}

// Sources/Sinks are snapped in a different way than the objects.
bool RectGrid::snapSource(const Vector2f& inPoint, Vector2f& outPoint)
{
    Vector2f point = inPoint - m_Origin;

    // Find the closest snap point for shifted grid in -X direction by half cell width
	int sx = (point.x+m_CellSize*0.5f)/m_CellSize;
	int sy = point.y/m_CellSize;
	
    Vector2f p0;
	p0.x = sx*m_CellSize + m_Origin.x;
	p0.y = (sy+0.5f)*m_CellSize + m_Origin.y;
	
    // Find the closest snap point for shifted grid in +Y direction by half cell width
    sx = (point.x)/m_CellSize;
    sy = (point.y+m_CellSize*0.5f)/m_CellSize;
	
    Vector2f p1;
	p1.x = (sx+0.5f)*m_CellSize + m_Origin.x;
	p1.y = sy*m_CellSize + m_Origin.y;

    // Find the closest snap point from the inPoint.
    if( point.distanceSquared(p0) < point.distanceSquared(p1) )
        outPoint = p0;
    else
        outPoint = p1;
    
    return true;
}

bool RectGrid::snap(Shape2D* pShape, Vector2f& outPoint)
{
	int sx = 0;// = inPoint.x/(dx);
	int sy = 0;// = inPoint.y/(dx);
	
	if( m_Occupancy[INDEX(sx,sy,nx)] == 1 )
	{
		return false;
	}
	
	m_Occupancy[INDEX(sx,sy,nx)] = 1;
    m_PlacementMarkerArray[INDEX(sx, sy, nx)] = 1;
	outPoint.x = (sx+0.5)*m_CellSize;
	outPoint.y = (sy+0.5)*m_CellSize;
	
	return true;
}

Vector2i RectGrid::getGridCoordinates(const Vector2f& inPoint)
{
    Vector2f point = inPoint - m_Origin;

	int sx = point.x/m_CellSize;
	int sy = point.y/m_CellSize;
	return Vector2i(sx,sy);
}

Vector2f RectGrid::getWorldCoordinatesFromGrid(const Vector2f& inPoint)
{
    Vector2f point = inPoint - m_Origin;

    return Vector2f(point.x*m_CellSize, point.y*m_CellSize);
}

bool RectGrid::isOccupied( int x, int y, int colorCode )
{
    //m_Origin.setZero();

	int sx = (x-m_Origin.x)/m_CellSize;
	int sy = (y-m_Origin.y)/m_CellSize;
	
	bool bRes = m_Occupancy[INDEX(sx,sy,nx)] == 1;// && m_ColorCode[INDEX(sx,sy,nx)] == colorCode;
    return bRes;
}

bool RectGrid::isPlacementMarked( int x, int y, int colorCode )
{
    //m_Origin.setZero();

	int sx = (x-m_Origin.x)/m_CellSize;
	int sy = (y-m_Origin.y)/m_CellSize;
	
	bool bRes = m_PlacementMarkerArray[INDEX(sx,sy,nx)] == 1;// && m_ColorCode[INDEX(sx,sy,nx)] == colorCode;
    return bRes;
}

void RectGrid::setPlacementMarked( int x, int y, int colorCode )
{
    m_Origin.setZero();
	int sx = (x-m_Origin.x)/m_CellSize;
	int sy = (y-m_Origin.y)/m_CellSize;
	
    m_PlacementMarkerArray[INDEX(sx,sy,nx)] = 1;
}

void RectGrid::setOccupied( int x, int y, int colorCode )
{
    //m_Origin.setZero();

	int sx = (x-m_Origin.x)/m_CellSize;
	int sy = (y-m_Origin.y)/m_CellSize;
	
    m_Occupancy[INDEX(sx,sy,nx)] = 1;
}

HexGrid::HexGrid( int nx, int ny, int cellSize )  : Grid(nx, ny, cellSize)
{
	
}

void HexGrid::render()
{
}

bool HexGrid::snap(const Vector2f& inPoint, Vector2f& outPoint)
{
	return false;
}

bool HexGrid::snapSource(const Vector2f& inPoint, Vector2f& outPoint)
{
    return false;
}

bool HexGrid::snap(Shape2D* pShape, Vector2f& outPoint)
{
	return false;
}

Vector2i HexGrid::getGridCoordinates(const Vector2f& inPoint)
{
	return Vector2i();
}

Vector2f HexGrid::getWorldCoordinatesFromGrid(const Vector2f& inPoint)
{
    return Vector2f();
}

TriGrid::TriGrid( int nx, int ny, int cellSize )  : Grid(nx, ny, cellSize)
{
	
}

void TriGrid::render()
{
}

bool TriGrid::snap(const Vector2f& inPoint, Vector2f& outPoint)
{
	return false;
}

bool TriGrid::snapSource(const Vector2f& inPoint, Vector2f& outPoint)
{
    return false;
}

bool TriGrid::snap(Shape2D* pShape, Vector2f& outPoint)
{
	return false;
}

Vector2i TriGrid::getGridCoordinates(const Vector2f& inPoint)
{
	return Vector2i();
}

Vector2f TriGrid::getWorldCoordinatesFromGrid(const Vector2f& inPoint)
{
    return Vector2f();
}
