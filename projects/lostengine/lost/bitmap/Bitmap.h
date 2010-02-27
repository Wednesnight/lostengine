#ifndef LOST_BITMAP_BITMAP_H
#define LOST_BITMAP_BITMAP_H

#include <boost/noncopyable.hpp>
#include <boost/cstdint.hpp>
#include "lost/common/Color.h"
#include "lost/math/Rect.h"
#include "lost/platform/shared_ptr.h"
#include "lost/common/Data.h"

namespace lost
{
  namespace bitmap
  {
    struct Bitmap;
    typedef lost::shared_ptr<Bitmap> BitmapPtr;
  
    struct Bitmap : private boost::noncopyable
    {
      enum Components
      {
        COMPONENTS_UNDEFINED = 0, // for undefined init, can't be used for anything else
        COMPONENTS_ALPHA = 1,     // 8 bit
        COMPONENTS_RGB = 3,       // 24 bit
        COMPONENTS_RGBA = 4,      // 32 bit
        COMPONENTS_NUM
      };
      
      boost::uint8_t*    data;   // points to the raw pixel data
      boost::uint32_t    width;  // width in pixels
      boost::uint32_t    height; // height in pixels
      Components  format; // format of bitmap (rgb, rgba)

      /** creates an empty bitmap with zero size.
       * Use init to resize it in a given format.
       */
      Bitmap();
        
      /** creates a bitmap with the given size and format. The initial content is undefined.
       */
      Bitmap(boost::uint32_t inWidth,
             boost::uint32_t inHeight,
             Components format);
    
      /** creates a bitmap by reading the provided data with width*height pixels in source format
       *  copying it to a new internal buffer, converting the components to destination format
       * if necessary
       */
      Bitmap(boost::uint32_t inWidth,
             boost::uint32_t inHeight,
             Components destComponents,
             Components srcComponents,
             boost::uint8_t* data);
             
      // tries to create a bitmap by interpreting the data as a bitmap format understood by
      // the image library.       
      Bitmap(const common::DataPtr& inData);
      
      static BitmapPtr create() { return BitmapPtr(new Bitmap()); }
      static BitmapPtr create(const common::DataPtr& inData) { return BitmapPtr(new Bitmap(inData)); }
      static BitmapPtr create(boost::uint32_t inWidth,
             boost::uint32_t inHeight,
             Components format) { return BitmapPtr(new Bitmap(inWidth, inHeight, format)); }
      
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
      void init(boost::uint32_t inWidth,
                boost::uint32_t inHeight,
                Components destComponents,
                Components srcComponents,
                boost::uint8_t* srcData);

      /** initialises the bitmap with the given size and format. 
       * The initial contents are undefined.
       *
       * @param inWidth width of the bitmap.
       * @param inHeight height of the bitmap.
       * @param format pixel format of the bitmap.
       *
       */
      void init(boost::uint32_t inWidth,
                boost::uint32_t inHeight,
                Components format);

      /** attempts to interpret a chunk ofmemory as a bitmap, with the help of the underlying image library.
       * An exception is thrown if an error occurs.
       */
      void init(const common::DataPtr& inData);
      
      /** calculates the number of pixels for a given bitmap components constant.
       */
      static boost::uint32_t bytesPerPixelFromComponents(Components components);

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
      static void copyPixel(boost::uint8_t* dest,
                            Components destComponents, 
                            boost::uint8_t* src,
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
      boost::uint8_t* pixelPointer(boost::uint32_t x, boost::uint32_t y);
        
      /** sets a pixel with the given color */
      void pixel(boost::uint32_t x, boost::uint32_t y, const common::Color& inColor);
      
      /** reads a pixel from the given coordinates and returns it as a Color. */
      common::Color pixel(boost::uint32_t x, boost::uint32_t y);
      
      /** draws a horizontal line into the bitmap.
       * No bounds checks are performed, you'll crsah if you draw outside the bitmap.
       * @param y       the y coordinate of the line.
       * @param xl      the left x coordinate of the starting point.
       * @param xr      the right x coordinate of the end point.
       * @param inColor color to draw the line in.
       */
      void hline(boost::uint32_t y, boost::uint32_t xl, boost::uint32_t xr, const common::Color& inColor);
      
      /** draws a horizontal line into the bitmap.
       * No bounds checks are performed, you'll crsah if you draw outside the bitmap.
       * @param x         the x coordinate of the line.
       * @param yb        the bottom y coordinate of the starting point.
       * @param yt        the top y coordinate of the end point.
       * @param inColor   color to draw the line in.
       */
      void vline(boost::uint32_t x, boost::uint32_t yb, boost::uint32_t yt, const common::Color& inColor);

      // draws a filled disc at given center and radius
      // the disc is drawn by evaluating all pixels of the bitmap and setting them accordingly
      void disc(float x, float y, float r);
        
      /** draws the rect outline into the bitmap with the given color
       */
      void drawRectOutline(const lost::math::Rect& inRect, const common::Color& inColor);  
        
      /** rotates the bitmap clock wise and returns it as a new bitmap*/
      lost::shared_ptr<Bitmap> rotCW();
      
      /** returns the occupied are in pixels. */
      float area() { return (float)(width*height); }
      
      /** draws this bitmap into the specified target. */
      void draw(boost::uint32_t x, boost::uint32_t y, lost::shared_ptr<Bitmap> target);
      
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
