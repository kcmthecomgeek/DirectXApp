// Disable warning about GetTickCount
#pragma warning(disable:28159)

#include "AppWindow.h"
#include <Windows.h>
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"

// 3 point vector.
struct vec3
{
	float x, y, z;
};

// vertex primitive for triangle.
struct vertex
{
	Vector3D position;
	Vector3D color;
	Vector3D color1;
};

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
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
	m_ib = nullptr;
	m_world_cam = {};
}

AppWindow::~AppWindow()
{

}

void AppWindow::update()
{
	// Time calculation for constant buffer.
	/*unsigned long new_time = 0;
	if (m_old_time)
		new_time = ::GetTickCount() - m_old_time;
	m_delta_time = new_time / 1000.0f;
	m_old_time = ::GetTickCount();
	m_angle += 1.57f * m_delta_time;*/

	// Populate constant values for ConstantBuffer.
	constant cc = {};
	cc.m_time = ::GetTickCount();

	// Calculate delta position and scale.
	m_delta_pos += m_delta_time / 10.0f;
	if (m_delta_pos > 1.0f)
		m_delta_pos = 0;
	m_delta_scale += m_delta_time / 0.55f;

	Matrix4x4 temp = {};
	cc.m_world.setIdentity();

	Matrix4x4 world_cam = {};
	world_cam.setIdentity();

	temp.setIdentity();
	temp.setRotationX(m_rot_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	world_cam *= temp;

	Vector3D new_pos = m_world_cam.getTranslation() + world_cam.getZDirection() * (m_forward * 0.3f);

	new_pos = new_pos + world_cam.getXDirection() * (m_rightward * 0.3f);

	world_cam.setTranslation(new_pos);
	m_world_cam = world_cam;
	world_cam.inverse();

	cc.m_view = world_cam;
	/*cc.m_proj.setOthroLH
	(
		(float)(this->getClientWindowRect().right - this->getClientWindowRect().left) / 300.0f,
		(float)(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 300.0f,
		-4.0f,
		4.0f
	);*/

	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	cc.m_proj.setPerspectiveFovLH
	(
		1.57f,
		((float)width / (float)height),
		0.1f,
		100.0f
	);

	// Update buffer with constant data.
	m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
}

void AppWindow::onCreate()
{
	// Base call.
	Window::onCreate();

	// Add InputListener for Keyboard.
	InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(false);

	// Init graphics engine.
	//GraphicsEngine::get()->init();
	// Create swap chain.
	// Get window rectangle and init the swap chain.
	RECT rc = this->getClientWindowRect();
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->
		createSwapChain(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	m_world_cam.setTranslation(Vector3D(0.0f, 0.0f, -2.0f));

	// List od points for the quad, first three are initial triangle, last one is the remaining.
	vertex vertex_list[] =
	{
		// Front face
		{ Vector3D(-0.5f, -0.5f, -0.5f),	Vector3D(1, 0, 0), Vector3D(0.2f, 0, 0) },		// Point 0
		{ Vector3D(-0.5f, 0.5f, -0.5f),		Vector3D(1, 1, 0), Vector3D(0.2f, 0.2f, 0) },	// Point 1
		{ Vector3D(0.5f, 0.5f, -0.5f),		Vector3D(1, 1, 0), Vector3D(0.2f, 0.2f, 0) },	// Point 2
		{ Vector3D(0.5f, -0.5f, -0.5f),		Vector3D(1, 0, 0), Vector3D(0.2f, 0, 0) },		// Point 3
		// Back face
		{ Vector3D(0.5f, -0.5f, 0.5f),		Vector3D(0, 1, 0), Vector3D(0, 0.2f, 0) },		// Point 4
		{ Vector3D(0.5f, 0.5f, 0.5f),		Vector3D(0, 1, 1), Vector3D(0, 0.2f, 0.2f) },	// Point 5
		{ Vector3D(-0.5f, 0.5f, 0.5f),		Vector3D(0, 1, 1), Vector3D(0, 0.2f, 0.2f) },	// Point 6
		{ Vector3D(-0.5f, -0.5f, 0.5f),		Vector3D(0, 1, 0), Vector3D(0, 0.2f, 0) }		// Point 7
	};
	UINT size_list = ARRAYSIZE(vertex_list);

	unsigned int index_list[] =
	{
		// Front side
		0, 1, 2, // First triangle
		2, 3, 0, // Second triangle
		// Back side
		4, 5, 6,
		6, 7, 4,
		// Top side
		1, 6, 5,
		5, 2, 1,
		// Bottom side
		7, 0, 3,
		3, 4, 7,
		// Right side
		3, 2, 5,
		5, 4, 3,
		// Left side
		7, 6, 1,
		1, 0, 7
	};

	UINT size_index_list = ARRAYSIZE(index_list);
	m_ib = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(index_list, size_index_list);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	// Compile the shader and load it.
	m_vs = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);
	// Create the vertext buffer and shader.
	m_vb = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(
		vertex_list, sizeof(vertex), size_list, shader_byte_code, static_cast<UINT>(size_shader));

	// Release compiled shader.
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	constant cc = {};
	cc.m_time = 0;
	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	// Update key values.
	InputSystem::get()->update();

	// Spit out color on the screen :)
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0, 0.37f, 0.5f, 1);

	// Set view port and vertex buffer.
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);


	update();

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);
	// Set vertex and pixel shader.
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(m_ps);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	// Set the index buffer.
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(m_ib);

	// Draw the triangle using the vertices pushed to the context.
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);
	// Present back buffer.
	m_swap_chain->present(true);

	/*m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount();
	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;*/
	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount();
	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	// Remove InputSystem listener.
	InputSystem::get()->removeListener(this);
	//GraphicsEngine::get()->release();
}

void AppWindow::onFocus()
{
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

void AppWindow::onKeyDown(int key)
{
	if (key == 'W')
	{
		//m_rot_x += 3.14f * m_delta_time;
		m_forward = 1.0f;
	}
	else if (key == 'S')
	{
		//m_rot_x -= 3.14f * m_delta_time;
		m_forward = -1.0f;
	}
	else if (key == 'A')
	{
		//m_rot_y += 3.14f * m_delta_time;
		m_rightward = -1.0f;
	}
	else if (key == 'D')
	{
		//m_rot_y -= 3.14f * m_delta_time;
		m_rightward = 1.0f;
	}
}

void AppWindow::onKeyUp(int key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;
}

void AppWindow::onMouseMove(const Point& mouse_pos)
{
	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	m_rot_x += (mouse_pos.m_y - (height / 2.0f)) * m_delta_time * 0.1f;
	m_rot_y += (mouse_pos.m_x - (width / 2.0f)) * m_delta_time * 0.1f;

	InputSystem::get()->setCursorPosition(Point(width / 2.0f, height / 2.0f));
}

void AppWindow::onLeftMouseDown(const Point& mouse_pos)
{
	m_scale_cube = 0.5f;
}

void AppWindow::onLeftMouseUp(const Point& mouse_pos)
{
	m_scale_cube = 1.0f;
}

void AppWindow::onRightMouseDown(const Point& mouse_pos)
{
	m_scale_cube = 2.0f;
}

void AppWindow::onRightMouseUp(const Point& mouse_pos)
{
	m_scale_cube = 1.0f;
}
