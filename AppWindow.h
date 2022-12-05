#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputListener.h"
#include "Point.h"
#include "Matrix4x4.h"

class AppWindow :
    public Window,
    public InputListener
{
public:
    AppWindow();
    ~AppWindow();

    void update();

    // Inherited
    virtual void onCreate() override;
    virtual void onUpdate() override;
    virtual void onDestroy() override;
    virtual void onFocus() override;
    virtual void onKillFocus() override;
    // Inherited via InputListener
    virtual void onKeyDown(int key) override;
    virtual void onKeyUp(int key) override;
    virtual void onMouseMove(const Point& mouse_pos) override;
    virtual void onLeftMouseDown(const Point& mouse_pos) override;
    virtual void onLeftMouseUp(const Point& mouse_pos) override;
    virtual void onRightMouseDown(const Point& mouse_pos) override;
    virtual void onRightMouseUp(const Point& mouse_pos) override;

private:
    SwapChainPtr m_swap_chain;
    VertexBufferPtr m_vb;
    ConstantBufferPtr m_cb;
    IndexBufferPtr m_ib;
    VertexShaderPtr m_vs;
    PixelShaderPtr m_ps;
    TexturePtr m_wood_tex;

    long m_old_delta = 0;
    long m_new_delta = 0;
    float m_delta_time = 0;
    //float m_angle = 0;

    float m_delta_pos = 0;
    float m_delta_scale = 0;
    float m_delta_rot = 0;

    float m_rot_x = 0.0f;
    float m_rot_y = 0.0f;

    float m_scale_cube = 1.0f;
    float m_forward = 0.0f;
    float m_rightward = 0.0f;
    Matrix4x4 m_world_cam;
};

