#ifndef TEXT_H
#define TEXT_H

#include <Ref.h>
#include <Object.h>

//freetype headers
#include <ft2build.h>
#include FT_FREETYPE_H

namespace engine
{
	class Text : public Object
	{
	public:
		Text();
		~Text();

		int initText(const char* fontFilename);
		FT_Face getFace();
	
	private:
		const char *m_fontFilename;
		FT_Face m_face;
	};
}

#endif