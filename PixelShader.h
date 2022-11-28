#pragma once
#include <d3d11.h>

class GraphicsEngine;
class DeviceContext;

class PixelShader
{
public:
	PixelShader();
	~PixelShader();

	bool release();

private:
	bool init(const void* shader_byte_code, size_t byte_code_size);

	ID3D11PixelShader* m_ps;

	friend class GraphicsEngine;
	friend class DeviceContext;
};

