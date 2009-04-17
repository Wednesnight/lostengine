#ifndef LOST_APPLICATION_MAC_THREADAUTORELEASEPOOLHACK_H
#define LOST_APPLICATION_MAC_THREADAUTORELEASEPOOLHACK_H

extern "C"
{
#if defined __APPLE__
void threadAutoreleasePoolHack_createPool();
void threadAutoreleasePoolHack_drainAndRecreatePool();
void threadAutoreleasePoolHack_drainPool();        
#else
// only mac/iphone need the autoreleasepool hack, all others get empty implementations
static inline void threadAutoreleasePoolHack_createPool() {};
static inline void threadAutoreleasePoolHack_drainAndRecreatePool() {};
static inline void threadAutoreleasePoolHack_drainPool() {};        
#endif
}
#endif