#include "lost/lua/bindings/ThirdpartyBoost.h"
#include "lost/lua/lua.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/system/error_code.hpp>

#include <slub/slub.h>

using namespace boost::filesystem;

std::ostream& operator<<(std::ostream& stream, const path& p)
{
  stream << p.string();
  return stream;
}

std::ostream& operator<<(std::ostream& stream, const directory_entry& e)
{
  stream << e.path().filename();
  return stream;
}

namespace slub {
  template<>
  struct converter<std::string> {
    
    static bool check(lua_State* L, int index) {
      return lua_isstring(L, index);
    }
    
    static std::string get(lua_State* L, int index) {
      return luaL_checkstring(L, index);
    }
    
    static int push(lua_State* L, const std::string& value) {
      lua_pushstring(L, value.c_str());
      return 1;
    }
    
  };
  
  template<>
  struct converter<std::string&> : converter<std::string> {};
  
  template<>
  struct converter<const std::string&> : converter<std::string> {};
  
  template<>
  struct converter<std::string*> {
    
    static bool check(lua_State* L, int index) {
      return lua_isstring(L, index);
    }
    
    static std::string* get(lua_State* L, int index) {
      throw std::runtime_error("Cannot get a string* value");
    }
    
    static int push(lua_State* L, const std::string* value) {
      lua_pushstring(L, value->c_str());
      return 1;
    }
    
  };
  
  template<>
  struct converter<const std::string*> : converter<std::string*> {};
  
}

namespace lost
{
  namespace lua
  {

    void ThirdpartyBoostFilesystem(lua_State* state)
    {
      slub::package fs = slub::package(state, "boost").package("filesystem");

      fs.clazz<path>("path")
        .constructor()
        .constructor<const string&>()
        .method("remove_filename", &path::remove_filename)
        .method("filename", &path::filename)
        .method("stem", &path::stem)
        .method("extension", &path::extension)
        .method("native", &path::native)
        .tostring()
        .div<path, string>();

      fs.clazz<directory_iterator>("directory_iterator");

      fs.clazz<directory_entry>("directory_entry")
        .method("status", (file_status (directory_entry::*)() const) &directory_entry::status)
        .tostring();

      fs.clazz<file_status>("file_status");

      fs.function("is_directory", (bool(*)(file_status)) &is_directory);
      fs.function("is_regular_file",(bool(*)(const path& p)) &is_regular_file);
      fs.function("exists",(bool(*)(const path& p)) &exists);
      fs.function("create_directories",(bool(*)(const path& p)) &create_directories);
    }

    void ThirdpartyBoost(lua_State* state)
    {
      ThirdpartyBoostFilesystem(state);
    }

  }
}
