#ifndef LOST_FONT_BITMAPFONT_H
#define LOST_FONT_BITMAPFONT_H

#include "lost/font/Font.h"
#include "lost/gl/gl.h"

// FIXME: rename this to BuiltinFont


namespace lost
{
  namespace font
  {

    /** this class draws the string a simple bitmapped character font at the specified position.
     * Use the GLUT constants to select an appropriate font.
     * Depending on your GLUT implementation, these might be the allowed constants for 'font':
     * GLUT_BITMAP_9_BY_15
     * GLUT_BITMAP_8_BY_13
     * GLUT_BITMAP_TIMES_ROMAN_10
     * GLUT_BITMAP_TIMES_ROMAN_24
     * GLUT_BITMAP_HELVETICA_10
     * GLUT_BITMAP_HELVETICA_12
     * GLUT_BITMAP_HELVETICA_18
     */
    struct BitmapFont : public lost::font::Font
    {
    private:
      void* font;

      virtual void size( unsigned long inSize )
      {
        // N/A
      }

    public:
      /** Constructor
       * @param font GLUT builtin bitmap font constant. On Mac, this is a void*
       */
      BitmapFont(void* inFont)
      : font(inFont)
      {
        id = "BitmapFont";
      }

      void render(float x, float y)
      {
        int len, i;
        glRasterPos2f(x, y);
        glColor4fv(mColor.fv);
        len = mText.size();
        for (i = 0; i < len; i++)
        {
          glutBitmapCharacter(font, mText[i]);
        }
      }

      virtual unsigned long size()
      {
        unsigned long result = 0;

        if (font == GLUT_BITMAP_9_BY_15)
          result = 15;
        else if (font == GLUT_BITMAP_8_BY_13)
          result = 13;
        else if (font == GLUT_BITMAP_TIMES_ROMAN_10)
          result = 10;
        else if (font == GLUT_BITMAP_TIMES_ROMAN_24)
          result = 24;
        else if (font == GLUT_BITMAP_HELVETICA_10)
          result = 10;
        else if (font == GLUT_BITMAP_HELVETICA_12)
          result = 12;
        else if (font == GLUT_BITMAP_HELVETICA_18)
          result = 18;

        return result;
      }

      virtual float textWidth()
      {
        float result = 0;

        for (unsigned int idx = 0; idx < mText.size(); ++idx)
          result += glutBitmapWidth( font, mText[idx] );

        return result;
      }
      virtual float textHeight()
      {
        float result;

          (font == GLUT_BITMAP_8_BY_13)        ? result = 13
        : (font == GLUT_BITMAP_9_BY_15)        ? result = 15
        : (font == GLUT_BITMAP_TIMES_ROMAN_10) ? result = 10
        : (font == GLUT_BITMAP_TIMES_ROMAN_24) ? result = 24
        : (font == GLUT_BITMAP_HELVETICA_10)   ? result = 10
        : (font == GLUT_BITMAP_HELVETICA_12)   ? result = 12
        : (font == GLUT_BITMAP_HELVETICA_18)   ? result = 18
        : result = 0;

        return result;
      }
    };

  }
}

#endif
