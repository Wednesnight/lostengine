#ifndef LOST_GUIRO_LABEL_H
#define LOST_GUIRO_LABEL_H

#include <boost/shared_ptr.hpp>

#include "lost/guiro/control/View.h"
#include "lost/gl/Scissor.h"
#include "lost/font/Font.h"
#include "lost/common/Color.h"

#include "lost/object/ObjectReference.h"
#include "lost/object/ObjectReferenceResolver.h"

namespace lost
{
  namespace guiro
  {
    namespace control
    {
    struct Label : public View
    {
    private:
      std::string                               mCaption;
      object::ObjectReference<lost::font::Font> mFont;
      lost::common::Color                       mFontColor;
      unsigned long                             mFontSize;
      bool                                      mDirty;

    public:
      Label( const std::string& inName )
        : View( inName ),
          mFontColor(lost::common::whiteColor),
          mFontSize(12),
          mDirty(true)
      {
      }

      virtual ~Label() {}

      virtual void draw(const lost::math::Rect& parentRect, const lost::common::Color& parentColor )
      {
        if (getVisible())
        {
          if (mDirty)
          {
            mFont->color(mFontColor);
            mFont->size(mFontSize);
            mFont->text(mCaption);
            mDirty = false;

            DOUT( "font texture has size: " << mFont->textWidth() << "x" << mFont->textHeight() );
          }

          lost::math::Rect clippedRect = rect;
          clippedRect.clipTo( parentRect );

          lost::common::Color currentColor = mFontColor * parentColor;
          lost::gl::Scissor scissor( clippedRect );

          mFont->render(clippedRect.x, clippedRect.y);
        }
      }

      virtual void resolve( object::ObjectReferenceResolver& inResolver )
      {
        mFont.resolve( inResolver );
        View::resolve( inResolver );
      }

      std::string& caption()
      {
        return mCaption;
      }
      void caption( const std::string& inCaption )
      {
        mCaption = inCaption;
        mDirty   = true;
      }

      object::ObjectReference<lost::font::Font>& font()
      {
        return mFont;
      }

      lost::common::Color& fontColor()
      {
        return mFontColor;
      }
      void fontColor( const lost::common::Color& inColor )
      {
        mFontColor = inColor;
        mDirty     = true;
      }

      unsigned long& fontSize()
      {
        return mFontSize;
      }
      void fontSize(unsigned long inFontSize)
      {
        mFontSize = inFontSize;
        mDirty    = true;
      }

    };
    }
  }
}

#endif
