#include <iostream>
#include "nedmalloc.h"
#include "malloc.c.h"
#include <stdexcept>
#include "HandlePool.h"
#include "Bitmap.h"

#include <vector>
#include <map>
#include <list>

using namespace std;

HandlePool pool; // global handle pool, implicitly used by all create functions

int main (int argc, char * const argv[])
{
    BitmapPtr bh1 = Bitmap::create(); 
    cout << bh1 << endl;
    
    BitmapPtr bh2 = bh1;
    bh1->width = 32;

    cout << bh1 << endl;
    cout << bh2 << endl;

    list<BitmapPtr> bl;
    vector<BitmapPtr> bv;
    map<uint32_t, BitmapPtr> bm1;
    map<BitmapPtr, uint32_t> bm2;

    bl.push_back(bh1);
    bv.push_back(bh1);
    bm1[13] = bh1;
    bm2[bh1] = 13;

    bh1->height = 18;


    return 0;
}
