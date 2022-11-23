#include "VertexShader.h"
#include "GraphicsEngine.h"


VertexShader::VertexShader()
{
	m_vs = nullptr;
}

VertexShader::~VertexShader()
{

}

bool VertexShader::release()
{
	if (m_vs) m_vs->Release();
	return true;
}

bool VertexShader::init(const void* shader_byte_code, size_t byte_code_size)
{
	if (!SUCCEEDED(GraphicsEngine::get()->m_d3d_device->CreateVertexShader(shader_byte_code, byte_code_size, nullptr, &m_vs)))
		return false;

	return true;
}