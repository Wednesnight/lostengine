#include "lost/font/TextBuffer.h"
#include "lost/font/Font.h"
#include <boost/algorithm/string.hpp>
#include "lost/common/Logger.h"
#include "lost/font/Render.h"
#include "lost/font/Glyph.h"
#include "utf8.h"

namespace lost
{
namespace font
{

uint32_t skipToNewlineOrEnd(uint32_t pos, const lost::font::TextBuffer::Utf32String& txt)
{
  if(pos < txt.size())
  {
    while((txt[pos] != '\n') && (pos < txt.size()))
    {
      ++pos;
    }
  }
  return pos;
}


TextBuffer::TextBuffer()
{
  _breakMode = BREAKMODE_NONE;
  _characterMetrics = false;
  _align = 0;
  _dirty = true;
}

TextBuffer::~TextBuffer()
{
}

void TextBuffer::text(const string& inUtf8String)
{
  _dirty = true;
  _logicalLines.clear(); // discard all previous lines
  _physicalLines.clear();    
  // normalise newlines into copy of text
  string normalised(inUtf8String); 
  normaliseNewlines(normalised);
  _text.clear();
  // transcode to utf32 for parsing
  utf8::utf8to32(normalised.begin(), normalised.end(), back_inserter(_text));  
}

string TextBuffer::utf8String()
{
  string result;

  utf8::utf32to8(_text.begin(), _text.end(), back_inserter(result));  
         
  return result;
}

string TextBuffer::substring(uint32_t fromLine, uint32_t fromIndex, uint32_t toLine, uint32_t toIndex)
{
  string result;
  if (fromLine < _physicalLines.size() && toLine < _physicalLines.size())
  {
    uint32_t f;
    uint32_t t;
    if (fromLine < toLine || (fromLine == toLine && fromIndex < toIndex)) {
      f = _physicalLines[fromLine].begin + fromIndex;
      t = _physicalLines[toLine].begin + toIndex;
    }
    else {
      f = _physicalLines[toLine].begin + toIndex;
      t = _physicalLines[fromLine].begin + fromIndex;
    }
    if (f < _text.size() && t > 0 && t <= _text.size()) {
      utf8::utf32to8(_text.begin()+f, _text.begin()+t, back_inserter(result));
    }
  }
  return result;
}

void TextBuffer::font(const FontPtr& inFont) 
{
  _dirty = true;
  _font = inFont;
}

void TextBuffer::breakMode(BreakMode inBreakMode) 
{
  _dirty = true;
  _breakMode = inBreakMode;
}

void TextBuffer::width(float inWidth) 
{
  _dirty = true;
  _width = inWidth;
}

void TextBuffer::reset()
{
  if(!_dirty)
  {
//    DOUT("!!!! skipping reset because not dirrty");
    return;
  }
  _dirty = false;
  _logicalLines.clear(); // discard all previous lines
  _physicalLines.clear();
  
  uint32_t pos = 0;
  uint32_t nextPos = 0;
  do {
    pos = nextPos;
    nextPos = skipToNewlineOrEnd(pos, _text);
    if(nextPos != pos)
    {
      _logicalLines.push_back(LogicalLine(Range(pos, nextPos)));
      ++nextPos;
    }
    else {
      // must check for empty line
      if(nextPos < _text.size())
      {
        _logicalLines.push_back(LogicalLine(Range(pos, pos)));
        ++nextPos;
      }
    }
  } while (pos != nextPos);
  
  resetPhysicalLines();
}

uint32_t TextBuffer::numLogicalLines()
{
  return _logicalLines.size();
}

uint32_t TextBuffer::numPhysicalLines()
{
  return _physicalLines.size();
}

void TextBuffer::normaliseNewlines(string& inText)
{
  #ifndef LOST_USE_EASTL
    boost::algorithm::ireplace_all(inText, "\r\n", "\n");
    boost::algorithm::ireplace_all(inText, "\r", "\n");
  #else
    // FIXME disabled for use with eastl
  #endif
}

void TextBuffer::breakModeNone()
{
  for(vector<LogicalLine>::iterator pos=_logicalLines.begin(); pos!=_logicalLines.end(); ++pos)
  {
    _physicalLines.push_back(pos->line);
  }
}

void TextBuffer::breakModeChar()
{
  for(vector<LogicalLine>::iterator line=_logicalLines.begin(); line!=_logicalLines.end(); ++line)
  {
    Range& lr = line->line;
    if(lr.begin == lr.end) // skip empty lines
    {
      _physicalLines.push_back(lr); // doesn't matter what we push here as long as begin == end
      continue;
    }
    uint32_t prevChar = 0;
    float sum = 0.0f;
    Range curRange;
    for(uint32_t i=lr.begin; i<lr.end; ++i)
    {
      uint32_t curChar = _text[i];
      float adv = _font->characterAdvance(prevChar, curChar);
      prevChar = curChar;
      if((sum+adv)<_width)
      {
        curRange.end++;
        sum+=adv;
        if(i == lr.end-1)
        {
          _physicalLines.push_back(Range(lr.begin+curRange.begin, lr.begin+curRange.end));
        }
      }
      else
      {
        _physicalLines.push_back(Range(lr.begin+curRange.begin, lr.begin+curRange.end));
        curRange.begin = curRange.end;
        curRange.end++;
        prevChar = 0;
        sum = _font->characterAdvance(prevChar, curChar);
        // if the trigger was the last character, we have to add it since the loop will now finish and it would be dropped
        if(i == lr.end-1)
        {
          _physicalLines.push_back(Range(lr.begin+curRange.begin, lr.begin+curRange.end));            
        }
      }
    }
  }
}

bool isWhiteSpace(uint32_t c)
{
  bool result = false;
  switch(c)
  {
    case ' ':
    case '\t':
    case '\n':
      result=true;
      break;
    default:
      result=false;
  }
  return result;
}

TextBuffer::WordPos TextBuffer::skipToWordStart(const WordPos& fromHere, const Range& lineRange)
{
  WordPos result = fromHere;
  while(isWhiteSpace(_text[result.pos]) && (result.pos<lineRange.end))
  {
    result.size += _font->characterAdvance(result.prevchar, _text[result.pos]);
    ++result.pos;
  }
  return result;
}

TextBuffer::WordPos TextBuffer::skipToWordEnd(const WordPos& fromHere, const Range& lineRange)
{
  WordPos result = fromHere;
  while(!isWhiteSpace(_text[result.pos]) && (result.pos<lineRange.end))
  {
    result.size += _font->characterAdvance(result.prevchar, _text[result.pos]);
    ++result.pos;
  }
  return result;
}

// assuming skip was started in range and we only need to check end
bool inRange(const TextBuffer::WordPos& pos, const Range& r)
{
  return pos.pos < r.end;
}

bool inRange(const TextBuffer::Word& w, const Range& r)
{
  return inRange(w.start, r) && inRange(w.end, r);
}

TextBuffer::Word TextBuffer::skipToNextWord(const Word& curword, const Range& lineRange)
{
  Word result;
  
  result.start = skipToWordStart(curword.end, lineRange);
  result.end = skipToWordEnd(result.start, lineRange);
  
  return result;
}

bool terminalWord(const TextBuffer::Word& w, const Range& r)
{
  return ((w.start.pos == r.end) && (w.end.pos == r.end));
}

void TextBuffer::breakModeWord()
{
  for(vector<LogicalLine>::iterator line=_logicalLines.begin(); line!=_logicalLines.end(); ++line)
  {
    Range& lr = line->line;
    if(lr.begin == lr.end) // skip empty lines
    {
      _physicalLines.push_back(lr); // doesn't matter what we push here as long as begin == end
      continue;
    }
  
    uint32_t segmentstart = lr.begin;
    Word prevword;
    prevword.start.pos = lr.begin;
    prevword.end.pos = lr.begin;
    Word curword;
    do {
      curword = skipToNextWord(prevword, lr);
      if((curword.size() > _width) || (terminalWord(curword, lr)))
      {
        _physicalLines.push_back(Range(segmentstart, prevword.end.pos));
        segmentstart = curword.start.pos;
        curword.end.size -= curword.start.size; // reset size for further line measurement
        curword.start.size = 0;
      }
      prevword = curword;
    } while (!terminalWord(curword, lr));

  }
}

void TextBuffer::resetPhysicalLines()
{
  _physicalLines.clear();
//  DOUT("using breakmode "<<_breakMode);
  switch(_breakMode)
  { 
    case BREAKMODE_NONE:breakModeNone();break;
    case BREAKMODE_CHAR:breakModeChar();break;
    case BREAKMODE_WORD:breakModeWord();break;
  }
}

void TextBuffer::renderPhysicalLine(uint32_t num, const RenderedTextPtr& target)
{
  render(_text, _physicalLines[num], _font, target, _characterMetrics, _align);
}

void TextBuffer::renderAllPhysicalLines(const RenderedTextPtr& target)
{
  render(_text, _physicalLines, _font, target, _characterMetrics, _align);
}

bool TextBuffer::getCharacterMetrics()
{
  return _characterMetrics;
}

void TextBuffer::setCharacterMetrics(bool v)
{
  _characterMetrics = v;
}

uint32_t TextBuffer::numCharsInPhysicalLine(uint32_t lineIndex)
{
  uint32_t result = 0;
  if(lineIndex < _physicalLines.size())
  {
    const Range& r = _physicalLines[lineIndex];
    result = r.end - r.begin;
  }
  return result;
}

void TextBuffer::insertUtf8StringAtPosition(uint32_t lineIndex, uint32_t charIndex, const string& inString)
{
  Utf32String decoded;
  utf8::utf8to32(inString.begin(), inString.end(),
         back_inserter(decoded));

  Range r;
  if(lineIndex < _physicalLines.size())
  {
    r = _physicalLines[lineIndex];
  }
  _text.insert(r.begin+charIndex, decoded);
  if (_physicalLines.size() == 0 || inString.find("\r") != string::npos || inString.find("\n") != string::npos || inString.find("\r\n") != string::npos) {
    _dirty = true;
    reset();
  }
  else {
    // update lines
    _physicalLines[lineIndex].end += decoded.size();
    for(vector<Range>::iterator r = _physicalLines.begin()+lineIndex+1; r != _physicalLines.end(); ++r) {
      r->begin += decoded.size();
      r->end += decoded.size();
    }
  }
}

void TextBuffer::eraseCharAtPosition(uint32_t lineIndex, uint32_t charIndex)
{
  eraseChars(lineIndex, charIndex, lineIndex, charIndex+1);
}

void TextBuffer::eraseChars(uint32_t fromLine, uint32_t fromIndex, uint32_t toLine, uint32_t toIndex)
{
  if (fromLine < _physicalLines.size() && toLine < _physicalLines.size())
  {
    uint32_t f, t, d;
    if (fromLine < toLine || (fromLine == toLine && fromIndex < toIndex)) {
      f = _physicalLines[fromLine].begin + fromIndex;
      t = _physicalLines[toLine].begin + toIndex;
      d = toLine - fromLine;
    }
    else {
      f = _physicalLines[toLine].begin + toIndex;
      t = _physicalLines[fromLine].begin + fromIndex;
      d = fromLine - toLine;
    }
    if (f < _text.size() && t > 0 && t <= _text.size()) {
      _text.erase(f, t-f);
      if (d > 0) {
        _dirty = true;
        reset();
      }
    }
  }
}

int TextBuffer::getAlign()
{
  return _align;
}

void TextBuffer::setAlign(int v)
{
  _align = v;
}
  
}
}

