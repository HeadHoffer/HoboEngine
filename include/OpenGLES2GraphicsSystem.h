#ifndef OPENGLES2_GRAPHICS_SYSTEM_H
#define OPENGLES2_GRAPHICS_SYSTEM_H

#include <Window.h>
#include <GraphicsSystem.h>
#include <Ref.h>

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <PNGFile.h>

//freetype headers
#include <ft2build.h>
#include FT_FREETYPE_H

#include <lpng1626\png.h>

namespace engine
{
	class Window;

	class OpenGLES2GraphicsSystem : public GraphicsSystem
	{
	public:
		OpenGLES2GraphicsSystem(Window* window);
		~OpenGLES2GraphicsSystem();

		virtual int initText(const char* fontFilename);

		virtual void clearScreen(float red, float green, float blue);
		//virtual void drawTriangle(Shader* shader, float vertices[], int numvertices);
		virtual void drawTriangle(Shader* shader, Texture* texture, float textCords[], float vertices[], int numVertices);
		//virtual void drawPNG(Shader* shader, GLuint texture);
		//text drawing here ---> application
		virtual void drawText(Shader* shader, const char *text, float x, float y, float sx, float sy, float r, float g, float b);
		virtual void drawTexture(float textCords[], float vertices[], int numVertices, const std::string filename, int width, int height);
		//virtual void setTexture(PNGFile* png);

		virtual void swapBuffers();

	private:
		const char *m_fontFilename;
		
		GLuint m_text;

		FT_Library m_ft;
		FT_Face m_face;
		GLuint m_vbo;

		struct m_point
		{
			GLfloat x;
			GLfloat y;
			GLfloat s;
			GLfloat t;
		};

		engine::Ref<Window> m_window;
		bool m_active;
		
		EGLDisplay m_eglDisplay;
		EGLContext m_eglContext;
		EGLSurface m_eglSurface;			
	};
}

#endif
