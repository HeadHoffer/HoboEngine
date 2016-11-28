#ifndef TEXT_TEXTURE_H
#define TEXT_TEXTURE_H

#include <Object.h>
#include <GLES2/gl2.h>
#include <Shader.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace engine
{
	class TextTexture : public Object
	{
	public:
		TextTexture(Shader* shader);
		~TextTexture();
		
		FT_Library m_ft;
		FT_Face m_face;
		GLuint m_vbo;

	private:

	};
}
#endif