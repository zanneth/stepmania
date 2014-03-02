#ifndef ANDROID_GLOBALS_H
#define ANDROID_GLOBALS_H

// Proper app glue.
#include <android_native_app_glue.h>
#include <android/log.h>

// Typedefs
#include "global.h"

namespace AndroidGlobals {
    // Android App Instance -- GLOBAL.
    extern android_app* ANDROID_APP_INSTANCE;

    namespace Audio {
        extern int GetNativeFramesPerBuffer();
        extern int GetNativeSampleRate();
    };

    namespace Crash {
        extern void ForceCrash(const char* reason);
    };

    // Hardcoding SHIELD; it's a direct testbench and I don't want to code some
    //  weird conditionals and the like.
    extern RString GetVideoDriverName();

    extern void PollEventLoop();

    extern void Log(android_LogPriority prio, RString string);
    extern void Log(RString string);
    extern void Log_Error(RString string);
    extern void Log_Warn(RString string);
    extern void Log_Debug(RString string);
    extern void Log_Info(RString string);

    extern void SetAppInstance(android_app* state);
    extern char** GetDefaultCommandArguments();
    extern char* commandArguments[1];

    extern void InitializeApp(int (*Launch)(int, char**), int argc, char* argv[]);
    extern void AttachInputHandler(int32_t (*InputHandler)(android_app* app, AInputEvent* ev));
    extern void AttachCommandHandler(void (*InputHandler)(android_app* app, int32_t cmd));

    extern bool InitDone;


    // Internal
    static void HandleCommand(struct android_app* app, int32_t cmd);
    static int32_t HandleInput(struct android_app* app, AInputEvent* IE);

    /*
    class EventCore {
    public:
        virtual void HandleCommand(struct android_app* app, int32_t cmd);
        virtual void HandleInput(struct android_app* app, AInputEvent* IE);
    protected:
        virtual EventCore(){};
        virtual ~EventCore(){};
    private:

    };

    class InitEventCore : public EventCore {
    public:
        void HandleCommand(struct android_app* app, int32_t cmd);
        void HandleInput(struct android_app* app, AInputEvent* IE);
    protected:
        InitEventCore();
        virtual ~InitEventCore(){};
    };*/
};

#endif //ANDROID_GLOBALS_H

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
