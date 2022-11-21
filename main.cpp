#include <Windows.h>
#include "AppWindow.h"

int WINAPI wWinMain(
	_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_ PWSTR lpCmdLine, 
	_In_ int nCmdShow)
{
	AppWindow app;
	if (app.init())
	{
		while (app.isRunning())
		{
			app.broadcast();
		}
	}
	return 0;
}