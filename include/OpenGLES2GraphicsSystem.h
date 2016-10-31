#ifndef OPENGLES2_GRAPHICS_SYSTEM_H
#define OPENGLES2_GRAPHICS_SYSTEM_H

#include <Window.h>
#include <GraphicsSystem.h>
#include <Ref.h>

#include <EGL/egl.h>
#include <GLES2/gl2.h>

namespace engine
{
	class Window;

	class OpenGLES2GraphicsSystem : public GraphicsSystem
	{
	public:
		OpenGLES2GraphicsSystem(Window* window);
		~OpenGLES2GraphicsSystem();

		virtual void clearScreen(float red, float green, float blue);
		//virtual void drawTriangle(Shader* shader, float vertices[], int numvertices);
		virtual void drawTriangle(Shader* shader, Texture* texture, float textCords[], float vertices[], int numVertices);

		virtual void swapBuffers();

	private:
		engine::Ref<Window> m_window;
		bool m_active;
		
		EGLDisplay m_eglDisplay;
		EGLContext m_eglContext;
		EGLSurface m_eglSurface;	
		
	};
}

#endif
