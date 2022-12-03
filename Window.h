#pragma once
#include <Windows.h>

class Window
{
public:
	Window();
	~Window();

	RECT getClientWindowRect();

	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();

	bool isRunning();

private:
	bool broadcast();

protected:
	HWND m_hwnd;
	bool m_isRunning;
	bool m_isInit = false;
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);