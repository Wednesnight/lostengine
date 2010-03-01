#ifndef LOST_APPLICATION_THREADAUTORELEASEPOOLHACK_H
#define LOST_APPLICATION_THREADAUTORELEASEPOOLHACK_H

extern "C"
{
#if defined __APPLE__
void* threadAutoreleasePoolHack_createPool();
void* threadAutoreleasePoolHack_drainAndRecreatePool(void* p);
void threadAutoreleasePoolHack_drainPool(void* p);        
#else
// only mac/iphone need the autoreleasepool hack, all others get empty implementations
static inline void* threadAutoreleasePoolHack_createPool() { return 0;};
static inline void* threadAutoreleasePoolHack_drainAndRecreatePool(void* p) {return 0;};
static inline void threadAutoreleasePoolHack_drainPool(void* p) {};        
#endif
}
#endif