#ifndef LOST_IMAGE_BITMAP_H
#define LOST_IMAGE_BITMAP_H

#include <boost/noncopyable.hpp>

namespace boost { template<typename T> class shared_ptr; }

namespace lost
{
  namespace resource{ struct File; };

  namespace bitmap
  {
    struct Bitmap : private boost::noncopyable
    {
      enum Components
      {
        COMPONENTS_UNDEFINED = 0, // for defined init, can't be used for anything
        COMPONENTS_ALPHA = 1,     // 8 bit
        COMPONENTS_RGB = 3,       // 24 bit
        COMPONENTS_RGBA = 4,      // 32 bit
        COMPONENTS_NUM
      };

    
      unsigned char*  data;   // points to the raw pixel data
      unsigned long   width;  // width in pixels
      unsigned long   height; // height in pixels
      Components      format; // format of bitmap as GL constant (e.g. rgb, rgba)

      Bitmap();
      Bitmap(unsigned long inWidth,
                unsigned long inHeight);
      Bitmap(boost::shared_ptr<lost::resource::File> inFile);
      void init();
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
    private:
      void destroy();
      bool loaded; // true if the image was loaded with the image library and data be freed by it.
                   // false if data is just a chunk of memory and can simply be deleted
    };

  } // namespace bitmap
} // namespace lost

#endif
