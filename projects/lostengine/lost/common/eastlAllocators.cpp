#include "EASTL/string.h"


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