#include "Input.h"
#include <stdio.h>

namespace engine
{


	Input::Input()
	{
	}


	Input::~Input()
	{
	}

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
	}

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
