#include "stepstart.h"

// http://docs.oracle.com/javase/1.5.0/docs/guide/jni/spec/types.html#wp276

/*void SkelNative::CallAppMethod(const char* input) {
    // In this use case, input is basically a

    JNIEnv *jni;
    app->activity->vm->AttachCurrentThread(&jni, NULL);

    // Classret and calling
    jclass clazz = jni->GetObjectClass(app->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "CallAppMethod", "(Ljava/lang/String;)V");
    jni->CallVoidMethod(app->activity->clazz, methodID, jni->NewStringUTF(input));

    app->activity->vm->DetachCurrentThread();
    return;
}*/

void StepStart::launch() {
    // Initialize everything. Go. Go. We are GO.
}

/*ctor*/
StepStart::StepStart(android_app* initState) {
    app = initState;
}

void android_main(android_app* state) {
    app_dummy(); // always.

    // Get halp. Will need fixing...
    //ndk_helper::JNIHelper::GetInstance()->Init( state->activity, HALP );

    StepStart* stepmania = new StepStart(state);

    // Just call the method upstream and exit. This is a skeleton, not an app.
    //stepmania->CallAppMethod("derp");
    //stepmania->launch();
}