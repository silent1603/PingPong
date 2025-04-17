#ifndef ERRORLOGGERWINIMPL_H
#define ERRORLOGGERWINIMPL_H
#include "Config_Platforms.h"
#if PLATFORMS_WINDOWS
inline void UIErrorPopup(char *msg)
{
    MessageBoxA(NULL, msg, "Error", MB_ICONERROR);
}
#endif
#endif