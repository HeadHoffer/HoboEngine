#ifndef GRAPHICS_SYSTEM_H
#define GRAPHICS_SYSTEM_H

#include <Object.h>
#include <GLES2\gl2.h>

namespace engine
{
	class Shader;
	class Window;
	class Texture;
	class PNGFile;

	class GraphicsSystem : public Object
	{
	public:
		GraphicsSystem();
		~GraphicsSystem();

		virtual int initText(const char* fontFilename) = 0;

		virtual void clearScreen(float red, float green, float blue) = 0;
		virtual void drawTriangle(Shader* shader, Texture* texture, float textCoords[], float vertices[], int numVertices) = 0;
		//virtual void drawPNG(Shader* shader, GLuint texture) = 0;
		virtual void drawText(Shader* shader, const char *text, float x, float y, float sx, float sy, float r, float g, float b) = 0;
		//virtual void setTexture(PNGFile* png) = 0;

		virtual void swapBuffers() = 0;
	};
}

#endif