/**
 * $Id: block_pool.h 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_MEMORY_BLOCK_POOL_H
#define FHTAGN_MEMORY_BLOCK_POOL_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <fhtagn/fhtagn.h>

#include <fhtagn/memory/utility.h>
#include <fhtagn/memory/memory_pool.h>
#include <fhtagn/memory/common.h>
#include <fhtagn/threads/lock_policy.h>

namespace fhtagn {
namespace memory {

/**
 * The block_pool class implements a memory pool with very limited use in
 * combination with the pool_allocator_policy: it only hands out chunks of
 * the same, static size.
 *
 * Any attempt to allocate memory of a different size from it will fail. As
 * a side-effect, realloc will always fail unless the new size happens to be
 * the same as the old.
 *
 * Like the fixed_pool class, it allocates from memory handed to it's
 * constructor, and like the fixed_pool class it tries to return only memory
 * aligned at some definable block boundary.
 **/
template <
  fhtagn::size_t BLOCK_SIZE,
  typename mutexT = fhtagn::threads::fake_mutex,
  typename block_alignmentT = block_alignment<>,
  template <typename> class adoption_policyT = fhtagn::memory::ignore_array_policy
>
class block_pool : public adoption_policyT<char>
{
public:
  /**
   * Convenience typedefs
   **/
  typedef mutexT            mutex_t;
  typedef block_alignmentT  block_alignment_t;

  /**
   * Explicit subtypes of this pool type that adopt the memory block handed to
   * them or not.
   **/
  typedef block_pool<
    BLOCK_SIZE,
    mutexT,
    block_alignmentT,
    fhtagn::memory::ignore_array_policy
  > with_ignore_policy_t;

  typedef block_pool<
    BLOCK_SIZE,
    mutexT,
    block_alignmentT,
    fhtagn::memory::adopt_array_policy
  > with_adopt_policy_t;


  /**
   * The constructor accepts a pointer to a block of memory of the given size.
   * Note that the class does not take ownership of this memory; you must ensure
   * yourself that the memory lives at least as long as this block_pool
   * instance.
   **/
  inline block_pool(void * memblock, fhtagn::size_t size);

  /**
   * API - see memory_pool.h for details
   **/
  inline void * alloc(fhtagn::size_t size);
  inline void * realloc(void * ptr, fhtagn::size_t new_size);
  inline void free(void * ptr);
  inline bool in_use() const;
  inline fhtagn::size_t alloc_size(void * ptr) const;

private:

  enum {
    BITS_PER_SIZE_T = sizeof(fhtagn::size_t) * 8,
  };

  void *            m_memblock;
  fhtagn::size_t *  m_metadata;
  fhtagn::size_t    m_size;

  mutable mutex_t   m_mutex;
};


}} // namespace fhtagn::memory

#include <fhtagn/memory/detail/block_pool.tcc>

#endif // guard
