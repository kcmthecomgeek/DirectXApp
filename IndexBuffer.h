#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class IndexBuffer
{
public:
	IndexBuffer(void* list_indices, UINT size_list, RenderSystem* system);
	~IndexBuffer();

	// Load the buffer data from a list of vertices. Includes shaders, if necessary.
	UINT getSizeIndexList() { return this->m_size_list; }; // Get the size list.

private:
	// Size values.
	UINT m_size_list;
	// Buffer and layout variables.
	ID3D11Buffer* m_buffer;
	RenderSystem* m_system = nullptr;
	// Friend classes.
	friend class DeviceContext;
};

