#include "lost/resource/Writer.h"
#include "lost/resource/Repository.h"
#include "lost/common/Logger.h"
#include "lost/common/Data.h"
#include <stdexcept>

namespace lost
{
  namespace resource
  {

    Writer::Writer(const RepositoryPtr& repository) : repository(repository) {}

    Writer::~Writer() {}

    void Writer::write(const boost::filesystem::path& path, const common::DataPtr& data)
    {
      repository->write(path, data);
    }

  }
}
