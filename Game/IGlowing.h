//
//  IGlowing.h
//  Optiks
//
//  Created by Prashant on 12/03/14.
//
//

#ifndef Optiks_IGlowing_h
#define Optiks_IGlowing_h

#include <vector>
#include "Ray.h"

using namespace std;

class IGlowing
{
public:
    
    virtual vector<Ray>& getGlowingRays() = 0;
};

#endif
