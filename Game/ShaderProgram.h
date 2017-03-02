#ifndef _SHADER_PROGRAM_H

#define _SHADER_PROGRAM_H

#include "SourceCodeShader.h"
#include <map>
#include <vector>
#include <string>

using namespace std;

class Shader;

enum SHADER_VARIABLE_TYPE { ESVT_SINGLE_INT, ESVT_SINGLE_FLOAT, ESVT_VEC2, ESVT_VEC3, ESVT_VEC4, ESVT_MAT2, ESVT_MAT3, ESVT_MAT4 };

struct ShaderAttribute
{
    string m_Name;
    unsigned int    m_Location;
};

struct ShaderUniform
{
public:
	SHADER_VARIABLE_TYPE m_VarType;
	char *m_Name;
	float *m_Value;
	
	friend class GLSLShaderProgram;
    friend class GLSL2ShaderProgram;
	friend class CGShaderProgram;
	int m_Location;
};

class ShaderProgram
{
public:
	
	ShaderProgram()
	{
	}
	
	virtual void setVertexShader(Shader* vertexShader) = 0;
	
	virtual void setFragmentShader(Shader* fragmentShader) = 0;
	
    virtual void setShaders(Shader* vertexShader, Shader* fragmentShader) = 0;

	virtual void addShaderUniform(ShaderUniform* shaderUniform) = 0;

    virtual void addShaderAttribute(ShaderAttribute* shaderAttribute) = 0;

    virtual void bindShaderAttribute(ShaderAttribute* shaderAttribute) = 0;

	ShaderUniform* getShaderUniformByName(const char *name )
	{
		ShaderUniform* outUniform = 0;
		for( int i=0; i<m_vecShaderUniforms.size(); i++ )
		{
            if( strcmp(name, m_vecShaderUniforms[i]->m_Name) == 0 )
                outUniform = m_vecShaderUniforms[i];
		}
		return outUniform;
	}
    
    ShaderAttribute* getShaderAttributeByName(const char *name )
	{
		ShaderAttribute* outShaderAttribute = 0;
		for( int i=0; i<m_vecShaderAttributes.size(); i++ )
		{
            if( strcmp(name, m_vecShaderAttributes[i]->m_Name.c_str()) == 0 )
				outShaderAttribute = m_vecShaderAttributes[i];
		}
		return outShaderAttribute;
	}
	
	virtual void addShaderAttributeName(const char *name ) = 0;
	
	inline vector<int>& getAllAttributeLocations()
	{
		return m_vecShaderAttributeLocation;
	}
		
public:
	virtual void OnSet() = 0;

	virtual void UnSet() = 0;

protected:
	Shader *m_pVertexShader;
	Shader *m_pFragmentShader;
	map<const char*, int> m_mapShaderUniformLocation;
	vector<ShaderUniform*> m_vecShaderUniforms;
    vector<ShaderAttribute*> m_vecShaderAttributes;
	vector<int> m_vecShaderAttributeLocation;
};


#endif