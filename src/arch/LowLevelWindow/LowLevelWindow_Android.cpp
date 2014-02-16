#include "LowLevelWindow_Android.h"

using namespace EGLHelper;

LowLevelWindow_Android::LowLevelWindow_Android() : LowLevelWindow_EGL::LowLevelWindow_EGL()
{
    m_bWasWindowed = false;

    // Get NativeWindow from Android itself.
    g_WindowContext = ANDROID_APP_INSTANCE->window;
}

EGLint LowLevelWindow_Android::GetAttibutesInitConfig()
{
    return { EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
             EGL_ALPHA_SIZE, 8
             EGL_RED_SIZE, 8,
             EGL_GREEN_SIZE, 8,
             EGL_BLUE_SIZE, 8,
             EGL_NONE
    };
}


void LowLevelWindow_Android::PreContextSetup()
{
    // Set config on the Android window.
    EGLint format;
    eglGetConfigAttrib(EGLDisplayContext, EGLSelectedConf, EGL_NATIVE_VISUAL_ID, &format);
    ANativeWindow_setBuffersGeometry(EGLWindowContext, 0, 0, format);
}


EGLConfig* LowLevelWindow_Android::GetEGLConfig()
{
    return EGLSelectedConf;
}
EGLSurface LowLevelWindow_Android::GetEGLSurface()
{
    return EGLSurface;
}
EGLDisplay* LowLevelWindow_Android::GetEGLDisplayContext()
{
    return EGLDisplayContext;
}
EGLNativeWindowType* LowLevelWindow_Android::GetEGLWindowContext()
{
    return EGLWindowContext;
}

class RenderTarget_Android : public RenderTarget_EGL
{
public:
    EGLint GetPBufferConfigAttribs();
    EGLint GetRTConfigAttribs(bool pWithAlpha, bool pWithDepthBuffer);
};

EGLint RenderTarget_Android::GetRTConfigAttribs(bool pWithAlpha, bool pWithDepthBuffer)
{
    return { EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
             EGL_ALPHA_SIZE, 8
             EGL_RED_SIZE, 8,
             EGL_GREEN_SIZE, 8,
             EGL_BLUE_SIZE, 8,
             EGL_ALPHA_SIZE, 8,
             EGL_DEPTH_SIZE, 16,
             EGL_NONE
             //EGL_ALPHA_SIZE, pWithAlpha?8:EGL_DONT_CARE, //IF WE USE WITHALPHA EVER
             //EGL_DEPTH_SIZE, pWithDepthBuffer?16:EGL_DONT_CARE,  //IF WE USE WITHALPHA EVER
    };
}

/*
 * (c) 2014 Renaud Lepage
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
