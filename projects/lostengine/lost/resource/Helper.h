#ifndef LOST_RESOURCE_HELPER_H
#define LOST_RESOURCE_HELPER_H

#include <string>
#include "lost/common/forward.h"

namespace lost
{
namespace resource
{

// reads file from path into new Data instance, leaves location member unset.
common::DataPtr loadFromAbsolutePath(const std::string& inPath);

// checks if file exists
bool fileExists(const std::string& inPath);

}
}

#endif