#ifndef LOST_BITMAP_PACKER_H
#define LOST_BITMAP_PACKER_H

#include "lost/bitmap/Bitmap.h"
#include "lost/math/Rect.h"
#include "lost/math/RectPacker.h"
#include <boost/cstdint.hpp>

namespace lost
{
namespace bitmap
{
struct Packer
{
  struct Result
  {

    lost::shared_ptr<lost::bitmap::Bitmap> packedBitmap; // the resulting bitmap
    std::vector<lost::math::Rect>           rects;        // the resulting rects. Size of this vector is number of bitmaps that fit into the target area
    std::vector<boost::int32_t>                    bitmapIds;    // the initial index of the bitmap in the incoming data
    std::vector<bool>                       rotated;      // true if the bitmap was rotated

    void clear();
  };

  Packer();
  virtual ~Packer();

  /** Tries to pack as many bitmaps as possible into a new bitmap with the given size.
   *
   * @param outResult   receives the result of the packing
   * @param targetSize  size of the bitmap that contains all the provided bitmaps.
   * @param bitmaps     all bitmaps to pack.
   * @param format      format of the pixel components of the resulting bitmap.
   * @param rotate      set this to true if you want to allow rotation of the packed bitmaps
   */
  void pack(Packer::Result& outResult,
            const lost::math::Vec2& targetSize,
            std::vector<lost::shared_ptr<lost::bitmap::Bitmap> > bitmaps,
            Bitmap::Components format = Bitmap::COMPONENTS_RGBA,
            bool rotate = false,  /* won't do anything for now, so you can leave it at false */
            bool sort = true); // use sorting for best packing density

private:
  void buildRectsFromBitmaps(std::vector<lost::math::Rect>& outRects,
                             std::vector<lost::shared_ptr<lost::bitmap::Bitmap> > inBitmaps);

  lost::math::RectPacker rectPacker;
};
}
}

#endif