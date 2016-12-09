#include "Input.h"
#include <stdio.h>
#include <Windows.h>

namespace engine
{
	namespace
	{
		bool clicks[3];
		bool prevClicks[3];
		int mouseXValue = 0;
		int mouseYValue = 0;
		int mouseWheelDelta = 0;
		bool keys[0xff];
		bool prevKeys[0xff];
	}
	//void Input::clearInput()
	//{
	//	mouseWheelDelta = 0;
	//	memcpy(&prevClicks[0], &clicks[0], sizeof(clicks));

	//	memcpy(&prevKeys[0], &keys[0], sizeof(keys));
	//	for (int i = 0; i < 0xff; ++i)
	//	{
	//		keys[i] = (GetAsyncKeyState(i) & 0x8000) ? 1 : 0;
	//	}
	//}

	Input::Input()
	{
	}


	Input::~Input()
	{

	}

	int Input::isMouseButtonPressed(MouseButtons button)
	{
		// Now pressed && prev not pressed
		return clicks[button] && !prevClicks[button];
	}

	int Input::getMouseAxisX()
	{
		return mouseXValue;
	}

	int Input::getMouseAxisY()
	{
		return mouseYValue;
	}

	int Input::getMouseWheelDelta()
	{
		return mouseWheelDelta;
	}

	int Input::getKeyState(KeyCodes keyCode)
	{
		return keys[keyCode];
	}

	int Input::isKeyPressed(KeyCodes keyCode)
	{
		// Now pressed && prev not pressed
		return keys[keyCode] && !prevKeys[keyCode];
	}

	int Input::isKeyReleased(KeyCodes keyCode)
	{
		// Prev pressed && now not pressed
		return prevKeys[keyCode] && !keys[keyCode];
	}
	/**
	void Input::keyboard(unsigned char key, int x, int y)
	{
		if (key == 27)
		{
			exit(0);
		}

		if (key == 'f')
		{
			glutFullScreen();
			printf("pay respects");
		}
	}*/

//	void Input::keyOperations()
//	{
//		if (keyStates['a'])
//		{
//			//If the a key has been pressed preform a key operations
//			
//		}
//
//	}
//
//	void Input::keyPressed(unsigned char key, int x, int y)
//	{
//		keyStates[key] = true;
//
//		//printf("key", key, "pressed");
//		printf("something is press");
//	}
//
//	void Input::keyUp(unsigned char key, int x, int y)
//	{
//		keyStates[key] = false;
//		//printf("key", key, "up");
//	}
}
