#include <Text.h>

namespace engine
{
	Text::Text() : Object()
	{
	}

	Text::~Text()
	{
	}

	int Text::initText(const char* fontFilename)
	{
		m_fontFilename = fontFilename;
		
		FT_Library ft;
		FT_Face face;
		
		//initialize the freetype library
		if (FT_Init_FreeType(&ft))
		{
			fprintf(stderr, "Could not init freetype library\n");
			return 0;
		}

		//load a font
		printf("Loading the font...");
		if (FT_New_Face(ft, m_fontFilename, 0, &face))
		{
			fprintf(stderr, "Could not open font %s\n", m_fontFilename);
			return 0;
		}
		else
			printf("Font loaded!\n");

		//debug stuff---------------------------
		if (ft == NULL)
		{
			printf("m_ft is NULL");
		}
		else
			printf("m_ft: %s\n", ft);

		if (face == NULL)
		{
			printf("m_face is NULL");
		}
		else
			printf("m_face: %s\n", face);
		//--------------------------------------
		
		m_face = face;
		
		return 1;
	}
	
	FT_Face Text::getFace()
	{
		return m_face;
	}
}