#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H
#include "InputData.h"

inline void KeyDown(unsigned int input)
{
    m_keys[input] = true;
}

inline void KeyUp(unsigned int input)
{
    m_keys[input] = false;
}

inline bool IsKeyDown(unsigned int key)
{
    return m_keys[key];
}

#endif