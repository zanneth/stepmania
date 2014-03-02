#include "InputHandler_Android_Gamepad.h"

// Bring in an Android global variable / accessor
//#include <archutils/Android/Globals.h>
#include <Foreach.h>
#include <InputFilter.h>

REGISTER_INPUT_HANDLER_CLASS2( AndroidGamepad, Android_Gamepad );

InputHandler_Android_Gamepad::InputHandler_Android_Gamepad() {
    // Set everything up from here.
    appInstance = AndroidGlobals::ANDROID_APP_INSTANCE;

    // Keep a pointer to this object/context at all times.
    // We will be using this in a static context.
    appInstance->userData = this; // \todo : this is obviously bad. Do an interobj in Gameloop

    AndroidGlobals::AttachInputHandler(&InputEvent);

}

int32_t InputHandler_Android_Gamepad::InputEvent(android_app* pApplication, AInputEvent* pEvent) {
    InputHandler_Android_Gamepad& lHandler = *(InputHandler_Android_Gamepad*)pApplication->userData;
    return lHandler.consume(pEvent);
}

// TODO: BAD. BAD. BAD. Correct this by using the proper Android APIs.
void InputHandler_Android_Gamepad::GetDevicesAndDescriptions( vector<InputDeviceInfo>& vDevicesOut ) {
    vDevicesOut.push_back(InputDeviceInfo(SoleInput, "Android Hardcoded Gamepad"));
}

int32_t InputHandler_Android_Gamepad::consume(AInputEvent* pEvent) {
    int32_t lEventType = AInputEvent_getType(pEvent);
    switch(lEventType) {
        case AINPUT_EVENT_TYPE_KEY: {return onKeyEvent(pEvent);}
        case AINPUT_EVENT_TYPE_MOTION: {return onTouchEvent(pEvent);}
    }
    return INT32T_RET_FAILURE;
}

int32_t InputHandler_Android_Gamepad::onKeyEvent(AInputEvent* pEvent) {

	FOREACH_CONST( DeviceInput, generateDevInputPack(pEvent), element )
		INPUTFILTER->ButtonPressed( *element );

}

vector<DeviceInput> InputHandler_Android_Gamepad::generateDevInputPack(InputDevice inDev,
                                                                       AInputEvent* pEvent) {
    DeviceButton output;
    int action = AKeyEvent_getAction(pEvent);
    int repeats = 1;
    if(action == 2) {
        repeats = AKeyEvent_getRepeatCount(pEvent);
        action = 0;
    }

    // Following Boolean procedure
    action = !action; // 1=>0, 0=>1, where 1 is "actionned", or "pressed"

    vector<DeviceInput> ret;

    for(;repeats > 0; repeats--)
        ret.push_back(DeviceInput(inDev, output, (float)action));


    return ret;
}

// Unimplemented.
int32_t InputHandler_Android_Gamepad::onTouchEvent(AInputEvent* pEvent) {
    return INT32T_RET_FAILURE;
}

InputHandler_Android_Gamepad::~InputHandler_Android_Gamepad() {
}

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
