/**
 * $Id: dynamic_pool.h 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_MEMORY_DYNAMIC_POOL_H
#define FHTAGN_MEMORY_DYNAMIC_POOL_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <fhtagn/fhtagn.h>

#include <map>

#include <fhtagn/shared_ptr.h>
#include <fhtagn/memory/memory_pool.h>
#include <fhtagn/memory/utility.h>
#include <fhtagn/threads/lock_policy.h>

namespace fhtagn {
namespace memory {

/**
 * The dynamic_pool class is a thin wrapper around such MemoryPool objects that
 * manage memory blocks of a fixed size, such as fixed_pool or block_pool.
 *
 * Note that as dynamic_pool allocates memory from the heap, the underlying pool
 * type must adopt the pointer handed to it. For that purpose, block_pool and
 * fixed_pool both define nested types which explicitly use the adopt or ignore
 * policy from common.h. The former nested type is what dynamic_pool uses and
 * expects any poolT to supply.
 *
 * It maintains a defers to a list of such pools. If allocation from one of them
 * fails, an additional such pool is created, and memory is allocated from there
 * instead.
 *
 * If after freeing a pointer, the pool it was from becomes empty, the pool
 * is destroyed again.
 *
 * Pools are created with a fixed size, passed to dynamic_pool as a template
 * parameter. One obvious result of that is that dynamic_pool can never allocate
 * more data than it's memory block size.
 *
 * XXX There's a caveat when using dynamic_pool with an underlying block_pool,
 *     as reallocations or allocations with the wrong size will usually fail,
 *     which would cause dynamic_pool to create a new block_pool. The only way
 *     to work with the combination of the two is to ensure the block_pool
 *     always receives requests of a valid size, which means passing valid
 *     sizes to the dynamic_pool as well.
 **/
template <
  typename poolT,
  int MEMORY_BLOCK_SIZE,
  typename mutexT = fhtagn::threads::fake_mutex
>
class dynamic_pool
{
public:
  /**
   * Convenience typedefs
   **/
  typedef mutexT                              mutex_t;
  typedef typename poolT::with_adopt_policy_t pool_t;

  /**
   * API - see memory_pool.h for details
   **/
  inline void * alloc(fhtagn::size_t size);
  inline void * realloc(void * ptr, fhtagn::size_t new_size);
  inline void free(void * ptr);
  inline bool in_use() const;
  inline fhtagn::size_t alloc_size(void * ptr) const;

private:

  typedef fhtagn::shared_ptr<pool_t>      pool_ptr;
  typedef std::pair<void *, void *>       pool_key_t;
  typedef std::map<pool_key_t, pool_ptr>  pool_map_t;

  /**
   * For a given pointer, returns the pool that should contain it.
   **/
  inline typename pool_map_t::iterator find_pool(void * ptr);
  inline typename pool_map_t::const_iterator find_pool(void * ptr) const;

  /**
   * Lock-free version of alloc, used internally.
   **/
  inline void * lockfree_alloc(fhtagn::size_t size);

  pool_map_t      m_pool_map;
  mutable mutex_t m_mutex;
};


}} // namespace fhtagn::memory

#include <fhtagn/memory/detail/dynamic_pool.tcc>

#endif // guard
