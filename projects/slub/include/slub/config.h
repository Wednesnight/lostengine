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
 you shouldn't change anything below this line
 */

#include SLUB_STRING_INCLUDE

namespace slub {
  typedef SLUB_STRING_TYPE string;
}

#endif
