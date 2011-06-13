#ifndef LOST_RESOURCE_HELPER_H
#define LOST_RESOURCE_HELPER_H

#include "lost/common/forward.h"

namespace lost
{
namespace resource
{

// reads file from path into new Data instance, leaves location member unset.
common::DataPtr loadFromAbsolutePath(const char* path);

// checks if file exists
bool fileExists(const char* path);
}
}

#endif
