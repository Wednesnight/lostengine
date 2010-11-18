#ifndef LOST_FONT_FONT_H
#define LOST_FONT_FONT_H

namespace lost
{
namespace font
{
struct Font
{
  Font();
  virtual ~Font();
  
  uint32_t size;
};
}
}

#endif