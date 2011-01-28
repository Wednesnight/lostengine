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
  
  // activating character metrics will add additional character rect information to the rendered mesh
  bool getCharacterMetrics();
  void setCharacterMetrics(bool v);
    
  uint32_t numLogicalLines();
  uint32_t numPhysicalLines();

  uint32_t numCharsInPhysicalLine(uint32_t lineIndex); // returns number of utf32 characters in that line

  void renderPhysicalLine(uint32_t num, const RenderedTextPtr& target);
  void renderAllPhysicalLines(const RenderedTextPtr& target);

  typedef fhtagn::text::utf32_string Utf32String;
  struct WordPos
  {
    WordPos() : size(0), pos(0), prevchar(0) {}
    float size; // size of the segment up to but not including the current char
    uint32_t pos; // the start or end of the current word
    uint32_t prevchar; // for kerning/advance calculation
  };

  struct Word
  {
    WordPos start;
    WordPos end;
    float size() {return end.size;}
  };

// =========================================
private:  
  
  
  // the original line data and it's split physical lines
  struct LogicalLine
  {
    LogicalLine(const Range& r) : line(r) {}
    Range line;
  };

  std::vector<LogicalLine>  _logicalLines;
  std::vector<Range>        _physicalLines;
  FontPtr                   _font;
  BreakMode                 _breakMode;
  Utf32String               _text;
  float                     _width;
  bool                      _characterMetrics;
  
  void resetLogicalLines(const std::string& inText);
  void resetPhysicalLines();
  void normaliseNewlines(std::string& inText);
  
  void breakModeNone();
  void breakModeChar();
  void breakModeWord();
  
  
  WordPos skipToWordStart(const WordPos& fromHere, const Range& lineRange);
  WordPos skipToWordEnd(const WordPos& fromHere, const Range& lineRange);
  Word skipToNextWord(const Word& curword, const Range& lineRange);
  
};
}
}

#endif