#include <Win32Window.h>
#include <Application.h>

namespace engine
{
	LRESULT WINAPI WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRet = 1;

		switch (uMsg)
		{
			
		case WM_PAINT:
		{
			Win32Window* window = (Win32Window*)(LONG_PTR)GetWindowLongPtr(hWnd, GWL_USERDATA);
			
			window->getApplication()->render(window, window->getGraphicsSystem());

			ValidateRect(window->getNativeWindow(), NULL);
		}
		break;

		case WM_DESTROY:
			PostQuitMessage(0);
		break;

		case WM_CHAR:
		{
			POINT point;
			Win32Window * esContext = (Win32Window*)(LONG_PTR)GetWindowLongPtr(hWnd, GWL_USERDATA);
			GetCursorPos(&point);
		}
		break;

		default:
			lRet = DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;		
		}

		return lRet;	
	}

	Win32Window::Win32Window(int width, int height, const std::wstring& title) : Window(),
		m_hwnd(NULL), 
		m_active(false)	
	{ 
		WNDCLASS wndClass = { 0 };
		DWORD wStyle = 0;
		RECT windowRect;
		HINSTANCE hInstance = GetModuleHandle(NULL);

		wndClass.style = CS_OWNDC;
		wndClass.lpfnWndProc = (WNDPROC)WindowProc;
		wndClass.hInstance = hInstance;
		wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wndClass.lpszClassName = L"opengles2.0";

		if (!RegisterClass(&wndClass))
			return; //fail

		wStyle = WS_VISIBLE | WS_POPUP | WS_BORDER | WS_SYSMENU | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;

		windowRect.left = 0;
		windowRect.top = 0;
		windowRect.right = width - windowRect.left;
		windowRect.bottom = height - windowRect.top;

		AdjustWindowRect(&windowRect, wStyle, FALSE);

		m_hwnd = CreateWindow(
			L"opengles2.0",
			title.c_str(),
			wStyle,
			0,
			0,
			windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top,
			NULL,
			NULL,
			hInstance,
			NULL);
		
		SetWindowLongPtr(m_hwnd, GWL_USERDATA, (LONG)(LONG_PTR)this);

		if (m_hwnd == NULL)
		{
			return; //fail
		}

		ShowWindow(m_hwnd, TRUE);
		
		m_active = true;
	}

	Win32Window::~Win32Window()
	{
	}

	EGLNativeWindowType Win32Window::getNativeWindow() const
	{
		return m_hwnd;
	}

	bool Win32Window::updateMessages()
	{
		MSG msg = { 0 };
		int gotMsg = (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0);
		if (gotMsg)
		{
			if (msg.message == WM_QUIT)
			{
				m_active = false;
			}

			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		else
		{
			SendMessage(getNativeWindow(), WM_PAINT, 0, 0);
		}

		return m_active;
	}
}
