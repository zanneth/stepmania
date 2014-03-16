#include "Globals.h"

#include "archutils/Common/EGLHelper.h"

android_app* AndroidGlobals::ANDROID_APP_INSTANCE = NULL;
char* AndroidGlobals::commandArguments[1];
bool AndroidGlobals::InitDone = false;

void AndroidGlobals::SetAppInstance(android_app* state) {
    ANDROID_APP_INSTANCE = state;
    commandArguments[0] = new char[strlen(ANDROID_APP_INSTANCE->activity->externalDataPath)+1];
    strcpy(commandArguments[0], ANDROID_APP_INSTANCE->activity->externalDataPath);
}

void AndroidGlobals::HandleCommand(struct android_app* app, int32_t IE) {
    switch( IE )
    {
    case APP_CMD_SAVE_STATE:
        break;
    case APP_CMD_INIT_WINDOW:
        // The window is being shown, get it ready.
        if( app->window != NULL ) {
            EGLHelper::EGLWindowContext = app->window;
            AndroidGlobals::InitDone = true;
        }
        break;
    case APP_CMD_TERM_WINDOW:
        // The window is being hidden or closed. Clean.
        break;
    case APP_CMD_STOP:
        // Activity::FullStop
        break;
    case APP_CMD_GAINED_FOCUS:
        // Restart from where we were
        break;
    case APP_CMD_LOST_FOCUS:
        // PAUSE
        break;
    case APP_CMD_LOW_MEMORY:
        // Can we free up some shite?
        break;
    }
}

int32_t AndroidGlobals::HandleInput(struct android_app* app, AInputEvent* IE) { /*nop*/ }

void AndroidGlobals::PollEventLoop() {
    int ident, fdesc, events;
    struct android_poll_source* source;
    while((ident = ALooper_pollAll(0, &fdesc, &events, (void**)&source)) >= 0) {
        // process this event
        if (source)
            source->process(ANDROID_APP_INSTANCE, source);
    }
}

void AndroidGlobals::InitializeApp(int (*Launch)(int, char**), int argc, char* argv[]) {
    //ANDROID_APP_INSTANCE->&IEC; // First Event Core: the Init EC.
    ANDROID_APP_INSTANCE->onAppCmd = HandleCommand;
    ANDROID_APP_INSTANCE->onInputEvent = HandleInput;

    // Read all pending events.
    int id;
    int events;
    android_poll_source* source;

    while(!InitDone) {
        // While the App Init isn't done in itself, we loop here.
        while( (id = ALooper_pollAll( 0, NULL, &events,(void**) &source)) >= 0 )
        {
            // Process this event.
            if( source != NULL )
                source->process( ANDROID_APP_INSTANCE, source );
        }
    }

    ANDROID_APP_INSTANCE->onAppCmd = NULL;
    ANDROID_APP_INSTANCE->onInputEvent = NULL;

    // We actually have the staticref to the proper entry point. GO.
    Launch(argc, argv);
}

int AndroidGlobals::Audio::GetNativeSampleRate() {
    JNIEnv *jni;
    ANDROID_APP_INSTANCE->activity->vm->AttachCurrentThread(&jni, NULL);

    // Classret and calling
    jclass clazz = jni->GetObjectClass(ANDROID_APP_INSTANCE->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "getNativeSampleRate", "()I");
    int retval =
        jni->CallIntMethod(ANDROID_APP_INSTANCE->activity->clazz, methodID, NULL);

    ANDROID_APP_INSTANCE->activity->vm->DetachCurrentThread();
    return retval;
}

int AndroidGlobals::Audio::GetNativeFramesPerBuffer() {
    JNIEnv *jni;
    ANDROID_APP_INSTANCE->activity->vm->AttachCurrentThread(&jni, NULL);

    // Classret and calling
    jclass clazz = jni->GetObjectClass(ANDROID_APP_INSTANCE->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "getNativeFramesPerBuffer", "()I");
    int retval =
        jni->CallIntMethod(ANDROID_APP_INSTANCE->activity->clazz, methodID, NULL);

    ANDROID_APP_INSTANCE->activity->vm->DetachCurrentThread();
    return retval;
}

void AndroidGlobals::Crash::ForceCrash(const char* reason) {
    JNIEnv *jni;
    ANDROID_APP_INSTANCE->activity->vm->AttachCurrentThread(&jni, NULL);

    // Classret and calling
    jclass clazz = jni->GetObjectClass(ANDROID_APP_INSTANCE->activity->clazz);
    jmethodID methodID =
        jni->GetMethodID(clazz, "crash", "(Ljava/lang/String;)V");

    jni->CallVoidMethod(ANDROID_APP_INSTANCE->activity->clazz,
                        methodID,
                        jni->NewStringUTF(reason)
    );

    // Log the error, in case the detach happens out of sync and the tombstone doesn'T get generated
    Log_Error(reason);

    abort();
    abort();

    ANDROID_APP_INSTANCE->activity->vm->DetachCurrentThread();
}

RString AndroidGlobals::GetVideoDriverName() {
    return "SHIELD";
}

char** AndroidGlobals::GetDefaultCommandArguments() {
    return commandArguments;
}

void AndroidGlobals::AttachInputHandler(int32_t (*InputHandler)(android_app* pApplication, AInputEvent* pEvent)) {
    ANDROID_APP_INSTANCE->onInputEvent = InputHandler;
}

void AndroidGlobals::AttachCommandHandler(void (*CommandHandler)(android_app* app, int32_t cmd)) {
}


#define APPNAME "StepMania"
void AndroidGlobals::Log(android_LogPriority prio, RString string) {
    __android_log_write(prio, APPNAME, string.c_str());
}
void AndroidGlobals::Log(RString string) {
    Log(ANDROID_LOG_VERBOSE, string.c_str());
}
void AndroidGlobals::Log_Error(RString string) {
    Log(ANDROID_LOG_ERROR, string.c_str());
}
void AndroidGlobals::Log_Warn(RString string) {
    Log(ANDROID_LOG_WARN, string.c_str());
}
void AndroidGlobals::Log_Debug(RString string) {
    Log(ANDROID_LOG_DEBUG, string.c_str());
}
void AndroidGlobals::Log_Info(RString string) {
    Log(ANDROID_LOG_INFO, string.c_str());
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
