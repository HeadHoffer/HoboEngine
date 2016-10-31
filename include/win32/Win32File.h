#ifndef WIN32_FILE_H
#define WIN32_FILE_H

#include <File.h>
#include <vector>

namespace engine
{
	class Win32File : public File
	{
	public:
		Win32File();
		~Win32File();

		bool read(int numBytes);

	private:

	};
}

#endif