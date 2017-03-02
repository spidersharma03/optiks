//
//  OGL2Renderer.h
//  Optiks
//
//  Created by Prashant on 21/01/14.
//
//

#ifndef __Optiks__OGL2Renderer__
#define __Optiks__OGL2Renderer__

#include "Renderer.h"
//#include <GL/gl.h>
//#include <GL/glext.h>

class Texture;

namespace spider3d
{
	namespace render
	{
		class Ogl2Renderer:public Renderer
		{
		public:
            
			virtual void DrawPoint(const Vector2f& , const Color4f&) ;
            
			void DrawLine(const Vector2f& , const Vector2f& , Color4f&) ;
            
			void DrawRect(const AABB2f& aabb , const Color4f& color) ;
            
            virtual void DrawQuad(float width, float height, const Color4f& color);

            void DrawCircle(float radius);
            
			void setTransform(const NAMESPACE_DIRAC2D::Matrix4f& , TANSFORMATION_STATE);
            
			virtual void drawVertexPrimitiveList(unsigned vertexCount, void* coords , int vertexSize, void* colors, int colorSize, float* texcoords, int texcoordsize, int stride, PRIMITIVE_TYPE type);
            
            virtual void drawIndexVertexPrimitiveList(unsigned vertexCount, float* coords, void* colors, int colorSize, float* texcoords, int stride, void* indices, PRIMITIVE_TYPE type);

            virtual void setTextureObject(Texture*, E_TEXTURE_UNIT_STATE = ETU_0);
            
            virtual void setBlendingEnable( bool bEnableBlend );
            
            virtual void setBlendingFactors( E_BLEND_FACTOR srcFactor, E_BLEND_FACTOR destFactor );
            
            virtual void readPixels( const AABB2i& area, void* pixels);

            Ogl2Renderer();

            virtual ShaderProgram* createShaderProgram();

		private:
            
			Texture* previousAppearance;
            
			Texture* currentAppearance;
            
            //static OglRenderer* m_pRenderer;
            
		};
	}
    
}

#endif /* defined(__Optiks__OGL2Renderer__) */
