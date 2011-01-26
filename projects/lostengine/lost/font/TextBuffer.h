#ifndef LOST_FONT_TEXTBUFFER_H
#define LOST_FONT_TEXTBUFFER_H

#include <string>
#include "lost/font/forward.h"
#include "lost/font/BreakMode.h"
#include "lost/font/Range.h"
#include <vector>

namespace lost
{
namespace font
{
struct TextBuffer
{
public:
  // --------------------
  TextBuffer();
  ~TextBuffer();

  void reset(const std::string& utf8String, 
             const FontPtr& inFont,
             BreakMode inBreakMode,
             float width);
  
  uint32_t numLogicalLines();
  uint32_t numPhysicalLines();

  void renderPhysicalLine(uint32_t num, const RenderedTextPtr& target);
  void renderAllPhysicalLines(const RenderedTextPtr& target);

  typedef fhtagn::text::utf32_string Utf32String;

// =========================================
private:  
  
  
  // the original line data and it's split physical lines
  struct LogicalLine
  {
    LogicalLine(const Range& r) : line(r) {}
    Range line;
    std::vector<Range> physicalLines;
  };

  std::vector<LogicalLine>  _logicalLines;
  std::vector<Range>        _physicalLines;
  FontPtr                   _font;
  BreakMode                 _breakMode;
  Utf32String               _text;
  float                     _width;
  
  void resetLogicalLines(const std::string& inText);
  void resetPhysicalLines();
  void normaliseNewlines(std::string& inText);
};
}
}

#endif