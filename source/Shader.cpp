#include <Shader.h>
#include <stdio.h>
#include <malloc.h>

namespace engine
{
	Shader::Shader(const char* const vShaderStr, const char* const fShaderStr) : Object(),
		m_programObject(0)
	{
		GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vShaderStr);
		GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fShaderStr);

		m_programObject = glCreateProgram();

		if (m_programObject == 0)
		{
			printf("ERROR!\n");
			assert(0);
		}

		glAttachShader(m_programObject, vertexShader);
		glAttachShader(m_programObject, fragmentShader);

		glBindAttribLocation(m_programObject, 0, "vPosition");
		glLinkProgram(m_programObject);

		GLint linked;
		glGetProgramiv(m_programObject, GL_LINK_STATUS, &linked);

		if (!linked)
		{
			GLint infoLen = 0;

			glGetProgramiv(m_programObject, GL_INFO_LOG_LENGTH, &infoLen);
			
			if (infoLen > 1)
			{
				char* infoLog = (char*)malloc(sizeof(char)* infoLen);
				glGetProgramInfoLog(m_programObject, infoLen, NULL, infoLog);
				printf("ERROR!\n");
				free(infoLog);
				assert(0);
			}

			glDeleteProgram(m_programObject);
			return;
		}
	}

	Shader::~Shader()
	{
	}

	void Shader::UseShader()
	{
		glUseProgram(m_programObject);
	}

	GLuint Shader::getUniformLocation(const char* const uniformName)
	{
		return glGetUniformLocation(m_programObject, uniformName);
	}

	GLuint Shader::loadShader(GLenum type, const char* shaderSrc)
	{
		GLuint shader;
		
		shader = glCreateShader(type);

		if (shader == 0)
			return 0;

		glShaderSource(shader, 1, &shaderSrc, 0);
		glCompileShader(shader);

		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

		if (!compiled)
		{
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen > 1)
			{
				char* infoLog = (char*)malloc(sizeof(char)* infoLen);
				glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
				printf("ERROR!\n");
				free(infoLog);
				assert(0);
			}

			glDeleteShader(shader);
			return -1;
		}

		return shader;
	}
}


