#pragma once
#include <d3d11.h>

class SwapChain;
class VertexBuffer;
class ConstantBuffer;
class VertexShader;
class PixelShader;

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* device_context);
	~DeviceContext();

	void clearRenderTargetColor(SwapChain* swap_chain, float r, float g, float b, float a); // Clear window with color.
	void setVertexBuffer(VertexBuffer* vertex_buffer);
	void drawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void drawTriangleStrip(UINT vertext_count, UINT start_vertex_index);
	void setViewportSize(UINT width, UINT height);
	void setVertexShader(VertexShader* vertex_shader);
	void setPixelShader(PixelShader* pixel_shader);
	bool release();

private:
	ID3D11DeviceContext* m_device_context;

	friend class ConstantBuffer;
};

