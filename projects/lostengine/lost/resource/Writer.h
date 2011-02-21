#ifndef LOST_RESOURCE_WRITER_H
#define LOST_RESOURCE_WRITER_H

#include <vector>
#include <stdexcept>
#include "lost/common/forward.h"
#include "lost/resource/forward.h"
#include <boost/filesystem.hpp>

namespace lost
{
  namespace resource
  {
    struct WriterError : public std::runtime_error { WriterError(const std::string& error) : runtime_error(error) {} };

    struct Writer {
      Writer(const RepositoryPtr& repository);
      virtual ~Writer();

      static WriterPtr create(const RepositoryPtr& repository) { return WriterPtr(new Writer(repository)); }

      void write(const boost::filesystem::path& path, const common::DataPtr& data);

    private:
      RepositoryPtr repository;
    };

  }
}

#endif
