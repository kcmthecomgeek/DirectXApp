#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include <d3dcompiler.h>

GraphicsEngine::GraphicsEngine()
{
	m_d3d_device = nullptr;
	m_feature_level = D3D_FEATURE_LEVEL_11_0;
	m_imm_device_context = nullptr;
	m_dxgi_device = nullptr;
	m_dxgi_adapter = nullptr;
	m_dxgi_factory = nullptr;
	m_imm_context = nullptr;
	m_vsblob = nullptr;
	m_psblob = nullptr;
	m_vs = nullptr;
	m_ps = nullptr;
}

GraphicsEngine::~GraphicsEngine()
{

}

bool GraphicsEngine::init()
{
	// Arrays for driver type preferences and target feature levels.
	D3D_DRIVER_TYPE driver_types[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};

	// Size of aforementioned arrays.
	UINT num_driver_types = ARRAYSIZE(driver_types);
	UINT num_feature_levels = ARRAYSIZE(feature_levels);

	// Enumerate and test each device, checking for a compatible combination.
	HRESULT result = NULL;
	//ID3D11DeviceContext* m_imm_context = nullptr;
	for (UINT driver_type_index = 0; driver_type_index < num_driver_types;)
	{
		result = D3D11CreateDevice(
			NULL, // IDXGI Adapter
			driver_types[driver_type_index], // Current driver type to test.
			NULL, // Software type
			NULL, // Flags
			feature_levels, // Target feature levels (in array format)
			num_feature_levels, // Total number of feature levels in the supplied array.
			D3D11_SDK_VERSION, // SDK version.
			&m_d3d_device, // D3D11 Device object.
			&m_feature_level, // D3D Feature Level object for storage.
			&m_imm_context); // D3D11 Device context.

		// Good one found. Break out.
		if (SUCCEEDED(result))
			break;

		++driver_type_index;
	}

	// App isn't compatible. Return failure...
	if (FAILED(result))
		return false;

	// Store back buffer for later use.
	m_imm_device_context = new DeviceContext(m_imm_context);

	// Create and populate the DXGI objects.
	m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device);
	m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter);
	m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory);

	return true;
}

bool GraphicsEngine::release()
{
	if (m_vs) m_vs->Release();
	if (m_ps) m_ps->Release();

	if (m_vsblob) m_vsblob->Release();
	if (m_psblob) m_psblob->Release();

	m_dxgi_device->Release();
	m_dxgi_adapter->Release();
	m_dxgi_factory->Release();

	m_imm_device_context->release();
	m_d3d_device->Release();
	return true;
}

SwapChain* GraphicsEngine::createSwapChain()
{
	return new SwapChain();
}

VertexBuffer* GraphicsEngine::createVertexBuffer()
{
	return new VertexBuffer();
}

bool GraphicsEngine::createShaders()
{
	// Create and compile shader data.
	ID3DBlob* errblob = nullptr;
	D3DCompileFromFile(L"shader.fx", nullptr, nullptr, "vsmain", "vs_5_0", NULL, NULL, &m_vsblob, &errblob);
	D3DCompileFromFile(L"shader.fx", nullptr, nullptr, "psmain", "ps_5_0", NULL, NULL, &m_psblob, &errblob);
	m_d3d_device->CreateVertexShader(m_vsblob->GetBufferPointer(), m_vsblob->GetBufferSize(), nullptr, &m_vs);
	m_d3d_device->CreatePixelShader(m_psblob->GetBufferPointer(), m_psblob->GetBufferSize(), nullptr, &m_ps);
	return true;
}

bool GraphicsEngine::setShaders()
{
	// Push the compiled shader to the DXGI context.
	m_imm_context->VSSetShader(m_vs, nullptr, 0);
	m_imm_context->PSSetShader(m_ps, nullptr, 0);
	return true;
}

void GraphicsEngine::getShaderBufferAndSize(void** bytecode, UINT* size)
{
	// Get shader buffer info/data.
	*bytecode = this->m_vsblob->GetBufferPointer();
	*size = (UINT)m_vsblob->GetBufferSize();
}

GraphicsEngine* GraphicsEngine::get()
{
	static GraphicsEngine engine;
	return &engine;
}