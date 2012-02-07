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

#include "lost/fs/Path.h"

std::ostream& operator<<(std::ostream& stream, const lost::fs::Path& p)
{
  stream << p.string();
  return stream;
}

#include "lost/lua/bindings/LostFs.h"
#include "lost/lua/lua.h"
#include <slub/slub.h>



/*std::ostream& operator<<(std::ostream& stream, const directory_entry& e)
{
  stream << e.path().filename();
  return stream;
}

namespace slub {

  template<>
  struct converter<path::string_type> {
    
    static bool check(lua_State* L, int index) {
      return lua_isstring(L, index);
    }
    
    static path::string_type get(lua_State* L, int index) {
#ifdef WIN32
      const char* ascii = luaL_checkstring(L, index);
      wchar_t* wstr = new wchar_t[mblen(ascii, MAX_PATH) + sizeof(wchar_t)];
      mbstowcs(wstr, ascii, mblen(ascii, MAX_PATH) + sizeof(wchar_t));
      path::string_type result(wstr);
      delete wstr;
      return result;
#else
      return luaL_checkstring(L, index);
#endif
    }
    
    static int push(lua_State* L, const path::string_type& value) {
#ifdef WIN32
      const wchar_t* wstr = value.c_str();
      char* ascii = new char[wcslen(wstr) + 1];
      wcstombs(ascii, wstr, wcslen(wstr) + 1);
      lua_pushstring(L, ascii);
      delete ascii;
#else
      lua_pushstring(L, value.c_str());
#endif
      return 1;
    }
    
  };
  
  template<>
  struct converter<path::string_type&> : converter<path::string_type> {};
  
  template<>
  struct converter<const path::string_type&> : converter<path::string_type> {};
  
}
*/

namespace lost
{
  
  namespace lua
  {

    std::ostream& operator<<(std::ostream& stream, const lost::fs::Path& p)
    {
      stream << p.string();
      return stream;
    }
    void LostFs(lua_State* state)
    {
      slub::package fs = slub::package(state, "lost").package("fs");

      fs.clazz<fs::Path>("Path")
        .constructor()
        .constructor<const string&>()
//        .method("remove_filename", &path::remove_filename)
//        .method("filename", &path::filename)
//        .method("stem", &path::stem)
//        .method("extension", &path::extension)
        .method("native", &fs::Path::native)
        .tostring()
      .div<fs::Path, string>();

//      fs.clazz<directory_iterator>("directory_iterator");

//      fs.clazz<directory_entry>("directory_entry")
//        .method("status", (file_status (directory_entry::*)() const) &directory_entry::status)
//        .tostring();

//      fs.clazz<file_status>("file_status");

//      fs.function("is_directory", (bool(*)(file_status)) &is_directory);
//      fs.function("is_regular_file",(bool(*)(const path& p)) &is_regular_file);
      fs.function("exists",&fs::exists);
      fs.function("create_directories",&fs::create_directories);
    }

/*    void ThirdpartyBoost(lua_State* state)
    {
      ThirdpartyBoostFilesystem(state);
    }*/

  }
}
