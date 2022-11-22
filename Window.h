#pragma once
#include <Windows.h>

class Window
{
public:
	Window();
	~Window();

	bool init();
	bool broadcast();
	bool release();

	RECT getClientWindowRect();
	void setHWND(HWND hwnd) { this->m_hwnd = hwnd; };

	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();

	bool isRunning() { return m_isRunning; };

protected:
	HWND m_hwnd;
	bool m_isRunning;
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);