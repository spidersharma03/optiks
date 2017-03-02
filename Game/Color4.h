//
//  Header.h
//  Optiks
//
//  Created by Prashant Sharma on 15/06/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "definitions.h"
#include "math.h"


#ifndef _COLOR4_H_
#define _COLOR4_H_

BEGIN_NAMESPACE_DIRAC2D


template< class T >
class Color4
{
public:
    
    Color4()
    {
        r = g = b = a = 1.0f;
    }
    
    Color4(T r, T g, T b, T a):r(r), g(g), b(b), a(a)
    {
        
    }
    
    void set(T _r, T _g, T _b, T _a)
    {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }

    Color4<T> operator*( T c )
    {
        return Color4<T>(r*c, g*c, b*c, a*c);
    }
    
    Color4<T> operator+( const Color4<T>& other )
    {
        return Color4<T>(r+other.r, g+other.g, b+other.b, a+other.a);
    }

    Color4<T> operator-( const Color4<T>& other )
    {
        return Color4<T>(r-other.r, g-other.g, b-other.b, a-other.a);
    }
    
    void operator+=( const Color4<T>& other )
	{
		r += other.r;
		g += other.g;
		b += other.b;
        a += other.a;
	}
    
    void operator*=( T c )
	{
		r *= c;
		g *= c;
		b *= c;
        a *= c;
	}
    
    T r,g,b,a;
};

typedef Color4<float>  Color4f;

END_NAMESPACE_DIRAC2D

#endif
