#include "EGLProvider_Android.h"

#include "archutils/Android/Globals.h"

// EGLhalp
#include "archutils/Common/EGLHelper.h"

EGLProvider_Android::EGLProvider_Android()
{
    // Get NativeWindow from Android itself; set into the EGLhalp
    EGLHelper::EGLWindowContext = AndroidGlobals::ANDROID_APP_INSTANCE->window;

    EGLint sizeArray[] = { EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                          EGL_ALPHA_SIZE, 8,
                          EGL_RED_SIZE, 8,
                          EGL_GREEN_SIZE, 8,
                          EGL_BLUE_SIZE, 8,
                          EGL_NONE
                          };
    attrsInit = (EGLint*)malloc(sizeof(sizeArray));
    attrsInit = sizeArray;
}

EGLProvider_Android::~EGLProvider_Android()
{
    free(attrsInit);
}

EGLint* EGLProvider_Android::GetAttibutesInitConfig()
{
    return attrsInit;
}

bool EGLProvider_Android::GetWasWindowedValue()
{
    return false;
}

void EGLProvider_Android::Log(RString string)
{
    AndroidGlobals::Log(string);
}

void EGLProvider_Android::GetDisplayResolutions(DisplayResolutions &out) const
{
    DisplayResolution res = {
                              ANativeWindow_getWidth(AndroidGlobals::ANDROID_APP_INSTANCE->window),
                              ANativeWindow_getHeight(AndroidGlobals::ANDROID_APP_INSTANCE->window),
                              true
                            };
    out.insert( res );
}

void EGLProvider_Android::PreContextSetup()
{
    // Set config on the Android window.
    EGLint format;
    eglGetConfigAttrib(EGLHelper::EGLDisplayContext,
                       EGLHelper::EGLSelectedConf,
                       EGL_NATIVE_VISUAL_ID,
                       &format);

    ANativeWindow_setBuffersGeometry(EGLHelper::EGLWindowContext, 0, 0, format);
}


EGLRenderTargetProvider_Android::EGLRenderTargetProvider_Android()
{
    EGLint argsSize[] = { EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
                          EGL_ALPHA_SIZE, 8,
                          EGL_RED_SIZE, 8,
                          EGL_GREEN_SIZE, 8,
                          EGL_BLUE_SIZE, 8,
                          EGL_ALPHA_SIZE, 8,
                          EGL_DEPTH_SIZE, 16,
                          EGL_NONE
             //EGL_ALPHA_SIZE, pWithAlpha?8:EGL_DONT_CARE, //IF WE USE WITHALPHA EVER
             //EGL_DEPTH_SIZE, pWithDepthBuffer?16:EGL_DONT_CARE,  //IF WE USE WITHALPHA EVER
    };
    targetAttrs = (EGLint*)malloc(sizeof(argsSize));
    targetAttrs = argsSize;
}

EGLint* EGLRenderTargetProvider_Android::GetRenderTargetConfigAttribs
    (bool pWithAlpha, bool pWithDepthBuffer)
{
    return targetAttrs;
}

GLint EGLRenderTargetProvider_Android::GetInternalFormatInt(bool pWithAlpha)
{
    return (pWithAlpha? GL_RGBA8_OES:GL_RGB8_OES);
}

EGLRenderTargetProvider_Android::~EGLRenderTargetProvider_Android()
{
    free(targetAttrs);
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
