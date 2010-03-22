#ifndef LOST_PREFIX_SHARED_H
#define LOST_PREFIX_SHARED_H

#ifdef __cplusplus

  #define BOOST_SP_NO_ATOMIC_ACCESS
  
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

  // stdint
  #if defined WIN32
    #include "lost/platform/stdint.h"
  #else
    #include <stdint.h>
  #endif

  // lostengine
  #include "lost/platform/shared_ptr.h"
  //#include "lost/gl/gl.h"

#endif

#endif
