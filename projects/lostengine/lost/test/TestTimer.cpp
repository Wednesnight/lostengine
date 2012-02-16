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

#include "lost/time/ThreadedTimerScheduler.h"
#include "lost/time/Timer.h"
#include "UnitTest++.h"
#include "lost/common/Logger.h"
#include "lost/platform/Platform.h"

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
  lost::shared_ptr<TimerScheduler> scheduler(new ThreadedTimerScheduler("test"));
  for (int i = 0; i < 100; ++i) {
    scheduler->createTimer(1, &timerCallback, true);
  }
  sleep(10);
}
