#include "lost/time/ThreadedTimerScheduler.h"
#include "lost/time/Timer.h"
#include "UnitTest++.h"
#include "lost/common/Logger.h"
#include "lost/platform/Platform.h"
#include <boost/bind.hpp>

using namespace lost::time;
using namespace lost::platform;

static int count = 0;
bool timerCallback(const Timer* timer)
{
  DOUT("timer fired! now: " << currentTimeSeconds() << ", timer: " << timer->getTime() << ", interval: " << timer->getInterval());
  return ++count < 5;
}

TEST(timer)
{
  lost::shared_ptr<TimerScheduler> scheduler(new ThreadedTimerScheduler());
  lost::shared_ptr<Timer> timer(scheduler->createTimer(1, boost::bind(&timerCallback, _1), true));
  sleep(10);
}
