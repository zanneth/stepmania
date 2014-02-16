#include "LowLevelWindow_EGL.h"

// EGLhalp
#include "archutils/Common/EGLHelper.h"

// RageEng
#include "RageLog.h"
#include "RageException.h"

// Bring in EGL
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglplatform.h>

// Bring in GLES2.
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>

static EGLContext g_MainContext = NULL;

LowLevelWindow_EGL::LowLevelWindow_EGL()
{
    if(!EGLHelper::ObtainContext())
		RageException::Throw( "%s", "Failed to fetch EGL Context" );

    Initialize();
}

void LowLevelWindow_EGL::Initialize()
{
    // The EGL LLW has been designed using the Android LLW and trying to keep it abstracted.
    {
        // Do a lock on the current EGLconfig. We'll hardlock on 32bits for now, because whatev.
        EGLint attributes_initconf = GetAttibutesInitConfig();

        EGLint numberConfigs;
        EGLConfig* matchingConfigs;

        if (EGL_FALSE == eglChooseConfig(EGLDisplayContext, attributes_initconf, NULL, 0, &numberConfigs) ||
            (numberConfigs == 0)
           )
        {
            /* Hardcrashes! */
            if(numberConfigs == 0)
                RageException::Throw( "%s", "No EGL configurations available." );
            else
                RageException::Throw( "%s", "EGL just crashed. :(" );
        }

        matchingConfigs = (EGLConfig*)malloc( numberConfigs * sizeof(EGLConfig));

        if (EGL_FALSE == eglChooseConfig(EGLDisplayContext, attributes_initconf, matchingConfigs,
                                         numberConfigs, &numberConfigs))
        {
            /* Hardcrash, AGAIN. */
            RageException::Throw( "%s", "EGL config just crashed. :(" );
        }

        // Loop through all the configurations.
        for(int monitor = 0; monitor < numberConfigs; monitor++)
        {
            EGLBoolean success;
            EGLint red, green, blue;

            success =
             eglGetConfigAttrib (EGLDisplayContext, matchingConfigs[monitor], EGL_RED_SIZE, &red)
             &eglGetConfigAttrib(EGLDisplayContext, matchingConfigs[monitor], EGL_BLUE_SIZE, &blue)
             &eglGetConfigAttrib(EGLDisplayContext, matchingConfigs[monitor], EGL_GREEN_SIZE, &green);

            /* Check that no error occurred and the attributes match */
            if (( success == EGL_TRUE) && (red==8) && (green==8) && (blue==8) )
            {
                EGLSelectedConf = match[monitor];
                break;
            }
        }
        free(matchingConfigs);
    }

    if(EGLSelectedConf == NULL)
        RageException::Throw( "%s", "For some reason, we didn't get a config/crash before here." );

    PreContextSetup();

    // in theory, for this execution path and context, the Display and Surface contexts are
    // as of yet completely unpopulated, as we're directly being called from the CTOR layer.
    // Also, reloading to try video modes is, well, basically a huge NOEP.

    EGLSurfaceContext = eglCreateWindowSurface(GetEGLDisplayContext(),
                                               GetEGLConfig(),
                                               GetEGLWindowContext(),
                                               {EGL_NONE});

    // Create drawcontext.
    g_MainContext = eglCreateContext(GetEGLDisplayContext(),
                                     GetEGLConfig(),
                                     EGL_NO_CONTEXT,
                                     {EGL_NONE});

    // Context switchover!
    eglMakeCurrent(GetEGLDisplayContext(), GetEGLSurface(), GetEGLSurface(), g_MainContext);

}

LowLevelWindow_EGL::~LowLevelWindow_EGL()
{
    eglDestroySurface(EGLHelper::EGLDisplayContext, EGLHelper::EGLSurfaceContext);
    EGLHelper::EGLSurfaceContext = None;
}

// Modelled over the X11
void LowLevelWindow_EGL::SwapBuffers()
{
    eglSwapBuffers(EGLHelper::EGLDisplayContext, EGLHelper::EGLSurfaceContext);
}

void *LowLevelWindow_EGL::GetProcAddress( RString s )
{
    // see LowLevelWindow_X11 for derp statement.
	return (void*) eglGetProcAddress( (const GLubyte*) s.c_str() );
}

RString LowLevelWindow_EGL::TryVideoMode( const VideoModeParams &p, bool &bNewDeviceOut )
{
    // As I'm working on the Android port, this is empty because I'm overriding it to nop.
}


RenderTarget *LowLevelWindow_EGL::CreateRenderTarget()
{
	return new RenderTarget_EGL( this );
}

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

	// Downstream configuration fetching.
    virtual EGLint GetPBufferConfigAttribs();
	virtual EGLint GetRTConfigAttribs(bool pWithAlpha, bool pWithDepthBuffer);

private:
	int m_iWidth, m_iHeight;
	LowLevelWindow_EGL *m_pWind;
	EGLSurface m_iPbuffer;
	EGLContext m_pPbufferContext;
	unsigned int m_iTexHandle;

	EGLContext m_pOldContext;
	EGLSurface m_pOldSurface;
};

RenderTarget_EGL::RenderTarget_EGL( LowLevelWindow_EGL *pWind )
{
	m_pWind = pWind;
	m_iPbuffer = 0;
	m_pPbufferContext = NULL;
	m_iTexHandle = 0;
	m_pOldContext = NULL;
	m_pOldDrawable = 0;
}

RenderTarget_EGL::~RenderTarget_EGL()
{
	if( m_pPbufferContext )
		eglDestroyContext( GetEGLDisplayContext(), m_pPbufferContext );
	if( m_iPbuffer )
		eglDestroySurface( GetEGLDisplayContext(), m_iPbuffer );

    // GLES2 method. Legit.
	if( m_iTexHandle )
		glDeleteTextures( 1, reinterpret_cast<GLuint*>(&m_iTexHandle) );
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
