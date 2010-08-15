#include "Bitmap.h"
#include <iostream>

using namespace std;

extern HandlePool pool;

Bitmap::Bitmap() 
{
  cout << "// Bitmap constructor +++" << endl;
}

Bitmap::~Bitmap()
{
  cout << "// Bitmap destructor ---" << endl;
}
    
BitmapPtr Bitmap::create()
{
  BitmapPtr result = createObjectHandle<Bitmap>(pool); // create correctly typed handle, only allocates memory and handle slot
  new(result.getRawPointer()) Bitmap; // call operator via placement new
  return result;
}
