#include "lost/resource/AbsoluteResourceRepository.h"
#include "lost/platform/Platform.h"
#include "lost/common/Logger.h"
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
      lost::shared_ptr<lost::resource::File> result(new lost::resource::File);
      result->location = absolutePath.string();
      
      std::ifstream ifs( absolutePath.string().c_str(), std::ios_base::in | std::ios_base::binary );
      
      if(!ifs.good()) { throw std::runtime_error( "couldn't open file: '"+ absolutePath.string() +"'" ); }
      
      ifs.seekg(0,std::ios_base::end);
      if(ifs.fail()) throw std::runtime_error("failed");
      unsigned long filesize = ifs.tellg();
      ifs.seekg(std::ios_base::beg);
      result->size = filesize;
      result->data.reset(new char[filesize]);
      ifs.read(result->data.get(), filesize);
      
      return result;
    }
  }
}
