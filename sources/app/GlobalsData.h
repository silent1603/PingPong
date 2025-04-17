#ifndef GLOBALS_DATA_H
#define GLOBALS_DATA_H
#include "Config_Platforms.h"
#define APP_NAME pingpong
#define MAJOR_NUMBER 0
#define MINOR_NUMBER 0
#define PATCH_NUMBER 1
#define LETTER a


enum class AppPropertiesDefine : int
{
    FEATURES = 0,
    WINDOW_WIDTH,
    WINDOW_HEIGHT,
    SIZE
};

enum class AppFeaturesDefine : uint32_t
{
    NONE = 0,
    IS_RUNNING = 1 << 0,
    HEADLESS_APP = 1 << 1,
    FULLSCREEN = 1 << 2
};


struct AppInformations
{
    int properties[(int)AppPropertiesDefine::SIZE] = {0,DEFAULT_WINDOW_WIDTH,DEFAULT_WINDOW_HEIGHT};
#if PLATFORMS_WINDOWS
    HWND hwnd = 0;
    HDC hdc = 0;
    HINSTANCE hInstance = 0;
#elif PLATFORMS_LINUX
#endif

};

extern AppInformations appInformation;
#endif