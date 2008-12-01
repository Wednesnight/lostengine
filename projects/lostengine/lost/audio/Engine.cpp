#include "lost/audio/Engine.h"
#include "lost/al/Context.h"

namespace lost
{
namespace audio
{

Engine::Engine()
{
    context.reset(new al::Context());    
    context->makeCurrent();
}

Engine::~Engine()
{
}

}
}