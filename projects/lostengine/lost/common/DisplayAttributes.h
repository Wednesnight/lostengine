#ifndef LOST_COMMON_DISPLAYATTRIBUTES_H
#define LOST_COMMON_DISPLAYATTRIBUTES_H

#include <stdint.h>
#include "lost/common/Config.h"

namespace lost
{
  namespace common
  {

    struct DisplayAttributes
    {
      // FIXME: Everything uint16_t because Config can't handle uint8_t (TextFormat)
      uint16_t    width;
      uint16_t    height;
      uint16_t    redbits;
      uint16_t    bluebits;
      uint16_t    greenbits;
      uint16_t    alphabits;
      uint16_t    depthbits;
      uint16_t    stencilbits;
      bool        fullscreen;
      std::string title;

      static const uint16_t defaultWidth = 640;
      static const uint16_t defaultHeight = 480;
      static const uint16_t defaultRedBits = 8;
      static const uint16_t defaultBlueBits = 8;
      static const uint16_t defaultGreenBits = 8;
      static const uint16_t defaultAlphaBits = 8;
      static const uint16_t defaultDepthBits = 32;
      static const uint16_t defaultStencilBits = 0;
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

      DisplayAttributes(lost::common::Config& config)
      {
        width       = config.get<uint16_t>("screen.width",defaultWidth);
        height      = config.get<uint16_t>("screen.height",defaultHeight);
        redbits     = config.get<uint16_t>("screen.redbits",defaultRedBits);
        bluebits    = config.get<uint16_t>("screen.bluebits",defaultBlueBits);
        greenbits   = config.get<uint16_t>("screen.greenbits",defaultGreenBits);
        alphabits   = config.get<uint16_t>("screen.alphabits",defaultAlphaBits);
        depthbits   = config.get<uint16_t>("screen.depthbits",defaultDepthBits);
        stencilbits = config.get<uint16_t>("screen.stencilbits",defaultStencilBits);
        fullscreen  = config.get<bool>("screen.fullscreen",defaultFullscreen);
        title       = config.get<std::string>("screen.title","Application");
      }
    };

  }
}

#endif
