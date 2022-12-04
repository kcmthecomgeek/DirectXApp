#pragma once
#include <memory>

// Class signatures.
class SwapChain;
class DeviceContext;
class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;
class VertexShader;
class PixelShader;
class Resource;
class ResourceManager;
class Texture;
class TextureManager;
class RenderSystem;
class GraphicsEngine;

typedef std::shared_ptr<SwapChain> SwapChainPtr;
typedef std::shared_ptr<DeviceContext> DeviceContextPtr;
typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;
typedef std::shared_ptr<IndexBuffer> IndexBufferPtr;
typedef std::shared_ptr<ConstantBuffer> ConstantBufferPtr;
typedef std::shared_ptr<VertexShader> VertexShaderPtr;
typedef std::shared_ptr<PixelShader> PixelShaderPtr;
typedef std::shared_ptr<Resource> ResourcePtr;
typedef std::shared_ptr<ResourceManager> ResourceManagerPtr;
typedef std::shared_ptr<Texture> TexturePtr;
typedef std::shared_ptr<TextureManager> TextureManagerPtr;
typedef std::shared_ptr<RenderSystem> RenderSystemPtr;
typedef std::shared_ptr<GraphicsEngine> GraphicsEnginePtr;