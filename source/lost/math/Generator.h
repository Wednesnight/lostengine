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

#ifndef LOST_MATH_GENERATOR_H
#define LOST_MATH_GENERATOR_H

#include <cmath>

namespace lost
{
 
namespace math
{
 
namespace generator
{

  static const float pi = 3.14159f;

  // returns current time within period as radians value
  float timeAsRad(float curtime, float curperiod)
  {
    return (curtime/curperiod)*2.0f*pi;    
  }
  
  
  struct sine
  {
    float periodoffset;
    float ampoffset;
    float period;
    float absamplitude;  // absolute amplitude of sine, e.g. a sine ranging between -1 and 1 would have an amplitude of 1
    
    float currentx;
    
    float dummytarget;
    float* writetarget;
    
    sine()
    {
      writetarget = &dummytarget;
      periodoffset = 0;
      ampoffset = 0;
      period = 0;
      currentx = 0;
    }
    
    void update(float delta)
    {
      // advance time
      currentx = fmod((currentx + periodoffset + delta), period);     
      
      // calculate new value and store in target
      *writetarget = ampoffset+std::sin(timeAsRad(currentx, period))*absamplitude;
    }
    
    void target(float& inTarget)
    {
      writetarget = &inTarget;
    }
    
  };

  // FIXME: this is a candidate for heavy templating since the only differencebetween sine and cosine generator is the trigo function
  struct cosine
  {
    float periodoffset;
    float ampoffset;
    float period;
    float absamplitude;  // absolute amplitude of sine, e.g. a sine ranging between -1 and 1 would have an amplitude of 1
    
    float currentx;
    
    float dummytarget;
    float* writetarget;
    
    cosine()
    {
      writetarget = &dummytarget;
      periodoffset = 0;
      ampoffset = 0;
      period = 0;
      currentx = 0;
    }
    
    void update(float delta)
    {
      // advance time
      currentx = fmod((currentx + periodoffset + delta), period);     
      
      // calculate new value and store in target
      *writetarget = ampoffset+std::cos(timeAsRad(currentx, period))*absamplitude;
    }
    
    void target(float& inTarget)
    {
      writetarget = &inTarget;
    }
    
  };
  
  
}

}
  
}


#endif
