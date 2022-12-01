#include "Window.h"


Window::Window()
{
	m_hwnd = NULL;
	m_isRunning = false;
}

Window::~Window()
{

}

bool Window::init()
{
	// Window registration parameters.
	WNDCLASSEX wc = {}; // Init empty variable.
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX); // Size of a new object.
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW; // Use system brush for background color of window.
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Use default system "arrow" cursor.
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); // Use default system "application" icon.
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION); // Use default system "application" icon.
	wc.hInstance = NULL; // Null instance value.
	wc.lpszClassName = L"MyWindowClass"; // Class name. Must be unique per running app.
	wc.lpszMenuName = L""; // Empty menu name.
	wc.style = NULL; // No style values yet.
	wc.lpfnWndProc = &WndProc;
	
	// Register, return if failed.
	if (!::RegisterClassEx(&wc))
		return false;

	// Create and store Window object.
	m_hwnd = ::CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW, // Extended style flags.
		L"MyWindowClass", // Window class name (should match what's defined in "wc.lpszClassName").
		L"DirectX Application", // Window title.
		WS_OVERLAPPEDWINDOW, // Standard style flags.
		CW_USEDEFAULT, CW_USEDEFAULT, // X,Y values.
		1024, 768, // Width,Height
		NULL, // Parent window.
		NULL, // Menu object.
		NULL, // HINSTANCE
		this); // Create paramters. Adding "this" will pass a point to this object for use in WM_CREATE.

	// Return false if window couldn't be created.
	if (!m_hwnd)
		return false;

	// Show and update window (draw).
	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);

	// "Start" the game.
	m_isRunning = true;

	return true;
}

bool Window::broadcast()
{
	// Call the exposed update function.
	this->onUpdate();

	// Peek and redirect messages in the queue.
	MSG msg;
	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	// Prevent 99% CPU.
	Sleep(1);

	return true;
}

bool Window::release()
{
	// Destroy window object if it exists.
	if (!::DestroyWindow(m_hwnd))
		return false;

	return true;
}

RECT Window::getClientWindowRect()
{
	// Get client rect of this window using WinAPI.
	RECT rc = {};
	::GetClientRect(this->m_hwnd, &rc);
	return rc;
}

void Window::onCreate()
{

}

void Window::onUpdate()
{

}

void Window::onDestroy()
{
	m_isRunning = false;
}

void Window::onFocus()
{

}

void Window::onKillFocus()
{

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		// Copy the pointer passed in last parameter in "CreateWindowEx".
		Window* window = (Window*)((LPCREATESTRUCT)lParam)->lpCreateParams;
		// Store the pointer in custom user data for the window.
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)window);
		// Store the HWND object in the "Window" pointer.
		window->setHWND(hWnd);
		window->onCreate();
		break;
	}
	case WM_SETFOCUS:
	{
		Window* window = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		window->onFocus();
		break;
	}
	case WM_KILLFOCUS:
	{
		Window* window = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		window->onKillFocus();
		break;
	}
	case WM_DESTROY:
	{
		// Get the pointer to the "Window" object that generated this window.
		Window* window = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		window->onDestroy();
		::PostQuitMessage(0);
		break;
	}
	default:
		return ::DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return NULL;
}