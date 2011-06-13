#ifndef LOST_PLATFORM_PATHS_H
#define LOST_PLATFORM_PATHS_H

#include "boost/filesystem.hpp"

namespace lost 
{
namespace platform
{
  //*** platform specific forwards, implemented in Platform_*.cpp

    // returns the application resource directory
    // Mac OS X: when building a Cocoa app, the resource directory is located inside
    //           the application bundle
    // Windows : the directory the main executable remains in
    // Linux   : not implemented
    boost::filesystem::path getResourcePath();

    // the directory the main executable remains in
    // Mac OS X: not implemented
    // Windows : done
    // Linux   : not implemented
    boost::filesystem::path getApplicationDirectory();
    
    boost::filesystem::path getUserDataPath();
}
}

#endif