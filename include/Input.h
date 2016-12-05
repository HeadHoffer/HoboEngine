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

		void keyboard(unsigned char key, int x, int y);



		/*void keyOperations();
		void keyPressed(unsigned char key, int x, int y);
		void keyUp(unsigned char key, int x, int y);*/

		//int glutKeyboardFunc(void keyPressed()); // Tell GLUT to use the method "keyPressed" for key presses  
		//int glutKeyboardUpFunc(void keyUp()); // Tell GLUT to use the method "keyUp" for key up events  

	private:
		bool* keyStates = new bool[265]();



	};

}

#endif