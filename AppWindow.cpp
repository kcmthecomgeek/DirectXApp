#include "AppWindow.h"
#include <Windows.h>

// 3 point vector.
struct vec3
{
	float x, y, z;
};

// vertex primitive for triangle.
struct vertex
{
	vec3 position;
	vec3 position1;
	vec3 color;
	vec3 color1;
};

__declspec(align(16))
struct constant
{
	unsigned int m_time;
};

AppWindow::AppWindow()
	:Window()
{
	m_swap_chain = nullptr;
	m_vb = nullptr;
	m_vs = nullptr;
	m_ps = nullptr;
	m_cb = nullptr;
}

AppWindow::~AppWindow()
{

}

void AppWindow::onCreate()
{
	// Base call.
	Window::onCreate();
	// Init graphics engine.
	GraphicsEngine::get()->init();
	// Create swap chain.
	m_swap_chain = GraphicsEngine::get()->createSwapChain();

	// Get window rectangle and init the swap chain.
	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	// List of points for our triangle.
	/**vertex list[] =
	{
		// x - y - z
		{ -0.5f, -0.5f, 0.0f }, // POS 1
		{ -0.5f, 0.5f, 0.0f }, // POS 2
		{ 0.5f, 0.5f, 0.0f }, // POS 3

		{ 0.5f, 0.5f, 0.0f }, // POS 4
		{ 0.5f, -0.5f, 0.0f }, // POS 5
		{ -0.5f, -0.5f, 0.0f }
	};**/

	// List od points for the quad, first three are initial triangle, last one is the remaining.
	vertex list[] =
	{
		{ -0.5f, -0.5f, 0.0f,	-0.32f, -0.11f, 0.0f,	0, 0, 0,	0, 1, 0 }, // Bottom left,
		{ -0.5f, 0.5f, 0.0f,	-0.11f, 0.78f, 0.0f,	1, 1, 0,	0, 1, 1 }, // Top left,
		{ 0.5f, -0.5f, 0.0f,	0.75f, -0.73f, 0.0f,	0, 0, 1,	1, 0, 0 }, // Bottom right,
		{ 0.5f, 0.5f, 0.0f,		0.88f, 0.77f, 0.0f, 	1, 1, 1,	0, 0, 1 } // Top right.
	};

	// Create the vertext buffer and shader.
	m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(list);
	//GraphicsEngine::get()->createShaders();

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	// Compile the shader and load it.
	//GraphicsEngine::get()->getShaderBufferAndSize(&shader_byte_code, &size_shader);
	m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	m_vb->load(list, sizeof(vertex), size_list, shader_byte_code, static_cast<UINT>(size_shader));

	// Release compiled shader.
	GraphicsEngine::get()->releaseCompiledShader();

	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	constant cc = {};
	cc.m_time = 0;

	m_cb = GraphicsEngine::get()->createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));
}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	// Spit out color on the screen :)
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0, 0.37f, 0.5f, 1);

	// Set view port and vertex buffer.
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	constant cc = {};
	cc.m_time = ::GetTickCount();
	m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);

	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	//GraphicsEngine::get()->setShaders();
	// Set vertex and pixel shader.
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	// Draw the triangle using the vertices pushed to the context.
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb->getSizeVertexList(), 0);
	// Present back buffer.
	m_swap_chain->present(false);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	if (m_vb) m_vb->release();
	m_swap_chain->release();
	if (m_vs) m_vs->release();
	if (m_ps) m_ps->release();
	GraphicsEngine::get()->release();
}