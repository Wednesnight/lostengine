#ifndef LOST_MODEL_LOADER_DEFAULTMODELLOADER_H
#define LOST_MODEL_LOADER_DEFAULTMODELLOADER_H

#include <boost/shared_ptr.hpp>
#include "lost/model/loader/ModelLoader.h"
#include "lost/model/loader/parser/Parser.h"
#include "lost/model/loader/parser/ParserOBJ.h"

namespace lost
{
  namespace model
  {
    namespace loader
    {
      
      struct DefaultModelLoader : public ModelLoader
      {
      private:
        boost::shared_ptr<parser::ParserOBJ> parserObj;
      public:
        DefaultModelLoader(boost::shared_ptr<lost::resource::Loader> inLoader)
        : ModelLoader::ModelLoader(inLoader)
        {
          parserObj.reset(new parser::ParserOBJ());
          registerParser(parserObj);
        }
      };
      
    }
  }
}

#endif
