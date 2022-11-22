#pragma once
#include <d3d11.h>

class SwapChain;
class DeviceContext;
class VertexBuffer;

class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine();

	bool init();
	bool release();
	SwapChain* createSwapChain();
	DeviceContext* getImmediateDeviceContext() { return this->m_imm_device_context; }; // Get immediate context.
	VertexBuffer* createVertexBuffer();
	bool createShaders();
	bool setShaders();
	void getShaderBufferAndSize(void** bytecode, UINT* size);

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
	ID3D11DeviceContext* m_imm_context;
	// Shaders
	ID3DBlob* m_vsblob;
	ID3DBlob* m_psblob;
	ID3D11VertexShader* m_vs;
	ID3D11PixelShader* m_ps;
	// Friend classes
	friend class SwapChain;
	friend class VertexBuffer;
};

