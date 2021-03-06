#include "stdafx.h"

#include <Win32Window.h>
#include <Ref.h>
#include <OpenGLES2GraphicsSystem.h>
#include <Application.h>
#include <ElapsedTimer.h>
#include <Input.h>
#include <PNGFile.h>
#include <Windows.h>
#include <Text.h>

#include <irrKlang\irrKlang.h>

int _tmain(int argc, char *argv[])
{
	using namespace irrklang;

	int w = 800;
	int h = 800;

	printf("Created window (%d, %d)\n", w, h);

	engine::Ref<engine::Win32Window> window =
		new engine::Win32Window(w, h, L"Harambe Strong");

	engine::Ref<engine::OpenGLES2GraphicsSystem> graphicsSystem =
		new engine::OpenGLES2GraphicsSystem(window);
	window->setGraphicsSystem(graphicsSystem);

	engine::Ref<engine::Application> app =
		new engine::Application();
	window->setApplication(app);

	engine::Ref<engine::Text> txt = new engine::Text();
	
	//load and initialize font
	const char *fontfilename = "../fonts/comic.ttf";
	txt->initText(fontfilename);
	//setting the fontface
	graphicsSystem->setText(txt);

	//sound stuff
	ISoundEngine *SoundEngine = createIrrKlangDevice();
	//play3D is better and the audio can be looped
	SoundEngine->play3D("../audio/kebab.mp3", vec3df(0, 0, 0), true, false, true);
	//SoundEngine->play2D("audio/kebab.mp3", GL_TRUE);
	
	engine::ElapsedTimer frameTimer;
	frameTimer.reset();

	engine::Input* input = new engine::Input();
	
	/*if (engine::KeyCodes::KEY_NUMPAD1);
	printf("moi");*/
	

	while (window->updateMessages())
	{
		float deltaTime = frameTimer.getTime();
		frameTimer.reset();

		app->update(deltaTime);

		
	}
	printf("Closing...\n");

	return 0;
}



