#ifndef APPLICATION_H
#define APPLICATION_H

#include <Object.h>
#include <Ref.h>
#include <vector>
#include <Shader.h>
#include <Texture.h>

namespace engine
{
	class Window;
	class GraphicsSystem;
	class Shader;
	class Texture;

	class Application : public Object
	{
	public:
		Application();
		~Application();

		bool update(float deltaTime);
		void render(Window* window, GraphicsSystem* graphicsSystem);
		
	private:
		float m_totalTime;		
		
		std::vector<Ref<Shader>> m_shader;
		std::vector<Ref<Texture>> m_texture;
	};
}

#endif

