#include "EGLHelper.h"

// Default init.
EGLDisplay EGLHelper::EGLDisplayContext = NULL;
EGLSurface EGLHelper::EGLSurfaceContext = None;
EGLConfig  EGLHelper::EGLSelectedConf = NULL;
EGLNativeWindowType EGLHelper::EGLWindowContext = NULL;

bool EGLHelper::ObtainContext() {
    /**
     * Technically, EGL_DEFAULT_DISPLAY is ((EGLNativeDisplayType)0).
     * Nonetheless, use the pre-defined variable.
     *
     * In the future, if we're inside this very context,
     * we can use eglGetCurrentDisplay
     **/
    EGLint major, minor;
    EGLDisplayContext = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    if(EGLDisplayContext == NULL || EGLDisplayContext == EGL_NO_DISPLAY)
        return false;

    // Yep. Init after getting the display
    if(!eglInitialize(EGLDisplayContext, &major, &minor))
        return false;

    // Set UI Error handlers up here. If at all.

    return true;
}

void EGLHelper::TerminateContext() {

    // Liberate.
    eglTerminate(EGLDisplayContext);
    EGLDisplayContext = NULL;
//    EGLWindowContext = NULL;
    //free(EGLSelectedConf);
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
