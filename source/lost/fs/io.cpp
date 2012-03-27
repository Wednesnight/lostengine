#include "lost/common/StringStream.h"
#include "lost/fs/Path.h"
#include "lost/fs/io.h"

std::ostream& operator << (std::ostream& stream, const lost::fs::Path& p)
{
  stream << p.string();
  return stream;
}

lost::common::StringStream& operator<<(lost::common::StringStream& stream, const lost::fs::Path& p)
{
  stream << p.string();
  return stream;
}
