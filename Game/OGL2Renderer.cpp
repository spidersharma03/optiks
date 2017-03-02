//
//  OGL2Renderer.cpp
//  Optiks
//
//  Created by Prashant on 21/01/14.
//
//

#include "OGL2Renderer.h"
#include "GLSL2ShaderProgram.h"
#include "Texture.h"
#include <cassert>


using namespace std;


namespace spider3d
{
	namespace render
	{
        
        
        Ogl2Renderer::Ogl2Renderer()
        {
            m_RendererType = ERT_OPENGL2;
        }
        
        void Ogl2Renderer::drawVertexPrimitiveList
        (unsigned vertexCount, void* coords , int vertexSize, void* colors, int colorSize, float* texcoords, int texcoordsize, int stride, PRIMITIVE_TYPE type)
		{
            if( !m_pCurrentShaderProgram )
            {
                printf("must have a shader program assosiated before draw call\n");
                assert(0);
            }
            
            ShaderAttribute* pAttributePosition = m_pCurrentShaderProgram->getShaderAttributeByName("position");
            ShaderAttribute* pAttributeTexCoord = m_pCurrentShaderProgram->getShaderAttributeByName("texCoord");
            ShaderAttribute* pAttributeColor = m_pCurrentShaderProgram->getShaderAttributeByName("color");
            
            //if( !pAttributePosition || !pAttributeTexCoord || !pAttributeColor )
              //  assert(0);
            
			if ( coords )
			{
				glEnableVertexAttribArray(pAttributePosition->m_Location);
                glVertexAttribPointer(pAttributePosition->m_Location, vertexSize, GL_FLOAT, GL_FALSE, stride, coords);
			}
			if ( colors )
			{
				glEnableVertexAttribArray(pAttributeColor->m_Location);
                glVertexAttribPointer(pAttributeColor->m_Location, colorSize, GL_FLOAT, GL_FALSE, stride, colors);
			}
            
			if ( texcoords && pAttributeTexCoord )
			{
				glEnableVertexAttribArray(pAttributeTexCoord->m_Location);
                glVertexAttribPointer(pAttributeTexCoord->m_Location, texcoordsize, GL_FLOAT, GL_FALSE, stride, texcoords);
			}

			switch ( type )
			{
                case EPT_TRIANGLE:
                    glDrawArrays(GL_TRIANGLES , 0 , vertexCount);
                    break;
                    
                case EPT_TRIANGLE_FAN:
                    glDrawArrays(GL_TRIANGLE_FAN , 0 , vertexCount);
                    break;
                case EPT_LINE:
                    glDrawArrays(GL_LINES , 0 , vertexCount);
                    break;
                    
                case EPT_POINT:
                    glDrawArrays(GL_POINTS , 0 , vertexCount);
                    break;
			}
			//Disable Client States
            glDisableVertexAttribArray(pAttributePosition->m_Location);
			if(colors)
                glDisableVertexAttribArray(pAttributeColor->m_Location);
            if( texcoords && pAttributeTexCoord )
                glDisableVertexAttribArray(pAttributeTexCoord->m_Location);
		}
        
        void Ogl2Renderer::drawIndexVertexPrimitiveList(unsigned vertexCount, float* coords, void* colors, int colorSize, float* texcoords, int stride, void* indices, PRIMITIVE_TYPE type)
        {
            if( !m_pCurrentShaderProgram )
            {
                printf("must have a shader program assosiated before draw call\n");
                assert(0);
            }
            
            ShaderAttribute* pAttributePosition = m_pCurrentShaderProgram->getShaderAttributeByName("position");
            ShaderAttribute* pAttributeTexCoord = m_pCurrentShaderProgram->getShaderAttributeByName("texCoord");
            ShaderAttribute* pAttributeColor = m_pCurrentShaderProgram->getShaderAttributeByName("color");
            
            //if( !pAttributePosition || !pAttributeTexCoord || !pAttributeColor )
            //  assert(0);
            
			if ( coords )
			{
				glEnableVertexAttribArray(pAttributePosition->m_Location);
                glVertexAttribPointer(pAttributePosition->m_Location, 2, GL_FLOAT, GL_FALSE, stride, coords);
			}
			if ( colors )
			{
				glEnableVertexAttribArray(pAttributeColor->m_Location);
                glVertexAttribPointer(pAttributeColor->m_Location, colorSize, GL_FLOAT, GL_FALSE, stride, colors);
			}
            
			if ( texcoords && pAttributeTexCoord )
			{
				glEnableVertexAttribArray(pAttributeTexCoord->m_Location);
                glVertexAttribPointer(pAttributeTexCoord->m_Location, 2, GL_FLOAT, GL_FALSE, stride, texcoords);
			}
            
			switch ( type )
			{
                case EPT_TRIANGLE:
                    glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_SHORT, indices);
                    break;
                    
                case EPT_TRIANGLE_FAN:
                    glDrawElements(GL_TRIANGLE_FAN, vertexCount, GL_UNSIGNED_SHORT, indices);
                    break;
                case EPT_LINE:
                    glDrawElements(GL_LINES, vertexCount, GL_UNSIGNED_SHORT, indices);
                    break;
                    
                case EPT_POINT:
                    glDrawElements(GL_POINTS, vertexCount, GL_UNSIGNED_SHORT, indices);
                    break;
			}
			//Disable Client States
            glDisableVertexAttribArray(pAttributePosition->m_Location);
			if(colors)
                glDisableVertexAttribArray(pAttributeColor->m_Location);
            if( texcoords && pAttributeTexCoord )
                glDisableVertexAttribArray(pAttributeTexCoord->m_Location);
		}
        
        void Ogl2Renderer::setTransform(const NAMESPACE_DIRAC2D::Matrix4f &matrix , TANSFORMATION_STATE transform_state )
		{
            float data[16];

            if( !m_pCurrentShaderProgram )
            {
                printf("must have a shader program assosiated before setting up matrices\n");
                assert(0);
            }
            
            ShaderUniform* suModelView = m_pCurrentShaderProgram->getShaderUniformByName("ModelViewMatrix");
            ShaderUniform* suProjection = m_pCurrentShaderProgram->getShaderUniformByName("ProjectionMatrix");
            if( !suModelView || !suProjection )
                assert(0);
            
			switch( transform_state )
			{
                case WORLD:
                case VIEW:
                {
                    NAMESPACE_DIRAC2D::Matrix4f viewMatrix;
                    viewMatrix = matrix;
                    //viewMatrix.m23 = -1;
                    //glMatrixMode(GL_MODELVIEW);
                    viewMatrix.getPointer(data);
                    //glLoadIdentity();
                    //glLoadMatrixf(data);
                    glUniformMatrix4fv(suModelView->m_Location, 1, false, data);
                    break;
                }
                case PROJECTION:
                    //glMatrixMode(GL_PROJECTION);
                    matrix.getPointer(data);
                    //glLoadIdentity();
                    //glLoadMatrixf(data);
                    glUniformMatrix4fv(suProjection->m_Location, 1, false, data);
                    break;
                case TEXTURE:
                    //glMatrixMode(GL_TEXTURE);
                    //matrix.getPointer(data);
                    //glLoadIdentity();
                    //glLoadMatrixf(data);
                    break;
			}
		}
        
        
		void Ogl2Renderer::DrawLine(const Vector2f& start , const Vector2f& end , Color4f& color )
		{
			/*glLineWidth(1.0f);
			glBegin(GL_LINES);
			glColor3f(color.r, color.g , color.b );
			glVertex2f(start.x, start.y);
			glVertex2f(end.x, end.y);
			glEnd();
			glColor3f(1.0f, 1.0f , 1.0f);*/
		}
		
        
		void Ogl2Renderer::DrawPoint(const Vector2f& point , const Color4f& color )
		{
			/*glPointSize(1.0f);
			glBegin(GL_POINTS);
			glColor4f(color.r, color.g , color.b, color.a );
			glVertex2f(point.x, point.y);
			glEnd();*/
		}
        
        void Ogl2Renderer::DrawCircle(float radius)
        {
            
        }
        
		void Ogl2Renderer::DrawRect(const AABB2f& aabb , const Color4f& color)
		{
            /*const Vector2f& leftBottom = aabb.m_LowerBounds;
            const Vector2f& topRight   = aabb.m_UpperBounds;
            glLineWidth(2);
            glBegin(GL_LINES);
            glColor4f(color.r, color.g, color.b, color.a);
            glVertex2f(leftBottom.x, leftBottom.y);
            glVertex2f(topRight.x, leftBottom.y);
            glVertex2f(topRight.x, leftBottom.y);
            glVertex2f(topRight.x, topRight.y);
            glVertex2f(topRight.x, topRight.y);
            glVertex2f(leftBottom.x, topRight.y);
            glVertex2f(leftBottom.x, topRight.y);
            glVertex2f(leftBottom.x, leftBottom.y);
            glEnd();*/
		}
        
        void Ogl2Renderer::DrawQuad(float width, float height, const Color4f& color)
        {
            float vertices[8];
            vertices[0] = width;
            vertices[1] = height;
            vertices[2] = 0;
            vertices[3] = height;
            vertices[4] = 0;
            vertices[5] = 0;
            vertices[6] = width;
            vertices[7] = 0;
            
            float texCoords[8];
            texCoords[0] = 1.0f;
            texCoords[1] = 1.0f;
            texCoords[2] = 0.0f;
            texCoords[3] = 1.0f;
            texCoords[4] = 0.0f;
            texCoords[5] = 0.0f;
            texCoords[6] = 1.0f;
            texCoords[7] = 0.0f;
            
            float colors[16];
            for(int i=0; i<16; i+=4 )
            {
                colors[i] = color.r;
                colors[i+1] = color.g;
                colors[i+2] = color.b;
                colors[i+3] = color.a;
            }
            
            drawVertexPrimitiveList(4, vertices, 2, colors, 4, texCoords, 2, 0, EPT_TRIANGLE_FAN);
        }
        
		void Ogl2Renderer::setTextureObject(Texture* texture, E_TEXTURE_UNIT_STATE eTexUnitState)
		{
            GLuint texUnit;
            switch (eTexUnitState) {
                case ETU_0:
                    texUnit = GL_TEXTURE0;
                    break;
                case ETU_1:
                    texUnit = GL_TEXTURE1;
                    break;
                case ETU_2:
                    texUnit = GL_TEXTURE2;
                    break;
                case ETU_3:
                    texUnit = GL_TEXTURE3;
                    break;
                case ETU_4:
                    texUnit = GL_TEXTURE4;
                    break;
                case ETU_5:
                    texUnit = GL_TEXTURE5;
                    break;
                case ETU_6:
                    texUnit = GL_TEXTURE6;
                    break;
                case ETU_7:
                    texUnit = GL_TEXTURE7;
                    break;
                default:
                    break;
            }
			if ( !texture )
			{
                glActiveTexture(texUnit);
                glDisable(GL_TEXTURE_2D);
                //glDisable(GL_TEXTURE_1D);
                glBindTexture(GL_TEXTURE_2D , 0 );

				return;
            }
            
			if ( texture->getTextureType() == ETT_TEXTURE_2D )
            {
                glActiveTexture(texUnit);
                glEnable(GL_TEXTURE_2D);
                //glDisable(GL_TEXTURE_1D);
				glBindTexture(GL_TEXTURE_2D , *(texture->getTextureName()) );
                
                //glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            }
            if ( texture->getTextureType() == ETT_TEXTURE_1D )
            {
                glActiveTexture(texUnit);
                //glEnable(GL_TEXTURE_1D);
                glDisable(GL_TEXTURE_2D);
				//glBindTexture(GL_TEXTURE_1D , *(texture->getTextureName()) );
                //glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            }
		}
        
        void Ogl2Renderer::setBlendingEnable( bool bEnableBlend )
        {
            if( bEnableBlend )
            {
                glEnable(GL_BLEND);
            }
            else
            {
                glDisable(GL_BLEND);
            }
        }
        
        void Ogl2Renderer::setBlendingFactors( E_BLEND_FACTOR srcFactor, E_BLEND_FACTOR destFactor )
        {
            GLenum glSrcFactor = GL_ONE;
            GLenum glDstFactor = GL_ONE;
            
            switch (srcFactor) {
                case EBF_ONE:
                    glSrcFactor = GL_ONE;
                    break;
                case EBF_ONE_MINUS_SRC_ALPHA:
                    glSrcFactor = GL_ONE_MINUS_SRC_ALPHA;
                    break;
                case EBF_SRC_ALPHA:
                    glSrcFactor = GL_SRC_ALPHA;
                    break;
                case EBF_ZERO:
                    glSrcFactor = GL_ZERO;
                    break;
                default:
                    break;
            }
            switch (destFactor) {
                case EBF_ONE:
                    glDstFactor = GL_ONE;
                    break;
                case EBF_ONE_MINUS_SRC_ALPHA:
                    glDstFactor = GL_ONE_MINUS_SRC_ALPHA;
                    break;
                case EBF_SRC_ALPHA:
                    glDstFactor = GL_SRC_ALPHA;
                    break;
                case EBF_ZERO:
                    glDstFactor = GL_ZERO;
                    break;
                default:
                    break;
            }
            
            glBlendFunc(glSrcFactor, glDstFactor);
        }
        
        ShaderProgram* Ogl2Renderer::createShaderProgram()
        {
            return new GLSL2ShaderProgram();
        }
        
        void Ogl2Renderer::readPixels( const AABB2i& area, void* pixels)
        {
            Vector2i size = area.m_UpperBounds - area.m_LowerBounds;
            assert(size.x > 0 && size.y > 0 );
            
            glReadPixels(area.m_LowerBounds.x, area.m_LowerBounds.y, size.x, size.y, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        }
    }
}
