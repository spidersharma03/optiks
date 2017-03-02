/*
 *  ISavable.h
 *  Optiks
 *
 *
 */

#ifndef _ISAVABLE_H_
#define _ISAVABLE_H_

#include <stdio.h>

class ISavable
{
public:
	virtual void save(FILE* pFile) = 0;
	
	virtual void load(FILE* pFile) = 0;
};

#endif