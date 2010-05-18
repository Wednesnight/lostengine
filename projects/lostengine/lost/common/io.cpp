#include "lost/common/io.h"
#include <iostream>
#include "lost/common/Color.h"

std::ostream& operator<<(std::ostream& inStream, const lost::common::Color& inColor)
{
  inStream << inColor.r() << "/"<<inColor.g()<<"/"<<inColor.b()<<"/"<<inColor.a();
  return inStream;
}
