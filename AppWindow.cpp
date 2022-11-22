#include "AppWindow.h"

AppWindow::AppWindow()
	:Window()
{
	m_swap_chain = nullptr;
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
}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	// Spit out color on the screen :)
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		1, 0, 1, 1);

	// Present back buffer.
	m_swap_chain->present(false);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_swap_chain->release();
	GraphicsEngine::get()->release();
}