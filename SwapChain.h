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
	
	void present(bool vsync);

private:
	IDXGISwapChain* m_swap_chain;
	ID3D11RenderTargetView* m_rtv;

	friend class DeviceContext;
};

