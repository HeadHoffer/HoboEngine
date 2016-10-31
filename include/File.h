#ifndef FILE_H
#define FILE_H

#include <Object.h>
#include <vector>

namespace engine
{
	class File : public Object
	{
	public:
		File();
		~File();

		bool read(int numBytes);

	private:

	};
}

#endif