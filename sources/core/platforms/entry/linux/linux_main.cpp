#include "Config_Platforms.h"

#if PLATFORMS_LINUX
#include <assert.h>
#include <cstring>

static bool isExtensionSupported(const char *extList, const char *extension) {
	return strstr(extList, extension) != 0;
}

int main(int argc,char** argv)
{
    Display* dpy = XOpenDisplay(NULL);
    assert(dpy != NULL);
    if(dpy == NULL)
    {
        return 1;
    }
    Screen* sreen = DefaultScreenOfDisplay(dpy);
    int screenId = DefaultScreen(dpy);

    //Get glx version
    GLint majorGLX = 0 ;
    GLint minorGLX = 0;
    glXQueryVersion(dpy,&majorGLX,&minorGLX);
    if (majorGLX <= 1 && minorGLX < 2) {
		assert(0==1); //require GLX 1.2
		XCloseDisplay(dpy);
		return 1;
	}


    Window root = RootWindow(dpy,screenId);
    GLint glxAttritbute[] = 
    {
        GLX_X_RENDERABLE, True,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_RED_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_ALPHA_SIZE , 8,
        GLX_DEPTH_SIZE,24,
        GLX_STENCIL_SIZE, 8,
        GLX_DOUBLEBUFFER, True,
        None
    };

    int fbcount = 0;
    GLXFBConfig* fbc = glXChooseFBConfig(dpy,screenId,glxAttritbute,&fbcount);
    if(fbc == 0)
    {
        assert(fbc == 0);
        XCloseDisplay(dpy);
        return 1;
    }
    // Pick the FB config/visual with the most samples per pixel
	int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;
    for(int i = 0; i < fbcount; ++i)
    {
        XVisualInfo* vi = glXGetVisualFromFBConfig(dpy,fbc[i]);
        if(vi!=0)
        {
            int samp_buf = 0;
            int samples = 0;
            glXGetFBConfigAttrib(dpy,fbc[i],GLX_SAMPLE_BUFFERS,&samp_buf);
            glXGetFBConfigAttrib(dpy,fbc[i],GLX_SAMPLES,&samples);
            if(best_fbc < 0 || (samp_buf && samples > best_num_samp))
            {
                best_fbc = i;
                best_num_samp = samples;
            }
            if(worst_fbc < 0 || samp_buf == 0 || samples < worst_num_samp)
            {
                worst_fbc = i;
            }
            worst_num_samp =samples;
        }
        XFree(vi);
    }
    GLXFBConfig bestFbc = fbc[best_fbc];
    XFree(fbc);

    XVisualInfo* visual = glXGetVisualFromFBConfig(dpy,bestFbc);
    if(visual == 0)
    {
        assert(0==1); //can't find visual
        XCloseDisplay(dpy);
        return 1;
    }

    if(screenId != visual->screen)
    {
        assert(screenId == visual->screen);
        XCloseDisplay(dpy);
        return 1;
    }


    // Open the window
    XSetWindowAttributes windowAttribs;
    windowAttribs.border_pixel = BlackPixel(dpy, screenId);
    windowAttribs.background_pixel = WhitePixel(dpy, screenId);
	windowAttribs.override_redirect = True;
	windowAttribs.colormap = XCreateColormap(dpy,root, visual->visual, AllocNone);
	windowAttribs.event_mask = ExposureMask | KeyPressMask | KeyPressMask | PointerMotionMask | ButtonMotionMask;

    Window window = XCreateWindow(dpy,root,0,0,DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, 0, visual->depth, InputOutput, visual->visual, CWBackPixel | CWColormap | CWBorderPixel | CWEventMask, &windowAttribs);

    //Redirect Close whwen clich or x button
    Atom atomWmDeleteWindow = XInternAtom(dpy,"WM_DELETE_WINDOW",false);
    XSetWMProtocols(dpy,window,&atomWmDeleteWindow,1);
    //set title 
    XStoreName(dpy, window, "PingPong");
    //Create GLX OpenGL context
    PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB = 0;
    glXCreateContextAttribsARB = (PFNGLXCREATECONTEXTATTRIBSARBPROC) glXGetProcAddressARB((const GLubyte *)"glXCreateContextAttribsARB");
    int context_attribs[] = {
		GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
		GLX_CONTEXT_MINOR_VERSION_ARB, 3,
		GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
        GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
		None
	};
    
    GLXContext context = 0;
	const char *glxExts = glXQueryExtensionsString( dpy,  screenId );
	if (!isExtensionSupported( glxExts, "GLX_ARB_create_context")) {
		assert(0==1);// "GLX_ARB_create_context not supported\n";

		context = glXCreateNewContext( dpy, bestFbc, GLX_RGBA_TYPE, 0, True );
	}
	else {

		context = glXCreateContextAttribsARB( dpy, bestFbc, 0, true, context_attribs );
    }
    XSync(dpy,False);

    // Verifying that context is a direct context
	if (!glXIsDirect (dpy, context)) {
		assert(0==1); //"Indirect GLX rendering context obtained\n";
	}
	glXMakeCurrent(dpy, window, context);

    const GLubyte * renderer= glGetString(GL_RENDERER) ;
    const GLubyte * verion= glGetString(GL_VERSION) ;
    const GLubyte * shaing_language_version= glGetString(GL_SHADING_LANGUAGE_VERSION) ;

    //setup opengl
    glClearColor(0.5f, 0.6f, 0.7f, 1.0f);
	glViewport(0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

    //show the window
    XClearWindow(dpy,window);
    XMapRaised(dpy,window);

    XEvent ev;
    bool isShutdown = false;
    // game loop
    while(!isShutdown)
    {
        //get message block
        if(XPending(dpy) > 0)
        {
            XNextEvent(dpy,&ev);
            switch (ev.type)
            {
            case Expose:
                {
                    XWindowAttributes attribs;
                    XGetWindowAttributes(dpy,window,&attribs);
                    glViewport(0,0,attribs.width,attribs.height);
                }
                break;
            case ClientMessage:
                {
                    if(ev.xclient.data.l[0] == atomWmDeleteWindow)
                    {
                        isShutdown = true;
                    }
                }
                break;
            case DestroyNotify:
                {
                    isShutdown = true;
                }
                break;
            default:
                break;
            }
        }
        //update
        
        // render
        glClear(GL_COLOR_BUFFER_BIT);
        //resent frame
        glXSwapBuffers(dpy,window);
    }

    //Cleanup GLX
    glXDestroyContext(dpy,context);

    //Cleanup x11
    XFree(visual);
    XFreeColormap(dpy,windowAttribs.colormap);
    XDestroyWindow(dpy,window);
    XCloseDisplay(dpy);

    return 0;
}
#endif