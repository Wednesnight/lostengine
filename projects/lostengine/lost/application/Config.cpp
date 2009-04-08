#include "lost/application/Config.h"

using namespace boost;
using namespace luabind;
using namespace std;
using namespace lost::lua;

namespace lost
{
  namespace application
  {
    ConfigValue::ConfigValue()
    : object()
    {
    }
    
    ConfigValue::ConfigValue(const object& inObject)
    : object(inObject)
    {
    }
    
    ConfigValue ConfigValue::operator[](const string& key)
    {
      ConfigValue result;
      if (luabind::type((object)(*this)[key]) != LUA_TNIL) result = ConfigValue((object)(*this)[key]);
      return result;
    }
    
    Config::Config(const StatePtr& inInterpreter)
    : object(),
      interpreter(inInterpreter)
    {
      *(object*)this = globals(interpreter->state);
    }

    ConfigPtr::ConfigPtr()
    : shared_ptr<Config>::shared_ptr()
    {
    }
    
    ConfigPtr::ConfigPtr(Config* inConfig)
    : shared_ptr<Config>::shared_ptr(inConfig)
    {
    }
    
    ConfigValue ConfigPtr::operator[](const string& key)
    {
      object self = (*((object*)this->get()));
      ConfigValue result;
      if (luabind::type(self[key]) != LUA_TNIL)
      {
        result = ConfigValue(self[key]);
      }
      return result;
    }
    
  }
}
