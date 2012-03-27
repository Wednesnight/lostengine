/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef LOST_PLATFORM_SHARED_PTR_H
#define LOST_PLATFORM_SHARED_PTR_H

#ifdef WIN32
  #include <memory>
#else
  #include <tr1/memory>
#endif

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
