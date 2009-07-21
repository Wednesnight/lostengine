#include "lost/resource/File.h"

namespace lost
{
namespace resource
{

// reads file from path into new File instance, leaves location member unset.
FilePtr loadFromAbsolutePath(const std::string& inPath);

}
}