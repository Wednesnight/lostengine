#include <boost/filesystem.hpp>
#include "lost/resource/DefaultWriter.h"
#include "lost/resource/ApplicationResourceRepository.h"

namespace lost
{
  namespace resource
  {
    DefaultWriter::DefaultWriter()
    : Writer(lost::shared_ptr<Repository>(new ApplicationResourceRepository))
    {
    }

    DefaultWriter::~DefaultWriter() {}
  }
}
