#include "global.h"
#include "CocoaEventDispatcher.h"

CocoaEventDispatcher CocoaEventDispatcher::sharedDispatcher;

CocoaEventDispatcher::CocoaEventDispatcher() : m_Mutex("Cocoa Event Dispatcher")
{}

void CocoaEventDispatcher::AddResponder( const function<void(EVENT_TYPE)> &resp )
{
    LockMut( m_Mutex );
    m_Responders.push_back( resp );
}

void CocoaEventDispatcher::DispatchEvent( EVENT_TYPE e )
{
    LockMut( m_Mutex );
    
    for( const auto &responder : m_Responders ) {
        responder( e );
    }
}
