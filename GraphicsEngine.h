#pragma once
#include <d3d11.h>

class SwapChain;
class DeviceContext;

class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine();

	bool init();
	bool release();
	SwapChain* createSwapChain();
	DeviceContext* getImmediateDeviceContext() { return this->m_imm_device_context; }; // Get immediate context.

	static GraphicsEngine* get();

private:
	// D3D Base Init
	ID3D11Device* m_d3d_device;
	D3D_FEATURE_LEVEL m_feature_level;
	DeviceContext* m_imm_device_context;
	// Swap chain
	IDXGIDevice* m_dxgi_device;
	IDXGIAdapter* m_dxgi_adapter;
	IDXGIFactory* m_dxgi_factory;
	// Friend classes
	friend class SwapChain;
};

