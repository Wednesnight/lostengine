#ifndef LOST_OBJ_PARSER_H_
#define LOST_OBJ_PARSER_H_

#include <string>
#include "lost/obj/Scene.h"

namespace lost
{
  namespace obj
  {
    namespace parser
    {    
        /** parses the textual data into
         * the supplied scene object.
         *
         * @return true if data was fully understood, false otherwise.
         */
        bool parse(const std::string& data, obj::Scene& newscene);
    }
  }
}

#endif
