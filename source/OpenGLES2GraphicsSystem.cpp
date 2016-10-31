#include <OpenGLES2GraphicsSystem.h>
#include <Shader.h>
#include <Texture.h>

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <initializer_list>

namespace engine
{
	OpenGLES2GraphicsSystem::OpenGLES2GraphicsSystem(Window* window) : GraphicsSystem(), 
		m_window(window), 
		m_active(false)
	{
		assert(m_window != 0);

		EGLint attribList[] = {
			EGL_RED_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_BLUE_SIZE, 8,
			EGL_ALPHA_SIZE, 8,
			EGL_DEPTH_SIZE, 16,
			EGL_STENCIL_SIZE, EGL_DONT_CARE,
			EGL_SAMPLE_BUFFERS, 0,
			EGL_NONE
		};

		EGLint w, h, format;
		EGLint numConfigs;
		EGLConfig config = NULL;
		
		m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
		
		eglInitialize(m_eglDisplay, 0, 0);
		
		//Here, the application chooses the configuration it desires.
		//find the best match if possible, otherwise use the very first one
		eglChooseConfig(m_eglDisplay, attribList, nullptr, 0, &numConfigs);
		EGLConfig* supportedConfigs = new EGLConfig[numConfigs];
		assert(supportedConfigs);
		eglChooseConfig(m_eglDisplay, attribList, supportedConfigs, numConfigs, &numConfigs);
		assert(numConfigs);
		int i = 0;
		
		for (; i < numConfigs; i++)
		{
			EGLConfig& cfg = supportedConfigs[i];
			EGLint r, g, b, d;
			if (eglGetConfigAttrib(m_eglDisplay, cfg, EGL_RED_SIZE, &r) &&
				eglGetConfigAttrib(m_eglDisplay, cfg, EGL_GREEN_SIZE, &g) &&
				eglGetConfigAttrib(m_eglDisplay, cfg, EGL_BLUE_SIZE, &b) &&
				eglGetConfigAttrib(m_eglDisplay, cfg, EGL_DEPTH_SIZE, &d) &&
				r == 8 && g == 8 && b == 8 && d == 0) 
			{ 
				config = supportedConfigs[i];
				break;
			}
		}
		
		if (i == numConfigs) 
		{
			config = supportedConfigs[0];
		} 
	
		//EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
	    //guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
	    //As soon as we picked a EGLConfig, we can safely reconfigure the
	    //ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID.

		eglGetConfigAttrib(m_eglDisplay, config, EGL_NATIVE_VISUAL_ID, &format);
		m_eglSurface = eglCreateWindowSurface(m_eglDisplay, config, window->getNativeWindow(), NULL);
		EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };
		m_eglContext = eglCreateContext(m_eglDisplay, config, NULL, contextAttribs);
	
		if (eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext) == EGL_FALSE)
		{
			assert(0);
			//LOGW("Unable to eglMakeCurrent");
		}
	
		//Get size of the surface
		eglQuerySurface(m_eglDisplay, m_eglSurface, EGL_WIDTH, &w);
		eglQuerySurface(m_eglDisplay, m_eglSurface, EGL_HEIGHT, &h);
		window->setSize(w, h);
	
		//Check openGL on the system
		auto opengl_info = { GL_VENDOR, GL_RENDERER, GL_VERSION, GL_EXTENSIONS };
		for (auto name : opengl_info) 
		{
			auto info = glGetString(name);
			//LOGI("OpenGL Info: %s", info);
		}

		m_active = true;
		
		//int n = eglGetError();
		//if (n != EGL_SUCCESS)
		//{
		//	assert(0);
		//}

		//int num = glGetError();
	}

	OpenGLES2GraphicsSystem::~OpenGLES2GraphicsSystem()
	{
	}

	void OpenGLES2GraphicsSystem::clearScreen(float red, float green, float blue)
	{
		glViewport(0, 0, m_window->getWidth(), m_window->getHeight());
		glClearColor(red, green, blue, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLES2GraphicsSystem::drawTriangle(Shader* shader, Texture* texture, float textCords[], float vertices[], int numVertices)
	{
		shader->UseShader();

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
		glEnableVertexAttribArray(0);

		//Set texture coordinates
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, textCords);
		glEnableVertexAttribArray(1);
		
		//Bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
		
		//Set the sampler texture unit to 0
		glUniform1i(shader->getUniformLocation("texture"), 0);

		glDrawArrays(GL_TRIANGLES, 0, numVertices);
	}

	void OpenGLES2GraphicsSystem::swapBuffers()
	{
		eglSwapBuffers(m_eglDisplay, m_eglSurface);
	}
}
