#include "AppWindow.h"

struct vec3
{
	float x, y, z;
};

struct vertex
{
	vec3 position;
};

AppWindow::AppWindow()
	:Window()
{
	m_swap_chain = nullptr;
	m_vb = nullptr;
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

	vertex list[] =
	{
		// x - y - z
		{ -0.5f, -0.5f, 0.0f }, // POS 1
		{ 0.0f, 0.5f, 0.0f }, // POS 2
		{ 0.5f, -0.5f, 0.0f } // POS 3
	};

	m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(list);
	GraphicsEngine::get()->createShaders();

	void* shader_byte_code = nullptr;
	UINT size_shader = 0;
	GraphicsEngine::get()->getShaderBufferAndSize(&shader_byte_code, &size_shader);
	m_vb->load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);

}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	// Spit out color on the screen :)
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		1, 0, 1, 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);
	GraphicsEngine::get()->setShaders();
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);

	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleList(m_vb->getSizeVertexList(), 0);
	// Present back buffer.
	m_swap_chain->present(false);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_vb->release();
	m_swap_chain->release();
	GraphicsEngine::get()->release();
}