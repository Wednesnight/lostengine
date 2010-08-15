#include "HandlePool.h"
#include "Bitmap.h"
#include "MemoryPool.h"

#include <vector>
#include <map>
#include <list>

#include "MemoryPoolAllocator.h"

using namespace std;

MemoryPool memPool;
HandlePool handlePool(&memPool); // global handle pool, implicitly used by all create functions

typedef basic_string<char, std::char_traits<char>, MemoryPoolAllocator<char> > mpstring;

int main (int argc, char * const argv[])
{
    mpstring s1;
    s1 = "asd";

/*    BitmapPtr bh1 = Bitmap::create(); 
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

    bh1->height = 18;*/
    
    vector<mpstring, MemoryPoolAllocator<mpstring> > mpsv;
    mpsv.push_back("asd");
    mpsv.push_back("hello");
    mpsv.push_back("was");
    mpsv.push_back("geht");
    mpsv.push_back("ab");
  
    s1 += "jetzt aber";
  
    return 0;
}
