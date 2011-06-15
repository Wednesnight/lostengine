#ifndef LOST_RESOURCE_HELPER_H
#define LOST_RESOURCE_HELPER_H

#include "lost/common/forward.h"

namespace lost
{
namespace resource
{

// reads file from path into new Data instance, leaves location member unset.
common::DataPtr loadFromAbsolutePath(const string& inPath);

// checks if file exists
bool fileExists(const string& inPath);

// writes inData to inPath
void writeToAbsolutePath(const string& inPath, const common::DataPtr& inData);

}
}

#endif
