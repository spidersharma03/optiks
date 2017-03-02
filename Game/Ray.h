#ifndef _RAY_H
#define _RAY_H

#include "Vector2.h"
#include "Color4.h"
#include "Matrix3.h"
#include "GameSettings.h"

USE_NAMESPACE_DIRAC2D

class Shape2D;
class Texture;

		class Ray
		{
		public: 
			Vector2f origin;
			// for Drawing purpose only
			Vector2f endPoint;
			Vector2f direction;
		public:
			Ray();
			Ray(const Ray& otherRay );
			Ray(const Vector2f& origin , const Vector2f& direction );
			Ray( float eyex , float eyey , float eyez , float dirx ,float diry ,float dirz ); 
			void set(const Ray& otherRay );
			void set(const Vector2f& origin , const Vector2f& direction  );
			void set(float eyex , float eyey , float eyez , float dirx ,float diry ,float dirz  );
			void transform( const Matrix3f& mat );
			
            void setColor(const Color4f& color)
            {
                m_Color = color;
            }
            
            Color4f getColor() const
            {
                return m_Color;
            }
            //bool intersectTriangle(const Vector2f& v0, Point3& v1, Point3& v2, Point3& out);
			//bool intersectTriangle(Point3& v0, Point3& v1, Point3& v2, float* u, float* v, float* t);
			//bool intersectBox(Point3& min, Point3& max);
			
			void render(float width = 10.0f, const Color4f color = RAY_COLOR);
			
            float getLength()
            {
                return m_Length*0.001;
            }
            
			int m_HitCount;
            int m_SourceID;
			
			Shape2D* m_Shapes[8];
			Shape2D* m_CurrentShape;
			Shape2D* m_SecondShape;
			
			bool m_bInside;
            bool m_bUserTerminate;
			float m_Power;
			float m_Angle;
			float m_Length;
            Color4f m_Color;
			static unsigned int textureID;
            static Texture *m_pTexture;
		};


		typedef struct IntersectionInformation
		{
			//vector<Point3*> hitPoints;
			Vector2f hitPoints[8];
			Vector2f Normal;
			Vector2f hitNormal[8];
			float hitT[8];
			float T;
			bool isHit;
			unsigned numHits;

			IntersectionInformation()
			{
				T = 1e20f;
				isHit = false;
				numHits = 0;
			}

		}Intersect_Info;



#endif