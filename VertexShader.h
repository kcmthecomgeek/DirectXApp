#pragma once
#include <d3d11.h>

class GraphicsEngine;
class DeviceContext;

class VertexShader
{
public:
	VertexShader();
	~VertexShader();

	bool release();


private:
	bool init(const void* shader_byte_code, size_t byte_code_size);

	ID3D11VertexShader* m_vs;

	friend class GraphicsEngine;
	friend class DeviceContext;
};

