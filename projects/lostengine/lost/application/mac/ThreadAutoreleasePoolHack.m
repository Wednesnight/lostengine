#include <Foundation/NSAutoreleasePool.h>

static NSAutoreleasePool* pool = NULL;

void threadAutoreleasePoolHack_createPool()
{
  pool = [[NSAutoreleasePool alloc] init];
}

void threadAutoreleasePoolHack_drainAndRecreatePool()
{
  [pool drain];
  pool = [[NSAutoreleasePool alloc] init];  
}

void threadAutoreleasePoolHack_drainPool()       
{
  [pool drain];
}

