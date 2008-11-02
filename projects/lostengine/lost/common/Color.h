#ifndef LOST_COMMON_COLOR_H
#define LOST_COMMON_COLOR_H

#include "lost/common/Logger.h"
#include <iostream>

namespace lost
{
  namespace common
  {

    struct Color
    {
      Color() {r(1.0f); g(1.0f); b(1.0f); a(1.0f); }

      Color(float newr, float newg, float newb) {r(newr); g(newg); b(newb); a(1.0f); }
      Color(float newr, float newg, float newb, float newa) {r(newr); g(newg); b(newb); a(newa); }

      inline float r() const { return fv[0]; }
      inline float g() const { return fv[1]; }
      inline float b() const { return fv[2]; }
      inline float a() const { return fv[3]; }

      inline void r(float v) { fv[0] = v; }
      inline void g(float v) { fv[1] = v; }
      inline void b(float v) { fv[2] = v; }
      inline void a(float v) { fv[3] = v; }

      // accessors for rgba values as usingned 8 bit integers
      inline uint8_t ru8() const { return fv[0]*255.0; };
      inline uint8_t gu8() const { return fv[1]*255.0; };
      inline uint8_t bu8() const { return fv[2]*255.0; };
      inline uint8_t au8() const { return fv[3]*255.0; };

      inline void ru8(uint8_t v) { fv[0] = float(v)/255.0; }
      inline void gu8(uint8_t v) { fv[1] = float(v)/255.0; }
      inline void bu8(uint8_t v) { fv[2] = float(v)/255.0; }
      inline void au8(uint8_t v) { fv[3] = float(v)/255.0; }
      
      float fv[4];
    };

    static inline lost::common::Color operator*(const Color& l, const Color& r)
    {
      lost::common::Color result;

      result.fv[0] = l.fv[0] * r.fv[0];
      result.fv[1] = l.fv[1] * r.fv[1];
      result.fv[2] = l.fv[2] * r.fv[2];
      result.fv[3] = l.fv[3] * r.fv[3];

      return result;
    }

    static inline bool operator ==(const Color& l, const Color& r)
    {
      return (l.fv[0] == r.fv[0]) && (l.fv[1] == r.fv[1]) && (l.fv[2] == r.fv[2]) && (l.fv[3] == r.fv[3]);
    }
    
    static inline bool operator !=(const Color& l, const Color& r)
    {
      return !(l==r);
    }
    
    static inline void operator+=(Color& l, const Color& r)
    {
      l.fv[0] = std::min( 1.0f, l.fv[0] + r.fv[0] );
      l.fv[1] = std::min( 1.0f, l.fv[1] + r.fv[1] );
      l.fv[2] = std::min( 1.0f, l.fv[2] + r.fv[2] );
      l.fv[3] = std::min( 1.0f, l.fv[3] + r.fv[3] );
    }

    static inline void operator-=(Color& l, const Color& r)
    {
      l.fv[0] = std::max( 0.0f, l.fv[0] - r.fv[0] );
      l.fv[1] = std::max( 0.0f, l.fv[1] - r.fv[1] );
      l.fv[2] = std::max( 0.0f, l.fv[2] - r.fv[2] );
      l.fv[3] = std::max( 0.0f, l.fv[3] - r.fv[3] );
    }

    inline std::ostream& operator<<(std::ostream& inStream, const lost::common::Color& inColor)
    {
      inStream << inColor.r() << "/"<<inColor.g()<<"/"<<inColor.b()<<"/"<<inColor.a();
      return inStream;
    }

    static const lost::common::Color transparentColor(0.0f, 0.0f, 0.0f, 0.0f);
    static const lost::common::Color whiteColor      (1.0f, 1.0f, 1.0f, 1.0f);
    static const lost::common::Color blackColor      (0.0f, 0.0f, 0.0f, 1.0f);
    static const lost::common::Color redColor        (1.0f, 0.0f, 0.0f, 1.0f);
    static const lost::common::Color greenColor      (0.0f, 1.0f, 0.0f, 1.0f);
    static const lost::common::Color blueColor       (0.0f, 0.0f, 1.0f, 1.0f);
    static const lost::common::Color yellowColor     (1.0f, 1.0f, 0.0f, 1.0f);
    static const lost::common::Color grayColor       (0.5f, 0.5f, 0.5f, 1.0f);

  }
}

#endif
