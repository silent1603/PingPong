#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H
#include "GlobalsData.h"

#if PLATFORMS_WINDOWS

#include <gl/GL.h>
#include <gl/GLU.h>
#include <opengl/glext.h>
#include <opengl/wglext.h>

#elif PLATFORMS_LINUX

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <GL/glxext.h>

#endif


#endif