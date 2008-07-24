#ifndef LOST_MODEL_PARSER_H_
#define LOST_MODEL_PARSER_H_

#include <string>
#include "lost/model/Mesh.h"

namespace lost
{
  namespace model
  {
    namespace parser
    {    
        /** parses the textual data into
         * the supplied scene object.
         *
         * @return true if data was fully understood, false otherwise.
         */
      boost::shared_ptr<Mesh> parse(const std::string& data);
    }
  }
}

#endif
