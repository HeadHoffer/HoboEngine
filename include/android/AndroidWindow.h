#ifndef ANDROID_WINDOW_H
#define ANDROID_WINDOW_H

#include <Window.h>

namespace engine
{
	class AndroidWindow : public Window
	{
	public:
		AndroidWindow(EGLNativeWindowType nativeWindow);
		~AndroidWindow();

		virtual EGLNativeWindowType getNativeWindow() const;

	private:
		EGLNativeWindowType m_hwnd;
	};
}

#endif