#ifndef RAGE_SOUND_DRIVER_ANDROID_H
#define RAGE_SOUND_DRIVER_ANDROID_H

#include "RageSoundDriver.h"
#include "RageThreads.h"

// Include Android NDK stuff
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

// Local android abstractions
#include <archutils/Android/Globals.h>

// Go with OpenSL ES. Seems to be the dominant, better choice for lower-latency stuff.
/**
 * \class RageSoundDriver_Android
 * \brief This is the Rage audio engine bridge to use OpenSLES on Android.
 */
class RageSoundDriver_Android : public RageSoundDriver
{
public:
	RageSoundDriver_Android();
	~RageSoundDriver_Android();
	RString Init();

	/* virtuals: */
	int64_t GetPosition() const;
	// float GetPlayLatency() const; // let's think we're always right, m'kay?
    int GetSampleRate() const;
private:
    /**
     * SLES Interface creator. Initializes the remaining interfaces and realizations once
     *  the Software knows that the components should start init'ing properly.
     *
     * \return An empty string if everything worked out well. Something weird if not.
     */
    RString CreateSLESInterfaces();
    /**
     * Utility method chained by CreateSLESInterfaces to simplify the variety of engine inits
     *  due to non-"GetInterface" calls that will truly initialize some background components.
     *
     * \return False if something failed.
     * \see CreateSLESInterfaces()
     **/
    bool InitAudioPlayback();

    // SLES Buffer Callback
    /**
     * SLES-compliant "gimme data" callback that gets triggered when SLES runs out of content
     *  to play.
     *
     * \param iface Android Buffer Queue interface to queue up more data.
     * \param context Execution context [normally this object's instance]. This is a static method.
     **/
    //static void BufferIsEmptyCB(SLAndroidBufferQueueItf iface, void *context);
    static void BufferIsEmptyCB(SLAndroidSimpleBufferQueueItf iface, void *context);

    // SLES management interfaces
    SLObjectItf slesCoreItf;
    SLEngineItf slesEngineItf;
    SLPlayItf slesPlayerItf;

    // SLES+Android management interfaces
    SLAndroidConfigurationItf slesAndroidPlayerConfItf;
    SLAndroidSimpleBufferQueueItf slesAndroidSimpleBufQueueItf;

    SLresult SimpleEnqueue(SLAndroidSimpleBufferQueueItf self,
                           const void *pBuffer, SLuint32 size);

    // Container Objects
    SLObjectItf slesOutputMixObj; // Can't be a SLOutputMixItf directly? Wat
    SLObjectItf slesPlayerObj;

    // SLES Data Locators, Formats and general sources.
    SLDataSink audioSnk;

    // Where we are
    int mCursorPosition;


    static const int M_CHANNELS_DEFAULT = 2;
    static const int M_SPEAKERS_MASK = (M_CHANNELS_DEFAULT >1
        ? (SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT)
        : SL_SPEAKER_FRONT_CENTER
    );
	//static const int M_SAMPLERATE = 44100;
	static const int M_OUTBUFFER_SAMPLES = 8*M_CHANNELS_DEFAULT; // Channels * byte //16b per frame

    // Frames per buffer. From JNI. Ask once, don't call Java again.
    int M_PREF_CHKSIZE_FRAMES;

    // Native sample rate. Get once, don't ask Java again.
    SLuint32 M_SAMPLERATE;

	// Let us define the max chunk number at 8; chunksize is 1024 so that makes a 8kb writeahead.
	//static const int M_PREF_MAXCHUNKNB = 8;
	//static const int M_PREF_CHUNKSIZE = 1024;
	//static const int M_PREF_WRITEAHEAD = (M_PREF_MAXCHUNKNB*M_PREF_CHUNKSIZE); // chunk write-ahead



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
