/*
 *  ObjectGenerator.h
 *  Dirac2D
 *
 *  Created by Prashant on 16/11/12.
 *
 */


#ifndef _OBJECT_FACTORY_H_
#define _OBJECT_FACTORY_H_

#include "GameObject.h"
#include "MemoryBlockAllocator.h"


class ObjectFactory
{
public:
	
	ObjectFactory();
		
	GameObject* createObject(GameObjectInfo& gInfo);

	void destroyObject( GameObject* pObject );
	        		
private:
	
    memory::MemoryBlockAllocator *m_pBlockAllocator;
};

#endif