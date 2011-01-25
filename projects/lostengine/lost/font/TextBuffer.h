#ifndef LOST_FONT_TEXTBUFFER_H
#define LOST_FONT_TEXTBUFFER_H

#include "lost/font/forward.h"
#include "lost/font/BreakMode.h"
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

  void text(const std::string& inText); // resets the text with the given utf8 string, recalculates everything
  void font(const FontPtr inFont); // sets font, triggers recalculation of physical lines
  void breakMode(BreakMode inBreakMode); // sets the breakMode, triggers recalculation of physical lines
  
  uint32_t numLogicalLines();
  uint32_t numPhysicalLines();

// =========================================
private:  
  typedef fhtagn::text::utf32_string Utf32String;
  typedef lost::shared_ptr<Utf32String> Utf32StringPtr;
  
  // defines a range within a string that describes a physical line
  // same semantics as stdlib iterators
  struct Range
  {
    uint32_t begin;
    uint32_t end;
  };
  
  // substring of the original line, defined by pointer to source data and range
  struct PhysicalLine
  {
    Utf32StringPtr line;
    Range range;
  };
  typedef lost::shared_ptr<PhysicalLine> PhysicalLinePtr;

  // the original line data and it's split physical lines
  struct LogicalLine
  {
    Utf32StringPtr line;
    std::vector<PhysicalLinePtr> physicalLines;
  };
  typedef lost::shared_ptr<LogicalLine> LogicalLinePtr;

  std::vector<LogicalLinePtr> _logicalLines;
  std::vector<PhysicalLine>   _physicalLines;
  FontPtr                     _font;
  BreakMode                   _breakMode;
  
  void resetLogicalLines(const std::string& inText);
  void resetPhysicalLines();
  void splitAtNewline(const std::string& inText);
};
}
}

#endif