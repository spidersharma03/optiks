//
//  GLSL2ShaderProgram.h
//  Optiks
//
//  Created by Prashant on 22/01/14.
//
//

#ifndef Optiks_GLSL2ShaderProgram_h
#define Optiks_GLSL2ShaderProgram_h

#ifndef IOS
    #include <OpenGL/gl.h>
#endif

#ifdef IOS
    #include <OpenGLES/ES2/gl.h>
#endif

#include "ShaderProgram.h"
#include "Game.h"
#include "OGL2Renderer.h"

class Shader;
namespace spider3d
{
    namespace render
    {
        class OglRenderer;
    }
}

using namespace spider3d::render;

class GLSL2ShaderProgram:public ShaderProgram
{
    GLSL2ShaderProgram()
	{
		shaderPrgram = glCreateProgram();
	}
    
    GLSL2ShaderProgram(Shader* vertexShader, Shader* fragmentShader)
	{
		shaderPrgram = glCreateProgram();
        setVertexShader(vertexShader);
        setFragmentShader(fragmentShader);
        setDefaultUniforms();
	}

    void setDefaultUniforms()
    {
        ShaderUniform *suModelView = new ShaderUniform();
        suModelView->m_Name = (char*)"ModelViewMatrix";
        suModelView->m_VarType = ESVT_MAT4;
        suModelView->m_Value = new float[16];
        addShaderUniform(suModelView);
        
        ShaderUniform *suProjection = new ShaderUniform();
        suProjection->m_Name = (char*)"ProjectionMatrix";
        suProjection->m_VarType = ESVT_MAT4;
        suProjection->m_Value = new float[16];
        addShaderUniform(suProjection);
    }
    
    void compileShader( GLuint shader, SHADER_TYPE shaderType )
    {
        //if ( m_pVertexShader && m_pVertexShader->getShaderSource() )
		{
			const char* shaderSource = m_pVertexShader->getShaderSource();
			// 2. Attach Shader Code
			glShaderSource(vs, 1, &shaderSource ,NULL);
			
			// 3. Compile Shader
			glCompileShader(vs);
			
            GLint logLength;
            glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &logLength);
            if (logLength > 0)
            {
                GLchar *log = (GLchar *)malloc(logLength);
                glGetShaderInfoLog(vs, logLength, &logLength, log);
                printf("%s\n",log);
                free(log);
                assert(0);
            }
        }
    }
    friend class Ogl2Renderer;
    friend class OglES2Renderer;
public:
    
    void setShaders(Shader* vertexShader, Shader* fragmentShader)
    {
        m_pVertexShader = vertexShader;
        m_pFragmentShader = fragmentShader;
        // COMPILE VERTEX SHADER
        if ( m_pVertexShader && m_pVertexShader->getShaderSource() )
		{
			// 1. Create ShaderObject
			vs = glCreateShader(GL_VERTEX_SHADER);
			
			const char* shaderSource = m_pVertexShader->getShaderSource();
			// 2. Attach Shader Code
			glShaderSource(vs, 1, &shaderSource ,NULL);
            
            // 3. Compile Shader
			glCompileShader(vs);
        }
        // COMPILE FRAGMENT SHADER
        if ( m_pFragmentShader && m_pFragmentShader->getShaderSource() )
		{
			// 1. Create ShaderObject
			fs = glCreateShader(GL_FRAGMENT_SHADER);
			
			const char* shaderSource = m_pFragmentShader->getShaderSource();
			// 2. Attach Shader Code
			glShaderSource(fs, 1, &shaderSource ,NULL);
			
			// 3. Compile Shader
			glCompileShader(fs);
			
            GLint logLength;
            glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &logLength);
            if (logLength > 0)
            {
                GLchar *log = (GLchar *)malloc(logLength);
                glGetShaderInfoLog(fs, logLength, &logLength, log);
                printf("%s\n",log);
                free(log);
                //assert(0);
            }
        }
        // Attach Vertex Shader Object to Shader Program
        glAttachShader(shaderPrgram,vs);
        // Attach Fragment Shader Object to Shader Program
        glAttachShader(shaderPrgram,fs);
        
        // Link Program
        glLinkProgram(shaderPrgram);
        
        GLint logLength;
        glGetProgramiv(shaderPrgram, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            GLchar *log = (GLchar *)malloc(logLength);
            glGetProgramInfoLog(shaderPrgram, logLength, &logLength, log);
            printf("%s\n",log);
            free(log);
            //assert(0);
        }
        setDefaultUniforms();
        ShaderAttribute *posAttribute = new ShaderAttribute();
        posAttribute->m_Name = "position";
        addShaderAttribute(posAttribute);
        
        ShaderAttribute *colorAttribute = new ShaderAttribute();
        colorAttribute->m_Name = "color";
        addShaderAttribute(colorAttribute);
        
        ShaderAttribute *texCoordAttribute = new ShaderAttribute();;
        texCoordAttribute->m_Name = "texCoord";
        addShaderAttribute(texCoordAttribute);
    }

	void setVertexShader(Shader* vertexShader)
	{
		m_pVertexShader = vertexShader;
		if ( m_pVertexShader && m_pVertexShader->getShaderSource() )
		{
			// 1. Create ShaderObject
			vs = glCreateShader(GL_VERTEX_SHADER);
			
			const char* shaderSource = m_pVertexShader->getShaderSource();
			// 2. Attach Shader Code
			glShaderSource(vs, 1, &shaderSource ,NULL);
			
			// 3. Compile Shader
			glCompileShader(vs);
			
            GLint logLength;
            glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &logLength);
            if (logLength > 0)
            {
                GLchar *log = (GLchar *)malloc(logLength);
                glGetShaderInfoLog(vs, logLength, &logLength, log);
                printf("%s\n",log);
                free(log);
                assert(0);
            }
            
			// 4. Attach Shader Object to Shader Program
			glAttachShader(shaderPrgram,vs);
			
			// 5. Link Shader Program
			glLinkProgram(shaderPrgram);
            
            glGetProgramiv(shaderPrgram, GL_INFO_LOG_LENGTH, &logLength);
            if (logLength > 0) {
                GLchar *log = (GLchar *)malloc(logLength);
                glGetProgramInfoLog(shaderPrgram, logLength, &logLength, log);
                printf("%s\n",log);
                free(log);
                //assert(0);
            }
		}
        // By Default the Vertex Shader will have built in Uniforms for ModelView and Projection matrices
//        ShaderUniform *suModelView = new ShaderUniform();
//        suModelView->m_Name = (char*)"ModelViewMatrix";
//        suModelView->m_VarType = ESVT_MAT4;
//        suModelView->m_Value = new float[16];
//        addShaderUniform(suModelView);
//        
//        ShaderUniform *suProjection = new ShaderUniform();
//        suProjection->m_Name = (char*)"ProjectionMatrix";
//        suProjection->m_VarType = ESVT_MAT4;
//        suProjection->m_Value = new float[16];
//        addShaderUniform(suProjection);
    
        ShaderAttribute *posAttribute = new ShaderAttribute();
        posAttribute->m_Name = "position";
        addShaderAttribute(posAttribute);
        
        ShaderAttribute *colorAttribute = new ShaderAttribute();
        colorAttribute->m_Name = "color";
        addShaderAttribute(colorAttribute);
        
        ShaderAttribute *texCoordAttribute = new ShaderAttribute();;
        texCoordAttribute->m_Name = "texCoord";
        addShaderAttribute(texCoordAttribute);
	}
	
	void setFragmentShader(Shader* fragmentShader)
	{
		m_pFragmentShader = fragmentShader;
		if ( m_pFragmentShader && m_pFragmentShader->getShaderSource() )
		{
			// 1. Create ShaderObject
			fs = glCreateShader(GL_FRAGMENT_SHADER);
			
			const char* shaderSource = m_pFragmentShader->getShaderSource();
			// 2. Attach Shader Code
			glShaderSource(fs, 1, &shaderSource ,NULL);
			
			// 3. Compile Shader
			glCompileShader(fs);
			
            GLint logLength;
            glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &logLength);
            if (logLength > 0)
            {
                GLchar *log = (GLchar *)malloc(logLength);
                glGetShaderInfoLog(fs, logLength, &logLength, log);
                printf("%s\n",log);
                free(log);
                //assert(0);
            }
			// 4. Attach Shader Object to Shader Program
			glAttachShader(shaderPrgram,fs);
			
			// 5. Link Shader Program
			glLinkProgram(shaderPrgram);
            
            glGetProgramiv(shaderPrgram, GL_INFO_LOG_LENGTH, &logLength);
            if (logLength > 0) {
                GLchar *log = (GLchar *)malloc(logLength);
                glGetProgramInfoLog(shaderPrgram, logLength, &logLength, log);
                printf("%s\n",log);
                free(log);
                //assert(0);
            }
		}
	}
	
	void addShaderUniform(ShaderUniform* shaderUniform)
	{
		GLint location = -1;
		static int i = -1;
		i++;
		if ( shaderUniform )
		{
			glUseProgram(shaderPrgram);
			location = glGetUniformLocation(shaderPrgram, shaderUniform->m_Name);
			glUseProgram(0);
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
			glUseProgram(shaderPrgram);
			location = glGetAttribLocation(shaderPrgram, name);
			if( location > -1 )
			{
				m_vecShaderAttributeLocation.push_back(location);
			}
			glUseProgram(0);
		}
	}
	
    virtual void addShaderAttribute(ShaderAttribute* shaderAttribute)
    {
        GLint location = -1;
		if ( shaderAttribute->m_Name.c_str() )
		{
			glUseProgram(shaderPrgram);
			location = glGetAttribLocation(shaderPrgram, shaderAttribute->m_Name.c_str());
			if( location > -1 )
			{
				shaderAttribute->m_Location = location;
                m_vecShaderAttributes.push_back(shaderAttribute);
			}
			glUseProgram(0);
		}
    }
    
    virtual void bindShaderAttribute(ShaderAttribute* shaderAttribute)
    {
		if ( shaderAttribute->m_Name.size() && shaderAttribute->m_Location >= 0 )
		{
			glUseProgram(shaderPrgram);
            glBindAttribLocation(shaderPrgram, shaderAttribute->m_Location, shaderAttribute->m_Name.c_str());
            m_vecShaderAttributes.push_back(shaderAttribute);
			glUseProgram(0);
		}
    }
    
	void OnSet()
	{
        Game::getInstance()->getRenderer()->setCurrentShader(this);
		// Set Program
		glUseProgram(shaderPrgram);
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
					glUniform1i(location,val);
				}
					break;
				case ESVT_SINGLE_FLOAT:
				{
					float val = *shaderUniform->m_Value;
					glUniform1f(location,val);
				}
					break;
				case ESVT_VEC2:
					glUniform2fv(location, 1, value);
					break;
				case ESVT_VEC3:
					glUniform3fv(location, 1, value);
					break;
				case ESVT_VEC4:
					glUniform4fv(location, 1, value);
					break;
				case ESVT_MAT2:
					glUniformMatrix2fv(location, 1, false, value);
					break;
				case ESVT_MAT3:
					glUniformMatrix3fv(location, 1, false, value);
					break;
				case ESVT_MAT4:
					glUniformMatrix4fv(location, 1, false, value);
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
		glUseProgram(0);
	}
	
    GLuint getProgram() const
    {
        return shaderPrgram;
    }
    
private:
	// Handle to Vertex and Fragment Shaders
	GLuint vs, fs;
	// Handle to Shader Program
	GLuint shaderPrgram;
};


#endif
