#include <Application.h>
#include <GraphicsSystem.h>
#include <Window.h>
#include <stdio.h>
#include <math.h>

namespace engine
{
	Application::Application() : Object(), 
		m_totalTime(0.0f)
	{
		char texturedVertexShader1[] =
			"attribute vec4 vPosition;    \n"
			"attribute vec2 vTexCord;     \n"
			"varying vec2 texCord;        \n"
			"void main()                  \n"
			"{                            \n"
			"   gl_Position = vPosition;  \n"
			"   texCord = vTexCord;       \n"
			"}                            \n";
		
		char texturedFragmentShader1[] =
			"precision mediump float;                         \n"
			"uniform sampler2D texture;                       \n"
			"varying vec2 texCord;                            \n"
			"void main()                                      \n"
			"{                                                \n"
			"  gl_FragColor = texture2D(texture, texCord);	  \n"
			"}                                                \n";

		//two same shaders
		m_shader.push_back(new Shader(texturedVertexShader1, texturedFragmentShader1));
		m_shader.push_back(new Shader(texturedVertexShader1, texturedFragmentShader1));

		//4x4 image, 3 bytes per pixel
		GLubyte pixels1[16 * 3] =
		{
			255, 255, 255,
			  0,   0, 255,		
			  0,   0, 255,		
			255, 255, 255,
			  0,   0, 255,		
			  0,   0, 255,		
			  0,   0, 255,		
			  0,   0, 255,		
			  0,   0, 255,		
			  0,   0, 255,		
			  0,   0, 255,		
			  0,   0, 255,		
			255, 255, 255,
			  0,   0, 255,		
			  0,   0, 255,		
			255, 255, 255,
		};
	
		m_texture.push_back(new Texture(4, 4, 3, pixels1));

		//2x2 image, 3 bytes per pixel
		GLubyte pixels2[4 * 3] =
		{
			255, 255,   0,
			  0, 255, 255,	 
			255,   0, 255,
			  0, 255,   0,  
		};
			
		m_texture.push_back(new Texture(2, 2, 3, pixels2));
	}

	Application::~Application()
	{
	}

	bool Application::update(float deltaTime)
	{
		printf("%s\n", __FUNCTION__);
		
		m_totalTime += deltaTime;
		return true;
	}

	void Application::render(Window* window, GraphicsSystem* graphicsSystem)
	{ 
		printf("%s\n", __FUNCTION__);

		//screen clearing
		float pulse = fabs(sinf(3.0f *  m_totalTime));
		graphicsSystem->clearScreen(pulse, 1.0f, 0.0f);

		//rotating squares
		float sini1 = (sin(m_totalTime));
		float cosi1 = (cos(m_totalTime));	
		float sini2 = (sin(-2.0f * m_totalTime));
		float cosi2 = (cos(-2.0f * m_totalTime));

		//bigger square vertices and rotating
		float square1[] =
		{
			-1.0f * sini1, -1.0f * cosi1, 0.0f,
			 1.0f * cosi1, -1.0f * sini1, 0.0f,
			 1.0f * sini1,  1.0f * cosi1, 0.0f,

			 1.0f * sini1,  1.0f * cosi1, 0.0f,
			-1.0f * cosi1,  1.0f * sini1, 0.0f,
			-1.0f * sini1, -1.0f * cosi1, 0.0f,
		};

		//smaller square vertices and rotating
		float square2[] =
		{
			-0.5f * sini2, -0.5f * cosi2, 0.0f,
			 0.5f * cosi2, -0.5f * sini2, 0.0f,
			 0.5f * sini2,  0.5f * cosi2, 0.0f,

			 0.5f * sini2,  0.5f * cosi2, 0.0f,
			-0.5f * cosi2,  0.5f * sini2, 0.0f,
			-0.5f * sini2, -0.5f * cosi2, 0.0f,
		};

		//texture cordinates
		float textCords[] = 
		{
			0, 0,
			0, 1,
			1, 1,
			1, 1,
			1, 0,
			0, 0,
		};

		//drawing the squares
		graphicsSystem->drawTriangle(m_shader[0], m_texture[0], textCords, square1, 6);
		graphicsSystem->drawTriangle(m_shader[1], m_texture[1], textCords, square2, 6);

		graphicsSystem->swapBuffers();	
	}
}


