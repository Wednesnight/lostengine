#ifndef LOST_IMAGE_BITMAP_H
#define LOST_IMAGE_BITMAP_H

#include "stb_image.h"
#include "lost/common/Logger.h"
#include "lost/platform/Platform.h"
#include "lost/gl/gl.h"

#include "lost/common/SerializationHelper.h"
#include "stb_image.h"

namespace lost
{
  namespace bitmap
  {

    struct Bitmap
    {
      unsigned char*  data;
      unsigned long   width;
      unsigned long   height;
      GLenum          format;
      GLenum          type;

      Bitmap()
      {
        data = NULL;
      }

      ~Bitmap()
      {
        if(data)
        {
          stbi_image_free(data);
        }
      }

      unsigned char* getPixels()
      {
        return data;
      }

      void render(float x, float y)
      {
        if (data)
        {
          glRasterPos2f(x, y);
          glDrawPixels( width, height, format, type, getPixels());
        }
      }
    };

  } // namespace bitmap
} // namespace lost

#endif
