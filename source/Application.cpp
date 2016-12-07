#include <Application.h>
#include <GraphicsSystem.h>
#include <Window.h>
#include <stdio.h>
#include <math.h>
#include <PNGFile.h>

namespace engine
{
	Application::Application() : Object(), 
		m_totalTime(0.0f)
	{
		//square shader
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

		//4x4 image, 3 bytes per pixel
		GLubyte pixels[16 * 3] =
		{
			255, 255, 255,
			0, 0, 255,
			0, 0, 255,
			255, 255, 255,
			0, 0, 255,
			0, 0, 255,
			0, 0, 255,
			0, 0, 255,
			0, 0, 255,
			0, 0, 255,
			0, 0, 255,
			0, 0, 255,
			255, 255, 255,
			0, 0, 255,
			0, 0, 255,
			255, 255, 255,
		};

		m_texture.push_back(new Texture(4, 4, 3, pixels));

		//square shader
		char VertexShader[] =
			"attribute vec4 vPosition;    \n"
			"attribute vec2 vTexCord;     \n"
			"varying vec2 texCord;        \n"
			"void main()                  \n"
			"{                            \n"
			"   gl_Position = vPosition;  \n"
			"   texCord = vTexCord;       \n"
			"}                            \n";

		char FragmentShader[] =
			"uniform sampler2D texture;                       \n"
			"varying vec2 texCord;                            \n"
			"void main()                                      \n"
			"{                                                \n"
			"  gl_FragColor = texture2D(texture, texCord);	  \n"
			"}                                                \n";

		m_shader.push_back(new Shader(VertexShader, FragmentShader));

		//text shaders
		char textVertexShader[] =
			"attribute vec4 textCoord;"
			"varying vec2 textPos;"

			"void main(void)"
			"{"
			"	gl_Position = vec4(textCoord.xy, 0, 1);"
			"	textPos = textCoord.zw;"
			"}";

		char textFragmentShader[] =
			"varying vec2 textPos;"
			"uniform sampler2D textTexture;"
			"uniform vec4 textColor;"

			"void main(void)"
			"{"
			"	gl_FragColor = vec4(1, 1, 1, texture2D(textTexture, textPos).a) * textColor;"
			"}";
		
		m_shader.push_back(new Shader(textVertexShader, textFragmentShader));

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

		float square2[] =
		{
			-1, -1, 0.0f,
			1, -1, 0.0f,
			1, 1, 0.0f,

			1, 1, 0.0f,
			-1, 1, 0.0f,
			-1, -1, 0.0f,
		};

		float square3[] =
		{
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,

			0.5f, 0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
		};
		const float OFFSET_Y = 0.4f;
		float square4[] =
		{
			-0.5f * sini1, OFFSET_Y + -0.5f * cosi1, 0.0f,
			0.5f * cosi1, OFFSET_Y + -0.5f  * sini1, 0.0f,
			0.5f * sini1, OFFSET_Y + 0.5f  * cosi1, 0.0f,

			0.5f * sini1, OFFSET_Y + 0.5f * cosi1, 0.0f,
			-0.5f * cosi1, OFFSET_Y + 0.5f * sini1, 0.0f,
			-0.5f * sini1, OFFSET_Y + -0.5f * cosi1, 0.0f,
		};

		//texture cordinates
		float textCords[] = 
		{
			0, 0,
			1, 0,
			1, 1,
			1, 1,
			0, 1,
			0, 0,
		};

		//drawing the square(s)
		graphicsSystem->drawTriangle(m_shader[0], m_texture[0], textCords, square1, 6);
		//graphicsSystem->drawPNG(m_shader[0], text);
		
		graphicsSystem->drawTexture(textCords, square2, 6, "../pictures/remove.png", 512, 512);
		graphicsSystem->drawTexture(textCords, square4, 6, "../pictures/harambe.png", 512, 512);
		//graphicsSystem->drawTexture(textCords, square1, 6, "Illuminati.png", 512, 512);

		//drawing the text(s), drawText(shader, text, x, y, sx, sy, r, g, b)
		//graphicsSystem->drawText(m_shader[2], "clinton killed harambe", -1, 0.825, sx, sy, 1, 0 , 0);
		graphicsSystem->drawText(m_shader[2], "remove winston", -0.5, -0.5, sx, sy, 0, 1, 0);

		graphicsSystem->swapBuffers();	
	}
}
