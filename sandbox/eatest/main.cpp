#include <stdio.h>
#include "EASTL/string.h"
#include "EASTL/map.h"

void* operator new[](size_t size, const char* pName, int flags,
        unsigned debugFlags, const char* file, int line)
{
//    printf("alloc1: %d\n",size);
    return malloc(size);
}
void* operator new[](size_t size, size_t alignment, size_t alignmentOffset,
    const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
//    printf("alloc2: %d\n",size);
    // doesn't support alignment
    return malloc(size);
}

int Vsnprintf8(char8_t* pDestination, size_t n, const char8_t* pFormat,
            va_list arguments)
{
    #ifdef _MSC_VER
        return _vsnprintf(pDestination, n, pFormat, arguments);
    #else
        return vsnprintf(pDestination, n, pFormat, arguments);
    #endif
}

int main (int argc, char * const argv[]) {
    eastl::string s;
    s="hello from eastl";
    printf("%s\n",s.c_str());
    
    eastl::map<eastl::string, int> string2int;
    string2int["first"] = 1;
    string2int["second"] = 2;
  
    eastl::map<eastl::string, int>::iterator pos = string2int.begin();
    for(pos; pos!=string2int.end(); ++pos)
    {
      printf("%s %d\n", pos->first.c_str(), pos->second);
    }
  
    return 0;
}
