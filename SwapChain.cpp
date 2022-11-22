#include "SwapChain.h"
#include "GraphicsEngine.h"

SwapChain::SwapChain()
{
	m_swap_chain = nullptr;
}

SwapChain::~SwapChain()
{

}

bool SwapChain::init(HWND hwnd, UINT width, UINT height)
{
	ID3D11Device* device = GraphicsEngine::get()->m_d3d_device;

	DXGI_SWAP_CHAIN_DESC desc = {};
	ZeroMemory(&desc, sizeof(desc));
	desc.BufferCount = 1; // 1 buffer because it doesn't count the front buffer managed by Windows.
	desc.BufferDesc.Width = width; // Width of the buffer.
	desc.BufferDesc.Height = height; // Height of the buffer.
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 1 bit per channel (RGBA).
	desc.BufferDesc.RefreshRate.Numerator = 60; // Refresh rate in hertz.
	desc.BufferDesc.RefreshRate.Denominator = 1; // Multiplier of numberator. Normally 1.
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // Buffer usage type.
	desc.OutputWindow = hwnd; // Window handle.
	desc.SampleDesc.Count = 1; 
	desc.SampleDesc.Quality = 0;
	desc.Windowed = TRUE; // Windowed mode.

	HRESULT result = GraphicsEngine::get()->m_dxgi_factory->CreateSwapChain(device, &desc, &m_swap_chain);

	if (FAILED(result))
		return false;

	return true;
}

bool SwapChain::release()
{
	m_swap_chain->Release();
	delete this;
	return true;
}

void SwapChain::present()
{

}