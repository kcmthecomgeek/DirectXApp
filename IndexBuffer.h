#pragma once
#include <d3d11.h>

class DeviceContext;

class IndexBuffer
{
public:
	IndexBuffer();
	~IndexBuffer();

	// Load the buffer data from a list of vertices. Includes shaders, if necessary.
	bool load(void* list_indices, UINT size_list);
	UINT getSizeIndexList() { return this->m_size_list; }; // Get the size list.
	bool release();

private:
	// Size values.
	UINT m_size_list;
	// Buffer and layout variables.
	ID3D11Buffer* m_buffer;
	// Friend classes.
	friend class DeviceContext;
};

