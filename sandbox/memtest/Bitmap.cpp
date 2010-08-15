#include "Bitmap.h"
//#include <iostream>

using namespace std;

extern HandlePool handlePool;

Bitmap::Bitmap() 
{
//  cout << "// Bitmap constructor +++" << endl;
}

Bitmap::~Bitmap()
{
//  cout << "// Bitmap destructor ---" << endl;
}
    
BitmapPtr Bitmap::create()
{
  BitmapPtr result = createObjectHandle<Bitmap>(handlePool); // create correctly typed handle, only allocates memory and handle slot
  new(result.getRawPointer()) Bitmap; // call operator via placement new
  return result;
}
