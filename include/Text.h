#ifndef TEXT_H
#define TEXT_H

#include <Object.h>
#include <Ref.h>
#include <vector>
#include <Shader.h>
#include <Texture.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace engine
{
	class GraphicsSystem;
	class Shader;
	class Texture;

	class Text : public Object
	{
	public:
		Text();
		~Text();

		int initResources();
		//ei renderˆi t‰‰ll‰ vaan graphicsSystem--->application (t‰‰ll‰ vaan initialisointi
		//ja fontin lataus??????) fug :DDDDDD
		//void renderText(Window* window, GraphicsSystem* graphicsSystem);

	private:
		//graphicsSystemiin
		FT_Library m_ft;
		FT_Face m_face;
		const char *m_fontfilename = "comic.ttf";

		float m_totalTime;

		std::vector<Ref<Shader>> m_shader;
		std::vector<Ref<Texture>> m_texture;
	};
}

#endif

