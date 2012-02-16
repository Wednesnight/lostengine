/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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

lost::common::StringStream& operator<<(lost::common::StringStream& s, unsigned long v)
{
  char buf[32];
  sprintf(buf, "%lu",v);
  s.append(buf);
  return s;
}

lost::common::StringStream& operator<<(lost::common::StringStream& s, long v)
{
  char buf[32];
  sprintf(buf, "%ld",v);
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
