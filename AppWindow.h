#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"

class AppWindow :
    public Window
{
public:
    AppWindow();
    ~AppWindow();

    // Inherited
    virtual void onCreate() override;
    virtual void onUpdate() override;
    virtual void onDestroy() override;

private:
    SwapChain* m_swap_chain;
    VertexBuffer* m_vb;
    ConstantBuffer* m_cb;
    VertexShader* m_vs;
};

