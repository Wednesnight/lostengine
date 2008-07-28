#ifndef LOST_MODEL_LOADER_PARSER_PARSER_H
#define LOST_MODEL_LOADER_PARSER_PARSER_H

#include <boost/shared_ptr.hpp>
#include "lost/model/Mesh.h"

namespace lost
{
  namespace model
  {
    namespace loader
    {
      namespace parser
      {
        
        struct Parser
        {
          virtual ~Parser() {}
          
          virtual std::string getExtension() = 0;
          virtual boost::shared_ptr<lost::model::Mesh> parseMesh(const std::string& inData) = 0;
        };
        
      }
    }
  }
}

#endif
