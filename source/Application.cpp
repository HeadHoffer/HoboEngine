#include <Application.h>
#include <GraphicsSystem.h>
#include <Window.h>
#include <stdio.h>
#include <math.h>
#include <Text.h>

namespace engine
{
	Application::Application() : Object(), 
		m_totalTime(0.0f)
	{

		char texturedVertexShader[] =
			"attribute vec4 vPosition;    \n"
			"attribute vec2 vTexCord;     \n"
			"varying vec2 texCord;        \n"
			"void main()                  \n"
			"{                            \n"
			"   gl_Position = vPosition;  \n"
			"   texCord = vTexCord;       \n"
			"}                            \n";
		
		char texturedFragmentShader[] =
			"precision mediump float;                         \n"
			"uniform sampler2D texture;                       \n"
			"varying vec2 texCord;                            \n"
			"void main()                                      \n"
			"{                                                \n"
			"  gl_FragColor = texture2D(texture, texCord);	  \n"
			"}                                                \n";

		m_shader.push_back(new Shader(texturedVertexShader, texturedFragmentShader));

		//text shader
		char textVertexShader[] =
			"attribute vec4 coord;"
			"varying vec2 texpos;"

			"void main(void)"
			"{"
			"	gl_Position = vec4(coord.xy, 0, 1);"
			"	texpos = coord.zw;"
			"}";

		char textFragmentShader[] =
			"varying vec2 texpos;"
			"uniform sampler2D tex;"
			"uniform vec4 color;"

			"void main(void)"
			"{"
			"	gl_FragColor = vec4(1, 1, 1, texture2D(tex, texpos).a) * color;"
			"}";

		m_shader.push_back(new Shader(textVertexShader, textFragmentShader));

		//4x4 image, 3 bytes per pixel
		GLubyte pixels[16 * 3] =
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
	
		m_texture.push_back(new Texture(4, 4, 3, pixels));

		////2x2 image, 3 bytes per pixel
		//GLubyte pixels2[4 * 3] =
		//{
		//	255, 255,   0,
		//	  0, 255, 255,	 
		//	255,   0, 255,
		//	  0, 255,   0,  
		//};
		//	
		//m_texture.push_back(new Texture(2, 2, 3, pixels2));
	}

	Application::~Application()
	{
	}

	bool Application::update(float deltaTime)
	{
		//printf("%s\n", __FUNCTION__);
		
		m_totalTime += deltaTime;
		return true;
	}

	void Application::render(Window* window, GraphicsSystem* graphicsSystem)
	{ 
		float sx = 2.0 / window->getWidth();
		float sy = 2.0 / window->getHeight();


		//printf("%s\n", __FUNCTION__);
		
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

		////smaller square vertices and rotating
		//float square2[] =
		//{
		//	-0.5f * sini2, -0.5f * cosi2, 0.0f,
		//	 0.5f * cosi2, -0.5f * sini2, 0.0f,
		//	 0.5f * sini2,  0.5f * cosi2, 0.0f,

		//	 0.5f * sini2,  0.5f * cosi2, 0.0f,
		//	-0.5f * cosi2,  0.5f * sini2, 0.0f,
		//	-0.5f * sini2, -0.5f * cosi2, 0.0f,
		//};

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

		//graphicsSystem->initText();
		graphicsSystem->drawText(m_shader[1], "helo wurld", -1, 0.825, sx, sy);
	//	graphicsSystem->drawText(m_shader[1], "paskda", -1, 0.5, sx, sy);
	//	graphicsSystem->drawText(m_shader[1], "wetwet", -1, 0.25, sx, sy);

		graphicsSystem->swapBuffers();	
	}
}


