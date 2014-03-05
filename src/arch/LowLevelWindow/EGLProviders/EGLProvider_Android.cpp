#include "EGLProvider_Android.h"

#include "archutils/Android/Globals.h"


// Logger
#include "RageLog.h"

EGLint EGLProvider_Android::attrsInit[] = {
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
    EGL_RED_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_BLUE_SIZE, 8,
    EGL_ALPHA_SIZE, 8,
    EGL_NONE
};

EGLint EGLRenderTargetProvider_Android::targetAttrs[] = {
    EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
    EGL_ALPHA_SIZE, 8,
    EGL_RED_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_BLUE_SIZE, 8,
    EGL_ALPHA_SIZE, 8,  // hardset
    EGL_DEPTH_SIZE, 16, // hardset
    EGL_NONE
    //EGL_ALPHA_SIZE, pWithAlpha?8:EGL_DONT_CARE, //IF WE USE WITHALPHA EVER
    //EGL_DEPTH_SIZE, pWithDepthBuffer?16:EGL_DONT_CARE,  //IF WE USE WITHALPHA EVER
};

EGLProvider_Android::EGLProvider_Android() {}

EGLProvider_Android::~EGLProvider_Android(){}

void EGLProvider_Android::PrintDebug()
{
    LOG->Trace("WindowContext ptr :: %d", EGLHelper::EGLWindowContext);
    LOG->Trace("WindowContext drct :: %d", AndroidGlobals::ANDROID_APP_INSTANCE->window);
    LOG->Trace("SelectedConf :: %d", EGLHelper::EGLSelectedConf);
}

void EGLProvider_Android::PreContextSetup()
{
    // Set config on the Android window.
    EGLint format;
    eglGetConfigAttrib(EGLHelper::EGLDisplayContext,
                       EGLHelper::EGLSelectedConf,
                       EGL_NATIVE_VISUAL_ID,
                       &format);

    PrintDebug();
    LOG->Trace("Format :: %d", format);
    ANativeWindow_setBuffersGeometry(
        //AndroidGlobals::ANDROID_APP_INSTANCE->window,
        EGLHelper::EGLWindowContext,
        0,
        0,
        format
    );
}

void EGLProvider_Android::SetAttibutesInitConfig(EGLint* &target)
{
    target = attrsInit;
}

bool EGLProvider_Android::GetWasWindowedValue()
{
    return false;
}

void EGLProvider_Android::Log(RString string)
{
    AndroidGlobals::Log(string);
}

void EGLProvider_Android::GetDisplayResolutions(DisplayResolutions &out)
{
    DisplayResolution res = {
          ANativeWindow_getWidth(AndroidGlobals::ANDROID_APP_INSTANCE->window),
          ANativeWindow_getHeight(AndroidGlobals::ANDROID_APP_INSTANCE->window),
          true
    };
    out.insert( res );
}


EGLRenderTargetProvider_Android::EGLRenderTargetProvider_Android(){}
EGLRenderTargetProvider_Android::~EGLRenderTargetProvider_Android(){}

void EGLRenderTargetProvider_Android::SetRenderTargetConfigAttribs
    (bool pWithAlpha, bool pWithDepthBuffer, EGLint* &target)
{
    target = targetAttrs;
}

GLint EGLRenderTargetProvider_Android::GetInternalFormatInt(bool pWithAlpha)
{
    return (pWithAlpha? GL_RGBA8_OES:GL_RGB8_OES);
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
