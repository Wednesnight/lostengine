#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include "lost/resource/DefaultLoader.h"
#include "lost/resource/ApplicationResourceRepository.h"

namespace lost
{
namespace resource
{
DefaultLoader::DefaultLoader()
{
  addRepository(boost::shared_ptr<Repository>(new ApplicationResourceRepository));
}

DefaultLoader::~DefaultLoader() {}
}
}
