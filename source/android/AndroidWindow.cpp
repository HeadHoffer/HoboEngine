#include <AndroidWindow.h>

namespace engine
{
	AndroidWindow::AndroidWindow(EGLNativeWindowType nativeWindow) : Window(),
		m_hwnd(nativeWindow)
	{
	}

	AndroidWindow::~AndroidWindow()
	{
	}

	EGLNativeWindowType AndroidWindow::getNativeWindow() const
	{
		return m_hwnd;
	}
}