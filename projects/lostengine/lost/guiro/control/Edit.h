#ifndef LOST_GUIRO_CONTROL_EDIT_H
#define LOST_GUIRO_CONTROL_EDIT_H

#include <sstream>
#include <boost/shared_ptr.hpp>

#include "lost/guiro/control/View.h"
#include "lost/gl/Scissor.h"
#include "lost/font/Font.h"
#include "lost/common/Color.h"
#include "lost/bitmap/Bitmap.h"

#include "lost/object/ObjectReference.h"
#include "lost/object/ObjectReferenceResolver.h"

namespace lost
{
  namespace guiro
  {
    namespace control
    {
    struct Edit : public View
    {
      std::string                                   text;
      object::ObjectReference<lost::bitmap::Bitmap> background;
      object::ObjectReference<lost::font::Font>     font;
      lost::common::Color                             fontColor;
      unsigned long                                 fontSize;
      bool                                          caretVisible;
      float                                         caretDelay;
      float                                         caretTimer;

      Edit( const std::string& inName )
      : View( inName ),
        fontColor(lost::common::blackColor),
        fontSize(10),
        caretVisible(false),
        caretDelay(.75),
        caretTimer(0)
      {
      }

      virtual ~Edit() {}

      virtual void draw(const lost::math::Rect& parentRect, const lost::common::Color& parentColor )
      {
        if (getVisible())
        {
          lost::math::Rect clippedRect = rect;
          clippedRect += parentRect.pos();
          clippedRect.clipTo( parentRect );

          lost::common::Color currentColor     = color * parentColor;
          lost::common::Color currentFontColor = fontColor * parentColor;
          lost::gl::Scissor scissor( clippedRect );

          glColor4fv( currentColor.fv );
          background->render( clippedRect.x, clippedRect.y );

          font->text( text );
          std::string shortText(text);
          float       fontWidth = font->textWidth();
          while (fontWidth > rect.width - 5 && shortText.size() > 0)
          {
            shortText.erase(0,1);
            fontWidth = font->textWidth();
          }
          glColor4fv( currentFontColor.fv );
          font->size( fontSize );
          font->text( shortText );
          font->render( clippedRect.x + 5, clippedRect.y + 5 );

          if (focused && caretVisible)
          {
            glColor4fv( currentFontColor.fv );
            glBegin(GL_LINES);
              glVertex2f( clippedRect.x + fontWidth + 5, clippedRect.y + 5 );
              glVertex2f( clippedRect.x + fontWidth + 5, clippedRect.y + rect.height - 5 );
            glEnd();
          }
        }
        View::draw( parentRect, parentColor );
      }

      virtual void update( double timeSinceLastCallSec )
      {
        caretTimer += timeSinceLastCallSec;
        if (caretTimer >= caretDelay)
        {
          caretTimer   = 0;
          caretVisible = !caretVisible;
        }

        View::update( timeSinceLastCallSec );
      }

      virtual void keyboardEvent( int inKey, bool inPressed )
      {
        if (inPressed && inKey >= 32 && inKey <= 126)
        {
          std::ostringstream stream;
          stream << text << (char)inKey;
          text = stream.str();
        }
      }

      virtual void resolve( object::ObjectReferenceResolver& inResolver )
      {
        background.resolve( inResolver );
        font.resolve( inResolver );
        View::resolve( inResolver );
      }

      lost::common::Color getFontColor()
      {
        return fontColor;
      }
      void setFontColor( const lost::common::Color& inColor )
      {
        fontColor = inColor;
      }

      unsigned long getFontSize()
      {
        return fontSize;
      }
      void setFontSize(unsigned long inFontSize)
      {
        fontSize = inFontSize;
      }

      std::string getText()
      {
        return text;
      }
      void setText( const std::string& inText )
      {
        text = inText;
      }

    };
    }
  }
}

#endif
