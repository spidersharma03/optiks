/*
 *  Grid.h
 *  Optiks
 *
 *
 */

#ifndef _GRID_H_
#define _GRID_H_

#include "IRenderable.h"
#include "Vector2.h"

USE_NAMESPACE_DIRAC2D

#define INDEX( I, J, N ) ((I) + ((N) + 2)*(J) )


enum GRID_TYPE { RECT, HEX, TRI };

class Shape2D;

class Grid : public IRenderable
{
public:
	Grid( int nx, int ny, int cellSize ) : nx(nx), ny(ny), m_CellSize(cellSize)
	{
		m_Occupancy             = new int[nx*ny];
		m_ColorCode             = new int[nx*ny];
        m_PlacementMarkerArray  = new int[nx*ny];
		
		for( int i=0; i<nx*ny; i++ )
		{
			m_Occupancy[i]             = 0;
			m_ColorCode[i]             = 0;
            m_PlacementMarkerArray[i]  = 0;
		}
	}
	
    void resetGrid()
    {
        for( int i=0; i<nx*ny; i++ )
		{
			m_Occupancy[i]             = 0;
			m_ColorCode[i]             = 0;
            m_PlacementMarkerArray[i]  = 0;
		}
    }
    
    void setOrigin(const Vector2f& origin )
    {
        m_Origin = origin;
    }
    
	GRID_TYPE getGridType()
	{
		return m_GridType;
	}
	
    float getCellSize() const
    {
        return (float)m_CellSize;
    }
    
	virtual void render() = 0;

	virtual bool snap(const Vector2f& inPoint, Vector2f& outPoint) = 0;

    virtual bool snapSource(const Vector2f& inPoint, Vector2f& outPoint) = 0;

	virtual bool snap(Shape2D* pShape, Vector2f& outPoint) = 0;

    // Get grid coordinates for the input point.
	virtual Vector2i getGridCoordinates(const Vector2f& inPoint) = 0;

    virtual Vector2f getWorldCoordinatesFromGrid(const Vector2f& inPoint) = 0;

	virtual bool isOccupied( int x, int y, int colorCode )
	{
		return false;
	}
    
    virtual void setOccupied( int x, int y, int colorCode )
	{
        m_Occupancy[INDEX(x, y, nx)] = 1;
	}

    virtual bool isPlacementMarked( int x, int y, int colorCode )
	{
		return false;
	}
	
    virtual void setPlacementMarked( int x, int y, int colorCode )
	{
	}
    
	void resetOccupancy( int x, int y );
	
    void resetPlacementMark( int x, int y );
    
	virtual ~Grid()
	{
		delete[] m_Occupancy;
		delete[] m_ColorCode;
	}
protected:
	int nx,ny;
	GRID_TYPE m_GridType;
	int m_CellSize;
    
	int* m_Occupancy;
    int* m_PlacementMarkerArray;
	int* m_ColorCode;
	
	int m_ScreenWidth, m_ScreenHeight;
    
    Vector2f m_Origin;
};


class RectGrid : public Grid
{
public:
	
	RectGrid( int nx, int ny, int cellSize );

	virtual void render();
	
	virtual bool snap(const Vector2f& inPoint, Vector2f& outPoint);

    virtual bool snapSource(const Vector2f& inPoint, Vector2f& outPoint);

	virtual bool snap(Shape2D* pShape, Vector2f& outPoint);

	virtual Vector2i getGridCoordinates(const Vector2f& inPoint);

    virtual Vector2f getWorldCoordinatesFromGrid(const Vector2f& inPoint);

	virtual bool isOccupied( int x, int y, int colorCode );
    
    virtual bool isPlacementMarked( int x, int y, int colorCode );
    
    virtual void setPlacementMarked( int x, int y, int colorCode );

    virtual void setOccupied( int x, int y, int colorCode );
};

class HexGrid : public Grid
{
public:
	
	HexGrid( int nx, int ny, int cellSize );
	
	virtual void render() = 0;
	
	virtual bool snap(const Vector2f& inPoint, Vector2f& outPoint);
	
    virtual bool snapSource(const Vector2f& inPoint, Vector2f& outPoint);

	virtual bool snap(Shape2D* pShape, Vector2f& outPoint);

	virtual Vector2i getGridCoordinates(const Vector2f& inPoint);

    virtual Vector2f getWorldCoordinatesFromGrid(const Vector2f& inPoint);
};

class TriGrid : public Grid
{
public:
	
	TriGrid( int nx, int ny, int cellSize );
	
	virtual void render() = 0;
	
	virtual bool snap(const Vector2f& inPoint, Vector2f& outPoint);
	
    virtual bool snapSource(const Vector2f& inPoint, Vector2f& outPoint);

	virtual bool snap(Shape2D* pShape, Vector2f& outPoint);

	virtual Vector2i getGridCoordinates(const Vector2f& inPoint);

    virtual Vector2f getWorldCoordinatesFromGrid(const Vector2f& inPoint);

};
#endif