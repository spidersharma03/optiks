//
//  RenderContext.cpp
//  Optiks
//
//  Created by Prashant on 21/01/14.
//
//

#include "RenderContext.h"
#include "OglRenderer.h"


RenderContext::RenderContext()
{
}

void RenderContext::Init( const RenderContextParams& params )
{
    m_Width  = params.contextWidth;
    m_Height = params.contextHeight;
    switch (params.renderType)
    {
        case ERT_OGL:
            //m_pRenderer = new OglRenderer();
            break;
        case ERT_OGL2:
            break;
        case ERT_OGLES2:
            break;
        default:
            break;
    }
}

Renderer* RenderContext::getRenderer() const
{
    return m_pRenderer;
}