#include "DeviceContext.h"
#include "SwapChain.h"

DeviceContext::DeviceContext(ID3D11DeviceContext* device_context)
	:m_device_context(device_context)
{

}

DeviceContext::~DeviceContext()
{

}

bool DeviceContext::clearRenderTargetColor(SwapChain* swap_chain, float r, float g, float b, float a)
{
	// Create float array for color.
	FLOAT clear_color[] = {r, g, b, a};
	// Push the color to the immediate device.
	m_device_context->ClearRenderTargetView(swap_chain->m_rtv, clear_color);

	return true;
}

bool DeviceContext::release()
{
	m_device_context->Release();
	delete this;
	return true;
}