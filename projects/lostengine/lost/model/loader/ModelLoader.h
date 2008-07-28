#ifndef LOST_MODEL_LOADER_MODELLOADER_H
#define LOST_MODEL_LOADER_MODELLOADER_H

#include <stdexcept>
#include <map>
#include <boost/shared_ptr.hpp>
#include "lost/model/loader/parser/Parser.h"

namespace lost
{
  namespace model
  {
    namespace loader
    {
      
      struct ModelLoader
      {
      private:
        boost::shared_ptr<lost::resource::Loader>                 loader;
        std::map<std::string, boost::shared_ptr<parser::Parser> > parser;

      public:
        ModelLoader(boost::shared_ptr<lost::resource::Loader> inLoader)
        : loader(inLoader)
        {
        }

        void registerParser(boost::shared_ptr<parser::Parser> inParser)
        {
          std::string extension = inParser->getExtension();
          transform(extension.begin(), extension.end(), extension.begin(), (int(*)(int))std::toupper);

          if (parser.find(extension) != parser.end())
            throw std::runtime_error("parser already registered: '"+ extension +"'");
          
          parser[extension] = inParser;
        }
        
        boost::shared_ptr<lost::model::Mesh> loadMesh(const std::string& inFilename)
        {
          boost::shared_ptr<lost::model::Mesh> result;

          std::string extension = inFilename.substr(inFilename.find_last_of('.')+1);
          transform(extension.begin(), extension.end(), extension.begin(), (int(*)(int))std::toupper);
          
          if (parser.find(extension) != parser.end())
          {
            result = parser[extension]->parseMesh(loader->load(inFilename)->str());
          }
          else
          {
            throw std::runtime_error("unknown parser: '"+ extension +"'");
          }
          
          return result;
        }
      };
      
    }
  }
}

#endif
