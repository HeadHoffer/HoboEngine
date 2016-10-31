#include "stdafx.h"

#include <Win32Window.h>
#include <Ref.h>
#include <OpenGLES2GraphicsSystem.h>
#include <Application.h>
#include <ElapsedTimer.h>

int _tmain(int argc, _TCHAR* argv[])
{
	int w = 640;
	int h = 480;

	printf("Created window (%d, %d)\n", w, h);

	engine::Ref<engine::Win32Window> window =
		new engine::Win32Window(w, h, L"Title");

	engine::Ref<engine::OpenGLES2GraphicsSystem> graphicsSystem =
		new engine::OpenGLES2GraphicsSystem(window);
	window->setGraphicsSystem(graphicsSystem);

	engine::Ref<engine::Application> application =
		new engine::Application();
	window->setApplication(application);

	engine::ElapsedTimer frameTimer;
	frameTimer.reset();

	while (window->updateMessages())
	{
		float deltaTime = frameTimer.getTime();
		frameTimer.reset();
		application->update(deltaTime);
	}

	printf("Closing...\n");

	return 0;
}



