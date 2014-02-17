#include "LowLevelWindow_EGL.h"

// EGLhalp
#include "archutils/Common/EGLHelper.h"

// RageEng
#include "RageLog.h"
#include "RageException.h"

static EGLContext g_MainContext = NULL;
static EGLint EGLempty[] = {EGL_NONE};

// Initial implementation lacks anything background because of its focus on Android

LowLevelWindow_EGL::LowLevelWindow_EGL()
{
    eglProvider = EGLProvider::Create();

    m_bWasWindowed = eglProvider->GetWasWindowedValue();

    if(!EGLHelper::ObtainContext())
		RageException::Throw( "%s", "Failed to fetch EGL Context" );

    // The EGL LLW has been designed using the Android LLW and trying to keep it abstracted.
    {
        // Do a lock on the current EGLconfig. We'll hardlock on 32bits for now, because whatev.
        EGLint* attributes_initconf = eglProvider->GetAttibutesInitConfig();

        EGLint numberConfigs;
        EGLConfig* matchingConfigs;

        if (EGL_FALSE == eglChooseConfig(EGLHelper::EGLDisplayContext,
                                         attributes_initconf,
                                         NULL, 0, &numberConfigs)
                                        || (numberConfigs == 0) )
        {
            /* Hardcrashes! */
            if(numberConfigs == 0)
                RageException::Throw( "%s", "No EGL configurations available." );
            else
                RageException::Throw( "%s", "EGL just crashed. :(" );
        }

        matchingConfigs = (EGLConfig*)malloc( numberConfigs * sizeof(EGLConfig));

        if (EGL_FALSE == eglChooseConfig(EGLHelper::EGLDisplayContext, attributes_initconf,
                                         matchingConfigs, numberConfigs, &numberConfigs))
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
                eglGetConfigAttrib(EGLHelper::EGLDisplayContext,
                                    matchingConfigs[monitor], EGL_RED_SIZE, &red)
                &eglGetConfigAttrib(EGLHelper::EGLDisplayContext,
                                    matchingConfigs[monitor], EGL_BLUE_SIZE, &blue)
                &eglGetConfigAttrib(EGLHelper::EGLDisplayContext,
                                    matchingConfigs[monitor], EGL_GREEN_SIZE, &green);

            /* Check that no error occurred and the attributes match */
            if (( success == EGL_TRUE) && (red==8) && (green==8) && (blue==8) )
            {
                EGLHelper::EGLSelectedConf = matchingConfigs[monitor];
                break;
            }
        }
        // Clean up after ourselves.
        free(matchingConfigs);
    }

    if(EGLHelper::EGLSelectedConf == NULL)
        RageException::Throw( "%s", "For some reason, we didn't get a config/crash before here." );

    // As this class is, originally, mostly/purely virtual, we call downstream.
    eglProvider->PreContextSetup();

    // in theory, for this execution path and context, the Display and Surface contexts are
    // as of yet completely unpopulated, as we're directly being called from the CTOR layer.
    // Also, reloading to try video modes is, well, basically a huge NOEP.

    EGLHelper::EGLSurfaceContext =
        eglCreateWindowSurface(EGLHelper::EGLDisplayContext,
                               EGLHelper::EGLSelectedConf,
                               EGLHelper::EGLWindowContext,
                               EGLempty);

    // Create drawcontext.
    g_MainContext = eglCreateContext(EGLHelper::EGLDisplayContext,
                                     EGLHelper::EGLSelectedConf,
                                     EGL_NO_CONTEXT,
                                     EGLempty);

    // Context switchover!
    eglMakeCurrent(EGLHelper::EGLDisplayContext, EGLHelper::EGLSurfaceContext,
                   EGLHelper::EGLSurfaceContext, g_MainContext);

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
	return (void*) eglGetProcAddress( s.c_str() );
}

RString LowLevelWindow_EGL::TryVideoMode( const VideoModeParams &p, bool &bNewDeviceOut )
{
    // As I'm working on the Android port, this is empty because I'm overriding it to nop.
}

void LowLevelWindow_EGL::LogDebugInformation() const {
    eglProvider->Log("EGL::LogDebugInformation::Stub");
}

// MOVED: RenderTarget_EGL to the header. Downstream needs it.

RenderTarget_EGL::RenderTarget_EGL( LowLevelWindow_EGL *pWind )
{
	m_pWind = pWind;
	m_iPbuffer = 0;
	m_pPbufferContext = NULL;
	m_iTexHandle = 0;
	m_pOldContext = NULL;
	m_pOldSurface = 0;

	eglRTP = EGLRenderTargetProvider::Create();
}

RenderTarget_EGL::~RenderTarget_EGL()
{
	if( m_pPbufferContext )
		eglDestroyContext( EGLHelper::EGLDisplayContext, m_pPbufferContext );
	if( m_iPbuffer )
		eglDestroySurface( EGLHelper::EGLDisplayContext, m_iPbuffer );

    // GLES2 method. Legit.
	if( m_iTexHandle )
		glDeleteTextures( 1, reinterpret_cast<GLuint*>(&m_iTexHandle) );

    free(pbufferAttributes);
}

void RenderTarget_EGL::Create( const RenderTargetParam &param, int &iTextureWidthOut,
                                                               int &iTextureHeightOut )
{
    // \todo : For now, hardset at false. Please review and fix.
    EGLint* RenderTargetAttribs = eglRTP->GetRenderTargetConfigAttribs(false, false);

    // \todo : THIS IS COMPLETELY WRONG AND HERE ONLY FOR COMPILATION.
    EGLint* PBufferAttribs = GetAsPBufferConfigAttribs(0,0);
    //EGLint* PBufferAttribs = GetAsPBufferConfigAttribs(param.iWidth,param.iHeight);

#if !defined(ANDROID_TEST)
#error You should REALLY NOT be here.
#endif
    EGLConfig* chosenConfigs = NULL;
    EGLint configCount = 0;
    if(EGL_FALSE == eglChooseConfig(EGLHelper::EGLDisplayContext, RenderTargetAttribs,
                                    NULL, 0, &configCount ))
    {
        RageException::Throw( "%s", "EGL::Create.1 just crashed. Horribly." );
    }
    // Allocate enough space for all the confs.
    chosenConfigs = (EGLConfig*)malloc(configCount*sizeof(EGLConfig));

    if(EGL_FALSE == eglChooseConfig(EGLHelper::EGLDisplayContext, RenderTargetAttribs,
                                    NULL, 0, &configCount ))
    {
        RageException::Throw( "%s", "EGL::Create.2 just crashed. Horribly." );
    }

    // Init to a nosurface for the sake of the loop
    m_iPbuffer = EGL_NO_SURFACE;
    // Loop through all the configurations.
    for(int monitor = 0; monitor < configCount; monitor++)
    {
        if(EGL_NO_SURFACE == (m_iPbuffer = eglCreatePbufferSurface(EGLHelper::EGLDisplayContext,
                                                                   chosenConfigs[monitor],
                                                                   PBufferAttribs)))
            continue; // Still no surface.

        // Set up some contexts; we seem to have reached a proper point.
        m_pPbufferContext = eglCreateContext(EGLHelper::EGLDisplayContext,
                                             chosenConfigs[monitor],
                                             g_MainContext,
                                             EGLempty);

    }

    // Clean up before maybe throwing an exception.
    free(chosenConfigs);
    if(m_iPbuffer == EGL_NO_SURFACE)
    {
        RageException::Throw( "%s", "No compatible PBuffer configuration found. Aborting." );
    }
    if(m_pPbufferContext == EGL_NO_CONTEXT)
    {
        RageException::Throw( "%s", "No PBuffer context could be created. Aborting." );
    }

    // Direct from LLW_X11
	// allocate OpenGL texture resource
	glGenTextures( 1, reinterpret_cast<GLuint*>(&m_iTexHandle) );
	glBindTexture( GL_TEXTURE_2D, m_iTexHandle );
	LOG->Trace( "n %i, %ix%i", m_iTexHandle, param.iWidth, param.iHeight );
		while( glGetError() != GL_NO_ERROR )
		;
    int iTextureWidth = power_of_two( param.iWidth );
    int iTextureHeight = power_of_two( param.iHeight );
    iTextureWidthOut = iTextureWidth;
    iTextureHeightOut = iTextureHeight;

    glTexImage2D( GL_TEXTURE_2D, 0, eglRTP->GetInternalFormatInt(param.bWithAlpha),
			      iTextureWidth, iTextureHeight, 0, param.bWithAlpha? GL_RGBA:GL_RGB,
			      GL_UNSIGNED_BYTE, NULL );

	GLenum error = glGetError();
	ASSERT_M( error == GL_NO_ERROR, RageDisplay_Legacy_Helpers::GLToString(error) );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
}

void RenderTarget_EGL::StartRenderingTo()
{
	m_pOldContext = eglGetCurrentContext();
	m_pOldSurface = eglGetCurrentSurface(EGL_DRAW);
	eglMakeCurrent( EGLHelper::EGLDisplayContext, m_iPbuffer, m_iPbuffer, m_pPbufferContext );

	glViewport( 0, 0, m_iWidth, m_iHeight );
}

void RenderTarget_EGL::FinishRenderingTo()
{
	glFlush();

	glBindTexture( GL_TEXTURE_2D, m_iTexHandle );

		while( glGetError() != GL_NO_ERROR )
		;

	glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 0, 0, m_iWidth, m_iHeight );

	GLenum error = glGetError();
	ASSERT_M( error == GL_NO_ERROR, RageDisplay_Legacy_Helpers::GLToString(error) );

	glBindTexture( GL_TEXTURE_2D, 0 );

	eglMakeCurrent( EGLHelper::EGLDisplayContext, m_pOldSurface, m_pOldSurface, m_pOldContext );
	m_pOldContext = NULL;
	m_pOldSurface = 0;
}

// \todo THIS METHOD IS BAD. BAD. BAD.
EGLint* RenderTarget_EGL::GetAsPBufferConfigAttribs(int pWidth, int pHeight)
{
    // EGL works differently from GLX in that there is a Pbuffer-specific CreateContext
    EGLint attrTemp[] =
    {
        EGL_WIDTH, pWidth,
        EGL_HEIGHT, pHeight,
        EGL_NONE
    };

    pbufferAttributes = (EGLint *)malloc(sizeof(attrTemp));
    pbufferAttributes = attrTemp;

    return pbufferAttributes;
}

bool LowLevelWindow_EGL::SupportsRenderToTexture() const
{
    EGLint quickDetect[] = {EGL_SURFACE_TYPE, EGL_PBUFFER_BIT, EGL_NONE};

    // Quick test to see if pbuffer is supported.
    if(EGL_FALSE == eglChooseConfig(EGLHelper::EGLDisplayContext,
                                    quickDetect,
                                    NULL,
                                    0,
                                    NULL
                                   ))
        return false;

	return true;
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
