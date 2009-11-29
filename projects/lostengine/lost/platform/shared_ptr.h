#ifndef LOST_PLATFORM_SHARED_PTR_H
#define LOST_PLATFORM_SHARED_PTR_H

// "detect" whether we have tr1::shared_ptr or must fall back to boost.
#if defined WIN32
  // May not actually be correct, but at least it'll fail loudly and early
  // if anyone uses lost::shared_ptr.
  #define HAVE_BOOST_SHARED_PTR
#else
  // Assuming this is a GCC-based platform, we'll expect tr1 to exist.
  #define HAVE_BOOST_SHARED_PTR
#endif

//#include <boost/shared_array.hpp>

// Use tr1::shared_ptr if available.
#if defined(HAVE_TR1_SHARED_PTR)

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

};

#else // HAVE_TR1_SHARED_PTR

// Only if tr1::shared_ptr is not available, use boost::shared_ptr
#if defined(HAVE_BOOST_SHARED_PTR)

#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/pointer_cast.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace lost {
using boost::shared_ptr;
using boost::shared_array;
using boost::weak_ptr;
using boost::enable_shared_from_this;

using boost::static_pointer_cast;
using boost::dynamic_pointer_cast;
using boost::const_pointer_cast;
};

#else // HAVE_BOOST_SHARED_PTR

// Can't really build this, error out.
#error Neither tr1::shared_ptr nor boost::shared_ptr available.

#endif // HAVE_BOOST_SHARED_PTR

#endif // HAVE_TR1_SHARED_PTR

#endif // guard
