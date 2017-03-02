#ifndef _RENDERER_H

#define _RENDERER_H

#include "Rectangle.h"
#include "Matrix4.h"
#include "Color4.h"
#include "AABB2.h"

using namespace NAMESPACE_DIRAC2D;

class ShaderProgram;
class Texture;

namespace spider3d
{
	enum RENDERER_TYPE { ERT_OPENGL = 0 , ERT_OPENGL2,  ERT_D3D, ERT_OPENGLES2 };

	enum TANSFORMATION_STATE { WORLD = 0 , VIEW , PROJECTION , TEXTURE };

    enum PRIMITIVE_TYPE { EPT_TRIANGLE, EPT_TRIANGLE_FAN, EPT_LINE, EPT_POINT };
    
	enum BUFFER_BIT { COLOR_BUFFER_BIT = 0x01 , DEPTH_BUFFER_BIT = 0x02 , STENCIL_BUFFER_BIT = 0x04};

    enum E_TEXTURE_UNIT_STATE { ETU_0, ETU_1, ETU_2, ETU_3, ETU_4, ETU_5, ETU_6, ETU_7 };
    
    enum E_BLEND_FACTOR { EBF_ONE, EBF_ZERO, EBF_SRC_ALPHA, EBF_ONE_MINUS_SRC_ALPHA };
    
	namespace render
	{
		class FrameBufferObject;

		class Renderer
		{
		public:
            Renderer()
            {
                m_pCurrentShaderProgram = 0;
            }

			virtual void DrawPoint(const Vector2f& , const Color4f&) = 0;

			virtual void DrawLine(const Vector2f& , const Vector2f& , Color4f&) = 0;

			virtual void DrawRect(const AABB2f& , const Color4f&) = 0;

            virtual void DrawQuad(float width, float height, const Color4f& color = Color4f(1.0f,1.0f,1.0f,1.0f)) = 0;

            virtual void DrawCircle(float radius) = 0;

			virtual void setTransform(const Matrix4f& , TANSFORMATION_STATE ) = 0;

			virtual void setTextureObject(Texture*, E_TEXTURE_UNIT_STATE = ETU_0) = 0;

            virtual void drawVertexPrimitiveList(unsigned vertexCount, void* coords , int vertexSize, void* colors, int colorSize, float* texcoords, int texcoordsize, int stride, PRIMITIVE_TYPE type) = 0;

            virtual void drawIndexVertexPrimitiveList(unsigned vertexCount, float* coords, void* colors, int colorSize, float* texcoords, int stride, void* indices, PRIMITIVE_TYPE type)=0;

            virtual void setBlendingEnable( bool bEnableBlend ) = 0;

            virtual void setBlendingFactors( E_BLEND_FACTOR srcFactor, E_BLEND_FACTOR destFactor ) = 0;

            virtual ShaderProgram* createShaderProgram() = 0;
            
            virtual void readPixels( const AABB2i& area, void* pixels) = 0;
            
            void setCurrentShader( ShaderProgram* pShaderProgram )
            {
                m_pCurrentShaderProgram = pShaderProgram;
            }
            
            RENDERER_TYPE getRendererType() const
            {
                return m_RendererType;
            }
            
		protected:
            
            RENDERER_TYPE    m_RendererType;
            
            ShaderProgram   *m_pCurrentShaderProgram;
			//virtual void setRenderState(Appearance*) = 0;
		};

	}
}
#endif 