#ifndef GRAPHICS_SYSTEM_H
#define GRAPHICS_SYSTEM_H

#include <Object.h>

namespace engine
{
	class Shader;
	class Window;
	class Texture;

	class GraphicsSystem : public Object
	{
	public:
		GraphicsSystem();
		~GraphicsSystem();

		virtual void clearScreen(float red, float green, float blue) = 0;
		virtual void drawTriangle(Shader* shader, Texture* texture, float textCoords[], float vertices[], int numVertices) = 0;

		virtual void swapBuffers() = 0;
	};
}

#endif