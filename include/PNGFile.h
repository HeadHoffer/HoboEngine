#ifndef PNG_FILE_H_
#define PNG_FILE_H_

#include <Object.h>
#include <GLES2\gl2.h>
#include <lpng1513\png.h>
#include <malloc.h>
//#include <assert.h>
//#include <Ref.h>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <Ref.h>
#define TEXTURE_LOAD_ERROR 0

namespace engine
{
	class PNGFile 
	{
	public:
		PNGFile();
		~PNGFile();

		/*GLuint loadTexture(const std::string filename, int &width, int &height);*/


		void readPNGFile(char *filename);
		void writePNGFile(char *filename);
		void processPNGFile();

		int stuffHappnes(int argc, char *argv[]);

	private:
		int width, height;
		png_byte color_type;
		png_byte bit_depth;
		png_bytep *row_pointers;
		char * filename = "pngtest.png";
		
	};
}

#endif