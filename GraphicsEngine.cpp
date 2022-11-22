#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"

GraphicsEngine::GraphicsEngine()
{
	m_d3d_device = nullptr;
	m_feature_level = D3D_FEATURE_LEVEL_11_0;
	m_imm_device_context = nullptr;
	m_dxgi_device = nullptr;
	m_dxgi_adapter = nullptr;
	m_dxgi_factory = nullptr;
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
	ID3D11DeviceContext* m_imm_context = nullptr;
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

GraphicsEngine* GraphicsEngine::get()
{
	static GraphicsEngine engine;
	return &engine;
}