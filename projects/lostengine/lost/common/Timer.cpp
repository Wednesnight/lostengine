#include "Timer.h"
#include "lost/platform/Platform.h"


#if defined(__APPLE__) || defined(linux)
#include <sys/time.h>
#endif // defined(__APPLE__) || defined(linux)


namespace lost {

  namespace common {


    Timer::Timer(double t0) {

      this->t0 = t0;
      this->frequency = this->initializeFrequency();
      this->ticks0 = this->getTicks();
    }


    double Timer::getTime() {

      return this->deltaSec(this->ticks0, getTicks()) + this->t0;
    }


    double Timer::getElapsed(double offset) {

      return this->getTime() - offset;
    }


    double Timer::getElapsedAndUpdateOffset(double& offset) {

      double start = offset;
      return (offset = this->getTime()) - start;
    }


    uint64_t Timer::initializeFrequency() {

#ifdef WIN32
      LARGE_INTEGER frequency;
      QueryPerformanceFrequency(&frequency);
      return frequency.QuadPart;
#endif //WIN32

#if defined(__APPLE__) || defined(linux)
      return 1000;
#endif // defined(__APPLE__) || defined(linux)
    }


    uint64_t Timer::getTicks() {
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


    double Timer::deltaSec(uint64_t ticks1, uint64_t ticks2) {

      return (double) ((ticks1 > ticks2) ? ticks1 - ticks2 : ticks2 - ticks1) / (double) this->frequency;
    }


  }
}
