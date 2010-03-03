/**
 * $Id: pool_allocator.tcc 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_MEMORY_DETAIL_BLOCK_ALLOCATOR_TCC
#define FHTAGN_MEMORY_DETAIL_BLOCK_ALLOCATOR_TCC

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <limits>
#include <stdexcept>

namespace fhtagn {
namespace memory {


/*****************************************************************************
 * pool_allocation_policy
 **/
template <
  typename T,
  typename memory_poolT,
  typename tagT
>
pool_allocation_policy<T, memory_poolT, tagT>::pool_allocation_policy()
{
  initialize_pool();
}



template <
  typename T,
  typename memory_poolT,
  typename tagT
>
pool_allocation_policy<T, memory_poolT, tagT>::~pool_allocation_policy()
{
}



template <
  typename T,
  typename memory_poolT,
  typename tagT
>
pool_allocation_policy<T, memory_poolT, tagT>::pool_allocation_policy(
    pool_allocation_policy<T, memory_poolT, tagT> const &)
{
  initialize_pool();
}



template <
  typename T,
  typename memory_poolT,
  typename tagT
>
template <
  typename U
>
pool_allocation_policy<T, memory_poolT, tagT>::pool_allocation_policy(
    pool_allocation_policy<U> const &)
{
  initialize_pool();
}



template <
  typename T,
  typename memory_poolT,
  typename tagT
>
template <
  typename U,
  typename other_poolT
>
pool_allocation_policy<T, memory_poolT, tagT>::pool_allocation_policy(
    pool_allocation_policy<U, other_poolT> const &)
{
  initialize_pool();
}



template <
  typename T,
  typename memory_poolT,
  typename tagT
>
void
pool_allocation_policy<T, memory_poolT, tagT>::initialize_pool()
{
  if (m_pool) {
    return;
  }

  if (per_type_memory_pool) {
    m_pool = per_type_memory_pool;
    return;
  }

  if (!pool_allocation_policy_base<memory_poolT, tagT>::global_memory_pool) {
    throw std::logic_error("Constructing a pool_allocation_policy<T> without "
        "either a global pool or per-type pool set.");
  }

  m_pool = pool_allocation_policy_base<memory_poolT, tagT>::global_memory_pool;
}



template <
  typename T,
  typename memory_poolT,
  typename tagT
>
typename pool_allocation_policy<T, memory_poolT, tagT>::memory_pool_ptr
pool_allocation_policy<T, memory_poolT, tagT>::get_memory_pool() const
{
  return m_pool;
}



template <
  typename T,
  typename memory_poolT,
  typename tagT
>
bool
pool_allocation_policy<T, memory_poolT, tagT>::set_memory_pool(
    typename pool_allocation_policy<T, memory_poolT, tagT>::memory_pool_ptr new_pool)
{
  if (m_pool && m_pool->in_use()) {
    return false;
  }

  m_pool = new_pool;
  return true;
}




template <
  typename T,
  typename memory_poolT,
  typename tagT
>
typename pool_allocation_policy<T, memory_poolT, tagT>::pointer
pool_allocation_policy<T, memory_poolT, tagT>::allocate(size_type count,
    typename std::allocator<void>::const_pointer /* = 0 */)
{
  return static_cast<T *>(m_pool->alloc(count * sizeof(T)));
}



template <
  typename T,
  typename memory_poolT,
  typename tagT
>
void
pool_allocation_policy<T, memory_poolT, tagT>::deallocate(pointer p, size_type)
{
  m_pool->free(p);
}


template <
  typename T,
  typename memory_poolT,
  typename tagT
>
typename pool_allocation_policy<T, memory_poolT, tagT>::size_type
pool_allocation_policy<T, memory_poolT, tagT>::max_size() const
{
  return std::numeric_limits<size_type>::max();
}




template <
  typename T,
  typename memory_poolT,
  typename tagT
>
inline bool operator==(
    pool_allocation_policy<T, memory_poolT, tagT> const & rhs,
    pool_allocation_policy<T, memory_poolT, tagT> const & lhs)
{
  // Same memory_poolT, so we can if it's also the same instance.
  return (rhs.get_memory_pool() == lhs.get_memory_pool());
}



template <
  typename T1,
  typename T2,
  typename memory_poolT,
  typename tagT
>
inline bool operator==(
    pool_allocation_policy<T1, memory_poolT> const & rhs,
    pool_allocation_policy<T2, memory_poolT> const & lhs)
{
  // Same memory_poolT, so we can if it's also the same instance.
  return (rhs.get_memory_pool() == lhs.get_memory_pool());
}



template <
  typename T,
  typename memory_poolT,
  typename tagT,
  typename other_allocatorT
>
inline bool operator==(
    pool_allocation_policy<T, memory_poolT, tagT> const &,
    other_allocatorT const &)
{
  // No. In theory, it might actually be possible, but let's not risk it.
  return false;
}



}} // namespace fhtagn::memory

#endif // guard
