#ifndef EGL_PROVIDER_ANDROID_H
#define EGL_PROVIDER_ANDROID_H

#include "EGLProvider.h"

// EGLhalp
#include "archutils/Common/EGLHelper.h"

// Bring in GLES2
#include <GLES2/gl2.h>
#include <GLES2/gl2platform.h>
#include <GLES2/gl2ext.h>

class EGLProvider_Android : public EGLProvider
{
public:
    EGLProvider_Android();
    ~EGLProvider_Android();


    void SetAttibutesInitConfig(EGLint* &target);
    void PreContextSetup();
    bool GetWasWindowedValue();
    void Log(RString string);
    void GetDisplayResolutions(DisplayResolutions &out) const;
    void PrintDebug();
private:
	static EGLint attrsInit[];
};

class EGLRenderTargetProvider_Android : public EGLRenderTargetProvider
{
public:
    EGLRenderTargetProvider_Android();
    ~EGLRenderTargetProvider_Android();

    void SetRenderTargetConfigAttribs(bool pWithAlpha, bool pWithDepthBuffer, EGLint* &target);
    GLint GetInternalFormatInt(bool pWithAlpha);

private:
	static EGLint targetAttrs[];
};

#ifdef ARCH_EGL_PROVIDER
#error "More than one EGLProvider active!"
#endif
#define ARCH_EGL_PROVIDER EGLProvider_Android

#ifdef ARCH_EGLRENDER_PROVIDER
#error "More than one EGLRenderProvider active!"
#endif
#define ARCH_EGLRENDER_PROVIDER EGLRenderTargetProvider_Android

#endif // EGL_PROVIDER_ANDROID_H
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
