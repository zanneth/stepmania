#ifndef COCOA_EVENT_DISPATCHER_H
#define COCOA_EVENT_DISPATCHER_H

#include <functional>
#include <vector>
#include "RageThreads.h"

#if __OBJC__
#   import <Cocoa/Cocoa.h>
#   define EVENT_TYPE NSEvent *
#else
#   define EVENT_TYPE void *
#endif

class CocoaEventDispatcher
{
public:
    static CocoaEventDispatcher sharedDispatcher;
    
    CocoaEventDispatcher();
    CocoaEventDispatcher( const CocoaEventDispatcher& ) = delete;
    CocoaEventDispatcher( CocoaEventDispatcher&& ) = delete;
    
    void AddResponder( const function<void(EVENT_TYPE)> &resp );
    void DispatchEvent( EVENT_TYPE e );
    
private:
    RageMutex m_Mutex;
    vector<function<void(EVENT_TYPE)>> m_Responders;
};

#endif // COCOA_EVENT_DISPATCHER_H
