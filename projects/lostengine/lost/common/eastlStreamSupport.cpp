#include <iostream>
#include <EASTL/string.h>

std::ostream& operator << (std::ostream& os, const eastl::string& s)
{
  os << s.c_str();
  return os;
}