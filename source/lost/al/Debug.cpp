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

#include "lost/al/Debug.h"
#include "lost/al/al.h"
#include "lost/common/Logger.h"
#include <sstream>
#include <stdexcept>

namespace lost
{
namespace al
{
  
void debug(const char* file, int line, const char* func)
{
  ALenum error = alGetError();
  if(error != AL_NO_ERROR)
  {
    EOUT("AL error: " << alGetString(error) << " from here: "<<file<< " " << line << " " <<func);
  }
}
  
void debug_throw(const char* file, int line, const char* func)
{
  ALenum error = alGetError();
  if(error != AL_NO_ERROR)
  {
    std::ostringstream os;
    os << "AL error: " << alGetString(error) << " from here: "<<file<< " " << line << " " <<func;
    throw std::runtime_error(os.str());
  }
}
  
}
}
