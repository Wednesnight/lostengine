/**
 * $Id: static_for.tcc 319 2009-06-08 22:17:47Z unwesen $
 *
 * This file is part of the Fhtagn! C++ Library.
 * Copyright (C) 2009 Jens Finkhaeuser <unwesen@users.sourceforge.net>.
 *
 * Author: Jens Finkhaeuser <unwesen@users.sourceforge.net>
 *
 * This program is licensed as free software for personal, educational or
 * other non-commerical uses: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Alternatively, licenses for commercial purposes are available as well.
 * Please send your enquiries to the copyright holder's address above.
 **/
#ifndef FHTAGN_META_DETAIL_STATIC_FOR_TCC
#define FHTAGN_META_DETAIL_STATIC_FOR_TCC

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

namespace fhtagn {
namespace meta {


/*****************************************************************************
 * Helper structures
 **/

namespace detail {
namespace static_for_helpers {

/**
 * Generic implementation of next is for cases where the TRUE_FALSE condition is
 * non-zero, i.e. true.
 *
 * In those cases, we first call the functor and pass the CURRENT value. Then we
 * instanciate a new next with CURRENT incremented, and pass control to that
 * instance's run function.
 **/
template <
  int TRUE_FALSE, // result of exit condition; next is specialized on this below
  int CURRENT,    // current value of the for loop counter
  int END,        // end of the for loop
  template <int> class incrementorT,
  template <int, int> class comparatorT,
  template <int> class functorT
>
struct next
{
  inline static void run()
  {
    functorT<CURRENT> func;
    func();
    next<
      comparatorT<incrementorT<CURRENT>::value, END>::value,
      incrementorT<CURRENT>::value,
      END,
      incrementorT,
      comparatorT,
      functorT
    >::run();
  }

  template <typename functor_paramT>
  inline static void run(functor_paramT & param)
  {
    functorT<CURRENT> func;
    func(param);
    next<
      comparatorT<incrementorT<CURRENT>::value, END>::value,
      incrementorT<CURRENT>::value,
      END,
      incrementorT,
      comparatorT,
      functorT
    >::run(param);
  }

  template <typename functor_paramT>
  inline static void run(functor_paramT const & param)
  {
    functorT<CURRENT> func;
    func(param);
    next<
      comparatorT<incrementorT<CURRENT>::value, END>::value,
      incrementorT<CURRENT>::value,
      END,
      incrementorT,
      comparatorT,
      functorT
    >::run(param);
  }
};


/**
 * Specialization for TRUE_FALSE == 0 - neither run function does anything, as
 * END is not included in the iterated range.
 **/

template <
  int CURRENT,
  int END,
  template <int> class incrementorT,
  template <int, int> class comparatorT,
  template <int> class functorT
>
struct next<0, CURRENT, END, incrementorT, comparatorT, functorT>
{
  inline static void run()
  {
  }

  template <typename functor_paramT>
  inline static void run(functor_paramT &)
  {
  }

  template <typename functor_paramT>
  inline static void run(functor_paramT const &)
  {
  }
};





}} // namespace detail::static_for_helpers

/*****************************************************************************
 * Wrapper function implementations
 **/

/**
 * No parameter
 **/
template <
  int START,
  int END,
  template <int> class functorT
>
inline void
static_for()
{
  detail::static_for_helpers::next<
    ::fhtagn::meta::less<START, END>::value,
    START,
    END,
    ::fhtagn::meta::inc_once,
    ::fhtagn::meta::less,
    functorT
  >::template run();
}


template <
  int START,
  int END,
  template <int> class incrementorT,
  template <int> class functorT
>
inline void
static_for()
{
  detail::static_for_helpers::next<
    ::fhtagn::meta::less<START, END>::value,
    START,
    END,
    incrementorT,
    ::fhtagn::meta::less,
    functorT
  >::template run();
}


template <
  int START,
  int END,
  template <int, int> class comparatorT,
  template <int> class functorT
>
inline void
static_for()
{
  detail::static_for_helpers::next<
    comparatorT<START, END>::value,
    START,
    END,
    ::fhtagn::meta::inc_once,
    comparatorT,
    functorT
  >::template run();
}


template <
  int START,
  int END,
  template <int> class incrementorT,
  template <int, int> class comparatorT,
  template <int> class functorT
>
inline void
static_for()
{
  detail::static_for_helpers::next<
    comparatorT<START, END>::value,
    START,
    END,
    incrementorT,
    comparatorT,
    functorT
  >::template run();
}



/**
 * Parameter by reference
 **/
template <
  int START,
  int END,
  template <int> class functorT,
  typename functor_paramT
>
inline void
static_for(functor_paramT & param)
{
  detail::static_for_helpers::next<
    ::fhtagn::meta::less<START, END>::value,
    START,
    END,
    ::fhtagn::meta::inc_once,
    ::fhtagn::meta::less,
    functorT
  >::template run(param);
}


template <
  int START,
  int END,
  template <int> class incrementorT,
  template <int> class functorT,
  typename functor_paramT
>
inline void
static_for(functor_paramT & param)
{
  detail::static_for_helpers::next<
    ::fhtagn::meta::less<START, END>::value,
    START,
    END,
    incrementorT,
    ::fhtagn::meta::less,
    functorT
  >::template run(param);
}


template <
  int START,
  int END,
  template <int, int> class comparatorT,
  template <int> class functorT,
  typename functor_paramT
>
inline void
static_for(functor_paramT & param)
{
  detail::static_for_helpers::next<
    comparatorT<START, END>::value,
    START,
    END,
    ::fhtagn::meta::inc_once,
    comparatorT,
    functorT
  >::template run(param);
}


template <
  int START,
  int END,
  template <int> class incrementorT,
  template <int, int> class comparatorT,
  template <int> class functorT,
  typename functor_paramT
>
inline void
static_for(functor_paramT & param)
{
  detail::static_for_helpers::next<
    comparatorT<START, END>::value,
    START,
    END,
    incrementorT,
    comparatorT,
    functorT
  >::template run(param);
}


/**
 * Parameter by const reference
 **/
template <
  int START,
  int END,
  template <int> class functorT,
  typename functor_paramT
>
inline void
static_for(functor_paramT const & param)
{
  detail::static_for_helpers::next<
    ::fhtagn::meta::less<START, END>::value,
    START,
    END,
    ::fhtagn::meta::inc_once,
    ::fhtagn::meta::less,
    functorT
  >::template run(param);
}


template <
  int START,
  int END,
  template <int> class incrementorT,
  template <int> class functorT,
  typename functor_paramT
>
inline void
static_for(functor_paramT const & param)
{
  detail::static_for_helpers::next<
    ::fhtagn::meta::less<START, END>::value,
    START,
    END,
    incrementorT,
    ::fhtagn::meta::less,
    functorT
  >::template run(param);
}


template <
  int START,
  int END,
  template <int, int> class comparatorT,
  template <int> class functorT,
  typename functor_paramT
>
inline void
static_for(functor_paramT const & param)
{
  detail::static_for_helpers::next<
    comparatorT<START, END>::value,
    START,
    END,
    ::fhtagn::meta::inc_once,
    comparatorT,
    functorT
  >::template run(param);
}


template <
  int START,
  int END,
  template <int> class incrementorT,
  template <int, int> class comparatorT,
  template <int> class functorT,
  typename functor_paramT
>
inline void
static_for(functor_paramT const & param)
{
  detail::static_for_helpers::next<
    comparatorT<START, END>::value,
    START,
    END,
    incrementorT,
    comparatorT,
    functorT
  >::template run(param);
}



}} // namespace fhtagn::meta

#endif // guard
