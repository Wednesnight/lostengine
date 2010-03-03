/**
 * $Id: defaults.tcc 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_MEMORY_DETAIL_DEFAULTS_TCC
#define FHTAGN_MEMORY_DETAIL_DEFAULTS_TCC

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <limits>

namespace fhtagn {
namespace memory {

/*****************************************************************************
 * default_allocation_policy
 **/
template <
  typename T
>
default_allocation_policy<T>::default_allocation_policy()
{
}



template <
  typename T
>
default_allocation_policy<T>::~default_allocation_policy()
{
}



template <
  typename T
>
default_allocation_policy<T>::default_allocation_policy(
    default_allocation_policy<T> const &)
{
}


template <
  typename T
>
template <
  typename U
>
default_allocation_policy<T>::default_allocation_policy(
    default_allocation_policy<U> const &)
{
}



template <
  typename T
>
typename default_allocation_policy<T>::pointer
default_allocation_policy<T>::allocate(size_type count,
    typename std::allocator<void>::const_pointer /* = 0 */)
{
  return reinterpret_cast<pointer>(::operator new(count * sizeof(T)));
}



template <
  typename T
>
void
default_allocation_policy<T>::deallocate(pointer p, size_type)
{
  ::operator delete(p);
}


template <
  typename T
>
typename default_allocation_policy<T>::size_type
default_allocation_policy<T>::max_size() const
{
  return std::numeric_limits<size_type>::max();
}



template <
  typename T1,
  typename T2
>
inline bool operator==(
    default_allocation_policy<T1> const &,
    default_allocation_policy<T2> const &)
{
  return true;
}



template <
  typename T,
  typename other_allocatorT
>
inline bool operator==(
    default_allocation_policy<T> const &,
    other_allocatorT const &)
{
  return false;
}



/*****************************************************************************
 * default_object_traits
 **/

template <
  typename T
>
default_object_traits<T>::default_object_traits()
{
}



template <
  typename T
>
default_object_traits<T>::~default_object_traits()
{
}



template <
  typename T
>
template <
  typename U
>
default_object_traits<T>::default_object_traits(default_object_traits<U> const &)
{
}



template <
  typename T
>
T *
default_object_traits<T>::address(T & t)
{
  return &t;
}



template <
  typename T
>
T const *
default_object_traits<T>::address(T const & t)
{
  return &t;
}



template <
  typename T
>
void
default_object_traits<T>::construct(T * p, T const & t)
{
  new(p) T(t);
}



template <
  typename T
>
void
default_object_traits<T>::destroy(T * p)
{
  p->~T();
}

}} // namespace fhtagn::memory

#endif // guard
