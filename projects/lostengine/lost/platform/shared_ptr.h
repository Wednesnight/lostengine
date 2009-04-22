#ifndef LOST_PLATFORM_SHARED_PTR_H
#define LOST_PLATFORM_SHARED_PTR_H


#if defined(HAVE_TR1_SHARED_PTR)

#include <tr1/memory>
namespace lost {
using std::tr1::shared_ptr;
};

#else // HAVE_TR1_SHARED_PTR


#if defined(HAVE_BOOST_SHARED_PTR)

#include <boost/shared_ptr.hpp>
namespace lost {
using boost::shared_ptr;
};

#endif // HAVE_BOOST_SHARED_PTR

#endif // HAVE_TR1_SHARED_PTR

#endif // guard
