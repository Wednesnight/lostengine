/**
 * $Id: allocator.tcc 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_MEMORY_DETAIL_ALLOCATOR_TCC
#define FHTAGN_MEMORY_DETAIL_ALLOCATOR_TCC

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

namespace fhtagn {
namespace memory {


template <
  typename T,
  typename allocation_policyT,
  typename object_traitsT
>
allocator<T, allocation_policyT, object_traitsT>::allocator()
{
}



template <
  typename T,
  typename allocation_policyT,
  typename object_traitsT
>
allocator<T, allocation_policyT, object_traitsT>::~allocator()
{
}



template <
  typename T,
  typename allocation_policyT,
  typename object_traitsT
>
allocator<T, allocation_policyT, object_traitsT>::allocator(
    allocator<T, allocation_policyT, object_traitsT> const & other)
  : allocation_policyT(other)
  , object_traitsT(other)
{
}



template <
  typename T,
  typename allocation_policyT,
  typename object_traitsT
>
template <
  typename U
>
allocator<T, allocation_policyT, object_traitsT>::allocator(
    allocator<U> const &)
{
}



template <
  typename T,
  typename allocation_policyT,
  typename object_traitsT
>
template <
  typename U,
  typename other_policyT,
  typename other_traitsT
>
allocator<T, allocation_policyT, object_traitsT>::allocator(
    allocator<U, other_policyT, other_traitsT> const & other)
  : allocation_policyT(other)
  , object_traitsT(other)
{
}



template <
  typename T,
  typename allocation_policyT,
  typename object_traitsT
>
inline bool operator==(
    allocator<T, allocation_policyT, object_traitsT> const & lhs,
    allocator<T, allocation_policyT, object_traitsT> const & rhs)
{
  return operator==(
      static_cast<allocation_policyT &>(lhs),
      static_cast<allocation_policyT &>(rhs));
}



template <
  typename T1,
  typename allocation_policyT1,
  typename object_traitsT1,
  typename T2,
  typename allocation_policyT2,
  typename object_traitsT2
>
inline bool operator==(
    allocator<T1, allocation_policyT1, object_traitsT1> const & lhs,
    allocator<T2, allocation_policyT2, object_traitsT2> const & rhs)
{
  return operator==(
      static_cast<allocation_policyT1 &>(lhs),
      static_cast<allocation_policyT2 &>(rhs));
}


template <
  typename T,
  typename allocation_policyT,
  typename object_traitsT,
  typename other_allocatorT
>
inline bool operator==(
    allocator<T, allocation_policyT, object_traitsT> const & lhs,
    other_allocatorT const & rhs)
{
  return operator==(static_cast<allocation_policyT &>(lhs), rhs);
}



template <
  typename T,
  typename allocation_policyT,
  typename object_traitsT
>
inline bool operator!=(
    allocator<T, allocation_policyT, object_traitsT> const & lhs,
    allocator<T, allocation_policyT, object_traitsT> const & rhs)
{
  return !operator==(lhs, rhs);
}



template <
  typename T1,
  typename allocation_policyT1,
  typename object_traitsT1,
  typename T2,
  typename allocation_policyT2,
  typename object_traitsT2
>
inline bool operator!=(
    allocator<T1, allocation_policyT1, object_traitsT1> const & lhs,
    allocator<T2, allocation_policyT2, object_traitsT2> const & rhs)
{
  return !operator==(lhs, rhs);
}


template <
  typename T,
  typename allocation_policyT,
  typename object_traitsT,
  typename other_allocatorT
>
inline bool operator!=(
    allocator<T, allocation_policyT, object_traitsT> const & lhs,
    other_allocatorT const & rhs)
{
  return !operator==(lhs, rhs);
}





}} // namespace fhtagn::memory
#endif // guard
