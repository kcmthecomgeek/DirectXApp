#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class RenderSystem
{
public:
	RenderSystem();
	~RenderSystem();
	//bool init();
	//bool release();

public:
	SwapChainPtr createSwapChain(HWND hwnd, UINT width, UINT height); // Create the swap chain (back buffer).
	DeviceContextPtr getImmediateDeviceContext() { return this->m_imm_device_context; }; // Get immediate context.
	VertexBufferPtr createVertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list,
		void* shader_byte_code, UINT size_byte_shader); // Create a new vertex buffer.
	IndexBufferPtr createIndexBuffer(void* list_indices, UINT size_list);
	ConstantBufferPtr createConstantBuffer(void* buffer, UINT size_buffer);
	VertexShaderPtr createVertexShader(const void* shader_byte_code, size_t byte_code_size);
	PixelShaderPtr createPixelShader(const void* shader_byte_code, size_t byte_code_size);

public:
	bool compileVertexShader(const wchar_t* file_name, const char* entry_point_name,
		void** shader_byte_code, size_t* byte_code_size);
	bool compilePixelShader(const wchar_t* file_name, const char* entry_point_name,
		void** shader_byte_code, size_t* byte_code_size);
	void releaseCompiledShader();

private:
	// D3D Base Init
	ID3D11Device* m_d3d_device = nullptr;
	D3D_FEATURE_LEVEL m_feature_level = D3D_FEATURE_LEVEL_11_0;
	DeviceContextPtr m_imm_device_context = nullptr;
	// Swap chain
	IDXGIDevice* m_dxgi_device = nullptr;
	IDXGIAdapter* m_dxgi_adapter = nullptr;
	IDXGIFactory* m_dxgi_factory = nullptr;
	ID3D11DeviceContext* m_imm_context = nullptr;
	// Shaders
	ID3DBlob* m_blob = nullptr;
	ID3DBlob* m_vsblob = nullptr;
	ID3DBlob* m_psblob = nullptr;
	ID3D11VertexShader* m_vs = nullptr;
	ID3D11PixelShader* m_ps = nullptr;
	// Friend classes
	friend class SwapChain;
	friend class VertexBuffer;
	friend class IndexBuffer;
	friend class ConstantBuffer;
	friend class VertexShader;
	friend class PixelShader;
};

