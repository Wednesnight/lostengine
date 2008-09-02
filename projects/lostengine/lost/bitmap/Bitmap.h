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

      /** inits a 32bit rgba bitmap with the given width and height.
       * The contents are undefined.
       */
      void init(unsigned long inWidth,
                unsigned long inHeight);

      /** attempts to interpret a chunk ofmemory as a bitmap, with the help of the underlying image library.
       * An exception is thrown if an error occurs.
       */
      void init(boost::shared_ptr<lost::resource::File> inFile);
      unsigned char* getPixels() { return data; }
    private:
      bool loaded; // true if the image was loaded with the image library and data be freed by it.
                   // false if data is just a chunk of memory and can simply be deleted
      Bitmap(const Bitmap& inBitmap) {}; // don't copy, manage it in smart pointer
    };

  } // namespace bitmap
} // namespace lost

#endif
