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
	float m_angle;
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
}

AppWindow::~AppWindow()
{

}

void AppWindow::updateQuadPosition()
{
	// Time calculation for constant buffer.
	unsigned long new_time = 0;
	if (m_old_time)
		new_time = ::GetTickCount() - m_old_time;
	m_delta_time = new_time / 1000.0f;
	m_old_time = ::GetTickCount();
	m_angle += 1.57f * m_delta_time;

	// Populate constant values for ConstantBuffer.
	constant cc = {};
	cc.m_angle = m_angle;

	// Calculate delta position and scale.
	m_delta_pos += m_delta_time / 10.0f;
	if (m_delta_pos > 1.0f)
		m_delta_pos = 0;
	m_delta_scale += m_delta_time / 0.55f;

	Matrix4x4 temp = {};
	//cc.m_world.setTranslation(Vector3D::lerp(Vector3D(-2, -2, 0), Vector3D(2, 2, 0), m_delta_pos));
	///cc.m_world.setScale(Vector3D::lerp(Vector3D(0.5f, 0.5f, 0), Vector3D(1.0f, 1.0f, 0), (sin(m_delta_scale) + 1.0f) / 2.0f));
	///temp.setTranslation(Vector3D::lerp(Vector3D(-1.5f, -1.5f, 0), Vector3D(1.5f, 1.5f, 0), m_delta_pos));
	///cc.m_world *= temp;
	cc.m_world.setScale(Vector3D(m_scale_cube, m_scale_cube, m_scale_cube));

	temp.setIdentity();
	temp.setRotationZ(0.0f);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationX(m_rot_x);
	cc.m_world *= temp;

	cc.m_view.setIdentity();
	cc.m_proj.setOthroLH
	(
		(float)(this->getClientWindowRect().right - this->getClientWindowRect().left) / 300.0f,
		(float)(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 300.0f,
		-4.0f,
		4.0f
	);

	// Update buffer with constant data.
	m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);
}

void AppWindow::onCreate()
{
	// Base call.
	Window::onCreate();

	// Add InputListener for Keyboard.
	InputSystem::get()->addListener(this);

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

	// Create the vertext buffer and shader.
	m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(vertex_list);
	//GraphicsEngine::get()->createShaders();

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

	m_ib = GraphicsEngine::get()->createIndexBuffer();
	UINT size_index_list = ARRAYSIZE(index_list);
	m_ib->load(index_list, size_index_list);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	// Compile the shader and load it.
	//GraphicsEngine::get()->getShaderBufferAndSize(&shader_byte_code, &size_shader);
	m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	m_vb->load(vertex_list, sizeof(vertex), size_list, shader_byte_code, static_cast<UINT>(size_shader));

	// Release compiled shader.
	GraphicsEngine::get()->releaseCompiledShader();

	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	constant cc = {};
	cc.m_angle = 0;

	m_cb = GraphicsEngine::get()->createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	// Update key values.
	InputSystem::get()->update();

	// Spit out color on the screen :)
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0, 0.37f, 0.5f, 1);

	// Set view port and vertex buffer.
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);


	updateQuadPosition();

	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	//GraphicsEngine::get()->setShaders();
	// Set vertex and pixel shader.
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	// Set the index buffer.
	GraphicsEngine::get()->getImmediateDeviceContext()->setIndexBuffer(m_ib);

	// Draw the triangle using the vertices pushed to the context.
	GraphicsEngine::get()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);
	// Present back buffer.
	m_swap_chain->present(true);

	/*m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount();
	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;*/
}

void AppWindow::onDestroy()
{
	Window::onDestroy();

	// Remove InputSystem listener.
	InputSystem::get()->removeListener(this);

	if (m_vb) m_vb->release();
	if (m_cb) m_cb->release();
	if (m_ib) m_ib->release();
	m_swap_chain->release();
	if (m_vs) m_vs->release();
	if (m_ps) m_ps->release();
	GraphicsEngine::get()->release();
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
		m_rot_x += 3.14f * m_delta_time;
	}
	else if (key == 'S')
	{
		m_rot_x -= 3.14f * m_delta_time;
	}
	else if (key == 'A')
	{
		m_rot_y += 3.14f * m_delta_time;
	}
	else if (key == 'D')
	{
		m_rot_y -= 3.14f * m_delta_time;
	}
}

void AppWindow::onKeyUp(int key)
{

}

void AppWindow::onMouseMove(const Point& delta_mouse_pos)
{
	m_rot_x -= delta_mouse_pos.m_y * m_delta_time;
	m_rot_y -= delta_mouse_pos.m_x * m_delta_time;
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
