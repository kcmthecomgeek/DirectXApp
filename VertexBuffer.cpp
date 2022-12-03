#include "VertexBuffer.h"
#include "RenderSystem.h"
#include <exception>

VertexBuffer::VertexBuffer(void* list_vertices,
	UINT size_vertex,
	UINT size_list,
	void* shader_byte_code,
	UINT size_byte_shader,
	RenderSystem* system)
	: m_system(system), m_buffer(0), m_layout(0)
{
	// Send the buffer desciption/parameters.
	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = size_vertex * size_list;
	buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;
	// Buffer subresource info (vertices values/positions).
	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = list_vertices;

	// Set the private members to the supplied data.
	m_size_vertex = size_vertex;
	m_size_list = size_list;

	// Create the new buffer.
	if (FAILED(m_system->m_d3d_device->CreateBuffer(&buff_desc, &init_data, &m_buffer)))
		throw std::exception("VertexBuffer: could not create successfully.");

	// Element layout definition. Needed when the data is sent to the device context.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		// SEMANTIC NAME - SEMANTIC INDEX - FORMAT - INPUT SLOT - ALIGNED BYTE OFFSET - INPUT SLOT CLASS - INSTANCE DATA STEP RATE
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	UINT size_layout = ARRAYSIZE(layout);

	// Create the input layout.
	if (FAILED(m_system->m_d3d_device->CreateInputLayout(
		layout,
		size_layout,
		shader_byte_code,
		size_byte_shader,
		&m_layout)))
		throw std::exception("VertexBuffer: could not create input layout.");
}

VertexBuffer::~VertexBuffer()
{
	if (m_layout) m_layout->Release();
	if (m_buffer) m_buffer->Release();
}