#include "lost/al/Debug.h"
#include "lost/al/al.h"
#include "lost/common/Logger.h"
#include <sstream>
#include <stdexcept>

using namespace std;

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
    ostringstream os;
    os << "AL error: " << alGetString(error) << " from here: "<<file<< " " << line << " " <<func;
    throw runtime_error(os.str());
  }
}
  
}
}
