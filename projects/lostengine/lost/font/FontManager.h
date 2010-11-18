#ifndef LOST_FONT_FONTMANAGER_H
#define LOST_FONT_FONTMANAGER_H

#include "lost/font/forward.h"

namespace lost
{
namespace font
{
struct FontManager
{
  FontManager();
  ~FontManager();
  
  static FontManagerPtr create();
};
}
}

#endif