#ifndef SHADER_H
#define SHADER_H

#include <Object.h>
#include <GLES2/gl2.h>

namespace engine
{
	class Shader : public Object
	{
	public:
		Shader(const char* const vShaderStr, const char* const fShaderStr);
		~Shader();

		void UseShader();
		GLuint getUniformLocation(const char* const uniformName);

	private:
		GLuint m_programObject;
		GLuint loadShader(GLenum type, const char* shaderSrc);
	};
}

#endif
