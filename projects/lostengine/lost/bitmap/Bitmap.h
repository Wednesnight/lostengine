#ifndef LOST_IMAGE_BITMAP_H
#define LOST_IMAGE_BITMAP_H

#include "lost/gl/gl.h"
#include <boost/shared_ptr.hpp>
#include "lost/resource/File.h"

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

      Bitmap();
      virtual ~Bitmap();

      void init(boost::shared_ptr<lost::resource::File> inFile);
      unsigned char* getPixels() { return data; }
    };

  } // namespace bitmap
} // namespace lost

#endif
