#include "stdafx.h"

#include <Win32Window.h>
#include <Ref.h>
#include <OpenGLES2GraphicsSystem.h>
#include <Application.h>
#include <ElapsedTimer.h>
#include <Input.h>
#include <PNGFile.h>

#include <irrKlang\irrKlang.h>

int _tmain(int argc, _TCHAR* argv[])
{
	using namespace irrklang;

	int w = 640;
	int h = 480;

	printf("Created window (%d, %d)\n", w, h);

	engine::Ref<engine::Win32Window> window =
		new engine::Win32Window(w, h, L"wundow");

	engine::Ref<engine::OpenGLES2GraphicsSystem> graphicsSystem =
		new engine::OpenGLES2GraphicsSystem(window);
	window->setGraphicsSystem(graphicsSystem);

	engine::Ref<engine::Application> app =
		new engine::Application();
	window->setApplication(app);

	//load and initialize font
	const char *fontfilename = "../fonts/comic.ttf";
	graphicsSystem->initText(fontfilename);
	
	//sound stuff
	ISoundEngine *SoundEngine = createIrrKlangDevice();
	//play3D is better and the audio can be looped
	SoundEngine->play3D("../audio/kebab.mp3", vec3df(0, 0, 0), true, false, true);
	//SoundEngine->play2D("audio/kebab.mp3", GL_TRUE);
	
	engine::ElapsedTimer frameTimer;
	frameTimer.reset();

	while (window->updateMessages())
	{
		float deltaTime = frameTimer.getTime();
		frameTimer.reset();
		
		app->update(deltaTime);
	}

	printf("Closing...\n");

	return 0;
}



