#ifndef RAGE_DISPLAY_GLES2_H
#define RAGE_DISPLAY_GLES2_H

// Because we must.
#include "RageDisplay.h"

class RageDisplay_GLES2: public RageDisplay
{
public:
	RageDisplay_GLES2();
	virtual ~RageDisplay_GLES2();
	virtual RString Init( const VideoModeParams &p, bool bAllowUnacceleratedRenderer );

	virtual RString GetApiDescription() const;
	virtual void GetDisplayResolutions( DisplayResolutions &out ) const;
	const RagePixelFormatDesc *GetPixelFormatDesc(RagePixelFormat pf) const;

	bool BeginFrame();
	void EndFrame();
	VideoModeParams GetActualVideoModeParams() const;
	void SetBlendMode( BlendMode mode );
	bool SupportsTextureFormat( RagePixelFormat pixfmt, bool realtime=false );
	bool SupportsPerVertexMatrixScale();
	unsigned CreateTexture( 
		RagePixelFormat pixfmt, 
		RageSurface* img,
		bool bGenerateMipMaps );
	void UpdateTexture( 
		unsigned iTexHandle, 
		RageSurface* img,
		int xoffset, int yoffset, int width, int height );
	void DeleteTexture( unsigned iTexHandle );
	void ClearAllTextures();
	int GetNumTextureUnits();
	void SetTexture( TextureUnit tu, unsigned iTexture );
	void SetTextureMode( TextureUnit tu, TextureMode tm );
	void SetTextureWrapping( TextureUnit tu, bool b );
	int GetMaxTextureSize() const;
	static bool SetTextureUnit(TextureUnit tu);
	void SetTextureFiltering( TextureUnit tu, bool b );
	bool IsEffectModeSupported( EffectMode effect );
	void SetEffectMode( EffectMode effect );
	bool IsZWriteEnabled() const;
	bool IsZTestEnabled() const;
	void SetZWrite( bool b );
	void SetZBias( float f );
	void SetZTestMode( ZTestMode mode );
	void ClearZBuffer();
	void SetCullMode( CullMode mode );
	void SetAlphaTest( bool b );
	void SetMaterial( 
		const RageColor &emissive,
		const RageColor &ambient,
		const RageColor &diffuse,
		const RageColor &specular,
		float shininess
		);
	void SetLighting( bool b );
	void SetLightOff( int index );
	void SetLightDirectional( 
		int index, 
		const RageColor &ambient, 
		const RageColor &diffuse, 
		const RageColor &specular, 
		const RageVector3 &dir );

	void SetSphereEnvironmentMapping( TextureUnit tu, bool b );
	void SetCelShaded( int stage );

	void SetLineWidth(float fWidth);
	void SetPolygonMode(PolygonMode pm);

	RageCompiledGeometry* CreateCompiledGeometry();
	void DeleteCompiledGeometry( RageCompiledGeometry* p );

	/* Create a render target, returning a texture handle. In addition to normal
	 * texture functions, this can be passed to SetRenderTarget. Delete with
	 * DeleteTexture. (UpdateTexture is not permitted.) Returns 0 if render-to-
	 * texture is unsupported.
	 */
	unsigned CreateRenderTarget (
	    const RenderTargetParam &, int &iTextureWidthOut, int &iTextureHeightOut );

	/* Set the render target, or 0 to resume rendering to the framebuffer. An active render
	 * target may not be used as a texture. If bPreserveTexture is true, the contents
	 * of the texture will be preserved from the previous call; otherwise, cleared.  If
	 * bPreserveTexture is true the first time a render target is used, behave as if
	 * bPreserveTexture was false.
	 */
	void SetRenderTarget( unsigned iHandle, bool bPreserveTexture = true );

protected:
    static void InvalidateObjects();
    void InitShaders();
	void DrawQuadsInternal( const RageSpriteVertex v[], int iNumVerts );
	void DrawQuadStripInternal( const RageSpriteVertex v[], int iNumVerts );
	void DrawFanInternal( const RageSpriteVertex v[], int iNumVerts );
	void DrawStripInternal( const RageSpriteVertex v[], int iNumVerts );
	void DrawTrianglesInternal( const RageSpriteVertex v[], int iNumVerts );
	void DrawCompiledGeometryInternal( const RageCompiledGeometry *p, int iMeshIndex );
	void DrawLineStripInternal( const RageSpriteVertex v[], int iNumVerts, float LineWidth );
	void DrawSymmetricQuadStripInternal( const RageSpriteVertex v[], int iNumVerts );
    void SendCurrentMatrices();

	RString TryVideoMode( const VideoModeParams &p, bool &bNewDeviceOut );
	RageSurface* CreateScreenshot();
	RageMatrix GetOrthoMatrix( float l, float r, float b, float t, float zn, float zf ); 
	bool SupportsSurfaceFormat( RagePixelFormat pixfmt );
	bool SupportsRenderToTexture() const;

	RagePixelFormat GetImgPixelFormat( RageSurface* &img, bool &FreeImg, int width, int height, bool bPalettedTexture );
};

#endif
/*
 * Copyright (c) 2012 Colby Klein
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

