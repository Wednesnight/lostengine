#ifndef LOST_MATH_LMATH_H
#define LOST_MATH_LMATH_H

// don't call this header "Math.h" or it might conflict with standard library "math.h" depending 
// on the order of includes in your current build setting and result in confusing errors.

#include <cmath>
#include <math.h>
#include <boost/shared_array.hpp>

namespace lost
{
  namespace math
  {
#if defined WIN32
    inline float fmaxf(const float& in1, const float& in2)
    {
      return (in1 > in2) ? in1 : in2;
    }
#endif
	  inline float max(const boost::shared_array<float> inArray, int inLength)
    {
      float result = (inLength > 0) ? inArray[0] : 0;
      for (int idx = 1; idx < inLength; ++idx)
        result = fmaxf(result, inArray[idx]);
      return result;
    }
    
    /* PI */
#ifndef M_PI
#define M_PI (float)3.1415926535897932384626433832795
#endif
    
    inline float deg2rad( float deg )
    {
      return deg / 360.0f * (2.0f * M_PI);
    }
    
    inline float rad2deg( float rad )
    {
      return (rad / (2.0f * M_PI)) * 360.0f;
    }

    // compares two floats, returning true if they are "near enough", i.e. their difference is within
    // the range of the provided epsilon value.
    // this is necessary to compensate for floating point rounding errors that occur with 32bit floats.
    inline bool compare(float a, float b, float epsilon)
    {
      float d = std::abs(a-b);
      if(d <= epsilon)
        return true;
      else
        return false;
    }

    // calculate the next value that is >= inVal and also a power of two  
    static inline uint32_t nextPowerOf2(uint32_t inVal)
    {
      unsigned long result = 1;

      while(result < inVal)
      {
        result <<= 1;
      }

      return result;
    }

  }
}

#endif
