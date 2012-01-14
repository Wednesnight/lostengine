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

#include <stdint.h>
#include "lost/types.h"

#define LOST_USE_EASTL
#ifndef LOST_USE_EASTL
  #include <string>
  #include <list>
  #include <map>
  #include <vector>
  namespace lost
  {
    using std::string;
    using std::basic_string;
    using std::list;
    using std::map;
    using std::vector;
    using std::make_pair;
    using std::pair;
  }
#else
  #include "EASTL/string.h"
  #include "EASTL/list.h"
  #include "EASTL/map.h"
  #include "EASTL/vector.h"
  namespace lost
  {
    using eastl::string;
    using eastl::basic_string;
    using eastl::list;
    using eastl::map;
    using eastl::vector;
    using eastl::make_pair;
    using eastl::pair;
  }
#include "lost/common/eastlStreamSupport.h"  

#endif
#include "lost/common/StringStream.h"
#define THROW_RTE(s) \
{ \
lost::common::StringStream os; \
os << s; \
throw std::runtime_error(os.str().c_str()); \
}


  // lostengine
  #include "lost/platform/shared_ptr.h"
  //#include "lost/gl/gl.h"
  #include "lost/platform/Platform.h"
#endif

#endif
