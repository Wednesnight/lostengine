#ifndef LOST_RESOURCE_HELPER_H
#define LOST_RESOURCE_HELPER_H

#include <string>

namespace lost
{
namespace resource
{

  struct File;
  typedef lost::shared_ptr<File> FilePtr;

// reads file from path into new File instance, leaves location member unset.
FilePtr loadFromAbsolutePath(const std::string& inPath);

}
}

#endif