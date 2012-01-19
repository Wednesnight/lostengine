#ifndef LOST_FONT_TEXTBUFFER_H
#define LOST_FONT_TEXTBUFFER_H

#include "lost/font/forward.h"
#include "lost/font/BreakMode.h"
#include "lost/font/Range.h"

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

  bool _dirty;
  void text(const string& inUtf8String);
  string utf8String(); // returns the current data as utf8 string
  string substring(uint32_t fromLine, uint32_t fromIndex, uint32_t toLine, uint32_t toIndex);
  void font(const FontPtr& inFont);
  void breakMode(BreakMode inBreakMode);
  void width(float inWidth);
  void reset();
  
  // activating character metrics will add additional character rect information to the rendered mesh
  bool getCharacterMetrics();
  void setCharacterMetrics(bool v);

  // horizontal align
  // 0 = left (default)
  // 1 = center
  // 2 = right
  int getAlign();
  void setAlign(int v);

  uint32_t numLogicalLines();
  uint32_t numPhysicalLines();

  uint32_t numCharsInPhysicalLine(uint32_t lineIndex); // returns number of utf32 characters in that line

  void renderPhysicalLine(uint32_t num, const RenderedTextPtr& target);
  void renderAllPhysicalLines(const RenderedTextPtr& target);

  void insertUtf8StringAtPosition(uint32_t lineIndex, uint32_t charIndex, const string& inString);
  void eraseCharAtPosition(uint32_t lineIndex, uint32_t charIndex);
  void eraseChars(uint32_t fromLine, uint32_t fromIndex, uint32_t toLine, uint32_t toIndex);

  typedef text::utf32_string Utf32String;
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

  vector<LogicalLine>  _logicalLines;
  vector<Range>        _physicalLines;
  FontPtr                   _font;
  BreakMode                 _breakMode;
  Utf32String               _text;
  float                     _width;
  bool                      _characterMetrics;
  int                       _align;
  
  void resetLogicalLines(const string& inText);
  void resetPhysicalLines();
  void normaliseNewlines(string& inText);
  
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