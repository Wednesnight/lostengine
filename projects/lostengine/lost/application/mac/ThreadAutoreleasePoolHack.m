#include <Foundation/NSAutoreleasePool.h>

void* threadAutoreleasePoolHack_createPool()
{
  return [[NSAutoreleasePool alloc] init];
}

void* threadAutoreleasePoolHack_drainAndRecreatePool(void* p)
{
  NSAutoreleasePool* pool = (NSAutoreleasePool*)p;
  [pool drain];
  return [[NSAutoreleasePool alloc] init];  
}

void threadAutoreleasePoolHack_drainPool(void* p)       
{
  NSAutoreleasePool* pool = (NSAutoreleasePool*)p;
  [pool drain];
}

