#ifndef TEXTURE_H
#define TEXTURE_H

#include <Object.h>
#include <GLES2/gl2.h>

namespace engine
{
	class Texture : public Object
	{
	public:
		Texture(int width, int height, int bytesPerPixel, const GLubyte* pixels);
		~Texture();

		//void textTexture();
		GLuint getTextureID() const;

	private:
		GLuint m_textureID;
	};
}
#endif