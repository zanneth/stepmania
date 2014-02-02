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
 * Audio stuff
 *
 * .To realize an interface is to actually create it in memory, with all bells and whistles.
 *
 **/

RString RageSoundDriver_Android::Init() {
    // Need to init the SLES stack.
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
                return "Init failed";

            return CreateSLESInterfaces();
        }
        case SL_RESULT_PARAMETER_INVALID:
        case SL_RESULT_MEMORY_FAILURE:
        case SL_RESULT_FEATURE_UNSUPPORTED:
        case SL_RESULT_RESOURCE_ERROR:
            return "Init failed";
    }
    return "Init failed";
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

    switch((*slesCoreItf)->GetInterface(slesCoreItf, SL_IID_ENGINE, &slesEngineItf)){
        case SL_RESULT_SUCCESS: {
            SLresult rslt;

            // Engine doesn't have a Realize method.
            rslt = (*slesCoreItf)->GetInterface
                (slesCoreItf, SL_IID_ENGINE, &slesEngineItf);
            if(rslt != SL_RESULT_SUCCESS) break;

            // Contain the playback objects into one method. Less clutter.
            if(!InitAudioPlayback()) break;

            rslt = (*slesCoreItf)->GetInterface
                (slesCoreItf, SL_IID_PLAY, &slesPlayerItf);
            if(rslt != SL_RESULT_SUCCESS) break;

            rslt = (*slesCoreItf)->GetInterface
                (slesCoreItf, SL_IID_ANDROIDBUFFERQUEUESOURCE, &slesAndroidBufQueueItf);
            if(rslt != SL_RESULT_SUCCESS) break;

            rslt = (*slesAndroidBufQueueItf)->RegisterCallback
                (slesAndroidBufQueueItf, BufferIsEmptyCB, );
            if(rslt != SL_RESULT_SUCCESS) break;

            rslt = (*slesCoreItf)->GetInterface
                (slesCoreItf, SL_IID_ANDROIDCONFIGURATION, &slesAndroidConfItf);
            if(rslt != SL_RESULT_SUCCESS) break;

            return "";
        }
        case SL_RESULT_PARAMETER_INVALID:
        case SL_RESULT_FEATURE_UNSUPPORTED:
        case SL_RESULT_PRECONDITIONS_VIOLATED:
            return "Init failure";
    }
    return "Init failure";
}

bool RageSoundDriver_Android::InitAudioPlayback() {

    const SLInterfaceID ids_volume_only_slitfid[] = {SL_IID_VOLUME};
    const SLboolean required_false_slbool[] = {SL_BOOLEAN_FALSE};

    SLresult rslt;
    rslt = (*slesEngineItf)->CreateOutputMix(
        slesEngineItf,
        &slesOutputMixObj,
        1, // Only one interface. By default?
        ids_volume_only_slitfid, // Connect to Volume interfaces?; Array
        required_false_slbool // Volume interface not required; Array.
     );
    if(rslt != SL_RESULT_SUCCESS) return false;
    // Realize outputmix
    rslt = (*slesOutputMixObj)->Realize(slesOutputMixObj, SL_BOOLEAN_FALSE);
    if(rslt != SL_RESULT_SUCCESS) return false;

    SLDataLocator_AndroidBufferQueue localBufferQueue = {
        SL_DATALOCATOR_ANDROIDBUFFERQUEUE,
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
    SLDataSource audioSrc = {&localBufferQueue, &format_pcm};
    SLDataLocator_OutputMix outMixer = {SL_DATALOCATOR_OUTPUTMIX, slesOutputMixObj};
    SLDataSink soundSink = {&outMixer, NULL};
    const SLInterfaceID ids_bufqueue[] = {SL_IID_ANDROIDBUFFERQUEUESOURCE};
    const SLboolean reqtrue[] = {SL_BOOLEAN_TRUE};

    rslt =(*slesEngineItf)->CreateAudioPlayer(
        slesEngineItf,
        &slesPlayerObj,
        &audioSrc,
        &soundSink,
        1, // interface count
        ids_bufqueue,
        reqtrue
    );
    if(rslt != SL_RESULT_SUCCESS) return false;

    rslt = (*slesPlayerObj)->Realize(slesPlayerObj, SL_BOOLEAN_FALSE);
    if(rslt != SL_RESULT_SUCCESS) return false;

    return true;
}

bool RageSoundDriver_Android::GetPosition() {
    // When we at?
}
bool RageSoundDriver_Android::GetPlayLatency() {
    // Where we at?
}

/* Returns the number of frames processed */
bool RageSoundDriver_Android::GetData()
{
    // getting the data to be played.
	//this->Mix( buf, frames_to_fill, play_pos, cur_play_pos );

	// Two options to play buffer: enqueue standard, and the androidy variant.

	return true;
}
void RageSoundDriver_Android::BufferIsEmptyCB(SLAndroidBufferQueueItf iface, void* context) {
    RageSoundDriver_Android *con = (RageSoundDriver_Android *)context;
    int bytes = (con->M_OUTBUFFER_SAMPLES)*sizeof(short);
    // READ BYTES INTO WHATEVER THE /redacted/; use bytes to know how many bytes to fetch.
    (*con->slesAndroidBufQueueItf)->Enqueue(
        con->slesAndroidBufQueueItf,
        /* DATA BUFFER */,
        bytes // number of bytes to read
    );
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
