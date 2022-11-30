#include "IndexBuffer.h"
#include "GraphicsEngine.h"

IndexBuffer::IndexBuffer()
{
	m_size_list = 0;
	m_buffer = nullptr;
}

IndexBuffer::~IndexBuffer()
{

}

bool IndexBuffer::load(void* list_indices, UINT size_list)
{
	// Release the buffer if it still exists. This is to ensure the objects are cleaned under all conditions.
	if (m_buffer) m_buffer->Release();

	// Send the buffer desciption/parameters.
	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = 4 * size_list;
	buff_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;
	// Buffer subresource info (vertices values/positions).
	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = list_indices;

	// Set the private members to the supplied data.
	m_size_list = size_list;

	// Create the new buffer.
	if (FAILED(GraphicsEngine::get()->m_d3d_device->CreateBuffer(&buff_desc, &init_data, &m_buffer)))
		return false;

	return true;
}

bool IndexBuffer::release()
{
	if (m_buffer) m_buffer->Release();
	delete this;
	return true;
}