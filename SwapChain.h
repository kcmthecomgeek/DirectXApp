#pragma once
#include <d3d11.h>

class SwapChain
{
public:
	SwapChain();
	~SwapChain();

	bool init(HWND hwnd, UINT width, UINT height);
	bool release();
	
	void present();

private:
	IDXGISwapChain* m_swap_chain;
};

