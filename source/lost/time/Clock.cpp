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

#include "lost/time/Clock.h"
#include "lost/platform/Time.h"


#if defined(__APPLE__) || defined(linux)
#include <sys/time.h>
#endif // defined(__APPLE__) || defined(linux)


namespace lost {

  namespace time {


    Clock::Clock(double t0) {

      this->t0 = t0;
      this->frequency = this->initializeFrequency();
      this->ticks0 = this->getTicks();
    }


    double Clock::getTime() {

      return this->deltaSec(this->ticks0, getTicks()) + this->t0;
    }


    double Clock::getElapsed(double offset) {

      return this->getTime() - offset;
    }


    double Clock::getElapsedAndUpdateOffset(double& offset) {

      double start = offset;
      return (offset = this->getTime()) - start;
    }


    uint64_t Clock::initializeFrequency() {

#ifdef WIN32
      LARGE_INTEGER frequency;
      QueryPerformanceFrequency(&frequency);
      return frequency.QuadPart;
#endif //WIN32

#if defined(__APPLE__) || defined(linux)
      return 1000;
#endif // defined(__APPLE__) || defined(linux)
    }


    uint64_t Clock::getTicks() {
#ifdef WIN32

      LARGE_INTEGER ticks;
      QueryPerformanceCounter(&ticks);
      return ticks.QuadPart;

#endif //WIN32
      
#if defined(__APPLE__) || defined(linux)

      timeval time;
      gettimeofday(&time, NULL);
      return (time.tv_sec * 1000) + (time.tv_usec / 1000);

#endif // defined(__APPLE__) || defined(linux)
    }


    double Clock::deltaSec(uint64_t ticks1, uint64_t ticks2) {

      return (double) ((ticks1 > ticks2) ? ticks1 - ticks2 : ticks2 - ticks1) / (double) this->frequency;
    }


  }
}
