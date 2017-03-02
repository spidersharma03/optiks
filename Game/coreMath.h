#ifndef _COREMATH_H

#define _COREMATH_H

#include "spiderDataTypes.h"
#include <math.h>

namespace spider3d
{

	namespace core
	{

#define M_PI (3.1415926535897932)

#define M_PI_2 (3.1415926535897932/2.0)

#define M_PI_3 (3.1415926535897932/3.0)

#define M_PI_4 (3.1415926535897932/4.0)

#define TWO_M_PI 2 * 3.1415926535897932;


		short solveQuadratic( float A, float B , float C , float* roots );

		
		struct Vector2
		{
			float x,y;
			
			Vector2()
			{
				x = y = 0.0f;
			}
			
			Vector2(float x, float y):x(x),y(y)
			{
			}
		};
		////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////
		class Tuple3
		{
		public:
			sf32 x;
			sf32 y;
			sf32 z;
			Tuple3();
			Tuple3(sf32 , sf32 , sf32);
			Tuple3(Tuple3&);
			
			inline float dist(Tuple3& other)
			{
				return sqrt( distSquared(other) ); 
			}
			inline float distSquared(Tuple3& other)
			{
				return ( (x-other.x)*(x-other.x) 
						+ (y-other.y)*(y-other.y) 
						+ (z-other.z)*(z-other.z) );
			}
		};


		class Tuple4
		{
		public:
			sf32 x;
			sf32 y;
			sf32 z;
			sf32 w;
		};


		class Point3:public Tuple3
		{
		public:
			Point3():Tuple3()
			{
			}
			
			Point3(const Point3& other)
			{
				x = other.x;
				y = other.y;
				z = other.z;
			}
			
			Point3(sf32 x, sf32 y, sf32 z):Tuple3(x,y,z)
			{
			}
			
			void set(sf32 x, sf32 y, sf32 z)
			{
				this->x = x;
				this->y = y;
				this->z = z;
			}
		};


		class Point4:public Tuple4
		{
		};

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
		class Vector3:public Tuple3
		{
		public:
			Vector3();
			
			
			void sub(Tuple3&);
			void sub(Tuple3& , Tuple3&);
            void add(Tuple3&);
			void add(Tuple3& , Tuple3&);
			void scale(sf32);
			void scale(Tuple3&);
			void scale(Tuple3& , Tuple3&);
			sf32 dot(Tuple3&);
			Vector3 cross(Tuple3&);
			static Vector3 cross(Tuple3&, Tuple3&);
			static void cross(Tuple3&, Tuple3&, Tuple3&);
			//void cross(Tuple3& tin, Tuple3& tout);
			Vector3 operator=(Vector3&);
			sf32 length();
			sf32 lengthSq();
			void normalize();

			void set( Vector3 other )
			{
				x = other.x;
				y = other.y;
				z = other.z;
			}
			
			void set( sf32 x, sf32 y, sf32 z)
			{
				this->x = x;
				this->y = y;
				this->z = z;
			}
			
			Vector3(Vector3&);
			Vector3(sf32 , sf32 , sf32);
		};

		class Vector4:public Tuple4
		{
		};

		class Color3:public Tuple3
		{
		public:
			Color3():Tuple3()
			{
				
			}
			
			Color3( const Color3& other)
			{
				x = other.x;
				y = other.y;
				z = other.z;
			}

		};

		class Color4:public Tuple4
		{
		};
//		///////////////////////////////////////////////////////////////////
//		///////////////////////////////////////////////////////////////////
//		//  Matrix3f
//		///////////////////////////////////////////////////////////////////
//		///////////////////////////////////////////////////////////////////
//		class Matrix3f
//		{
//		public:
//
//			void set(sf32*);
//
//			void set(Matrix3f&);
//
//			void multiply(Matrix3f&); 	
//
//			void multiply(Matrix3f& , Matrix3f&); 	
//
//			Matrix3f();
//
//			Matrix3f(Matrix3f&);
//
//
//			sf32 m00 , m01 , m02;
//			sf32 m10 , m11 , m12;
//			sf32 m20 , m21 , m22;
//
//			sf32 data[9];
//
//		};

		///////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////
		//  Matrix4f
		///////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////

		class Matrix4f
		{
		public:

			void set(sf32*);

			void set(Matrix4f&);

			void print();

			void setTranslation(Vector3&);

            void setTranslation(Point3&);

			void setIdentity();

			void setScale(Vector3&);

			void setScale(sf32);

			void setScale(float sx , float sy , float sz);
			
			void rotX(sf32);

			void rotY(sf32);

			void rotZ(sf32);

			void lookAt(Point3& eye , Point3& target , Vector3& up);
         
			void perspective( sf32 , sf32 , sf32 , sf32);

			void frustum( sf32 , sf32 , sf32 , sf32 , sf32 , sf32);

			void transform(Point3&);

			void transform(Vector3&);

			Vector3& getTranslation();

			void multiply(Matrix4f&); 	

			void multiply(Matrix4f& , Matrix4f&); 	

			Matrix4f getInverse();
			
			void getInverse(Matrix4f& out);

			//void operator+(Matrix4f& , Matrix4f&);
			float* pointer();
			
			void pointer(float* data);

			Matrix4f();

			Matrix4f(Matrix4f&);

			sf32 m00 , m01 , m02 , m03;
			sf32 m10 , m11 , m12 , m13;
			sf32 m20 , m21 , m22 , m23;
			sf32 m30 , m31 , m32 , m33;

			sf32 data[16];

		};


	}

}


#endif