#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/lua/State.h"
#include "lost/common/Logger.h"

#include "lost/lua/ErrorHandler.h"

using namespace std;
using namespace boost;
using namespace luabind;
using namespace lost::lua;

namespace lost
{
  namespace lua
  {

    int errorHandler(lua_State* state)
    {
      int result = 0;
      try
      {
        shared_ptr<State> interpreter = object_cast<shared_ptr<State> >(globals(state)["globals"]["state"]);
        result = interpreter->handleError();
      }
      catch (std::exception& e)
      {
        EOUT("could not call error handler: " << e.what());
      }

      return result;
    }

  }
}
