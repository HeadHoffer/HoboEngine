//#include <Text.h>
//#include <stdio.h>
//#include <math.h>
//
//namespace engine
//{
//	Text::Text() : Object(),
//		m_totalTime(0.0f)
//	{
//		//m_fontfilename = "comic.ttf";
//
//	}
//
//	Text::~Text()
//	{
//	}
//
//	int Text::initResources()
//	{
//		m_fontfilename = "comic.ttf";
//
//		/* Initialize the FreeType2 library */
//		if (FT_Init_FreeType(&m_ft))
//		{
//			fprintf(stderr, "Could not init freetype library\n");
//			return 0;
//		}
//
//		//load a font
//		printf("Loading the font...");
//		if (FT_New_Face(m_ft, m_fontfilename, 0, &m_face))
//		{
//			fprintf(stderr, "Could not open font %s\n", m_fontfilename);
//			return 0;
//		}
//		else
//			printf("Font loaded.\n");
//
//		return 1;
//	}
//}
