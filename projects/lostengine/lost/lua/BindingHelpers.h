#ifndef LOST_LUA_BINDINGHELPERS_H
#define LOST_LUA_BINDINGHELPERS_H

#define LOST_LUA_EXPORT_BEGIN(s) namespace lost\
                                 {\
                                   namespace lua\
                                   {\
                                     void s(lost::lua::State& state)
#define LOST_LUA_EXPORT_END        }\
                                 }

// this block is essential for correct luabind handling of boost::shared_ptr
#include <boost/shared_ptr.hpp>
namespace luabind
{
  template<class T>
  T* get_pointer(boost::shared_ptr<T>& p)
  {
    return p.get();
  }
}

#endif