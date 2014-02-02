#ifndef RAGE_SOUND_DRIVER_ANDROID_H
#define RAGE_SOUND_DRIVER_ANDROID_H

#include "RageSoundDriver.h"
#include "RageThreads.h"

// Include Android NDK stuff
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

// Go with OpenSL ES. Seems to be the dominant, better choice for lower-latency stuff.

class RageSoundDriver_Android : public RageSoundDriver
{
public:
	RageSoundDriver_Android();
	~RageSoundDriver_Android();
	RString Init();

	/* virtuals: */
	int64_t GetPosition() const;
	float GetPlayLatency() const;
	int GetSampleRate() const { return M_SAMPLERATE; }
protected:
    //put decode above normal
	void SetupDecodingThread();

private:
    // Initializers
    RString CreateSLESInterfaces();
    bool InitAudioPlayback();

    // SLES Buffer Callback
    static void BufferIsEmptyCB(SLAndroidBufferQueueItf iface, void* context);

    // SLES management interfaces
    SLObjectItf slesCoreItf;
    SLEngineItf slesEngineItf;
    SLPlayItf slesPlayerItf;

    // SLES+Android management interfaces
    SLAndroidConfigurationItf slesAndroidConfItf;
    SLAndroidBufferQueueItf slesAndroidBufQueueItf;

    // Container Objects
    SLObjectItf slesOutputMixObj; // Can't be a SLOutputMixItf directly? Wat
    SLObjectItf slesPlayerObj;

    // SLES Data Locators, Formats and general sources.
    SLDataSink audioSnk;


    static const int M_CHANNELS_DEFAULT = 2;
    static const int M_SPEAKERS = (M_CHANNELS_DEFAULT >1
        ? (SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT)
        : SL_SPEAKER_FRONT_CENTER
    );
	static const int M_SAMPLERATE = 44100;
	static const int M_OUTBUFFER_SAMPLES = 16;
	static const int preferred_writeahead = 8192;
	static const int preferred_chunksize = 1024;

};
#endif //RAGE_SOUND_DRIVER_ANDROID_H
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
