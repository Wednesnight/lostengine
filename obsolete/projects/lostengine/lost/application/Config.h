#ifndef LOST_APPLICATION_CONFIG_H
#define LOST_APPLICATION_CONFIG_H

#include "lost/lua/lua.h"
#include "lost/lua/State.h"

namespace lost
{
  namespace application
  {
    struct ConfigValue : public luabind::object
    {
      ConfigValue();
      ConfigValue(const luabind::object& inObject);

      ConfigValue operator[](const std::string& key);

      template <typename Type>
      Type as()
      {
        return luabind::object_cast<Type>((luabind::object)(*this));
      }

      template <typename Type>
      Type as(const Type& inDefault)
      {
        try 
        {
          if (luabind::type((luabind::object*)this) != LUA_TNIL) return as<Type>();
            else return inDefault;
        }
        catch (std::exception&)
        {
          return inDefault;
        }
      }
    };

    struct Config : public luabind::object
    {
    private:
      lost::shared_ptr<lua::State> interpreter;

    public:
      Config(const lost::shared_ptr<lua::State>& inInterpreter);
    };

    struct ConfigPtr : public lost::shared_ptr<Config>
    {
      ConfigPtr();
      ConfigPtr(Config* inConfig);
      
      ConfigValue operator[](const std::string& key);
    };
    
  }
}

#endif
