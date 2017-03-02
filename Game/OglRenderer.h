#ifndef _OGLRENDERER_H

#define _OGLRENDERER_H

#include "Renderer.h"
//#include <GL/gl.h>
//#include <GL/glext.h>

class Texture;

namespace spider3d
{
	namespace render
	{		
		class OglRenderer:public Renderer
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

            virtual ShaderProgram* createShaderProgram();

            OglRenderer();

		private:
            
			Texture* previousAppearance;

			Texture* currentAppearance;
            
		};
	}

}

#endif