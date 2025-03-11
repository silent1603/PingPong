#ifndef CONFIG_PLATFORMS
#define CONFIG_PLATFORMS
#include "Config_App.h"

#define PLATFORMS_LINUX 0
#define PLATFORMS_WINDOWS 1
#define PLATFORMS_WEBASM 0

#if HEADLESS_APP
#else
#if PLATFORMS_LINUX
#define USE_X11 1
#define USE_WAYLAND 0
#define USE_OPENGL 1
#define USE_VuLKAN 0
#define USE_OPENGLES 0

#if USE_X11
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/Xcursor/Xcursor.h>
#endif
#if USE_OPENGL
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <GL/glxext.h>
#endif

#endif

#if PLATFORMS_WINDOWS
#include <Windows.h>
#if USE_OPENGL
#include <gl/GL.h>
#endif
#endif

#endif

#endif