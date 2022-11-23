#include "DeviceContext.h"
#include "SwapChain.h"
#include "VertexBuffer.h"

DeviceContext::DeviceContext(ID3D11DeviceContext* device_context)
	:m_device_context(device_context)
{

}

DeviceContext::~DeviceContext()
{

}

void DeviceContext::clearRenderTargetColor(SwapChain* swap_chain, float r, float g, float b, float a)
{
	// Create float array for color.
	FLOAT clear_color[] = {r, g, b, a};
	// Push the color to the immediate device.
	m_device_context->ClearRenderTargetView(swap_chain->m_rtv, clear_color);
	m_device_context->OMSetRenderTargets(1, &swap_chain->m_rtv, NULL);
}

void DeviceContext::setVertexBuffer(VertexBuffer* vertex_buffer)
{
	// Send the supplied vertex buffer object over to the device context for use.
	UINT stride = vertex_buffer->m_size_vertex;
	UINT offset = 0;
	m_device_context->IASetVertexBuffers(0, 1, &vertex_buffer->m_buffer, &stride, &offset); // Set buffers.
	m_device_context->IASetInputLayout(vertex_buffer->m_layout); // Set layout.
}

void DeviceContext::drawTriangleList(UINT vertex_count, UINT start_vertex_index)
{
	// Draw the vertices using a primitive trianglelist.
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_device_context->Draw(vertex_count, start_vertex_index); // Draw the vertices.
}

void DeviceContext::drawTriangleStrip(UINT vertex_count, UINT start_vertex_index)
{
	// Draw the vertices using a primitive trianglestrip.
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_device_context->Draw(vertex_count, start_vertex_index); // Draw the vertices.
}

void DeviceContext::setViewportSize(UINT width, UINT height)
{
	// Set the view port (what the user sees?). The displayed screen space.
	D3D11_VIEWPORT vp = {};
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	m_device_context->RSSetViewports(1, &vp);
}

bool DeviceContext::release()
{
	m_device_context->Release();
	delete this;
	return true;
}