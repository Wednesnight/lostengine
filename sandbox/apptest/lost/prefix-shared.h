#ifndef LOST_PREFIX_SHARED_H
#define LOST_PREFIX_SHARED_H

#ifdef __cplusplus

  #define BOOST_SP_NO_ATOMIC_ACCESS
#ifdef ANDROID
  #define BOOST_FILESYSTEM_VERSION 2
#else
  #define BOOST_FILESYSTEM_VERSION 3
#endif
  #define BOOST_SYSTEM_NO_DEPRECATED
  // boost
  #ifndef __STDC_CONSTANT_MACROS
    #define __STDC_CONSTANT_MACROS
  #endif
  #ifndef BOOST_ALL_NO_LIB
    #define BOOST_ALL_NO_LIB
  #endif
  #ifndef BOOST_THREAD_BUILD_LIB
    #define BOOST_THREAD_BUILD_LIB
  #endif


  // lostengine
  #include "EASTL/string.h"
  #include "lost/platform/shared_ptr.h"
  #include "lost/platform/Platform.h"
#endif

#endif
