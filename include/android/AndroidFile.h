#ifndef ANDROID_FILE_H
#define ANDROID_FILE_H

#include <File.h>
#include <vector>

namespace engine
{
	class AndroidFile : public File
	{
	public:
		AndroidFile();
		~AndroidFile();

		bool read(int numBytes);

	private:

	};
}

#endif