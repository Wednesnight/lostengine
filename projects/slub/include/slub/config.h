/*
Copyright (c) 2011 Timo Boll, Tony Kostanjsek

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

#ifndef SLUB_CONFIG_H
#define SLUB_CONFIG_H

/*
 configurable string include/type
 */
#ifndef SLUB_STRING_INCLUDE
  #define SLUB_STRING_INCLUDE <string>
#endif

#ifndef SLUB_STRING_TYPE
  #define SLUB_STRING_TYPE std::string
#endif

/*
 configurable list include/type
 */
#ifndef SLUB_LIST_INCLUDE
  #define SLUB_LIST_INCLUDE <list>
#endif

#ifndef SLUB_LIST_TYPE
  #define SLUB_LIST_TYPE std::list
#endif

/*
 configurable map include/type
 */
#ifndef SLUB_MAP_INCLUDE
  #define SLUB_MAP_INCLUDE <map>
#endif

#ifndef SLUB_MAP_TYPE
  #define SLUB_MAP_TYPE std::map
#endif

/*
 you shouldn't change anything below this line
 */

#include SLUB_STRING_INCLUDE
#include SLUB_LIST_INCLUDE
#include SLUB_MAP_INCLUDE

namespace slub {

  typedef SLUB_STRING_TYPE string;

  template<typename T> class list : public SLUB_LIST_TYPE<T> {};

  template<typename K, typename V> class map : public SLUB_MAP_TYPE<K, V> {};

}

#endif
