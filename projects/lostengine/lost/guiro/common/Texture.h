#ifndef LOST_GUIRO_COMMON_TEXTURE
#define LOST_GUIRO_COMMON_TEXTURE

#include "lost/object/Object.h"
#include "lost/object/ObjectReference.h"
#include "lost/object/ObjectReferenceContainer.h"
#include "lost/object/ObjectReferenceResolver.h"
#include "lost/common/Logger.h"
#include "lost/gl/gl.h"
#include "lost/bitmap/Bitmap.h"

#include <boost/noncopyable.hpp>

namespace lost
{
  namespace guiro
  {
    namespace common
    {

    struct Texture : public lost::object::Object,
                     public lost::object::ObjectReferenceContainer,
                     private boost::noncopyable
    {
      // serializable
      lost::object::ObjectReference<lost::bitmap::Bitmap> bitmap;

      // non-serializable
      GLuint      glTexture;

      Texture()
      {
        glTexture = 0;
      }

      virtual ~Texture()
      {
        deleteGlTexture();
      }

      void deleteGlTexture()
      {
        if(glTexture != 0)
        {
          DOUT("deleting gl texture: " << glTexture << " : " << id);
          glDeleteTextures(1, &glTexture);
          glTexture = 0;
        }
      }
      
      virtual void resolve( object::ObjectReferenceResolver& inResolver )
      {
        bitmap.resolve( inResolver );
      }

      void resolveFinished()
      {
        DOUT("creating from bitmap "<<id);
        createFromBitmap();        
      }
      
    private:
      // reads required attributes from existing bitmap and creates a gl texture,
      // deleting the existing one
      void createFromBitmap()
      {
        if(!bitmap.pointer) // FIXME: implement * and bool operators for ObjectReference
        {
          WOUT("called init for texture " << id << " but bitmap was NULL");
        }
        else
        {
          deleteGlTexture();

          // FIXME: make texture parameters adjustable/serializable
          glGenTextures(1, &glTexture);
          glBindTexture(GL_TEXTURE_2D, glTexture);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
          int clampparam = GL_CLAMP;
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clampparam);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clampparam);
          glTexImage2D(GL_TEXTURE_2D,
                       0,
                       GL_RGBA,
                       bitmap->width,
                       bitmap->height,
                       0,
                       bitmap->format,
                       bitmap->type,
                       bitmap->getPixels() );

        }
      }
    };
    }
  }
}

#endif
