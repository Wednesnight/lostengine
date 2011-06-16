#ifndef LOST_COMMON_STRINGSTREAM_H
#define LOST_COMMON_STRINGSTREAM_H

namespace lost
{
namespace common
{
struct StringStream
{
  StringStream();
  virtual ~StringStream();
  void append(const string& d);
  string str();
  void clear();
  
  string _data;
};
}
const static string endl;
}

lost::common::StringStream& operator<<(lost::common::StringStream& s, int32_t v);
lost::common::StringStream& operator<<(lost::common::StringStream& s, uint32_t v);
lost::common::StringStream& operator<<(lost::common::StringStream& s, const char* v);
lost::common::StringStream& operator<<(lost::common::StringStream& s, const lost::string& v);

#endif