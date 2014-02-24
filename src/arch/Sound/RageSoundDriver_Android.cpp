#include "global.h"
#include "RageSoundDriver_Android.h"
#include "RageLog.h"
#include "PrefsManager.h"
#include "RageSound.h"
#include "RageSoundManager.h"
#include "RageUtil.h"
#include "RageTimer.h"
REGISTER_SOUND_DRIVER_CLASS2( Android, Android );

/**
 * Android Audio Layer
 *
 * .To realize an interface is to actually create it in memory, with all bells and whistles.
 *
 **/

RageSoundDriver_Android::RageSoundDriver_Android() {
    M_SAMPLERATE = AndroidGlobals::Audio::GetNativeSampleRate();
    M_PREF_CHKSIZE_FRAMES = AndroidGlobals::Audio::GetNativeFramesPerBuffer();
}
RageSoundDriver_Android::~RageSoundDriver_Android() {
    (*slesAndroidSimpleBufQueueItf)->Clear(slesAndroidSimpleBufQueueItf);
    (*slesOutputMixObj)->Destroy(slesOutputMixObj);
    (*slesPlayerObj)->Destroy(slesPlayerObj);
    (*slesCoreItf)->Destroy(slesCoreItf);
}

RString RageSoundDriver_Android::Init() {
    // Initialize audio cursor position.
    mCursorPosition = 0;

    /* Here, the slCreateEngine method, which is internal to SLES, is used to
     *  generate a core interface object required to access most of everything
     *  programmers will need to use OpenSLES
     *
     * Proper syntax:
     *  slCreateEngine(target, numOpts, EngineOpts, nbInterfaces, InterfaceIDs, InterfacesReq)
     *
     * TODO: PROTO NOTE: For the time being, we init with the basic "meh" set of restrictions.
     *
     **/
    switch( slCreateEngine(&slesCoreItf, 0, NULL, 0, NULL, NULL) ) {
        case SL_RESULT_SUCCESS: {
            /**
             * slesEngine is a SLObjectItf object.
             *  Resume, RegisterCallback, SetPriority, GetPriority,
             *  AbortAsyncOperation and SetLossOfControlInterfaces
             *  are not supported.
             *
             * Realize, GetState, GetInterface, Destroy are supported.
             *
             **/

            // Realize the Core
            // ARGS: self, SLboolean async [asynchronous init]
            if((*slesCoreItf)->Realize(slesCoreItf, SL_BOOLEAN_FALSE) != SL_RESULT_SUCCESS)
                return "RSD_Android :: Init failed";

            return CreateSLESInterfaces();
        }
        case SL_RESULT_PARAMETER_INVALID:
        case SL_RESULT_MEMORY_FAILURE:
        case SL_RESULT_FEATURE_UNSUPPORTED:
        case SL_RESULT_RESOURCE_ERROR:
            return "RSD_Android :: Init failed";
    }
    return "RSD_Android :: Init failed";
}


RString RageSoundDriver_Android::CreateSLESInterfaces() {

    /* Here, the GetInterface method, which is internal to the slesEngine, is,
     *  for OpenSLES, the utility method that gets the programmer the engine
     *  of her/his choice. To "ask" for an engine, one must invoke GetInterface
     *  with these:
     *   . self-reference to the Engine
     *   . Interface ID
     *    . In the header, the Interfaces are structs and normally preceeded by
     *       extern SLAPI const SLInterfaceID [[[[[]]]]]
     *       where the series of brackets represents the actual header.
     * Proper syntax:
     *  GetInterface(selfReference, ENGINEID, &TargetVariable)
     **/

    RString failure = "";
    switch((*slesCoreItf)->GetInterface(slesCoreItf, SL_IID_ENGINE, &slesEngineItf)){
        case SL_RESULT_SUCCESS: {
            SLresult rslt;

            // Engine doesn't have a Realize method.
            rslt = (*slesCoreItf)->GetInterface
                (slesCoreItf, SL_IID_ENGINE, &slesEngineItf);
            if(rslt != SL_RESULT_SUCCESS) {
                failure = "GetInterface SL_IID_ENGINE";
                break;
            }

            // Contain the playback objects into one method. Less clutter.
            if(!InitAudioPlayback()) {
                failure = "InitAudioPlayback";
                break;
            }

            rslt = (*slesCoreItf)->GetInterface
                (slesCoreItf, SL_IID_PLAY, &slesPlayerItf);
            if(rslt != SL_RESULT_SUCCESS) {
                failure = "GetInterface SL_IID_PLAY";
                break;
            }

            rslt = (*slesCoreItf)->GetInterface
                (slesCoreItf, SL_IID_ANDROIDSIMPLEBUFFERQUEUE, &slesAndroidSimpleBufQueueItf);
            if(rslt != SL_RESULT_SUCCESS) {
                failure = "GetInterface SL_IID_ANDROIDSIMPLEBUFFERQUEUE";
                break;
            }

            rslt = (*slesAndroidSimpleBufQueueItf)->RegisterCallback
                (slesAndroidSimpleBufQueueItf, BufferIsEmptyCB, this);
            if(rslt != SL_RESULT_SUCCESS) {
                failure = "RegisterCB Empty Buffer";
                break;
            }

            rslt = (*slesCoreItf)->GetInterface
                (slesCoreItf, SL_IID_ANDROIDCONFIGURATION, &slesAndroidConfItf);
            if(rslt != SL_RESULT_SUCCESS) {
                failure = "GetInterface SL_IID_ANDROIDCONFIGURATION";
                break;
            }

            return "";
        }
        case SL_RESULT_PARAMETER_INVALID:
        case SL_RESULT_FEATURE_UNSUPPORTED:
        case SL_RESULT_PRECONDITIONS_VIOLATED:
            return "RSD_Android :: Interface Creation failure";
    }
    return RString(RString("RSD_Android :: Interface Creation failure@")+RString(failure)).c_str();
}

bool RageSoundDriver_Android::InitAudioPlayback() {

    const SLInterfaceID ids_buffer_queue_slitfid[] = {SL_IID_ANDROIDBUFFERQUEUESOURCE};
    const SLInterfaceID ids_volume_only_slitfid[] = {SL_IID_VOLUME};
    const SLInterfaceID ids_no_interfaces_slitfid[] = {SL_IID_NULL};
    const SLboolean required_false_slbool[] = {SL_BOOLEAN_FALSE};
    const SLboolean required_true_slbool[] = {SL_BOOLEAN_TRUE};

    // Result Container
    SLresult rslt;

    // See Android documentation for OpenSLES details and interface supports.
    rslt = (*slesEngineItf)->CreateOutputMix(
        slesEngineItf,
        &slesOutputMixObj,
        0, // Only one interface. By default?
        ids_no_interfaces_slitfid,
        required_false_slbool // Volume interface not required; Array.
     );
    if(rslt != SL_RESULT_SUCCESS) return false;
    // Realize outputmix
    rslt = (*slesOutputMixObj)->Realize(slesOutputMixObj, SL_BOOLEAN_FALSE);
    if(rslt != SL_RESULT_SUCCESS) return false;

    SLDataLocator_AndroidBufferQueue localBufferQueue = {
        SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,
        2
    };

    SLDataFormat_PCM format_pcm = {
        SL_DATAFORMAT_PCM,
        M_CHANNELS_DEFAULT,
        M_SAMPLERATE,
        SL_PCMSAMPLEFORMAT_FIXED_16,
        SL_PCMSAMPLEFORMAT_FIXED_16,
        M_SPEAKERS,
        SL_BYTEORDER_LITTLEENDIAN
    };
    SLDataLocator_OutputMix outMixer = {SL_DATALOCATOR_OUTPUTMIX, slesOutputMixObj};
    SLDataSource audioSrc = {&localBufferQueue, &format_pcm};
    SLDataSink soundSink = {&outMixer, NULL};

    rslt =(*slesEngineItf)->CreateAudioPlayer(
        slesEngineItf,
        &slesPlayerObj,
        &audioSrc,
        &soundSink,
        1, // interface count
        ids_buffer_queue_slitfid,
        required_true_slbool
    );
    if(rslt != SL_RESULT_SUCCESS) return false;

    rslt = (*slesPlayerObj)->Realize(slesPlayerObj, SL_BOOLEAN_FALSE);
    if(rslt != SL_RESULT_SUCCESS) return false;

    return true;
}

int64_t RageSoundDriver_Android::GetPosition() const {
    // When we at?
    SLmillisecond retVal;
    if((*slesPlayerItf)->GetPosition(slesPlayerItf, &retVal) != SL_RESULT_SUCCESS) return -1;

    return retVal;
}

//void RageSoundDriver_Android::BufferIsEmptyCB(SLAndroidBufferQueueItf iface, void *context) {
void RageSoundDriver_Android::BufferIsEmptyCB(SLAndroidSimpleBufferQueueItf iface, void *context) {
    RageSoundDriver_Android *con = (RageSoundDriver_Android *)context;
    // READ BYTES INTO WHATEVER THE /redacted/; use bytes to know how many bytes to fetch.
	static int16_t *target = NULL;
	if(!target)
		target = new int16_t[con->M_PREF_CHKSIZE_FRAMES / sizeof(int16_t)];

    // Read into target
    con->Mix( target, con->M_PREF_CHKSIZE_FRAMES, con->mCursorPosition, con->GetPosition() );

    // Enqueue sound
    (*(con->slesAndroidSimpleBufQueueItf))->Enqueue(
        con->slesAndroidSimpleBufQueueItf,
        target,
        sizeof(target) // number of bytes to read
    );

    // ITERATE MCURSOR, DERPFACE.
}

int RageSoundDriver_Android::GetSampleRate() const {
    return M_SAMPLERATE;
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
