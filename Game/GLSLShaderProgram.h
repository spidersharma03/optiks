#ifndef IOS

#ifndef _GLSL_SHADER_PROGRAM_H
#define _GLSL_SHADER_PROGRAM_H

#ifndef IOS
    #include <OpenGL/gl.h>
#endif

#ifdef IOS
    #include <OpenGLES/ES2/gl.h>
#endif

#include "ShaderProgram.h"
#include "Game.h"
#include "OglRenderer.h"


class Shader;

namespace spider3d
{
    namespace render
    {
        class OglRenderer;
    }
}

using namespace spider3d::render;

class GLSLShaderProgram:public ShaderProgram
{
private:
    GLSLShaderProgram()
	{
		shaderPrgram = glCreateProgramObjectARB();
	}
    
    GLSLShaderProgram(Shader* vertexShader, Shader* fragmentShader)
	{
		shaderPrgram = glCreateProgramObjectARB();
        setVertexShader(vertexShader);
        setFragmentShader(fragmentShader);
	}
    
    friend class OglRenderer;
    
public:
    void setShaders(Shader* vertexShader, Shader* fragmentShader)
    {
        setVertexShader(vertexShader);
        setFragmentShader(fragmentShader);
    }
    
	void setVertexShader(Shader* vertexShader)
	{
		m_pVertexShader = vertexShader;
		if ( m_pVertexShader && m_pVertexShader->getShaderSource() )
		{
			// 1. Create ShaderObject
			vs = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
			
			const char* shaderSource = m_pVertexShader->getShaderSource();
			// 2. Attach Shader Code
			glShaderSourceARB(vs, 1, &shaderSource ,NULL);
			
			// 3. Compile Shader
			glCompileShaderARB(vs);
			
			// 4. Attach Shader Object to Shader Program
			glAttachObjectARB(shaderPrgram,vs);
			
			// 5. Link Shader Program
			glLinkProgramARB(shaderPrgram);
		}
	}
	
	void setFragmentShader(Shader* fragmentShader)
	{
		m_pFragmentShader = fragmentShader;
		if ( m_pFragmentShader && m_pFragmentShader->getShaderSource() )
		{
			// 1. Create ShaderObject
			fs = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
			
			const char* shaderSource = m_pFragmentShader->getShaderSource();
			// 2. Attach Shader Code
			glShaderSourceARB(fs, 1, &shaderSource ,NULL);
			
			// 3. Compile Shader
			glCompileShaderARB(fs);
			
			// 4. Attach Shader Object to Shader Program
			glAttachObjectARB(shaderPrgram,fs);
			
			// 5. Link Shader Program
			glLinkProgramARB(shaderPrgram);
		}
	}
	
	void addShaderUniform(ShaderUniform* shaderUniform) 
	{
		GLint location = -1;
		static int i = -1;
		i++;
		if ( shaderUniform )
		{
			glUseProgramObjectARB(shaderPrgram);
			location = glGetUniformLocationARB(shaderPrgram, shaderUniform->m_Name);
			glUseProgramObjectARB(0);
			if ( location > -1 )
			{
				m_vecShaderUniforms.push_back(shaderUniform);
				shaderUniform->m_Location = location;
			}
		}
	}
	
	void addShaderAttributeName(const char *name )
	{
		GLint location = -1;
		if ( name )
		{
			glUseProgramObjectARB(shaderPrgram);
			location = glGetAttribLocationARB(shaderPrgram, name);
			if( location > -1 )
			{
				m_vecShaderAttributeLocation.push_back(location);
			}
			glUseProgramObjectARB(0);
		}
	}
	
    virtual void addShaderAttribute(ShaderAttribute* shaderAttribute)
    {
        GLint location = -1;
		if ( shaderAttribute->m_Name.c_str() )
		{
			glUseProgramObjectARB(shaderPrgram);
			location = glGetAttribLocationARB(shaderPrgram, shaderAttribute->m_Name.c_str());
			if( location > -1 )
			{
				shaderAttribute->m_Location = location;
                m_vecShaderAttributes.push_back(shaderAttribute);
			}
			glUseProgramObjectARB(0);
		}
    }
    
    virtual void bindShaderAttribute(ShaderAttribute* shaderAttribute)
    {
        GLint location = -1;
		if ( shaderAttribute->m_Name.c_str() && shaderAttribute->m_Location > 0 )
		{
			glUseProgramObjectARB(shaderPrgram);
			location = glGetAttribLocationARB(shaderPrgram, shaderAttribute->m_Name.c_str());
            glBindAttribLocationARB(shaderPrgram, shaderAttribute->m_Location, shaderAttribute->m_Name.c_str());
            m_vecShaderAttributes.push_back(shaderAttribute);
			glUseProgramObjectARB(0);
		}
    }
    
	void OnSet()
	{
        Game::getInstance()->getRenderer()->setCurrentShader(this);
		// Set Program
		glUseProgramObjectARB(shaderPrgram);
		// Set Uniform variables for this Shader
		for ( unsigned i=0; i<m_vecShaderUniforms.size(); i++)
		{
			ShaderUniform *shaderUniform = m_vecShaderUniforms[i];
			GLint location = shaderUniform->m_Location;
			float* value    = shaderUniform->m_Value;
			switch( shaderUniform->m_VarType )
			{
				case ESVT_SINGLE_INT:
				{
					int val = *shaderUniform->m_Value;
					glUniform1iARB(location,val);
				}
					break;
				case ESVT_SINGLE_FLOAT:
				{
					float val = *shaderUniform->m_Value;
					glUniform1fARB(location,val);
				}
					break;
				case ESVT_VEC2:
					glUniform2fvARB(location, 1, value);
					break;
				case ESVT_VEC3:
					glUniform3fvARB(location, 1, value);
					break;
				case ESVT_VEC4:
					glUniform4fvARB(location, 1, value);
					break;
				case ESVT_MAT2:
					glUniformMatrix2fvARB(location, 1, false, value); 
					break;
				case ESVT_MAT3:
					glUniformMatrix3fvARB(location, 1, false, value);
					break;
				case ESVT_MAT4:
					glUniformMatrix4fvARB(location, 1, false, value);
					break;
				default:
					printf("Variable Type not Supported\n");
					break;
			}
		}
	}
	
	void UnSet()
	{
        Game::getInstance()->getRenderer()->setCurrentShader(0);
		glUseProgramObjectARB(0);
	}
	
private:
	// Handle to Vertex and Fragment Shaders
	GLhandleARB vs, fs;
	// Handle to Shader Program
	GLhandleARB shaderPrgram;
};


#endif

#endif // IOS