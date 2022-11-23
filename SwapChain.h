#pragma once
#include <d3d11.h>

class DeviceContext;

class SwapChain
{
public:
	SwapChain();
	~SwapChain();

	bool init(HWND hwnd, UINT width, UINT height);
	bool release();
	
	void present(bool vsync); // Send the back buffer to the front.

private:
	// Swap chain and render target variables.
	IDXGISwapChain* m_swap_chain;
	ID3D11RenderTargetView* m_rtv;
	// Friend classes.
	friend class DeviceContext;
};

