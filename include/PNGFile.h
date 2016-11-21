#ifndef PNG_FILE_H_
#define PNG_FILE_H_

#include <Object.h>
#include <GLES2\gl2.h>
#include <lpng1513\png.h>
//#include <malloc.h>
//#include <assert.h>
//#include <Ref.h>
#include <cstdio>
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

		GLuint loadTexture(const std::string filename, int &width, int &height);

	};
}

#endif