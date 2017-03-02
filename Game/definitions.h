/*
 *  definitions.h
 *  Dirac2D
 *
 *  Created by Prashant on 17/04/12.
 *
 */


#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#define BEGIN_NAMESPACE_DIRAC2D namespace NAMESPACE_DIRAC2D {
#define END_NAMESPACE_DIRAC2D }

#define USE_NAMESPACE_DIRAC2D using namespace NAMESPACE_DIRAC2D;

#define FORWARD_DECLARE_CLASS(classname) \
BEGIN_NAMESPACE_DIRAC2D \
class classname; \
END_NAMESPACE_DIRAC2D \

#define FORWARD_DECLARE_STRUCTURE(structname) \
BEGIN_NAMESPACE_DIRAC2D \
struct structname; \
END_NAMESPACE_DIRAC2D \

#define EPSILON 1e-15

#define PI 3.14159265358979f
#define PI_4 (PI/4.0f)
#define PI_2 (PI/2.0f)


typedef long dint64;
typedef unsigned long duint64;
typedef int dint32;
typedef unsigned int duint32;
typedef short dint16;
typedef unsigned short duint16;
typedef char dchar;
typedef unsigned char duchar;
typedef float dfloat;
typedef double ddouble;
typedef bool dbool;

#endif