#include "UnitTest++.h"

#include <boost/shared_ptr.hpp>

#include "lost/engine/Engine.h"
#include "lost/engine/no/VideoSubSystem.h"
#include "lost/engine/no/AudioSubSystem.h"
#include "lost/engine/no/InputSubSystem.h"
#include "lost/engine/no/TimerSubSystem.h"
#include "lost/util/Config.h"

namespace TestEngine
{
  struct Engine;
  typedef lost::engine::Engine<TestEngine::Engine,
                               lost::engine::no::VideoSubSystem<TestEngine::Engine>,
                               lost::engine::no::AudioSubSystem<TestEngine::Engine>,
                               lost::engine::no::InputSubSystem<TestEngine::Engine>,
                               lost::engine::no::TimerSubSystem<TestEngine::Engine> > BaseEngine;
  struct Engine : public BaseEngine
  {
    Engine( boost::shared_ptr<lost::util::Config> inConfig )
      : BaseEngine::Engine(inConfig)
    {
    }
    void run()
    {
    }
    void shutdown()
    {
    }
  };
}

TEST( BaseEngine )
{
  boost::shared_ptr<lost::util::Config> config(new lost::util::Config());
  TestEngine::Engine engine(config);

  CHECK( &engine );
}
