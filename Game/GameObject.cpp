//
//  GameObject.cpp
//  Optiks
//
//  Created by Prashant Sharma on 13/10/13.
//
//

#include "GameObject.h"
#include "Scene.h"
#include "Grid.h"
#include "Game.h"

int GameObject::m_ObjectCount = 0;


void GameObject::render()
{
    
}

Vector2i GameObject::getGridCoordinate() const
{
    Grid* pGrid = Game::getInstance()->getScene()->getGrid();
    return pGrid->getGridCoordinates(m_Position);
}