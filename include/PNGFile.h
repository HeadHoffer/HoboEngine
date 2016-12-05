#ifndef PNG_FILE_H_
#define PNG_FILE_H_

#include <Object.h>
#include <GLES2\gl2.h>
#include <lpng1626\png.h>
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
	class PNGFile : public Object
	{
	public:
		PNGFile();
		~PNGFile();

		GLuint loadTexture(const std::string filename, int width, int height);
	};
}

#endif