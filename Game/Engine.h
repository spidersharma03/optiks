//
//  Engine.h
//  Optiks
//
//  Created by Prashant on 03/02/14.
//
//

#ifndef __Optiks__Engine__
#define __Optiks__Engine__

#include <iostream>

enum E_RENDERER_TYPE { ERT_OGL, ERT_OGL2, ERT_OGLES2 };

struct RenderContextParams
{
    RenderContextParams()
    {
        contextWidth  = 0;
        contextHeight = 0;
        renderType = ERT_OGL;
    }
    
    int                 contextWidth;
    int                 contextHeight;
    E_RENDERER_TYPE     renderType;
};

class Engine
{
private:
    
    Engine();
    
public:
    
    static void InitEngine(const RenderContextParams&);
    
private:
        
};

#endif /* defined(__Optiks__Engine__) */
