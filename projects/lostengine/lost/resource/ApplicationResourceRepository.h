#ifndef LOST_RESOURCE_APPLICATIONRESOURCEREPOSITORY_H
#define LOST_RESOURCE_APPLICATIONRESOURCEREPOSITORY_H

#include "lost/resource/FilesystemRepository.h"

namespace lost
{
namespace resource
{

struct ApplicationResourceRepository : public FilesystemRepository
{
  ApplicationResourceRepository();
};
}
}

#endif
