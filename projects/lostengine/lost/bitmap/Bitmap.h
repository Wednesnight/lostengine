#ifndef LOST_IMAGE_BITMAP_H
#define LOST_IMAGE_BITMAP_H

#include <boost/noncopyable.hpp>
#include <stdint.h>

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
      
      uint8_t*    data;   // points to the raw pixel data
      uint32_t    width;  // width in pixels
      uint32_t    height; // height in pixels
      Components  format; // format of bitmap as GL constant (e.g. rgb, rgba)

      Bitmap();
      Bitmap(uint32_t inWidth,
             uint32_t inHeight,
             Components destComponents,
             Components srcComponents,
             uint8_t* data);
      Bitmap(boost::shared_ptr<lost::resource::File> inFile);
      void reset();
      virtual ~Bitmap();

      /** inits a 32bit rgba bitmap with the given width and height.
       * The contents are undefined.
       */
      void init(uint32_t inWidth,
                uint32_t inHeight,
                Components destComponents,
                Components srcComponents,
                uint8_t* srcData);

      /** attempts to interpret a chunk ofmemory as a bitmap, with the help of the underlying image library.
       * An exception is thrown if an error occurs.
       */
      void init(boost::shared_ptr<lost::resource::File> inFile);
      
      /** calculates the number of pixels for a given bitmap components constant.
       */
      static uint32_t bytesPerPixelFromComponents(Components components);

      /** copies exctaly one pixel from src to dest, converting the format 
       * from srcComponents to destComponents.
       *
       * Conversion is as follows:
       * alpha to rgb/rgba: the alpha value is copied to all r/g/b/a components, 
       *                    resulting in a white picture that fades where its alpha
       *                    fades.
       * rgb to rgba:       rgb components are copied, alpha is set to 1
       * rgba to rgb:       alpha value is omitted
       * rgb to alpha:      alpha is set 1 
       * rgba to alpha:     only the alpha channel is copied
       */
      static void copyPixel(uint8_t* dest,
                            Components destComponents, 
                            uint8_t* src,
                            Components srcComponents);
    private:
      void destroy();
      bool loaded; // true if the image was loaded with the image library and data must be freed by it.
                   // false if data is just a chunk of memory and can simply be deleted
    };

  } // namespace bitmap
} // namespace lost

#endif
