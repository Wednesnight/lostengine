#include "lost/lua/bindings/LostAudio.h"
#include "lost/audio/Engine.h"
#include "lost/audio/Source.h"

#include <slub/slub.h>

using namespace lost::audio;
using namespace slub;

namespace lost
{
  namespace lua
  {
    
    void LostAudioEngine(lua_State* state)
    {
      package(state, "lost").package("audio")
        .clazz<Engine>("Engine")
          .method("createSource", &Engine::createSource)
          .function("create", &Engine::create);
    }
    
    void LostAudioSource(lua_State* state)
    {
      package(state, "lost").package("audio")
        .clazz<Source>("Source")
          .method("initWithFile", &Source::initWithFile)
          .method("play", &Source::play)
          .method("stop", &Source::stop);
    }
        
    void LostAudio(lua_State* state)
    {
      LostAudioEngine(state);
      LostAudioSource(state);
    }
    
  }
}
