#ifndef LOST_COMMON_COLOR_H
#define LOST_COMMON_COLOR_H

namespace lost
{
  namespace common
  {

    struct Color
    {
      Color();
      Color(float newr, float newg, float newb);
      Color(float newr, float newg, float newb, float newa);

       float r() const;
       float g() const;
       float b() const;
       float a() const;

       void r(float v);
       void g(float v);
       void b(float v);
       void a(float v);

      // accessors for rgba values as usingned 8 bit integers
       uint8_t ru8() const;
       uint8_t gu8() const;
       uint8_t bu8() const;
       uint8_t au8() const;

       void ru8(uint8_t v);
       void gu8(uint8_t v);
       void bu8(uint8_t v);
       void au8(uint8_t v);
      
      float fv[4];

      Color premultiplied(); // creates a new color object from the current one with color values premultiplied with alpha
    };


    lost::common::Color operator*(const Color& l, const Color& r);
    lost::common::Color operator*(const Color& l, float);
    bool operator ==(const Color& l, const Color& r);
    bool operator !=(const Color& l, const Color& r);
    void operator+=(Color& l, const Color& r);
    void operator-=(Color& l, const Color& r);
    void operator/=(Color& l, float);

    extern const lost::common::Color transparentColor;
    extern const lost::common::Color whiteColor      ;
    extern const lost::common::Color blackColor      ;
    extern const lost::common::Color redColor        ;
    extern const lost::common::Color greenColor      ;
    extern const lost::common::Color blueColor       ;
    extern const lost::common::Color yellowColor     ;
    extern const lost::common::Color grayColor       ;

  }
}

#endif
