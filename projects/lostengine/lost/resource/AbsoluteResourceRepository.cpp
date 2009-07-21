#include "lost/resource/AbsoluteResourceRepository.h"
#include "lost/platform/Platform.h"
#include "lost/common/Logger.h"
#include "lost/resource/Helper.h"
#include <fstream>

namespace lost
{
  namespace resource
  {
    AbsoluteResourceRepository::AbsoluteResourceRepository()
    {
    }
    
    AbsoluteResourceRepository::~AbsoluteResourceRepository() {}
    
    lost::shared_ptr<lost::resource::File> AbsoluteResourceRepository::load( const boost::filesystem::path& absolutePath)
    {
      FilePtr result = loadFromAbsolutePath(absolutePath.string());
      result->location = absolutePath.string();
            
      return result;
    }
  }
}
