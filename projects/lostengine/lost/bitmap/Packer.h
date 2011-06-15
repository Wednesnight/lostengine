#ifndef LOST_BITMAP_PACKER_H
#define LOST_BITMAP_PACKER_H

#include "lost/math/Rect.h"
#include "lost/math/RectPacker.h"
#include "lost/bitmap/forward.h"
#include "lost/bitmap/Components.h"

namespace lost
{
namespace bitmap
{
struct Packer
{
  struct Result
  {

    BitmapPtr                 packedBitmap; // the resulting bitmap
    vector<math::Rect>   rects;        // the resulting rects. Size of this vector is number of bitmaps that fit into the target area
    vector<int32_t>      bitmapIds;    // the initial index of the bitmap in the incoming data
    vector<bool>         rotated;      // true if the bitmap was rotated

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
            vector<BitmapPtr> bitmaps,
            Components format = COMPONENTS_RGBA,
            bool rotate = false,  /* won't do anything for now, so you can leave it at false */
            bool sort = true); // use sorting for best packing density

private:
  void buildRectsFromBitmaps(vector<lost::math::Rect>& outRects,
                             vector<BitmapPtr> inBitmaps);

  lost::math::RectPacker rectPacker;
};
}
}

#endif
