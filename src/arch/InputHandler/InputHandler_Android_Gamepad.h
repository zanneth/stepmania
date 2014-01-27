#ifndef INPUT_HANDLER_ANDROID_GAMEPAD_H
#define INPUT_HANDLER_ANDROID_GAMEPAD_H

#include "InputHandler.h"
#include "RageThreads.h"

// Android NDK inclusions
#include <android/input.h>
#include <android_native_app_glue.h>

// STD
#include <vector>

class InputHandler_Android_Gamepad: public InputHandler
{
public:
    // Let's posit that we're using only one user for now.
    //enum { NUM_JOYSTICKS = 4 };
    InputHandler_Android_Gamepad();
    ~InputHandler_Android_Gamepad();
    bool DevicesChanged() { return m_bDevicesChanged; }
    void GetDevicesAndDescriptions( vector<InputDeviceInfo>& vDevicesOut );

private:

#define INT32T_RET_SUCCESS 1
#define INT32T_RET_FAILURE 0

    bool m_bShutdown;
    bool m_bDevicesChanged;
    android_app* appInstance;

    // for now, hardcoded. This is the debugging phase
    static const InputDevice SoleInput = DEVICE_JOY1;

    // Android event handlers
    int32_t consume(AInputEvent* pEvent);
    int32_t onKeyEvent(AInputEvent* pEvent);
    int32_t onTouchEvent(AInputEvent* pEvent);

    // Utilities
    vector<DeviceInput> generateDevInputPack(InputDevice inDev, AInputEvent* pEvent);

    // Inlined for now.
    vector<DeviceInput> generateDevInputPack(AInputEvent* pEvent) {
        return generateDevInputPack(SoleInput, pEvent);
    }
    static const vector<InputDeviceInfo> deviceInfoForced;

    // True Callback Methods
    static int32_t InputEvent(android_app* pApplication, AInputEvent* pEvent);
    //static int32_t TouchscreenEvent(android_app* pApplication, AInputEvent* pEvent);

};

#endif //INPUT_HANDLER_ANDROID_GAMEPAD_H

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
