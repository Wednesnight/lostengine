#include <stdint.h>

namespace lost {

  namespace common {

    class Timer {


    public:
      
      
      Timer(double t0 = 0.0);


      /**
       *  Returns the delta between t0 and now in seconds
       **/
      double getTime();


      /**
       *  Returns the delta between offset and now in seconds
       **/
      double getElapsed(double offset);


      /**
       *  Returns the delta between offset and now in seconds, sets offset to now
       **/
      double getElapsedAndUpdateOffset(double& offset);


    private:


      /**
       *  Stores the origin of this Timer's timeline
       **/
      double t0;


      /**
       *  Stores the initial ticks at time of creation
       **/
      uint64_t ticks0;


      /**
       *  Stores the platform specific frequency
       **/
      uint64_t frequency;


      /**
       *  Initializes the platform specific frequency value
       **/
      uint64_t initializeFrequency();


      /**
       *  Returns platform specific ticks
       **/
      uint64_t getTicks();


      /**
       *  Calculates delta between ticks1 and ticks2 in seconds
       **/
      double deltaSec(uint64_t ticks1, uint64_t ticks2);


    };

  }
}
