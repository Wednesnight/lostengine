#ifndef BUILD_FHTAGN_FHTAGN_CONFIG_H_SEEN
#define BUILD_FHTAGN_FHTAGN_CONFIG_H_SEEN

/*
 * g++ -o build/fhtagn/version.os -c
 * -ansi -std=c++98 -Wall -Wsign-promo -fstrict-aliasing -Wstrict-aliasing
 *  -ggdb -O2 -I/opt/local/include -pthread -fPIC -DNDEBUG=1 -I. -Ibuild build/fhtagn/version.cpp
 **/

/* Version of the installed boost library */
#define FHTAGN_BOOST_VERSION 1.36.0
/* Found boost library? */
#define HAVE_BOOST 1
/* Version of the installed cppunit library */
/* #define FHTAGN_CPPUNIT_VERSION */
/* Found cppunit library? */
/*#define HAVE_CPPUNIT */

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <cmath> header file. */
#define HAVE_CMATH 1

/* Define to 1 if you have the <iostream> header file. */
#define HAVE_IOSTREAM 1

/* Define to 1 if you have the <utility> header file. */
#define HAVE_UTILITY 1

/* Define to 1 if you have the <boost/shared_ptr.hpp> header file. */
#define HAVE_BOOST_SHARED_PTR_HPP 1

/* Define to 1 if you have the <boost/integer_traits.hpp> header file. */
#define HAVE_BOOST_INTEGER_TRAITS_HPP 1

/* Define to 1 if the system has the type `int64_t'. */
#define HAVE_INT64_T 1

/* Define to 1 if the system has the type `int32_t'. */
#define HAVE_INT32_T 1
/* Fhtagn! version */
#define FHTAGN_VERSION 0.1
/* Fhtagn! major version component */
#define FHTAGN_MAJOR 0
/* Fhtagn! minor version component */
#define FHTAGN_MINOR 1

#endif /* BUILD_FHTAGN_FHTAGN_CONFIG_H_SEEN */
