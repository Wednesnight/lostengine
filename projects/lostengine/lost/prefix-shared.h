/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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
