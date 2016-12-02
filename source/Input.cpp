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

	void Input::keyOperations(void)
	{
		if (keyStates['a'])
		{
			//If the a key has been pressed preform a key operations
			printf("hello gordom freeman there is still something wrong with my voice");
		}

	}

	void Input::keyPressed(unsigned char key, int x, int y)
	{
		keyStates[key] = true;

		printf("key", key, "pressed");
	}

	void Input::keyUp(unsigned char key, int x, int y)
	{
		keyStates[key] = false;
		printf("key", key, "up");
	}
}
