#include "stdafx.h"

#include <Win32Window.h>
#include <Ref.h>
#include <OpenGLES2GraphicsSystem.h>
#include <Application.h>
#include <ElapsedTimer.h>

#include <PNGFile.h>

int _tmain(int argc, _TCHAR* argv[])
{
	int w = 640;
	int h = 480;

	int ww = 512;
	int hh = 512;

	printf("Created window (%d, %d)\n", w, h);

	engine::Ref<engine::Win32Window> window =
		new engine::Win32Window(w, h, L"wundow");

	engine::Ref<engine::OpenGLES2GraphicsSystem> graphicsSystem =
		new engine::OpenGLES2GraphicsSystem(window);
	window->setGraphicsSystem(graphicsSystem);

	engine::Ref<engine::Application> app =
		new engine::Application();
	window->setApplication(app);

	const char *fontfilename = "../fonts/StencilStd.otf";

	graphicsSystem->initText(fontfilename);
	
	//teksti menee applicationiin ja renderöidää sieltä

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



