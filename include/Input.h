#ifndef INPUT_H
#define INPUT_H

//#include <GL\glew.h>
#include <GL\glut.h>

namespace engine
{

	class Input
	{
	public:
		Input();
		~Input();

		void keyOperations(void);
		void keyPressed(unsigned char key, int x, int y);
		void keyUp(unsigned char key, int x, int y);

	private:
		bool* keyStates = new bool[265];



	};

}

#endif