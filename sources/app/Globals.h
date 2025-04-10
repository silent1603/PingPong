#ifndef GLOBALS_H
#define GLOBALS_H
#include "Config_Platforms.h"
#define APP_NAME pingpong
#define MAJOR_NUMBER 0
#define MINOR_NUMBER 0
#define PATCH_NUMBER 1
#define LETTER a


enum class AppPropertiesDefine : int
{
    FEATURES = 0,
    OPENGL_FEATURES,
    VULKAN_FEATURES,
    DIRECT_X_FEATURES,
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

enum class AppOpenGLFeaturesDefine : uint32_t
{
    NONE = 0,
    ENABLE = 1 << 0,
    USE_EXTENSION = 1 << 1
};



int appProperties[(int)AppPropertiesDefine::SIZE] =  {
    0,
    (int)AppOpenGLFeaturesDefine::ENABLE ,//| (int)AppOpenGLFeaturesDefine::USE_EXTENSION,
    0,
    0,
    DEFAULT_WINDOW_WIDTH,
    DEFAULT_WINDOW_HEIGHT
};




#endif