#ifndef LOST_LUA_FORWARD_H
#define LOST_LUA_FORWARD_H

struct lua_State;
namespace luabind { class object; }

namespace lost
{
namespace lua
{
    struct State;
    typedef lost::shared_ptr<State> StatePtr;

}
}

#endif