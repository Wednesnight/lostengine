#ifndef LOST_FS_IO_H
#define LOST_FS_IO_H

namespace lost {
  namespace fs {
    struct Path;
  }
  namespace common {
    struct StringStream;
  }
}

std::ostream& operator<<(std::ostream& s, const lost::fs::Path& p);
lost::common::StringStream& operator<<(lost::common::StringStream& stream, const lost::fs::Path& p);

#endif
