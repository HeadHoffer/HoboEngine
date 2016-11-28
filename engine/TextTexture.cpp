#include <TextTexture.h>
#include <malloc.h>
#include <stdio.h>

#include <Shader.h>

namespace engine
{
	TextTexture::TextTexture(Shader* shader) : Object()
	{
		//glGenBuffers(1, &m_vbo);

		assert(bytesPerPixel == 3 || bytesPerPixel == 4);

		GLint attribute_coord = glGetAttribLocation(shader->getProgram(), "textCoord");
		GLint uniform_tex = glGetUniformLocation(shader->getProgram(), "textTexture");
		GLint uniform_color = glGetUniformLocation(shader->getProgram(), "textColor");

		FT_Set_Pixel_Sizes(m_face, 0, 48);

		//create a texture that will be used to hold one "glyph"
		GLuint tex;
		glGenTextures(1, &tex);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		glUniform1i(uniform_tex, 0);

		//we require 1 byte alignment when uploading texture data
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		//clamping to edges is important to prevent artifacts when scaling
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//linear filtering usually looks best for text
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//set up the VBO for our vertex data
		glEnableVertexAttribArray(attribute_coord);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);

		//font color (r, g, b, alpha)
		GLfloat color[4] = { 1, 0, 0, 1 };

		//set font size and color (pls scale font like this, not with sx, sy)
		FT_Set_Pixel_Sizes(m_face, 0, 48);
		glUniform4fv(uniform_color, 1, color);

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
				{ x2, -y2, 0, 0 },
				{ x2 + w, -y2, 1, 0 },
				{ x2, -y2 - h, 0, 1 },
				{ x2 + w, -y2 - h, 1, 1 },
			};

			//draw the character on the screen
			glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			//advance the cursor to the start of the next character
			x += (g->advance.x >> 6) * sx;
			y += (g->advance.y >> 6) * sy;
		}

	}

	TextTexture::~TextTexture()
	{
	}
}