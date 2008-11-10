#ifndef LOST_IMAGE_BITMAP_H
#define LOST_IMAGE_BITMAP_H

#include <boost/noncopyable.hpp>
#include <stdint.h>
#include "lost/common/Color.h"
#include "lost/math/Rect.h"

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

      /** creates an empty bitmap with zero size.
       * Use init to resize it in a given format.
       */
      Bitmap();
        
      /** creates a bitmap with the given size and format. The initial content is undefined.
       */
      Bitmap(uint32_t inWidth,
             uint32_t inHeight,
             Components format);
    
      Bitmap(uint32_t inWidth,
             uint32_t inHeight,
             Components destComponents,
             Components srcComponents,
             uint8_t* data);
      Bitmap(boost::shared_ptr<lost::resource::File> inFile);
      void reset();
      virtual ~Bitmap();

      /** inits a bitmap of destComponents format with the given data in the given format.
       * The data is copied and converted if necessary.
       *
       * @param inWidth width of the new bitmap
       * @param inHeight height of the new bitmap
       * @param destComponents format of the bitmap
       * @param srcComponents format of the source data
       * @param srcData source data from which the bitmap is constructed
       */
      void init(uint32_t inWidth,
                uint32_t inHeight,
                Components destComponents,
                Components srcComponents,
                uint8_t* srcData);

      /** initialises the bitmap with the given size and format. 
       * The initial contents are undefined.
       *
       * @param inWidth width of the bitmap.
       * @param inHeight height of the bitmap.
       * @param format pixel format of the bitmap.
       *
       */
      void init(uint32_t inWidth,
                uint32_t inHeight,
                Components format);

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
                            
                            
      /** clears the bitmap, overwriting all pixels with the given color.
       * Depending on the internal format of the bitmap, not all of the color components might be used.
       * I.e. for COMPONENTS_ALPHA, only the alpha value of the given color will be used for clearing. 
       *
       * @param inColor the clear color.
       *
       */                      
      void clear(const lost::common::Color& inColor);
      
      /** writes the bitmap to a TGA file.
       * The file is created or overwritten.
       *
       * @param inFullPathname full path name of the file to write to.
       *
       * @throws if the file couldn't be written.
       */
      void write(const std::string inFullPathname);

      /** flips the bitmap vertically.
       * This moves a lot of memory, so be careful performance wise.
       */
      void flip();
    
      /** returns a pointer to the data of the pixel at the given coordinates */
      uint8_t* pixelPointer(uint32_t x, uint32_t y);
        
      /** sets a pixel with the given color */
      void pixel(uint32_t x, uint32_t y, const common::Color& inColor);
      
      /** reads a pixel from the given coordinates and returns it as a Color. */
      common::Color pixel(uint32_t x, uint32_t y);
      
      /** draws a horizontal line into the bitmap.
       * No bounds checks are performed, you'll crsah if you draw outside the bitmap.
       * @param y       the y coordinate of the line.
       * @param xl      the left x coordinate of the starting point.
       * @param xr      the right x coordinate of the end point.
       * @param inColor color to draw the line in.
       */
      void hline(uint32_t y, uint32_t xl, uint32_t xr, const common::Color& inColor);
      
      /** draws a horizontal line into the bitmap.
       * No bounds checks are performed, you'll crsah if you draw outside the bitmap.
       * @param x         the x coordinate of the line.
       * @param yb        the bottom y coordinate of the starting point.
       * @param yt        the top y coordinate of the end point.
       * @param inColor   color to draw the line in.
       */
      void vline(uint32_t x, uint32_t yb, uint32_t yt, const common::Color& inColor);

      
        
      /** draws the rect outline into the bitmap with the given color
       */
      void drawRectOutline(const lost::math::Rect& inRect, const common::Color& inColor);  
        
      /** rotates the bitmap clock wise and returns it as a new bitmap*/
      boost::shared_ptr<Bitmap> rotCW();
      
      /** returns the occupied are in pixels. */
      float area() { return width*height; }
      
      /** draws this bitmap into the specified target. */
      void draw(uint32_t x, uint32_t y, boost::shared_ptr<Bitmap> target);
      
    private:
      void destroy();
      bool loaded; // true if the image was loaded with the image library and data must be freed by it.
                   // false if data is just a chunk of memory and can simply be deleted

      void clearA(const lost::common::Color& inColor);
      void clearRGB(const lost::common::Color& inColor);
      void clearRGBA(const lost::common::Color& inColor);
    };

  } // namespace bitmap
} // namespace lost

#endif
