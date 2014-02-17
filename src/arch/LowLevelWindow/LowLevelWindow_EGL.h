/* LowLevelWindow_EGL - OpenGL GLES window driver. */

#ifndef LOW_LEVEL_WINDOW_EGL_H
#define LOW_LEVEL_WINDOW_EGL_H

#include "RageDisplay.h" // VideoModeParams
#include "LowLevelWindow.h"

// Because we must.
#include "RageDisplay_OGL_Helpers.h"

// Way of working for platform specifics and platform-dependent configurations
#include "EGLProviders/EGLProvider.h"

// Bring in EGL
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglplatform.h>

// Bring in GLES2.
#include <GLES2/gl2.h>
#include <GLES2/gl2platform.h>
#include <GLES2/gl2ext.h>

class LowLevelWindow_EGL : public LowLevelWindow
{
public:
	LowLevelWindow_EGL();
	~LowLevelWindow_EGL();

    // Apparently not absolutely needed for this platform
	void *GetProcAddress(RString s); // damn.

    // Return NOPE if the video mode doesn't correspond to the current one.
    RString TryVideoMode(const VideoModeParams &p, bool &bNewDeviceOut);

    // Defined as =false upstream
	//bool IsSoftwareRenderer( RString &sError );

	//
	void SwapBuffers();

	void LogDebugInformation() const;

	const VideoModeParams &GetActualVideoModeParams() const { return CurrentParams; }

	void GetDisplayResolutions( DisplayResolutions &out ) const;

	bool SupportsRenderToTexture() const;

	// Downstreams will need to implement this. The core object will define it though.
	virtual RenderTarget *CreateRenderTarget();

	virtual bool SupportsThreadedRendering();
	virtual void BeginConcurrentRenderingMainThread();
	virtual void EndConcurrentRenderingMainThread();
	virtual void BeginConcurrentRendering();
	virtual void EndConcurrentRendering();

private:

    /**
     * \var eglProvider
     * \brief EGL platform specifics.
     **/
    EGLProvider* eglProvider;

	bool m_bWasWindowed;
	VideoModeParams CurrentParams;
};


/**
 * \class RenderTarget_EGL
 * \brief EGL implementation of RenderTarget
 **/
class RenderTarget_EGL : public RenderTarget
{
public:
    RenderTarget_EGL(LowLevelWindow_EGL *pWind);
    ~RenderTarget_EGL();

	void Create ( const RenderTargetParam &param, int &iTextureWidthOut, int &iTextureHeightOut );
	unsigned GetTexture() const { return m_iTexHandle; }
	void StartRenderingTo();
	void FinishRenderingTo();

	// Copying from the Pbuffer to the texture flips Y.
	virtual bool InvertY() const { return true; } // \todo review for EGL/droids

	// Configuration fetching.
    EGLint* GetAsPBufferConfigAttribs(int pWidth, int pHeight);

private:
	int m_iWidth, m_iHeight;
	LowLevelWindow_EGL *m_pWind;

	EGLSurface m_iPbuffer;
	EGLContext m_pPbufferContext;
	unsigned int m_iTexHandle;

	EGLContext m_pOldContext;
	EGLSurface m_pOldSurface;

	EGLint* pbufferAttributes;

	EGLRenderTargetProvider* eglRTP;

};

#ifdef ARCH_LOW_LEVEL_WINDOW
#error "More than one LowLevelWindow selected!"
#endif
#define ARCH_LOW_LEVEL_WINDOW LowLevelWindow_EGL


#endif // LOW_LEVEL_WINDOW_EGL_H
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
