#include "lost/common/Color.h"

namespace lost
{
  namespace common
  {

    const lost::common::Color transparentColor(0.0f, 0.0f, 0.0f, 0.0f);
    const lost::common::Color whiteColor      (1.0f, 1.0f, 1.0f, 1.0f);
    const lost::common::Color blackColor      (0.0f, 0.0f, 0.0f, 1.0f);
    const lost::common::Color redColor        (1.0f, 0.0f, 0.0f, 1.0f);
    const lost::common::Color greenColor      (0.0f, 1.0f, 0.0f, 1.0f);
    const lost::common::Color blueColor       (0.0f, 0.0f, 1.0f, 1.0f);
    const lost::common::Color yellowColor     (1.0f, 1.0f, 0.0f, 1.0f);
    const lost::common::Color grayColor       (0.5f, 0.5f, 0.5f, 1.0f);


      Color::Color() {r(1.0f); g(1.0f); b(1.0f); a(1.0f); }

      Color::Color(float newr, float newg, float newb) {r(newr); g(newg); b(newb); a(1.0f); }
      Color::Color(float newr, float newg, float newb, float newa) {r(newr); g(newg); b(newb); a(newa); }

       float Color::r() const { return fv[0]; }
       float Color::g() const { return fv[1]; }
       float Color::b() const { return fv[2]; }
       float Color::a() const { return fv[3]; }

       void Color::r(float v) { fv[0] = v; }
       void Color::g(float v) { fv[1] = v; }
       void Color::b(float v) { fv[2] = v; }
       void Color::a(float v) { fv[3] = v; }

      // accessors for rgba values as usingned 8 bit integers
       uint8_t Color::ru8() const { return (uint8_t)(fv[0]*255); };
       uint8_t Color::gu8() const { return (uint8_t)(fv[1]*255); };
       uint8_t Color::bu8() const { return (uint8_t)(fv[2]*255); };
       uint8_t Color::au8() const { return (uint8_t)(fv[3]*255); };

       void Color::ru8(uint8_t v) { fv[0] = float(v)/255.0f; }
       void Color::gu8(uint8_t v) { fv[1] = float(v)/255.0f; }
       void Color::bu8(uint8_t v) { fv[2] = float(v)/255.0f; }
       void Color::au8(uint8_t v) { fv[3] = float(v)/255.0f; }


    lost::common::Color operator*(const Color& l, const Color& r)
    {
      lost::common::Color result;

      result.fv[0] = l.fv[0] * r.fv[0];
      result.fv[1] = l.fv[1] * r.fv[1];
      result.fv[2] = l.fv[2] * r.fv[2];
      result.fv[3] = l.fv[3] * r.fv[3];

      return result;
    }

    bool operator ==(const Color& l, const Color& r)
    {
      return (l.fv[0] == r.fv[0]) && (l.fv[1] == r.fv[1]) && (l.fv[2] == r.fv[2]) && (l.fv[3] == r.fv[3]);
    }
    
    bool operator !=(const Color& l, const Color& r)
    {
      return !(l==r);
    }
    
    void operator+=(Color& l, const Color& r)
    {
      l.fv[0] = std::min( 1.0f, l.fv[0] + r.fv[0] );
      l.fv[1] = std::min( 1.0f, l.fv[1] + r.fv[1] );
      l.fv[2] = std::min( 1.0f, l.fv[2] + r.fv[2] );
      l.fv[3] = std::min( 1.0f, l.fv[3] + r.fv[3] );
    }

    void operator-=(Color& l, const Color& r)
    {
      l.fv[0] = std::max( 0.0f, l.fv[0] - r.fv[0] );
      l.fv[1] = std::max( 0.0f, l.fv[1] - r.fv[1] );
      l.fv[2] = std::max( 0.0f, l.fv[2] - r.fv[2] );
      l.fv[3] = std::max( 0.0f, l.fv[3] - r.fv[3] );
    }

}
}