#ifndef LOST_FONT_TRUETYPEFONT_H
#define LOST_FONT_TRUETYPEFONT_H

#include <boost/shared_ptr.hpp>

#include "lost/font/Font.h"

#include "lost/font/freetype/Face.h"
#include "lost/font/freetype/Glyph.h"
#include "lost/font/freetype/Library.h"

#include "lost/gl/Draw.h"
#include "lost/gl/DisplayList.h"
#include "lost/resource/File.h"
#include "lost/gl/PushAttrib.h"
#include "lost/math/Rect.h"

namespace lost
{
  namespace font
  {

    struct TrueTypeFont : public lost::font::Font
    {
    private:
      boost::shared_ptr<freetype::Library> mLibrary;
      boost::shared_ptr<freetype::Face>    mFace;
      boost::shared_ptr<gl::DisplayList>   mRenderBuffer;
      unsigned long                        mSize;

      void updateBuffer()
      {
        mFace->renderText( mSize, "asd", mRenderBuffer );
      }

    public:
      TrueTypeFont(const std::string& inPathToFile, unsigned long inSize)
        : mLibrary(new freetype::Library()),
          mFace(mLibrary->initFaceFromFile(inPathToFile)),
          mRenderBuffer(new lost::gl::DisplayList()),
          mSize(inSize)
      {
        DOUT("loading font: " << inPathToFile);
      }

      TrueTypeFont(const std::string& inPathToFile)
        : mLibrary(new freetype::Library()),
          mFace(mLibrary->initFaceFromFile(inPathToFile)),
          mRenderBuffer(new lost::gl::DisplayList())
      {
        DOUT("loading font: " << inPathToFile);
      }
      
      
      
      void render(float x, float y)
      {
        lost::gl::PushAttrib attrib(GL_ENABLE_BIT);GLDEBUG;
        glEnable(GL_BLEND);GLDEBUG;
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); GLDEBUG;
        glEnable(GL_TEXTURE_2D);GLDEBUG;
        glEnable(GL_SCISSOR_TEST);GLDEBUG;
        // translate the rendered string in the displaylist without destroying the original modelview matrix
        glMatrixMode(GL_MODELVIEW);GLDEBUG;
        glPushMatrix();GLDEBUG;
        glLoadIdentity();GLDEBUG;

        glTranslatef(x, y-mFace->rect().y, 0);GLDEBUG;
        mRenderBuffer->call();

        glPopMatrix(); // restore the original modelview
      }

      virtual unsigned long size()
      {
        return mSize;
      }
      virtual void size( unsigned long inSize )
      {
        mSize = inSize;
        updateBuffer();
      }
    };
  }
}

#endif
