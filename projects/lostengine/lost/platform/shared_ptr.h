#ifndef LOST_PLATFORM_SHARED_PTR_H
#define LOST_PLATFORM_SHARED_PTR_H

#include <tr1/memory>
#include "lost/common/ArrayDeleter.h"
namespace lost {
using std::tr1::shared_ptr;
using std::tr1::weak_ptr;
using std::tr1::enable_shared_from_this;

using std::tr1::static_pointer_cast;
using std::tr1::dynamic_pointer_cast;
using std::tr1::const_pointer_cast;

template<typename T>
struct shared_array : public std::tr1::shared_ptr<T>
{
    shared_array(T* p=0) : std::tr1::shared_ptr<T>(p, lost::common::ArrayDeleter<T>()) {}
    
    T & operator[] (std::ptrdiff_t i) const // never throws
    {
        return this->get()[i];
    }    
};

}

#endif // guard
