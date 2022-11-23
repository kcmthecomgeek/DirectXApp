#pragma once
#include <d3d11.h>

class SwapChain;
class DeviceContext;
class VertexBuffer;
class VertexShader;
class PixelShader;

class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine();
	bool init();
	bool release();

public:
	SwapChain* createSwapChain(); // Create the swap chain (back buffer).
	DeviceContext* getImmediateDeviceContext() { return this->m_imm_device_context; }; // Get immediate context.
	VertexBuffer* createVertexBuffer(); // Create a new vertex buffer.
	VertexShader* createVertexShader(const void* shader_byte_code, size_t byte_code_size);
	PixelShader* createPixelShader(const void* shader_byte_code, size_t byte_code_size);
public:
	bool compileVertexShader(const wchar_t* file_name, const char* entry_point_name,
		void** shader_byte_code, size_t* byte_code_size);
	bool compilePixelShader(const wchar_t* file_name, const char* entry_point_name,
		void** shader_byte_code, size_t* byte_code_size);
	void releaseCompiledShader();
	// Default simple shaders.
	// bool createShaders(); // Create shaders and compile.
	// bool setShaders(); // Get shadeers.
	// void getShaderBufferAndSize(void** bytecode, UINT* size); // Get shader buffer and size.

public:
	// Singleton.
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
	ID3DBlob* m_blob;

	ID3DBlob* m_vsblob;
	ID3DBlob* m_psblob;
	ID3D11VertexShader* m_vs;
	ID3D11PixelShader* m_ps;
	// Friend classes
	friend class SwapChain;
	friend class VertexBuffer;
	friend class VertexShader;
	friend class PixelShader;
};

