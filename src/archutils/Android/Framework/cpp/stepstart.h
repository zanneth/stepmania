#ifndef STEPMANIA_NATIVE_ACTIVITY_H
#define STEPMANIA_NATIVE_ACTIVITY_H
/**
 * Stepmania Bootstrapper.
 **/

// JNI inclusions
#include <jni.h>
#include <errno.h>
#include <vector>

// NDK stuff
#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/native_window_jni.h>

// Direct helper inclusion because the make import seems to be doing that.
#include "NDKHelper.h"

// Here will be the Stepmania inclusion that brings all hell.

/******************************************************
 * Helper Class for stuff like advanced JNI functions *
 *  to get Android-provisioned paths and such         *
 ******************************************************/
#define HALP "com/sample/helper/NDKHelper"
#define CLASS_NAME "com/stepmania/android/StepStart"

struct android_app;
class StepStart {

    android_app* app;

public:
    StepStart(android_app* initState);

    void launch(); // launch the game!

protected:

private:
};

extern StepStart* ANDROID_ENTRY;

#endif // STEPMANIA_NATIVE_ACTIVITY_H
/*
 * (c) 2014-x Renaud Lepage
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
