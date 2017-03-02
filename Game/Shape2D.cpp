//
//  Shape2D.cpp
//  Optiks
//
//  Created by Prashant on 06/09/11.
//  Copyright 2011 Sourcebits Technologies. All rights reserved.
//
#include "Shape2D.h"
#include "Ray.h"
#include <stdio.h>
#include "OglRenderer.h"
#include "Game.h"

using namespace spider3d::render;

Shape2D::Shape2D()
{
	m_bRenderable = true;
	m_bRayEntering = false;
	m_RefractionIndex = 1.0f;
	m_ReflectionFactor.x = m_ReflectionFactor.y = m_ReflectionFactor.z = 1.0f;
	m_TransmissionFactor.x = m_TransmissionFactor.y = m_TransmissionFactor.z = 0.0f;
	m_Pickable = true;
	m_isRefractive = false;
	m_IsReflective = true;
    m_Rotation = 0.0f;
}

Shape2D::Shape2D( const ShapeInfo& shapeInfo)
{
    m_IsReflective    = shapeInfo.m_IsReflective;
    m_isRefractive    = shapeInfo.m_isRefractive;
    m_RefractionIndex = shapeInfo.m_RefractionIndex;
    m_ShapeType       = shapeInfo.shapeType;
    m_isSolid         = shapeInfo.m_isSolid;
    
    m_bRenderable = true;
	m_bRayEntering = false;
	m_ReflectionFactor.x = m_ReflectionFactor.y = m_ReflectionFactor.z = 1.0f;
	m_TransmissionFactor.x = m_TransmissionFactor.y = m_TransmissionFactor.z = 0.0f;
	m_Pickable = true;
	  
    m_Position = shapeInfo.position;
    m_Rotation = shapeInfo.rotation;
    Matrix3f rotXform, transXform;
    rotXform.rotate(shapeInfo.rotation);
    transXform.translate(m_Position);
    m_Transform.multiply(transXform, rotXform);
    m_Transform.getInverse(m_InvTransform);
}

void Shape2D::save(FILE* pFile)
{
	fprintf(pFile, "SHAPE_TYPE::\n");
	fprintf(pFile, "%d\n", m_ShapeType);
	fprintf(pFile, "%s\n", "POSITION::");
    
    
	fprintf(pFile, "%d %d\n", (int)(m_Transform.col3.x/BASIC_ELEMENT_IMAGE_SIZE), (int)(m_Transform.col3.y/BASIC_ELEMENT_IMAGE_SIZE));
    
    fprintf(pFile, "%s\n","ROTATION::");
	fprintf(pFile, "%f\n", m_Rotation);
	//fprintf(pFile, "COLOR CODE::\n");
	//fprintf(pFile, "%d\n", m_ColorCode);
	fprintf(pFile, "MATERIAL_PROPERTIES::\n");
	fprintf(pFile, "%d  %d\n", m_IsReflective, m_isRefractive);
    fprintf(pFile, "REFRACTIVE_INDEX::\n");
    fprintf(pFile, "%f\n", m_RefractionIndex);
}

void Shape2D::load(FILE* pFile)
{

}

void Shape2D::render()
{
    m_Transform.translate(m_Position);
    m_Transform.rotate(m_Rotation);
}

Circle::Circle( float radius )
{
	m_bRayEntering = false;
	m_Radius = radius;
	m_Pickable = true;
	m_Area = M_PI * m_Radius * m_Radius;
	m_isSolid = true;
	m_ShapeType = EST_CIRCLE;
    m_isRefractive = false;
    m_RefractionIndex = 1.2f;
}

Circle::Circle( const CircleInfo& circleInfo ):Shape2D(circleInfo)
{
    m_Radius = circleInfo.radius;
}

void Circle::setRadius(float radius)
{
    m_Radius = radius;
}

float Circle::getRadius() const
{
    return m_Radius;
}

void Circle::render()
{
    float scale = 1.0f;
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    Matrix4f xForm;
    xForm.set(m_Transform);
    Game::getInstance()->getRenderer()->setTransform(xForm, spider3d::WORLD);
    
    float vertices[8];
    float texCoords[8];
    float colors[16];
    
    vertices[0] = -m_Radius*scale;
    vertices[1] = -m_Radius*scale;
    
    texCoords[0] = 0.0f;
    texCoords[1] = 0.0f;
    
    vertices[2] =  m_Radius*scale;
    vertices[3] = -m_Radius*scale;
    
    texCoords[2] = 1.0f;
    texCoords[3] = 0.0f;
    
    vertices[4] =  m_Radius*scale;
    vertices[5] =  m_Radius*scale;
    
    texCoords[4] = 1.0f;
    texCoords[5] = 1.0f;
    
    vertices[6] = -m_Radius*scale;
    vertices[7] = m_Radius*scale;
    
    texCoords[6] = 0.0f;
    texCoords[7] = 1.0f;
    
    for( int i=0; i<16; i++ )
    {
        colors[i] = 1.0f;
    }
    
    Game::getInstance()->getRenderer()->drawVertexPrimitiveList(4, vertices, 2, colors, 4, texCoords, 2, 0, spider3d::EPT_TRIANGLE_FAN);
}

bool Circle::intersectRay( Ray& ray , IntersectionInformation& intersectInfo)
{
	if( !m_Pickable )
		return false;
	
	Ray genericRay = ray;
	// Inverse Transform Ray
	m_InvTransform.transformAsPoint(genericRay.origin);
	m_InvTransform.transformAsVector(genericRay.direction);
	
	static float roots[] = { 0.0f , 0.0f };
	double Ox = genericRay.origin.x; float Oy = genericRay.origin.y; 
	double Dx = genericRay.direction.x; float Dy = genericRay.direction.y;
	double r2 = m_Radius*m_Radius;
	double A =  Dx*Dx + Dy*Dy;
	double B =  Ox*Dx + Oy*Dy;
	double C = Ox*Ox + Oy*Oy - r2;
	
	short ret;
	double D = ( B*B - A*C );
	if ( D < 1e-15f ) 
	{
		ret = 0; // no solution
		return false;
	}
	else
	{
		D = sqrt(D);
		double A2 = 1.0f/A;
		if ( D < 1E-10 )
		{
			roots[0] = roots[1] = -B*A2;
			ret = 1;
			intersectInfo.hitPoints[0].x = ray.origin.x + ray.direction.x*roots[0];
			intersectInfo.hitPoints[0].y = ray.origin.y + ray.direction.y*roots[0];
			intersectInfo.isHit = true;
			intersectInfo.hitNormal[0].x = Ox + Dx * roots[0];
			intersectInfo.hitNormal[0].y = Oy + Dy * roots[0];
			intersectInfo.hitNormal[0].normalize();
			m_Transform.transformAsVector(intersectInfo.hitNormal[0]);
			intersectInfo.numHits = 1;
			intersectInfo.T = roots[0];
			//if( roots[1] > 0.0f )
			//	m_bRayEntering = !m_bRayEntering;
			return true;
		}
		
		roots[0] = (-B - D)*A2;
		roots[1] = (-B + D)*A2;
		
		if( roots[0] < 0.0f && roots[1] > 0.0f)
		{
			roots[0] = roots[1];
		}
		
		ret = 2;
		intersectInfo.hitPoints[0].x = ray.origin.x + ray.direction.x*roots[0];
		intersectInfo.hitPoints[0].y = ray.origin.y + ray.direction.y*roots[0];
		intersectInfo.isHit = true;
		intersectInfo.hitNormal[0].x = Ox + Dx * roots[0];
		intersectInfo.hitNormal[0].y = Oy + Dy * roots[0];
		intersectInfo.hitNormal[0].normalize();
		m_Transform.transformAsVector(intersectInfo.hitNormal[0]);
		
		intersectInfo.hitPoints[1].x = ray.origin.x + ray.direction.x*roots[1];
		intersectInfo.hitPoints[1].y = ray.origin.y + ray.direction.y*roots[1];
		intersectInfo.isHit = true;
		//intersectInfo.Normal.x = intersectInfo.hitPoints[1].x - m_Transform.m03;
		//intersectInfo.Normal.y = intersectInfo.hitPoints[1].y - m_Transform.m13;
		//intersectInfo.Normal.normalize();
		//m_Transform.transform(intersectInfo.Normal);
		intersectInfo.numHits = 2;
		intersectInfo.T = roots[0];
	}
	
	//if( roots[1] > 0.0f )
	//	m_bRayEntering = !m_bRayEntering;
	
	return true;
}


bool Circle::pick(const Vector2f& p )
{
	Vector2f v(p.x, p.y);
	m_InvTransform.transformAsPoint(v);
	return v.x*v.x + v.y*v.y < m_Radius * m_Radius;
}

void Circle::save(FILE* pFile)
{
	Shape2D::save(pFile);
	fprintf(pFile, "%s\n", "RADIUS::");
	fprintf(pFile, "%f\n", m_Radius);
}

void Circle::load(FILE* pFile)
{
    Shape2D::load(pFile);
    fscanf(pFile, "%f", &m_Radius);
}

CircularArc::CircularArc( float radius , float angle):m_Radius(radius), m_Angle(angle)
{
	m_Angle = m_Angle > 360.0f ? 360.0f : m_Angle;
	m_isSolid = false;
	m_ShapeType = EST_CIRCULAR_ARC;
    m_NumInsideHits = 0;
}

CircularArc::CircularArc( const CircularArcInfo& circularArcInfo ):Shape2D(circularArcInfo)
{
    m_Angle  = circularArcInfo.angle;
    m_Radius = circularArcInfo.radius;///3.8;
    m_NumInsideHits = 0;
}

void CircularArc::render()
{	    
    float scale = 1.0f;
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    m_Transform.getPointer(matrixData);
    
    Matrix4f xForm;
    xForm.set(m_Transform);
    Game::getInstance()->getRenderer()->setTransform(xForm, spider3d::WORLD);
    
//    glBegin(GL_QUADS);
//    
//    glTexCoord2f(0.0f, 0.0f);
//    
//    glVertex2f(-m_Radius*scale, -m_Radius*scale);
//    
//    glTexCoord2f(1.0f, 0.0f);
//    
//    glVertex2f( m_Radius*scale, -m_Radius*scale);
//    
//    glTexCoord2f(1.0f, 1.0f);
//    
//    glVertex2f( m_Radius*scale, m_Radius*scale);
//    
//    glTexCoord2f(0.0f, 1.0f);
//    
//    glVertex2f(-m_Radius*scale, m_Radius*scale);
//    
//    glEnd();
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

bool CircularArc::intersectRay( Ray& ray , IntersectionInformation& intersectInfo)
{
	if( !m_Pickable )
		return false;
	
	Ray genericRay = ray;
	// Inverse Transform Ray
 	m_InvTransform.transformAsPoint(genericRay.origin);
	m_InvTransform.transformAsVector(genericRay.direction);
	
	static double roots[] = { 0.0f , 0.0f };
	double Ox = genericRay.origin.x; float Oy = genericRay.origin.y; 
	double Dx = genericRay.direction.x; float Dy = genericRay.direction.y;
	double r2 = m_Radius*m_Radius;
	double A =  Dx*Dx + Dy*Dy;
	double B =  Ox*Dx + Oy*Dy;
	double C = Ox*Ox + Oy*Oy - r2;
	
	short ret;
	double D = ( B*B - A*C );
	if ( D < 1e-15f ) 
	{
		ret = 0; // no solution
		return false;
	}
	else
	{
		D = sqrt(D);
		double A2 = 1.0f/A;
		if ( D < 1E-10 )
		{
			roots[0] = roots[1] = -B*A2;
			ret = 1;
			intersectInfo.hitPoints[0].x = ray.origin.x + ray.direction.x*roots[0];
			intersectInfo.hitPoints[0].y = ray.origin.y + ray.direction.y*roots[0];
			intersectInfo.isHit = true;
			intersectInfo.hitNormal[0].x = Ox + Dx * roots[0];
			intersectInfo.hitNormal[0].y = Oy + Dy * roots[0];
			intersectInfo.hitNormal[0].normalize();
			m_Transform.transformAsVector(intersectInfo.hitNormal[0]);
			intersectInfo.numHits = 1;
			intersectInfo.T = roots[0];
			return true;
		}
		
		roots[0] = (-B - D)*A2;
		roots[1] = (-B + D)*A2;
		
		if( roots[1] < 0.0f )
			return false;
		
		// Origin of the Ray is Inside the Circular Arc.
		bool bSourceInsideArc = false;
		if( roots[0] < 0.0f && roots[1] > 0.0f)
		{
			bSourceInsideArc = true;
			roots[0] = roots[1];
		}
		
		float localx0 = Ox + Dx * roots[0];
		float localy0 = Oy + Dy * roots[0];
		
		// Find if the Hit point is within the Arc
		float angle = atan2f(localy0, localx0);
		bool bFlipNormal = false;
		if( fabs(angle)*180.0f/M_PI < m_Angle*0.5f )
		{
			//if( roots[1] < 0.0f )
			//	return false;
			
			if( bSourceInsideArc )
				return false;
				
			roots[0] = roots[1];
			bFlipNormal = true;
		}
		
		float localx1 = Ox + Dx * roots[1];
		float localy1 = Oy + Dy * roots[1];
		angle = atan2f(localy1, localx1);
		if( bFlipNormal && fabs(angle)*180.0f/M_PI < m_Angle*0.5f )
		{
			return false;
		}
		
		double O_x = Ox - genericRay.direction.x * 0.01f;
		double O_y = Oy - genericRay.direction.y * 0.01f;

		double Diff = O_x*O_x + O_y*O_y - m_Radius*m_Radius;
		// If the Source/Origin of the Ray is within the Circle of this Arc's Radius, Flip Normal
		if( !bFlipNormal && (Diff) < 1e-02 )
		{
			bFlipNormal = true;
			roots[0] = roots[1];
            m_NumInsideHits++;
		}
		
		intersectInfo.hitNormal[0].x = Ox + Dx * roots[0];
		intersectInfo.hitNormal[0].y = Oy + Dy * roots[0];
		// If Hit Point is inside the Arc region, Flip Hit Normal
		if( bFlipNormal )
		{
			intersectInfo.hitNormal[0].x *= -1.0f;
			intersectInfo.hitNormal[0].y *= -1.0f;
		}
		
		ret = 2;
		
		intersectInfo.hitPoints[0].x = ray.origin.x + ray.direction.x*roots[0];
		intersectInfo.hitPoints[0].y = ray.origin.y + ray.direction.y*roots[0];
		intersectInfo.isHit = true;
		
		intersectInfo.hitNormal[0].normalize();
		
		m_Transform.transformAsVector(intersectInfo.hitNormal[0]);
		
		intersectInfo.hitPoints[1].x = ray.origin.x + ray.direction.x*roots[1];
		intersectInfo.hitPoints[1].y = ray.origin.y + ray.direction.y*roots[1];
		intersectInfo.isHit = true;
		//intersectInfo.Normal.x = intersectInfo.hitPoints[1].x - m_Transform.m03;
		//intersectInfo.Normal.y = intersectInfo.hitPoints[1].y - m_Transform.m13;
		//intersectInfo.Normal.normalize();
		//m_Transform.transform(intersectInfo.Normal);
		intersectInfo.numHits = 2;
		intersectInfo.T = roots[0];
	}
	return true;
}

bool CircularArc::pick(const Vector2f& p )
{
	Vector2f v(p.x, p.y);
	m_InvTransform.transformAsPoint(v);
	return v.x*v.x + v.y*v.y < m_Radius * m_Radius;
}

void CircularArc::save(FILE* pFile)
{
	Shape2D::save(pFile);
	fprintf(pFile, "%s\n", "RADIUS__ANGLE::");
	fprintf(pFile, "%f  %f\n", m_Radius, m_Angle);
}

void CircularArc::load(FILE* pFile)
{
    
}

ParabolicSegment::ParabolicSegment( float height, float k )
{
	m_Height = height;
	m_A = k;
	m_isSolid = false;
	m_ShapeType = EST_PARABOLIC_SEGMENT;
}

ParabolicSegment::ParabolicSegment( const ParabolicSegmentInfo& parabolicSegmentInfo)
{
    
}

void ParabolicSegment::render()
{
//    glDisable(GL_TEXTURE_2D);
//	m_Transform.getPointer(matrixData);
//	
//	glPushMatrix();
//	
//	glLoadMatrixf(matrixData);
//	
//	float y = -m_Height*0.5f;
//	float dy = m_Height/20;
//	
//	glColor3ub(255, 255, 255);
//
//	glBegin(GL_LINES);
//	for( int i=0; i<20; i++ )
//	{
//		float x1 = y*y/m_A;
//		float y1 = y+dy;
//		float x2 = y1*y1/m_A;
//		glVertex2f(x1, y);
//		glVertex2f(x2, y1);
//		y += dy;
//	}
//	glEnd();
//	
//	glPopMatrix();
//    glEnable(GL_TEXTURE_2D);
}

bool ParabolicSegment::intersectRay( Ray& ray , IntersectionInformation& intersectInfo)
{
	if( !m_Pickable )
		return false;
	
	Ray genericRay = ray;
	// Inverse Transform Ray
	m_InvTransform.transformAsPoint(genericRay.origin);
	m_InvTransform.transformAsVector(genericRay.direction);
	
	
	static double roots[] = { 0.0f , 0.0f };
	double Ox = genericRay.origin.x; float Oy = genericRay.origin.y; 
	double Dx = genericRay.direction.x; float Dy = genericRay.direction.y;
	double A =  Dy*Dy;
	double B =  2.0f*Oy*Dy - m_A*Dx;
	double C = Oy*Oy - m_A * Ox;
	
	short ret;
	double D = ( B*B - 4.0f*A*C );
	if ( D < 1e-15f ) 
	{
		ret = 0; // no solution
		return false;
	}
	else
	{
		double A2 = 0.5f/A;
					
		D = sqrt(D);
		if( D < 1e-10 )
		{
			roots[0] = roots[1] = -B*A2;
		}
		else
		{
			roots[0] = (-B - D)*A2;
			roots[1] = (-B + D)*A2;
		}
		
		double y0 = Oy + Dy * roots[0];
		double y1 = Oy + Dy * roots[1];
		
		if( roots[1] < roots[0] )
		{
			double temp = roots[0];
			roots[0] = roots[1];
			roots[1] = temp;
		}
		// Make Sure that All the Intersections are in the forward direction.( neglect intersections behind the ray origin)
		// Here we take the positive root by swapping with the negative root.
		if( roots[0] < 0.0f )
		{
			roots[0] = roots[1];
			y0 = Oy + Dy * roots[0];
		}
		
		if ( fabs(D) < 1e-10 )
		{
			ret = 1;
			intersectInfo.hitPoints[0].x = ray.origin.x + ray.direction.x*roots[0];
			intersectInfo.hitPoints[0].y = ray.origin.y + ray.direction.y*roots[0];
			intersectInfo.isHit = true;
			intersectInfo.hitNormal[0].x = -m_A;
			intersectInfo.hitNormal[0].y = 2.0f * y0;
			intersectInfo.hitNormal[0].normalize();
			m_Transform.transformAsVector(intersectInfo.hitNormal[0]);
			intersectInfo.numHits = 1;
			intersectInfo.T = roots[0];
			return true;
		}
		else 
		{
			ret = 0;
			int i = 0;
			if( (y0 < m_Height*0.5f) && (y0 >= -m_Height*0.5f) )
			{
				intersectInfo.hitPoints[i].x = ray.origin.x + ray.direction.x*roots[0];
				intersectInfo.hitPoints[i].y = ray.origin.y + ray.direction.y*roots[0];
				intersectInfo.isHit = true;
				
				intersectInfo.hitNormal[i].x = -m_A;
				intersectInfo.hitNormal[i].y = 2.0f * y0;
				intersectInfo.hitNormal[i].normalize();
				m_Transform.transformAsVector(intersectInfo.hitNormal[i]);
				intersectInfo.T = roots[0];
				i++;
				ret++;
			}
			if( (y1 < m_Height*0.5f) && (y1 >= -m_Height*0.5f) )
			{
				intersectInfo.hitPoints[i].x = ray.origin.x + ray.direction.x*roots[1];
				intersectInfo.hitPoints[i].y = ray.origin.y + ray.direction.y*roots[1];
				intersectInfo.isHit = true;
				
				intersectInfo.hitNormal[i].x = -m_A;
				intersectInfo.hitNormal[i].y = 2.0f * y1;
				intersectInfo.hitNormal[i].normalize();
				m_Transform.transformAsVector(intersectInfo.hitNormal[i]);
				if( intersectInfo.T > roots[1] )
                    intersectInfo.T = roots[1];
				ret++;
			}
			
            float testX = Oy*Oy/m_A;
			if( testX < Ox )
			{
				intersectInfo.hitNormal[0].x *= -1.0f;
				intersectInfo.hitNormal[0].y *= -1.0f;
			}
			intersectInfo.numHits = 2;
		}
	}
	return true;
}

bool ParabolicSegment::pick(const Vector2f& p )
{
	Vector2f v(p.x, p.y);
	m_InvTransform.transformAsPoint(v);
	float x0 = m_Height * m_Height * 0.25/m_A;
	
	return v.x <= x0 && v.x > v.y*v.y/m_A;
}


void ParabolicSegment::save(FILE* pFile)
{
	Shape2D::save(pFile);
	fprintf(pFile, "%s\n", "HEIGHT  CONSTANT::");
	fprintf(pFile, "%f  %f\n", m_Height, m_A);
}

void ParabolicSegment::load(FILE* pFile)
{
    
}


LineSegment::LineSegment( float width )
{
	m_Width = width;
	m_isSolid = false;
	m_ShapeType = EST_LINE;
}

LineSegment::LineSegment( const LineSegmentInfo& lineSegmentInfo)
{
    
}

void LineSegment::render()
{
	m_Transform.getPointer(matrixData);
	
//	glPushMatrix();
//	
//	glLoadMatrixf(matrixData);
//	
//	glBegin(GL_LINES);
//	glVertex2f(-m_Width*0.5f, 0.0f);
//	glVertex2f(m_Width*0.5f, 0.0f);
//	glEnd();
//	
//	glPopMatrix();
}

bool LineSegment::intersectRay( Ray& ray , IntersectionInformation& intersectInfo)
{
	if( !m_Pickable )
		return false;
	
	Ray genericRay = ray;
	// Inverse Transform Ray
	m_InvTransform.transformAsPoint(genericRay.origin);
	m_InvTransform.transformAsVector(genericRay.direction);

	float Ox = genericRay.origin.x; float Oy = genericRay.origin.y; 
	float Dx = genericRay.direction.x; float Dy = genericRay.direction.y;
	
	if( fabs(Dy) < 1e-15 )
		return false;
	
	float t = -Oy/Dy;
	
	float x0 = Ox + Dx * t;
	float y0 = Oy + Dy * t;

	if( x0 < -m_Width*0.5f || x0 > m_Width*0.5f )
		return false;
		
	intersectInfo.hitPoints[0].x = ray.origin.x + ray.direction.x * t;
	intersectInfo.hitPoints[0].y = ray.origin.y + ray.direction.y * t;
	intersectInfo.isHit = true;
	intersectInfo.hitNormal[0].x = 0.0f;
	intersectInfo.hitNormal[0].y = 1.0f;
	intersectInfo.numHits = 1;
	intersectInfo.T = t;
	
	if( y0 < 0.0f )
	{
		intersectInfo.hitNormal[0].y = -1.0f;
	}
	
	m_Transform.transformAsVector(intersectInfo.hitNormal[0]);

	return true;
}

bool LineSegment::pick(const Vector2f& p )
{
	return false;
}


void LineSegment::save(FILE* pFile)
{
	Shape2D::save(pFile);
	fprintf(pFile, "%s\n", "WIDTH::");
	fprintf(pFile, "%f\n", m_Width);
}

void LineSegment::load(FILE* pFile)
{
    
}

RegularPolygon::RegularPolygon( float size )
{
	m_Size = size;
	m_isSolid = true;
	m_RefractionIndex = 1.0f;
	m_ShapeType = EST_REGULAR_POLY;
}

RegularPolygon::RegularPolygon( const RegularPolygonInfo& regularPolygonInfo)
{
    
}

void RegularPolygon::render()
{
  	
}
	
bool RegularPolygon::intersectRay( Ray& ray , IntersectionInformation& intersectInfo)
{
	
	return false;
}
	
bool RegularPolygon::pick(const Vector2f& p )
{
	return false;
}

void RegularPolygon::save(FILE* pFile)
{
	Shape2D::save(pFile);
}

void RegularPolygon::load(FILE* pFile)
{
    
}

RectangleShape::RectangleShape( float width, float height )
{
	m_Width  = width;
	m_Height = height;
	m_isSolid = true;
	m_ShapeType = EST_RECT;
    m_isRefractive = true;
    m_RefractionIndex = 1.0f;
}

RectangleShape::RectangleShape( const RectangleInfo& rectangleInfo):Shape2D(rectangleInfo),
                                m_Width(rectangleInfo.width), m_Height(rectangleInfo.height)
{
    m_EdgeHit = -1;
}

void RectangleShape::render()
{
    float scale = 0.5f;
    
    Matrix4f xForm;
    xForm.set(m_Transform);
    Game::getInstance()->getRenderer()->setTransform(xForm, spider3d::WORLD);
    
    float vertices[8];
    float texCoords[8];
    float colors[16];
    
    vertices[0] = -m_Width*scale;
    vertices[1] = -m_Height*scale;
    
    texCoords[0] = 0.0f;
    texCoords[1] = 0.0f;
    
    vertices[2] =  m_Width*scale;
    vertices[3] = -m_Height*scale;
    
    texCoords[2] = 1.0f;
    texCoords[3] = 0.0f;
    
    vertices[4] =  m_Width*scale;
    vertices[5] =  m_Height*scale;
    
    texCoords[4] = 1.0f;
    texCoords[5] = 1.0f;
    
    vertices[6] = -m_Width*scale;
    vertices[7] = m_Height*scale;
    
    texCoords[6] = 0.0f;
    texCoords[7] = 1.0f;
    
    for( int i=0; i<16; i++ )
    {
        colors[i] = 1.0f;
    }
    
    Game::getInstance()->getRenderer()->drawVertexPrimitiveList(4, vertices, 2, colors, 4, texCoords, 2, 0, spider3d::EPT_TRIANGLE_FAN);	    
}

bool RectangleShape::intersectRay( Ray& ray , IntersectionInformation& intersectInfo)
{
    m_EdgeHit = -1;
    
	if( !m_Pickable )
		return false;
	
	Ray genericRay = ray;
	float halfWidth  = m_Width * 0.5f;
	float halfHeight = m_Height *0.5f; 
	// Inverse Transform Ray
	m_InvTransform.transformAsPoint(genericRay.origin);
	m_InvTransform.transformAsVector(genericRay.direction);
	
	float Ox = genericRay.origin.x; float Oy = genericRay.origin.y; 
	float Dx = genericRay.direction.x; float Dy = genericRay.direction.y;

	double tx_neg, tx_pos, ty_neg, ty_pos;
	float x_in = 10000.0f;
	float x_out = 10000.0f;
	float y_in = 10000.0f;
	float y_out = 10000.0f;
	
	double tin  = -100000.0f;
	double tout = -100000.0f;
	int index1 = -1;
	int index2 = -1;
	
	if( Dx != 0.0f )
	{
		tx_neg = -( halfWidth + Ox)/Dx;
		tx_pos =  ( halfWidth - Ox)/Dx;
		index1 = 1;
		index2 = 0;
		
		tin  = tx_neg;
		tout = tx_pos;
		
		if( tx_neg > tx_pos )
		{
			tin  = tx_pos;
			tout = tx_neg;
			index1 = 0;
			index2 = 1;
		}
	}
	if( Dy != 0.0f )
	{
		float t1,t2;
		t1 = ty_neg = -( halfHeight + Oy)/Dy;
		t2 = ty_pos =  ( halfHeight - Oy)/Dy;
		
		if( ty_neg > ty_pos )
		{
			t1  = ty_pos;
			t2  = ty_neg;
		}
		//tin  = tin > t1 ? tin : t1;
		if( tin > t1 )
		{

		}
		else {
			tin = t1;
			if( Dy > 0.0f )
			{
				index1 = 2;
				//index2 = 3;
			}
			if( Dy < 0.0f )
			{
				index1 = 3;
				//index2 = 2;
			}
		}

		
		//tout = tout < t2 ? tout : t2;
		if( tout < t2 )
		{
			//tout = t2;
		}
		else 
		{
			tout = t2;
			//index = 3;
            if( Dy > 0.0f )
			{
				index2 = 3;
				//index2 = 3;
			}
			if( Dy < 0.0f )
			{
				index2 = 2;
				//index2 = 2;
			}
		}

	}
	
	//if( tin < 0.0f )
	//	return false;
	
	if( tin > tout )
		return false;
	
	x_in = Ox + tin  * Dx;
	x_out = Ox + tout * Dx;
	
	
	y_in = Oy + tin * Dy;
	y_out = Oy + tout * Dy;
	
    if( index1 != -1 )
    {
        m_EdgeHit = index1;
    }
	if( index1 == 0 )
	{
		intersectInfo.hitNormal[0].x = 1.0f;
		intersectInfo.hitNormal[0].y = 0.0f;
	}
	if( index1 == 1 )
	{
		intersectInfo.hitNormal[0].x = -1.0f;
		intersectInfo.hitNormal[0].y = 0.0f;
	}
	if( index1 == 2 )
	{
		intersectInfo.hitNormal[0].x = 0.0f;
		intersectInfo.hitNormal[0].y = -1.0f;
	}
	if( index1 == 3 )
	{
		intersectInfo.hitNormal[0].x = 0.0f;
		intersectInfo.hitNormal[0].y = 1.0f;
	}
	if( index2 == 0 )
	{
		intersectInfo.hitNormal[1].x = 1.0f;
		intersectInfo.hitNormal[1].y = 0.0f;
	}
	if( index2 == 1 )
	{
		intersectInfo.hitNormal[1].x = -1.0f;
		intersectInfo.hitNormal[1].y = 0.0f;
	}
	if( index2 == 2 )
	{
		intersectInfo.hitNormal[1].x = 0.0f;
		intersectInfo.hitNormal[1].y = -1.0f;
	}
	if( index2 == 3 )
	{
		intersectInfo.hitNormal[1].x = 0.0f;
		intersectInfo.hitNormal[1].y = 1.0f;
	}
	   
	//if( fabs(x_in - halfWidth) < 1e-05 )
//	{
//		intersectInfo.hitNormal[0].x = 1.0f;
//		intersectInfo.hitNormal[0].y = 0.0f;
//	}
//	if( fabs(x_in + halfWidth) < 1e-05 )
//	{
//		intersectInfo.hitNormal[0].x = -1.0f;
//		intersectInfo.hitNormal[0].y = 0.0f;
//	}
//	if( fabs(y_in - halfHeight) < 1e-05 )
//	{
//		intersectInfo.hitNormal[0].x = 0.0f;
//		intersectInfo.hitNormal[0].y = 1.0f;
//	}
//	if( fabs(y_in + halfHeight) < 1e-05 )
//	{
//		intersectInfo.hitNormal[0].x = 0.0f;
//		intersectInfo.hitNormal[0].y = -1.0f;
//	}
//	 
//	if( fabs(x_out - halfWidth) < 1e-05 )
//	{
//		intersectInfo.hitNormal[1].x = 1.0f;
//		intersectInfo.hitNormal[1].y = 0.0f;
//	}
//	if( fabs(x_out + halfWidth) < 1e-05 )
//	{
//		intersectInfo.hitNormal[1].x = -1.0f;
//		intersectInfo.hitNormal[1].y = 0.0f;
//	}
//	if( fabs(y_out - halfHeight) < 1e-05 )
//	{
//		intersectInfo.hitNormal[1].x = 0.0f;
//		intersectInfo.hitNormal[1].y = 1.0f;
//	}
//	if( fabs(y_out + halfHeight) < 1e-05 )
//	{
//		intersectInfo.hitNormal[1].x = 0.0f;
//		intersectInfo.hitNormal[1].y = -1.0f;
//	}
	
	if( tin < 0.0f && tout > 0.0f )
	{
		tin = tout;
        Vector2f temp = intersectInfo.hitNormal[0];
        intersectInfo.hitNormal[0] = intersectInfo.hitNormal[1];
        intersectInfo.hitNormal[1] = temp;
        
		//intersectInfo.hitNormal[0].x *= -1.0f;
		//intersectInfo.hitNormal[0].y *= -1.0f;
		//intersectInfo.hitNormal[1].x *= -1.0f;
		//intersectInfo.hitNormal[1].y *= -1.0f;
	}
	
	intersectInfo.hitPoints[0].x = ray.origin.x + tin * ray.direction.x;
	intersectInfo.hitPoints[0].y = ray.origin.y + tin * ray.direction.y;
	
	intersectInfo.hitPoints[1].x = ray.origin.x + tout * ray.direction.x;
	intersectInfo.hitPoints[1].y = ray.origin.y + tout * ray.direction.y;
	
	m_Transform.transformAsVector(intersectInfo.hitNormal[0]);
	m_Transform.transformAsVector(intersectInfo.hitNormal[1]);
    
	intersectInfo.numHits = 2;
	intersectInfo.isHit = true;
	intersectInfo.T = tin;

	return true;
}

bool RectangleShape::pick(const Vector2f& p )
{
	Vector2f v(p.x, p.y);
	m_InvTransform.transformAsPoint(v);
	bool bRes = v.x <= m_Width*0.5f && v.x >= -m_Width*0.5f && v.y<= m_Height*0.5f && v.y>= -m_Height*0.5f;
	return bRes;
}

void RectangleShape::save(FILE* pFile)
{
	Shape2D::save(pFile);
	fprintf(pFile, "%s\n", "DIMENSIONS::");
	fprintf(pFile, "%f  %f\n", m_Width, m_Height);
}

void RectangleShape::load(FILE* pFile)
{
    
}

SphericalLens::SphericalLens( float height, float radius ):m_Height(height), m_Radius(radius)
{
	m_isSolid = true;
	if( m_Height > 1.9f * m_Radius )
	{
		m_Radius = m_Height/1.9f;
	}
	   
	float cstheta = sqrt(1.0f - m_Height*m_Height/(m_Radius*m_Radius*4.0f));
	
	m_Width = 2.0f * m_Radius * ( 1.0f - cstheta );
	
	m_CoC1.x = -m_Radius * cstheta;
	m_CoC1.y = 0.0f;
	m_CoC2.x = m_Radius * cstheta;
	m_CoC2.y = 0.0f;
	
	Angle = M_PI - atan2f(m_Height*0.5f, -m_Radius * cstheta);

	m_ShapeType = EST_LENS;

}

SphericalLens::SphericalLens( const SphericalLensInfo& sphericalLensInfo)
{
    
}

void SphericalLens::render()
{
	m_Transform.getPointer(matrixData);
	
//	glPushMatrix();
//	
//	glLoadMatrixf(matrixData);
//	
//	glColor3ub(255, 255, 255);
//	
//	float theta = -Angle;
//
//	float alpha = (Angle)/10;
//	glBegin(GL_LINES);
//	for( int i=0; i<20; i++ )
//	{
//		float xr1 = m_CoC1.x + m_Radius * cos(theta);
//		float yr1 = m_Radius * sin(theta);
//		float xr2 = m_CoC1.x + m_Radius * cos(theta + alpha);
//		float yr2 = m_Radius * sin(theta + alpha);
//		
//		glVertex2f(xr1, yr1);
//		glVertex2f(xr2, yr2);
//	
//		theta += alpha;
//	}
//	
//	theta = (M_PI + fabs(theta)); 
//	for( int i=0; i<20; i++ )
//	{
//		float xr1 =  m_CoC2.x + m_Radius * cos(theta);
//		float yr1 = m_Radius * sin(theta);
//		float xr2 =  m_CoC2.x + m_Radius * cos(theta - alpha);
//		float yr2 = m_Radius * sin(theta - alpha);
//		
//		glVertex2f(xr1, yr1);
//		glVertex2f(xr2, yr2);
//		
//		theta -= alpha;
//	}
//	
//	glEnd();
//	
//	glPopMatrix();
}

bool SphericalLens::intersectRay( Ray& ray , IntersectionInformation& intersectInfo)
{
	if( !m_Pickable )
		return false;
	
	bool bResult1 = false;
	bool bResult2 = false;

	IntersectionInformation info1, info2;
	bResult1 = intersectSphericalFace(ray, info1, 0);
	bResult2 = intersectSphericalFace(ray, info2, 1);

	if( bResult1 || bResult2)
	{
		intersectInfo = info1.T < info2.T ? info1 : info2;
	}
	//else if( bResult1 && !bResult2 )
//	{
//		intersectInfo = info1;
//	}
//	else if( !bResult1 && bResult2 )
//	{
//		intersectInfo = info2;
//	}
//	else
//	{
//		
//	}
	
	return (bResult1 || bResult2);
}


bool SphericalLens::intersectSphericalFace(Ray& ray , IntersectionInformation& intersectInfo, int face)
{
	Ray genericRay = ray;
	// Inverse Transform Ray
	m_InvTransform.transformAsPoint(genericRay.origin);
	m_InvTransform.transformAsVector(genericRay.direction);
	
	double Ox;
	double DeltaX;
	if( face == 0)
	{
		Ox = genericRay.origin.x - m_CoC1.x; 
		DeltaX = m_CoC1.x;
	}
	if( face == 1)
	{
		Ox = genericRay.origin.x - m_CoC2.x;
		DeltaX = m_CoC2.x;
	}
	
	double Oy = genericRay.origin.y; 
	double Dx = genericRay.direction.x; float Dy = genericRay.direction.y;
	static double roots[] = { 0.0f , 0.0f };
	
	double r2 = m_Radius*m_Radius;
	double A =  Dx*Dx + Dy*Dy;
	double B =  Ox*Dx + Oy*Dy;
	double C = Ox*Ox + Oy*Oy - r2;
	
	short ret;
	double D = ( B*B - A*C );
	if ( D < 1e-15f ) 
	{
		ret = 0; // no solution
		return false;
	}
	else
	{
		D = sqrt(D);
		double A2 = 1.0f/A;
		if ( D < 1E-10 )
		{
			roots[0] = roots[1] = -B*A2;
			ret = 1;
			intersectInfo.hitPoints[0].x = ray.origin.x + ray.direction.x*roots[0];
			intersectInfo.hitPoints[0].y = ray.origin.y + ray.direction.y*roots[0];
			intersectInfo.isHit = true;
			intersectInfo.hitNormal[0].x = Ox + Dx * roots[0];
			intersectInfo.hitNormal[0].y = Oy + Dy * roots[0];
			intersectInfo.hitNormal[0].normalize();
			m_Transform.transformAsVector(intersectInfo.hitNormal[0]);
			intersectInfo.numHits = 1;
			intersectInfo.T = roots[0];
			return true;
		}
		
		roots[0] = (-B - D)*A2;
		roots[1] = (-B + D)*A2;
		
		// Origin of the Ray is Inside the Circular Arc.
		bool bSourceInsideArc = false;
		if( roots[0] < 0.0f && roots[1] > 0.0f)
		{
			bSourceInsideArc = true;
			roots[0] = roots[1];
		}
		
		float localx0 = Ox + Dx * roots[0] + DeltaX;
		float localy0 = Oy + Dy * roots[0];
		
		// Outside the Lens X Region
		if( fabs(localx0) - m_Width*0.5f > 1e-04 )
			return false;
		
		// Outside the Lens Y Region
		if( fabs(localy0) > m_Height*0.5f )
			return false;
			
		//bool bFlipNormal = false;
		
		if( roots[1] < 0.0f )
			return false;
		
		//if( bSourceInsideArc )
		//	return false;
		
		//roots[0] = roots[1];
		//bFlipNormal = true;
		
		//float localx1 = Ox + Dx * roots[1];
//		float localy1 = Oy + Dy * roots[1];
//
//		if( fabs(localy1) < m_Height*0.5f )
//		{
//			return false;
//		}
		
		bool bFlipNormal = false;
		
		double O_x = Ox - genericRay.direction.x * 0.01f;
		double O_y = Oy - genericRay.direction.y * 0.01f;
//		
		double Diff = m_Radius * m_Radius - O_x*O_x - O_y*O_y;
		// If the Source/Origin of the Ray is within the Circle of this Arc's Radius, Flip Normal
		if( Diff > 1e-02 )
		{
			bFlipNormal = true;
			//roots[0] = roots[1];
		}
		
		intersectInfo.hitNormal[0].x = Ox + Dx * roots[0];
		intersectInfo.hitNormal[0].y = Oy + Dy * roots[0];
		// If Hit Point is inside the Arc region, Flip Hit Normal
		if( bFlipNormal )
		{
			//intersectInfo.hitNormal[0].x *= -1.0f;
			//intersectInfo.hitNormal[0].y *= -1.0f;
		}
		
		ret = 2;
		
		intersectInfo.hitPoints[0].x = ray.origin.x + ray.direction.x*roots[0];
		intersectInfo.hitPoints[0].y = ray.origin.y + ray.direction.y*roots[0];
		intersectInfo.isHit = true;
		
		intersectInfo.hitNormal[0].normalize();
		
		m_Transform.transformAsVector(intersectInfo.hitNormal[0]);
		
		intersectInfo.hitPoints[1].x = ray.origin.x + ray.direction.x*roots[1];
		intersectInfo.hitPoints[1].y = ray.origin.y + ray.direction.y*roots[1];
		intersectInfo.isHit = true;
		//intersectInfo.Normal.x = intersectInfo.hitPoints[1].x - m_Transform.m03;
		//intersectInfo.Normal.y = intersectInfo.hitPoints[1].y - m_Transform.m13;
		//intersectInfo.Normal.normalize();
		//m_Transform.transform(intersectInfo.Normal);
		intersectInfo.numHits = 2;
		intersectInfo.T = roots[0];
	}
	return true;
}

bool SphericalLens::pick(const Vector2f& p )
{
	return false;
}

void SphericalLens::save(FILE* pFile)
{
	Shape2D::save(pFile);
}

void SphericalLens::load(FILE* pFile)
{
    
}