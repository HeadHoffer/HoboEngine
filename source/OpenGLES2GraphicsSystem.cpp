#include <OpenGLES2GraphicsSystem.h>
#include <Shader.h>
#include <Texture.h>

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <initializer_list>

#define TEXTURE_LOAD_ERROR 0

namespace engine
{
	OpenGLES2GraphicsSystem::OpenGLES2GraphicsSystem(Window* window) : GraphicsSystem(), 
		m_window(window), 
		m_active(false)
	{
		assert(m_window != 0);

		EGLint attribList[] = 
		{
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
		glClear(GL_COLOR_BUFFER_BIT);
	
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

	void OpenGLES2GraphicsSystem::drawTexture(float textCords[], float vertices[], int numVertices, const std::string filename, int width, int height)
	{
		//shader->UseShader();

		//header for testing if it is a png
		png_byte header[8];

		//open file as binary
		FILE *fp = fopen(filename.c_str(), "rb");
		//if (!fp)
		//{
		//	return TEXTURE_LOAD_ERROR;
		//}

		//read the header
		fread(header, 1, 8, fp);

		//test if png
		int is_png = !png_sig_cmp(header, 0, 8);
		if (!is_png)
		{
			fclose(fp);
			//return TEXTURE_LOAD_ERROR;
		}

		//create png struct
		png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
			NULL, NULL);
		if (!png_ptr)
		{
			fclose(fp);
			//return (TEXTURE_LOAD_ERROR);
		}

		//create png info struct
		png_infop info_ptr = png_create_info_struct(png_ptr);
		if (!info_ptr)
		{
			png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
			fclose(fp);
			//return (TEXTURE_LOAD_ERROR);
		}

		//create png info struct
		png_infop end_info = png_create_info_struct(png_ptr);
		if (!end_info)
		{
			png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
			fclose(fp);
			//return (TEXTURE_LOAD_ERROR);
		}

		if (setjmp(png_jmpbuf(png_ptr)))
		{
			png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
			fclose(fp);
			//return (TEXTURE_LOAD_ERROR);
		}

		//init png reading
		png_init_io(png_ptr, fp);

		//let libpng know you already read the first 8 bytes
		png_set_sig_bytes(png_ptr, 8);

		// read all the info up to the image data
		png_read_info(png_ptr, info_ptr);

		//variables to pass to get info
		int bit_depth, color_type;
		png_uint_32 twidth, theight;

		// get info about png
		png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type,	NULL, NULL, NULL);
		//update width and height based on png info
		width = twidth;
		height = theight;

		// Update the png info struct.
		png_read_update_info(png_ptr, info_ptr);

		// Row size in bytes.
		int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

		// Allocate the image_data as a big block, to be given to opengl
		png_byte *image_data = new png_byte[rowbytes * height];
		if (!image_data)
		{
			//clean up memory and close stuff
			png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
			fclose(fp);
			//return TEXTURE_LOAD_ERROR;
		}

		//row_pointers is for pointing to image_data for reading the png with libpng
		png_bytep *row_pointers = new png_bytep[height];
		if (!row_pointers) 
		{
			//clean up memory and close stuff
			png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
			delete[] image_data;
			fclose(fp);
			//return TEXTURE_LOAD_ERROR;
		}

		//set the individual row_pointers to point at the correct offsets of image_data
		for (int i = 0; i < height; ++i)
			row_pointers[height - 1 - i] = image_data + i * rowbytes;

		//read the png into image_data through row_pointers
		png_read_image(png_ptr, row_pointers);

		//Now generate the OpenGL texture object
		GLuint texture;
		glGenTextures(1, &texture);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
		glEnableVertexAttribArray(0);

		//Set texture coordinates
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, textCords);
		glEnableVertexAttribArray(1);

		//Bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)image_data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		delete[] image_data;
		delete[] row_pointers;
		fclose(fp);

		//Set the sampler texture unit to 0
		//glUniform1i(shader->getUniformLocation("texture"), 0);
		glDrawArrays(GL_TRIANGLES, 0, numVertices);
	}

	int OpenGLES2GraphicsSystem::initText(const char* fontFilename)
	{
		m_fontFilename = fontFilename;

		//initialize the freetype library
		if (FT_Init_FreeType(&m_ft))
		{
			fprintf(stderr, "Could not init freetype library\n");
			return 0;
		}

		//load a font
		printf("Loading the font...");
		if (FT_New_Face(m_ft, m_fontFilename, 0, &m_face))
		{
			fprintf(stderr, "Could not open font %s\n", m_fontFilename);
			return 0;
		}
		else
			printf("Font loaded!\n");

		//debug stuff---------------------------
		if (m_ft == NULL)
		{
			printf("m_ft is NULL");
		}
		else
			printf("m_ft: %s\n", m_ft);

		if (m_face == NULL)
		{
			printf("m_face is NULL");
		}
		else
			printf("m_face: %s\n", m_face);
		//--------------------------------------

		return 1;
	}

	//text 
	void OpenGLES2GraphicsSystem::drawText(Shader* shader, const char* text, float x, float y, float sx, float sy, float red, float green, float blue)
	{
		shader->UseShader();		

		GLint attributeCoord = glGetAttribLocation(shader->getProgram(), "textCoord");
		GLint uniformTex = glGetUniformLocation(shader->getProgram(), "textTexture");
		GLint uniformColor = glGetUniformLocation(shader->getProgram(), "textColor");

		//----------------------------------------------------------------------------

		//create the vertex buffer object
		glGenBuffers(1, &m_vbo);

		//font color (r, g, b, alpha)
		GLfloat color[4] = { red, green, blue, 1 };
		
		//set font size and color (pls scale font like this, not with sx, sy)
		FT_Set_Pixel_Sizes(m_face, 0, 48);

		//create and bind a texture that will be used to hold one "glyph"
		GLuint texture;
		glGenTextures(1, &texture);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(uniformTex, 0);

		//we require 1 byte alignment when uploading texture data
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		//clamping to edges is important to prevent artifacts when scaling
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//linear filtering usually looks best for text
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		//set up the VBO for our vertex data
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		glVertexAttribPointer(attributeCoord, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(attributeCoord);
		
		glUniform4fv(uniformColor, 1, color);

		const char *p;
		FT_GlyphSlot g = m_face->glyph;

		//loop through all characters
		for (p = text; *p; p++)
		{
			//try to load and render the character
			if (FT_Load_Char(m_face, *p, FT_LOAD_RENDER))
				continue;

			//upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture
			glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

			//calculate the vertex and texture coordinates
			float x2 = x + g->bitmap_left * sx;
			float y2 = -y - g->bitmap_top * sy;
			
			float w = g->bitmap.width * sx;
			float h = g->bitmap.rows * sy;

			m_point box[4] =
			{
				{ x2, - y2, 0, 0 },
				{ x2 + w, - y2, 1, 0 },
				{ x2, - y2 - h, 0, 1 },
				{ x2 + w, - y2 - h, 1, 1 },
			};

			//draw the character on the screen
			glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			//advance the cursor to the start of the next character
			x += (g->advance.x >> 6) * sx;
			y += (g->advance.y >> 6) * sy;
		}

		//delete the vertex buffer object and textures
		glDeleteBuffers(1, &m_vbo);
		glDisableVertexAttribArray(attributeCoord);
		glDeleteTextures(1, &texture);
	}

	void OpenGLES2GraphicsSystem::swapBuffers()
	{
		eglSwapBuffers(m_eglDisplay, m_eglSurface);
	}
}
