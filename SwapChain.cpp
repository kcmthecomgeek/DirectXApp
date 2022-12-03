#include "SwapChain.h"
#include "RenderSystem.h"
#include <exception>

SwapChain::SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* system)
	: m_system(system), m_rtv(0)
{
	// Get the D3D device from graphics engine.
	ID3D11Device* device = m_system->m_d3d_device;

	// Swap chain configuration.
	DXGI_SWAP_CHAIN_DESC desc = {};
	ZeroMemory(&desc, sizeof(desc));
	desc.BufferCount = 1; // 1 buffer because it doesn't count the front buffer managed by Windows.
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // Swap effect.
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

	// Create the swap chain.
	HRESULT result = m_system->m_dxgi_factory->CreateSwapChain(device, &desc, &m_swap_chain);

	if (FAILED(result))
		throw std::exception("SwapChain: failed to create.");

	// Create buffer for back color.
	ID3D11Texture2D* buffer = NULL;
	result = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);

	if (FAILED(result))
		throw std::exception("SwapChain: could not create buffer for back color.");

	// Create the view for the render target.
	result = device->CreateRenderTargetView(buffer, NULL, &m_rtv);
	buffer->Release();

	if (FAILED(result))
		throw std::exception("SwapChain: could not create render view.");
}

SwapChain::~SwapChain()
{
	if (m_rtv) m_rtv->Release();
	if (m_swap_chain) m_swap_chain->Release();
}

void SwapChain::present(bool vsync)
{
	m_swap_chain->Present(vsync, NULL);
}