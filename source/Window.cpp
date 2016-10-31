#include <Window.h>

namespace engine
{
	Window::Window() : Object(), 
		m_application(0),
		m_graphicsSystem(0)
	{
	}

	Window::~Window()
	{
	}

	int Window::getWidth() const
	{
		return m_width;
	}

	int Window::getHeight() const
	{
		return m_height;
	}

	void Window::setSize(int w, int h)
	{
		m_width = w;
		m_height = h;
	}

	void Window::setApplication(Application* application)
	{
		m_application = application;
	}

	void Window::setGraphicsSystem(GraphicsSystem* graphicsSystem)
	{
		m_graphicsSystem = graphicsSystem;
	}

	Application* Window::getApplication() const
	{
		return m_application;
	}

	GraphicsSystem* Window::getGraphicsSystem() const
	{
		return m_graphicsSystem;
	}
}
