#ifndef LOST_LUA_BINDINGS_DIRECTORY_ITERATOR_H
#define LOST_LUA_BINDINGS_DIRECTORY_ITERATOR_H

#include <luabind/config.hpp>
#include <luabind/detail/policy.hpp>
#include <luabind/detail/convert_to_lua.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

namespace lost
{
  namespace lua
  {
  
    struct iterator
    {
      static int next(lua_State* L)
      {
        iterator* self = static_cast<iterator*>(lua_touserdata(L, lua_upvalueindex(1)));
        
        if (self->first != self->last)
        {
          luabind::detail::convert_to_lua(L, *self->first);
          ++self->first;
        }
        else
        {
          lua_pushnil(L);
        }
        
        return 1;
      }
      
      static int destroy(lua_State* L)
      {
        iterator* self = static_cast<iterator*>(lua_touserdata(L, 1));
        self->~iterator();
        return 0;
      }
      
      iterator(const boost::filesystem::directory_iterator& first,
               const boost::filesystem::directory_iterator& last)
      : first(first)
      , last(last)
      {}
      
      boost::filesystem::directory_iterator first;
      boost::filesystem::directory_iterator last;
    };
    
    int make_range(lua_State* L, const boost::filesystem::directory_iterator& first,
                   const boost::filesystem::directory_iterator& last)
    {
      void* storage = lua_newuserdata(L, sizeof(iterator));
      lua_newtable(L);
      lua_pushcclosure(L, iterator::destroy, 0);
      lua_setfield(L, -2, "__gc");
      lua_setmetatable(L, -2);
      lua_pushcclosure(L, iterator::next, 1);
      new (storage) iterator(first, last);
      return 1;
    }
    
    int make_range(lua_State* L, const boost::filesystem::path& path)
    {
      boost::filesystem::directory_iterator iter;
      try {
        iter = boost::filesystem::directory_iterator(path);
      }
      catch (...) {
      }
      return make_range(L, iter, boost::filesystem::directory_iterator());
    }
    
    struct directory_iterator_converter
    {
      typedef directory_iterator_converter type;
      
      void apply(lua_State* L, boost::filesystem::path& path)
      {
        make_range(L, path);
      }
      
      void apply(lua_State* L, boost::filesystem::path const& path)
      {
        make_range(L, path);
      }
    };
    
    struct directory_iterator_policy : luabind::conversion_policy<0>
    {
      static void precall(lua_State*, luabind::index_map const&)
      {}
      
      static void postcall(lua_State*, luabind::index_map const&)
      {}
      
      template <class T, class Direction>
      struct apply
      {
        typedef directory_iterator_converter type;
      };
    };
  
  }
}

namespace luabind
{
  namespace
  {
  
    LUABIND_ANONYMOUS_FIX detail::policy_cons<
    lost::lua::directory_iterator_policy, detail::null_type> return_directory_iterator;
  
  }
}

#endif
