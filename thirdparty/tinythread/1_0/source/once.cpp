#include "once.h"

#if ((defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ >= MAC_OS_X_VERSION_10_5) || (defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= __IPHONE_2_0))
#  include <libkern/OSAtomic.h>
#elif (defined(_WIN32_WINNT) && _WIN32_WINNT >= _WIN32_WINNT_WIN2K) || (defined(WINVER) && WINVER >= _WIN32_WINNT_WIN2K)
#  include <windows.h>
#endif

namespace tthread {

  bool __atomic_cas(once_flag old_value,
                    once_flag new_value,
                    volatile once_flag& ref)
  {
    
#if ((defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ >= MAC_OS_X_VERSION_10_5) || (defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= __IPHONE_2_0))
    
    return OSAtomicCompareAndSwapInt(old_value,
                                     new_value,
                                     &ref);
    
#elif (defined(_WIN32_WINNT) && _WIN32_WINNT >= _WIN32_WINNT_WIN2K) || (defined(WINVER) && WINVER >= _WIN32_WINNT_WIN2K)
    
    return InterlockedCompareExchange(&ref,
                                      new_value,
                                      old_value);
    
#elif (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__) > 40100
    
    return __sync_val_compare_and_swap(&ref,
                                       old_value,
                                       new_value);
    
#else
    
#  error No implementation
    
#endif
    
  }

}
