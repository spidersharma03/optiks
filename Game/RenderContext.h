//
//  RenderContext.h
//  Optiks
//
//  Created by Prashant on 21/01/14.
//
//

#ifndef __Optiks__RenderContext__
#define __Optiks__RenderContext__

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

namespace spider3d {
    namespace render
    {
        class Renderer;
    }
}

using namespace spider3d::render;

class RenderContext
{
public:
    
    void            Init( const RenderContextParams& params );
    
    Renderer*       getRenderer() const;
    
    RenderContext*  getInstance()
    {
        return s_pRenderContext;
    }
private:
    
    static  RenderContext* s_pRenderContext;
    
    RenderContext();

    Renderer        *m_pRenderer;
    int             m_Width;
    int             m_Height;
};

//RenderContext* createRenderContext( const RenderContextParams& params )
//{
//    return new RenderContext();
//}

#endif /* defined(__Optiks__RenderContext__) */
