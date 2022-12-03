#include <Windows.h>
#include "AppWindow.h"
#include "InputSystem.h"

int WINAPI wWinMain(
	_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_ PWSTR lpCmdLine, 
	_In_ int nCmdShow)
{
	try
	{
		GraphicsEngine::create();
		InputSystem::create();
	}
	catch (...) { return -1; }

	{
		try
		{
			AppWindow app;
			while (app.isRunning());
		}
		catch (...) 
		{
			InputSystem::release();
			GraphicsEngine::release();
			return -1;
		}
	}

	InputSystem::release();
	GraphicsEngine::release();

	return 0;
}