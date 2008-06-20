#ifndef LOST_RESOURCE_LOCALREPOSITORY_H
#define LOST_RESOURCE_LOCALREPOSITORY_H

#include <fstream>
#include <stdexcept>
#include "lost/resource/Repository.h"
#include "lost/platform/Platform.h"
#include "lost/common/Logger.h"
#include <boost/filesystem.hpp>

namespace lost
{
  namespace resource
  {

    struct LocalRepository : public Repository
    {
      boost::filesystem::path root;

      // FIXME: this is only of limited use if the local repository is not named.
      //        we should allow multiple local repositories. Therefore, the properties
      //        should contain the name of the repository.
      //        i.e. lost.resource.localrepository.zombie.root = C:\zombiethegame\data
      //             lost.resource.localrepository.werewolfmodforzombiethegame.root = C:\werewolfdata
      // ONCE WE NEED IT, NOT EARLIER! 
      //
      LocalRepository( boost::shared_ptr<common::Config> inConfig )
        : Repository::Repository(inConfig),
          root(config->get<std::string>( "lost.resource.localrepository.root", platform::getResourcePath() ))
      {
      }

      virtual boost::shared_ptr<lost::resource::File> load(const std::string& inPath)
      {
        boost::shared_ptr<lost::resource::File> result(new lost::resource::File());
                
        // FIXME: more error checking
        std::string path = (root / inPath).string();
        DOUT("trying to load file: " << path);
        result->location = path; // FIXME: what's better to store here? absolute or relative/incoming path

        std::ifstream ifs( path.c_str(), std::ios_base::in | std::ios_base::binary );

        if(!ifs.good()) { throw std::runtime_error( "couldn't open file: '"+ path +"'" ); }

        ifs.seekg(0,std::ios_base::end);
        if(ifs.fail()) throw std::runtime_error("failed");
        unsigned long filesize = ifs.tellg();
        ifs.seekg(std::ios_base::beg);
        result->size = filesize;
        result->data.reset(new char[filesize]);
        ifs.read(result->data.get(), filesize);

        return result;
      }
      
    };

  }
}

#endif
