#ifndef LOST_RESOURCE_BUNDLE_H
#define LOST_RESOURCE_BUNDLE_H

#include "lost/resource/forward.h"
#include "lost/resource/Loader.h"

namespace lost
{
namespace resource
{
struct Bundle : Loader
{
  static BundlePtr create(const fs::Path& inRootDir);
  
  
  
private:
  Bundle(const fs::Path& inRootDir);
};
}
}


#endif

