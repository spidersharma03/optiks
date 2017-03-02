#ifndef _SHADER_H

#define _SHADER_H

enum SHADER_TYPE { EST_VERTEX, EST_FRAGMENT };

enum SHADING_LANGUAGE { ESL_GLSL, ESL_CG, ESL_HLSL };

class Shader
{
public:
	
	Shader(SHADER_TYPE shaderType, SHADING_LANGUAGE shadingLanguage):m_eShaderType(shaderType),m_eShadingLanguage(shadingLanguage)
	{
	}
	
	inline SHADER_TYPE getShaderType()
	{
		return m_eShaderType;
	}
	
	inline SHADING_LANGUAGE getShadingLanguage()
	{
		return m_eShadingLanguage;
	}
	
	inline const char* getShaderSource()
	{
		return m_pShaderSourceCode;
	}
	
	
	
protected:
	SHADER_TYPE m_eShaderType;
	SHADING_LANGUAGE m_eShadingLanguage;
	char *m_pShaderSourceCode;
};

#endif