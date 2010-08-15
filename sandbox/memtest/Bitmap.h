#ifndef BITMAP_H
#define BITMAP_H

#include "TypedHandle.h"

struct Bitmap; typedef ObjectHandle<Bitmap> BitmapPtr;

struct Bitmap
{
  Bitmap();
  ~Bitmap();
  
  float width;
  float height;
  
  static BitmapPtr create();  
};

#endif