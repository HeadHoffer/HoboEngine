#include "stdafx.h"

#include <Win32Window.h>
#include <Ref.h>
#include <OpenGLES2GraphicsSystem.h>
#include <Application.h>
#include <ElapsedTimer.h>
#include <irrKlang\irrKlang.h>

int _tmain(int argc, _TCHAR* argv[])
{
	using namespace irrklang;

	int w = 640;
	int h = 480;

	ISoundEngine *SoundEngine = createIrrKlangDevice();

	printf("Created window (%d, %d)\n", w, h);

	engine::Ref<engine::Win32Window> window =
		new engine::Win32Window(w, h, L"Kebab");

	engine::Ref<engine::OpenGLES2GraphicsSystem> graphicsSystem =
		new engine::OpenGLES2GraphicsSystem(window);
	window->setGraphicsSystem(graphicsSystem);

	engine::Ref<engine::Application> application =
		new engine::Application();
	window->setApplication(application);

	engine::ElapsedTimer frameTimer;
	frameTimer.reset();

	SoundEngine->play2D("audio/kebab.mp3", GL_TRUE);

	while (window->updateMessages())
	{
		float deltaTime = frameTimer.getTime();
		frameTimer.reset();
		application->update(deltaTime);
	}

	printf("Closing...\n");

	return 0;
}



