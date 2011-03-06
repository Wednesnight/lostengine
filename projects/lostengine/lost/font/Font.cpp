#include "lost/font/Font.h"
#include "lost/profiler/Blackbox.h"

namespace lost
{
namespace font
{

#define bb_font_key "lost.font.Font"

Font::Font()
{
  BB_INC(bb_font_key);
}

Font::~Font()
{
  BB_DEC(bb_font_key);
}

}
}