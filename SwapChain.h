#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class SwapChain
{
public:
	SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* system);
	~SwapChain();
	
	void present(bool vsync); // Send the back buffer to the front.

private:
	// Swap chain and render target variables.
	IDXGISwapChain* m_swap_chain;
	ID3D11RenderTargetView* m_rtv;
	RenderSystem* m_system = nullptr;
	// Friend classes.
	friend class DeviceContext;
};

