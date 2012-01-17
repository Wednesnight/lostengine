#include "lost/common/StringStream.h"

namespace lost
{
namespace common
{

StringStream::StringStream()
{
}

StringStream::~StringStream()
{
}

void StringStream::append(const string& d)
{
  _data.append(d);
}

string StringStream::str()
{
  return _data;
}

void StringStream::clear()
{
  _data="";
}

string endl = "\n";
}
}

lost::common::StringStream& operator<<(lost::common::StringStream& s, float v)
{
  char buf[256];
  sprintf(buf, "%f",v);
  s.append(buf);
  return s;
  
}

lost::common::StringStream& operator<<(lost::common::StringStream& s, double v)
{
  char buf[256];
  sprintf(buf, "%lf",v);
  s.append(buf);
  return s;  
}

lost::common::StringStream& operator<<(lost::common::StringStream& s, int32_t v)
{
  char buf[32];
  sprintf(buf, "%d",v);
  s.append(buf);
  return s;
}

lost::common::StringStream& operator<<(lost::common::StringStream& s, uint32_t v)
{
  char buf[32];
  sprintf(buf, "%u",v);
  s.append(buf);
  return s;
}

lost::common::StringStream& operator<<(lost::common::StringStream& s, uint64_t v)
{
  char buf[32];
  sprintf(buf, "%llu",v);
  s.append(buf);
  return s;
}

lost::common::StringStream& operator<<(lost::common::StringStream& s, const char* v)
{
  s.append(v);
  return s;
}

lost::common::StringStream& operator<<(lost::common::StringStream& s, const lost::string& v)
{
  s.append(v);
  return s;
}
