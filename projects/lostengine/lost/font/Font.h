#ifndef LOST_FONT_FONT_H
#define LOST_FONT_FONT_H

#include <string>

#include "lost/object/Object.h"
#include "lost/common/Color.h"

namespace lost
{
  namespace font
  {

    /** base class for any 2D font thats drawn to screen without any additional transformation.
     *
     */
    struct Font : public object::Object
    {
      protected:
        common::Color mColor;
        std::string   mText;

    public:
      Font() : mColor(common::whiteColor), mText("") {}
      virtual ~Font() {}
      virtual void render( float x, float y ) = 0;

      virtual unsigned long size() = 0;
      virtual void size( unsigned long inSize ) = 0;

      common::Color color()
      {
        return mColor;
      }
      void color( const common::Color& inColor )
      {
        mColor = inColor;
      }

      std::string text()
      {
        return mText;
      }
      void text( const std::string& inText )
      {
        mText = inText;
      }

      virtual float textWidth() = 0;
      virtual float textHeight() = 0;
    };

  }
}
#endif
