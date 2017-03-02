#ifndef _SOURCE_CODE_SHADER_H

#define _SOURCE_CODE_SHADER_H

#include "Shader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class SourceCodeShader:public Shader
{
public:
	SourceCodeShader(SHADER_TYPE shaderType, SHADING_LANGUAGE shadingLanguage, const char *shaderFileName ):Shader(shaderType, shadingLanguage)
	{
		m_pShaderSourceCode = readShaderFile(shaderFileName);
		if ( !m_pShaderSourceCode )
		{
			printf("unable to read Shader File%s\n", shaderFileName );
		}
	}
	
	
private:
	char* readShaderFile(const char* fileName)
	{
		FILE *fp;
		char *content = NULL;
		
		int count=0;
		
		if (fileName != NULL) {
			fp = fopen(fileName,"rt");
			
			if (fp != NULL) {
				
				fseek(fp, 0, SEEK_END);
				count = ftell(fp);
				rewind(fp);
				
				if (count > 0) {
					content = (char *)malloc(sizeof(char) * (count+1));
					count = fread(content,sizeof(char),count,fp);
					content[count] = '\0';
				}
				fclose(fp);
			}
		}
		return content;
	}
	
};

#endif