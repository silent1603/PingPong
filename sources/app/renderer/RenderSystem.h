#ifndef RENDERERSYSTEM_H
#define RENDERERSYSTEM_H
#include "DX11Renderer.h"
enum class RendererBackend
{
    NONE = 0,
    OPENGL,
    OPENGLES,
    VULKAN,
    METAL,
    DIRECTX11,
    DIRECTX10,
    DIRECTX12,
    SIZE
};

void RenderSystem_Init(RendererBackend backendId)
{
    RendererInit[(int)backendId]();
}

void (*RendererInit[(int)RendererBackend::SIZE])() = {NULL};


#endif