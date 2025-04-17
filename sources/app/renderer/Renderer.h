#ifndef RENDERER_H
#define RENDERER_H

struct RendererProprerties
{
    float screen[2] = {0,0}; // depth , near
    bool vsync = false;
    int m_videoCardMemory = 0;
    char m_videoCardDescription[128];
};
#endif