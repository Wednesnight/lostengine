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
  FilesystemRepositoryPtr repo; // this bundles repo, for subundle creation
  
  static BundlePtr create(const fs::Path& inRootDir);

  // creates a new bundle that points to the path created by current repo + relativePath  
  BundlePtr subBundle(const lost::fs::Path& relativePath); 
  
private:
  Bundle(const fs::Path& inRootDir);
};
}
}


#endif

