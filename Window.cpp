#include "Window.h"

Window* window = nullptr;

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

	if (!window)
		window = this;

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
		NULL); // Always NULL.

	// Return false if window couldn't be created.
	if (!m_hwnd)
		return false;

	// Show and update window (draw).
	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);

	m_isRunning = true;

	return true;
}

bool Window::broadcast()
{
	MSG msg;
	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	window->onUpdate();

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		window->onCreate();
		break;
	}
	case WM_DESTROY:
	{
		window->onDestroy();
		::PostQuitMessage(0);
		break;
	}
	default:
		return ::DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return NULL;
}