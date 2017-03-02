/*
 *  ObjectGenerator.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 16/11/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "ObjectFactory.h"


ObjectFactory::ObjectFactory()
{    
	m_pBlockAllocator = new memory::MemoryBlockAllocator();
}

GameObject* ObjectFactory::createObject(GameObjectInfo& gInfo)
{
	GameObject* pObject = 0;
	
	switch (gInfo.m_ObjectType) {
		
		default:
			break;
	}
	return pObject;
}


void ObjectFactory::destroyObject( GameObject* pObject )
{
	switch (pObject->getGameObjectInfo().m_ObjectType) {
		
		default:
			break;
	}
}

