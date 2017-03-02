#include "Ray.h"
#include "OglRenderer.h"
#include "Game.h"

using namespace spider3d::render;


	
		unsigned int Ray::textureID;
        Texture* Ray::m_pTexture = NULL;

		Ray::Ray()
		{
			origin.x = 0.0f;origin.y = 0.0f;
			direction.x = 0.0f;direction.y = 0.0f;
			endPoint.x = origin.x + direction.x * 10000.0f;
			endPoint.y = origin.y + direction.y * 10000.0f;
			m_bInside = false;
			m_Power = 1.0f;
			m_Angle = atan2f(direction.y, direction.x) + M_PI;
            m_Angle = m_Angle * 180.0f/M_PI;
			m_CurrentShape = 0;
			m_SecondShape  = 0;
            m_Length = 0.0f;
            m_bUserTerminate = false;
            m_Color = RAY_COLOR;
            m_SourceID = 0;
		}

		Ray::Ray(const Ray& otherRay )
		{
			this->origin    = otherRay.origin;
			this->direction.x = otherRay.direction.x;
			this->direction.y = otherRay.direction.y;
			this->endPoint.x = otherRay.endPoint.x;
			this->endPoint.y = otherRay.endPoint.y;
			this->m_bInside = otherRay.m_bInside;
			m_Power = 1.0f;
			m_Angle = atan2f(direction.y, direction.x) ;
            m_Angle = m_Angle * 180.0f/M_PI;
			m_CurrentShape = otherRay.m_CurrentShape;
			m_SecondShape  = otherRay.m_SecondShape;
            m_bUserTerminate = false;
            m_Color = otherRay.m_Color;
            m_SourceID = otherRay.m_SourceID;
		}

		Ray::Ray(const Vector2f& origin , const Vector2f& direction )
		{
			this->origin    = origin;
			this->direction = direction;
			this->endPoint.x = origin.x + direction.x * 10000.0f;
			this->endPoint.y = origin.y + direction.y * 10000.0f;
			this->m_bInside = false;
			m_Power = 1.0f;
			m_Angle = atan2f(direction.y, direction.x) ;
            m_Angle = m_Angle * 180.0f/M_PI;
			m_CurrentShape = 0;
			m_SecondShape  = 0;
            m_bUserTerminate = false;
            m_Color = RAY_COLOR;
            m_SourceID = 0;
		}

		Ray::Ray( float eyex , float eyey , float eyez , float dirx ,float diry ,float dirz )
		{
			origin.x = eyex ; origin.y = eyey ;
			direction.x = dirx; direction.y = diry;
			this->m_bInside = false;
			m_Power = 1.0f;
			m_Angle = atan2f(direction.y, direction.x) ;
            m_Angle = m_Angle * 180.0f/M_PI;
			m_CurrentShape = 0;
			m_SecondShape  = 0;
            m_bUserTerminate = false;
            m_Color = RAY_COLOR;
            m_SourceID = 0;
		}

		void Ray::set(const Vector2f& origin , const Vector2f& direction  )
		{
			this->origin    = origin;
			this->direction = direction;
			this->m_bInside = false;
			m_Angle = atan2f(direction.y, direction.x) ;
            m_Angle = m_Angle * 180.0f/M_PI;
			m_CurrentShape = 0;
			m_SecondShape  = 0;
            m_bUserTerminate = false;
            m_Color = RAY_COLOR;
		}

		void Ray::set(const Ray& otherRay )
		{
			this->origin    = otherRay.origin;
			this->direction = otherRay.direction;
			this->m_bInside = false;
			m_Angle = atan2f(direction.y, direction.x) ;
            m_Angle = m_Angle * 180.0f/M_PI;
			m_CurrentShape = otherRay.m_CurrentShape;
			m_SecondShape  = otherRay.m_SecondShape;
            m_bUserTerminate = false;
            m_Color = RAY_COLOR;
            m_SourceID = 0;
		}

		void Ray::set(float eyex , float eyey , float eyez , float dirx ,float diry ,float dirz  )
		{
			origin.x = eyex ; origin.y = eyey ;
			direction.x = dirx; direction.y = diry;
			this->m_bInside = false;
			m_Angle = atan2f(direction.y, direction.x);
            m_Angle = m_Angle * 180.0f/M_PI;
			m_CurrentShape = 0;
			m_SecondShape  = 0;
            m_bUserTerminate = false;
            m_Color = RAY_COLOR;
            m_SourceID = 0;
		}

		void Ray::transform( const Matrix3f& mat )
		{
			mat.transformAsPoint(origin);
			mat.transformAsVector(direction);
		}

		
		
#define RAY_HALF_WIDTH 10.0f
		
		void Ray::render(float rayWidth, const Color4f color)
		{
            //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            /*
			Vector2f rayVector(origin.x - endPoint.x, origin.y - endPoint.y);
			float rayLength = rayVector.length();
            m_Length = rayLength;
            m_Length = m_Length > 100.0f ? 100.0f : m_Length;
			//glEnable(GL_BLEND);
			//glLineWidth(2.0);
			
            Matrix3f rot, xForm;
			Vector2f T(origin.x, origin.y);
			xForm.translate(T);
            rot.rotate(m_Angle);
			float matrixData[16];
			xForm.multiply(rot);
			xForm.getPointer(matrixData);
            Matrix4f xForm1;
            xForm1.set(xForm);
            Game::getInstance()->getRenderer()->setTransform(xForm1, spider3d::WORLD);

            glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			glBindTexture(GL_TEXTURE_2D, textureID);

			float v = 0.1f;
			float dt = 0.00001f;
			static float dx = 0.0f;
			dx -= v * dt;
            
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f + dx, 0.0f);
            glColor4f(m_Color.r, m_Color.g, m_Color.b, m_Color.a);
			glVertex2f(0.0f, -rayWidth);
			
			glTexCoord2f(rayLength/500  + dx, 0.0f);
			glVertex2f(rayLength, -rayWidth);
			
			glTexCoord2f(rayLength/500 + dx, 1.0f);
			glVertex2f(rayLength, rayWidth);
			
			glTexCoord2f(0.0f + dx, 1.0f);
			glVertex2f(0.0f, rayWidth);
			glEnd();		
			
			glDisable(GL_TEXTURE_2D);
            //glDisable(GL_BLEND);
	
			//glLineWidth(1.0);
            //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
             */
		}

