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
      ConfigValue()
      : luabind::object::object()
      {
      }
      
      ConfigValue(const luabind::object& inObject)
      : luabind::object::object(inObject)
      {
      }
      
      ConfigValue operator[](const std::string& key)
      {
        ConfigValue result;
        if (luabind::type((luabind::object)(*this)[key]) != LUA_TNIL) result = ConfigValue((luabind::object)(*this)[key]);
        return result;
      }

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
          Type result = as<Type>();
          return result;
        }
        catch (std::exception& e)
        {
          return inDefault;
        }
      }
    };

    struct Config
    {
      struct SharedPtr : public boost::shared_ptr<Config>
      {
        SharedPtr()
        : boost::shared_ptr<Config>::shared_ptr()
        {
        }
        
        SharedPtr(Config* inConfig)
        : boost::shared_ptr<Config>::shared_ptr(inConfig)
        {
        }
        
        ConfigValue operator[](const std::string& key)
        {
          ConfigValue result;
          if (luabind::type((*this)->config[key]) != LUA_TNIL)
          {
            result = ConfigValue((*this)->config[key]);
          }
          return result;
        }
      };

      boost::shared_ptr<lua::State> interpreter;
      luabind::object               config;

      Config(const boost::shared_ptr<lua::State>& inInterpreter)
      : interpreter(inInterpreter)
      {
        luabind::object globals = luabind::globals(*(interpreter.get()));
        config = globals["lost"]["application"]["Application"]["config"] = luabind::newtable(*(interpreter.get()));
      }
    };
  }
}

#endif
