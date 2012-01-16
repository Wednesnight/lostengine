#ifndef LOST_FONT_BREAKMODE_H
#define LOST_FONT_BREAKMODE_H

namespace lost
{
  namespace font
  {
      // describes the possible modes text is wrapped around when rendered by a font
      enum BreakMode
      {
        BREAKMODE_NONE = 0,// text won't be modified during rendering
        BREAKMODE_CHAR,    // text will be broken into new lines at character level
        BREAKMODE_WORD     // text will be broken into new lines at word level
      };
  }
}

#endif