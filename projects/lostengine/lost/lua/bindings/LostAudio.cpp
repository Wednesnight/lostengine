#include "lost/lua/bindings/LostAudio.h"
#include "lost/lua/lua.h"
#include "lost/audio/Engine.h"
#include "lost/audio/Source.h"

using namespace luabind;
using namespace lost::audio;

namespace lost
{
  namespace lua
  {
    
    void LostAudioEngine(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("audio")
        [
          class_<Engine>("Engine")
            .def("createSource", &Engine::createSource)
            .scope
            [
              def("create", &Engine::create)
            ]
        ]
      ];
    }
    
    void LostAudioSource(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("audio")
        [
          class_<Source>("Source")
            .def("initWithFile", &Source::initWithFile)
            .def("play", &Source::play)
            .def("stop", &Source::stop)
        ]
      ];
    }
        
    void LostAudio(lua_State* state)
    {
      LostAudioEngine(state);
      LostAudioSource(state);
    }
    
  }
}
