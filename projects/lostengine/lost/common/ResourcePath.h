#ifndef LOST_COMMON_RESOURCEPATH_H
#define LOST_COMMON_RESOURCEPATH_H

#include "lost/platform/Platform.h"

namespace lost
{
namespace common
{

/** This is a helper class so we can express a certain path structure in the interface.
 * If you want to express a certain path structure, you have two choices:
 * either write something like loadDataFromResourceDir(string) and hope that the user supplies you with
 * the correct path or try to enforce the path through this helper class.
 * This also keeps the method names shorter since you can overload them like this
 * loadData(string absolutePath)
 * loadData(ResourcePath path) // calls loadData(string) after having built the appropriate path
 * 
 * NOTE: this is mostly a test to see if it's any use
 *
 * NOTE: something I completely forgot about: due to the fact that ResourcePath is constructible from a string, 
 * you can still write loadFile("asd"); and it will automatically convert the const char* to a string to a ResourcePath 
 * and call the correct function. This also means that ResourcePath and string are ambiguous parameters 
 * and therefore functions cannot be overloaded easiyl. Ah, who cares!
 */
struct ResourcePath
{
  std::string fullResourcePath;

  ResourcePath(const std::string& inPartialPath)
  {
    fullResourcePath = lost::platform::buildResourcePath(inPartialPath);
  }
  
  virtual ~ResourcePath() {}  
  
  const std::string& str() const {return fullResourcePath;}
};
}
}


#endif