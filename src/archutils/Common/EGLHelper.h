#ifndef EGL_HELPER_H
#define EGL_HELPER_H

// Bring in EGL
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglplatform.h>

// Define this manually; it's defined in X11 and we want to keep it simple while reimplementing EGL
#define None 0L

namespace EGLHelper
{
    /**
     * \fn bool ObtainEGLContext()
     * \brief Fetch the available Embedded Graphics Library context.
     * \return Initialization status.
     **/
    bool ObtainContext();

    /**
     * \fn void TerminateContext();
     * \brief End current connection to the EGL context.
     **/
    void TerminateContext();

    /**
     * \var EGLDisplay *EGLDisplayContext
     * \brief EGL Display Context holder.
     **/
    extern EGLDisplay EGLDisplayContext;

    /**
     * \var EGLSurface EGLSurfaceContext
     * \brief EGL Surface Context holder.
     * \note Will probably/surely be renamed because the current name is completely stupid.
     **/
    extern EGLSurface EGLSurfaceContext;

    /**
     * \var EGLConfig* EGLSelectedConf
     * \brief EGL Surface Context holder.
     * \note Will probably/surely be renamed because the current name is completely stupid.
     **/
    extern EGLConfig EGLSelectedConf;

    /**
     * \var EGLNativeWindowType* EGLWindowContext
     * \brief EGL Window Context holder.
     **/
    extern EGLNativeWindowType EGLWindowContext;

};

#endif // EGL_HELPER_H

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
