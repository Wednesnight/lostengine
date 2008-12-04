#ifndef LOST_COMMON_DISPLAYATTRIBUTES_H
#define LOST_COMMON_DISPLAYATTRIBUTES_H

#include <boost/cstdint.hpp>
#include <string>

namespace lost
{
  namespace common
  {

    struct DisplayAttributes
    {
      // FIXME: Everything uint16_t because Config can't handle uint8_t (TextFormat)
      boost::uint16_t    width;
      boost::uint16_t    height;
      boost::uint16_t    redbits;
      boost::uint16_t    bluebits;
      boost::uint16_t    greenbits;
      boost::uint16_t    alphabits;
      boost::uint16_t    depthbits;
      boost::uint16_t    stencilbits;
      bool        fullscreen;
      std::string title;

      static const boost::uint16_t defaultWidth = 640;
      static const boost::uint16_t defaultHeight = 480;
      static const boost::uint16_t defaultRedBits = 8;
      static const boost::uint16_t defaultBlueBits = 8;
      static const boost::uint16_t defaultGreenBits = 8;
      static const boost::uint16_t defaultAlphaBits = 8;
      static const boost::uint16_t defaultDepthBits = 32;
      static const boost::uint16_t defaultStencilBits = 0;
      static const bool defaultFullscreen = false;

      DisplayAttributes()
      {
        width = defaultWidth;
        height = defaultHeight;
        redbits = defaultRedBits;
        bluebits = defaultBlueBits;
        greenbits = defaultGreenBits;
        alphabits = defaultAlphaBits;
        depthbits = defaultDepthBits;
        stencilbits = defaultStencilBits;
        fullscreen = defaultFullscreen;
        title = "Application";
      }
    };
  }
}

#endif
