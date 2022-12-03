#pragma once
#include <d3d11.h>
#include "Prerequisites.h"
#include "RenderSystem.h"

class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine();
	bool init();
	bool release();
	RenderSystem* getRenderSystem();

public:
	// Singleton.
	static GraphicsEngine* get();

private:
	RenderSystem* m_render_system = nullptr;
};

