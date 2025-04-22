#ifndef RENDERERSYSTEM_H
#define RENDERERSYSTEM_H
#include "DX11Renderer.h"
enum class RendererBackend
{
    NONE = 0,
    OPENGL,
    OPENGLES,
    WEBGL,
    WEBGL2,
    WEBGPU,
    VULKAN,
    METAL,
    DIRECTX9,
    DIRECTX10,
    DIRECTX11,
    DIRECTX12,
    SIZE
};

void RenderSystem_Init(RendererBackend backendId)
{
    RendererInit[(int)backendId]();
}

void DummyRenderer_Init()
{
    
}

//map compile time to function call
void (*RendererInit[(int)RendererBackend::SIZE])() = {
    DummyRenderer_Init , // NONE
    DummyRenderer_Init , // OPENGL
    DummyRenderer_Init , //OPENGLES
    DummyRenderer_Init , //WEBGL 
    DummyRenderer_Init , //WEBGL2 
    DummyRenderer_Init , //WEBGPU
    DummyRenderer_Init , //VULKAN
    DummyRenderer_Init ,  //METAL
    DummyRenderer_Init ,  //DIRECTX9
    DummyRenderer_Init ,  //DIRECTX10
    DX11Renderer_Init ,  //DIRECTX11
    DummyRenderer_Init ,  //DIRECTX12
};


#endif