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

#ifndef LOST_TIME_CLOCK_H
#define LOST_TIME_CLOCK_H

namespace lost {

  namespace time {

    class Clock {


    public:
      
      
      Clock(double t0 = 0.0);


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

#endif
