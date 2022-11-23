#include "PixelShader.h"
#include "GraphicsEngine.h"

PixelShader::PixelShader()
{
	m_ps = nullptr;
}

PixelShader::~PixelShader()
{

}

void PixelShader::release()
{
	if (m_ps) m_ps->Release();
}

bool PixelShader::init(const void* shader_byte_code, size_t byte_code_size)
{
	if (!SUCCEEDED(GraphicsEngine::get()->m_d3d_device->CreatePixelShader(shader_byte_code, byte_code_size, nullptr, &m_ps)))
		return false;

	return true;
}