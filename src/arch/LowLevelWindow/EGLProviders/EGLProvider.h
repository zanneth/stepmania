#ifndef EGL_PROVIDER_H
#define EGL_PROVIDER_H

#include "global.h"

#include "DisplayResolutions.h"

// Force fwddec typedefs
typedef int GLint;
typedef int32_t EGLint;

class EGLProvider
{
public:
    static EGLProvider *Create();

    //virtual void
    virtual void SetAttibutesInitConfig(EGLint* &target){};
    virtual void PreContextSetup(){};
    virtual bool GetWasWindowedValue(){};
    virtual void Log(RString string){};
    virtual void GetDisplayResolutions(DisplayResolutions &out){};
    virtual void PrintDebug(){};
};

class EGLRenderTargetProvider
{
public:
    static EGLRenderTargetProvider *Create();

    virtual void SetRenderTargetConfigAttribs(bool pWithAlpha, bool pWithDepthBuffer, EGLint* &target){};
    virtual GLint GetInternalFormatInt(bool pWithAlpha){};
};

#endif // EGL_PROVIDER_H
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
