#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class VertexBuffer
{
public:
	// Load the buffer data from a list of vertices. Includes shaders, if necessary.
	VertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list, 
		void* shader_byte_code, UINT size_byte_shader, RenderSystem* system);
	~VertexBuffer();

	UINT getSizeVertexList() { return this->m_size_list; }; // Get the size list.

private:
	// Size values.
	UINT m_size_vertex;
	UINT m_size_list;
	// Buffer and layout variables.
	ID3D11Buffer* m_buffer;
	ID3D11InputLayout* m_layout;
	RenderSystem* m_system = nullptr;
	// Friend classes.
	friend class DeviceContext;
};

