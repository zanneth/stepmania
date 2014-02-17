#include "Globals.h"

#include <global.h>
#include <android/log.h>

android_app* AndroidGlobals::ANDROID_APP_INSTANCE = NULL;

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
        jni->GetMethodID(clazz, "getNativeFramesPerBuffer", "(Ljava/lang/String;)V");

    jni->CallIntMethod
        (ANDROID_APP_INSTANCE->activity->clazz, methodID, reason);

    ANDROID_APP_INSTANCE->activity->vm->DetachCurrentThread();
}

RString AndroidGlobals::GetVideoDriverName() {
    return "SHIELD";
}

#define APPNAME "StepMania"
void AndroidGlobals::Log(RString string) {
    __android_log_write(ANDROID_LOG_VERBOSE, APPNAME, string.c_str());
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
