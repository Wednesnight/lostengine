#ifndef LOST_BITMAP_COMPONENTS_H
#define LOST_BITMAP_COMPONENTS_H

namespace lost
{
  namespace bitmap
  {
      enum Components
      {
        COMPONENTS_UNDEFINED = 0, // for undefined init, can't be used for anything else
        COMPONENTS_ALPHA = 1,     // 8 bit
        COMPONENTS_RGB = 3,       // 24 bit
        COMPONENTS_RGBA = 4,      // 32 bit
        COMPONENTS_NUM
      };
  }
}

#endif