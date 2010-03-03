/**
 * $Id: fixed_pool.h 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_MEMORY_FIXED_POOL_H
#define FHTAGN_MEMORY_FIXED_POOL_H

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
 * The fixed_pool class implements a MemoryPool that allocates space from a
 * fixed-sized chunk of preallocated memory. fixed_pool does not allocate this
 * chunk of memory itself, nor frees it (by default, that can be changed via
 * the adoption_policyT template parameter), but merely subdivides it.
 *
 * As a result, fixed_pool works equally well with a heap and stack memory;
 * it's the perfect pool for allocation a reasonably small amount of stack or
 * heap at the start of your program, to allocate memory from for fatal error
 * handling.
 *
 * Either way, you must ensure yourself that the memory handled by fixed_pool
 * lives at least as long as the pool itself, and the pool itself should not
 * be destroyed as long as in_use() returns true.
 *
 * And to mix things up a little, there's also the possibility to influence
 * the alignment of memory allocated from the pool. Alignment is handled via
 * a block size; essentially memory will internally always be allocated in
 * multiples of this block size, and aligned on such a block boundary.
 *
 * In most cases, you don't need to change the block alignment, but if you feel
 * the need to squeeze the last bytes out of things, feel free to do so.
 **/
template <
  typename mutexT = fhtagn::threads::fake_mutex,
  typename block_alignmentT = block_alignment<>,
  template <typename> class adoption_policyT = fhtagn::memory::ignore_array_policy
>
class fixed_pool : public adoption_policyT<char>
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
  typedef fixed_pool<
    mutexT,
    block_alignmentT,
    fhtagn::memory::ignore_array_policy
  > with_ignore_policy_t;

  typedef fixed_pool<
    mutexT,
    block_alignmentT,
    fhtagn::memory::adopt_array_policy
  > with_adopt_policy_t;


  /**
   * The constructor accepts a pointer to a block of memory of the given size.
   * Note that the class does not take ownership of this memory; you must ensure
   * yourself that the memory lives at least as long as this fixed_pool
   * instance.
   **/
  inline fixed_pool(void * memblock, fhtagn::size_t size);

  /**
   * API - see memory_pool.h for details
   **/
  inline void * alloc(fhtagn::size_t size);
  inline void * realloc(void * ptr, fhtagn::size_t new_size);
  inline void free(void * ptr);
  inline bool in_use() const;
  inline fhtagn::size_t alloc_size(void * ptr) const;

private:

  /**
   * Helper structure, that is the header of each free or allocated segment in
   * the fixed_pool.
   **/
  struct segment
  {
    enum {
      FREE      = 0,
      ALLOCATED = ~char(0)
    };

    enum {
      LAST_SEGMENT = fhtagn::size_t(0)
    };

    segment(fhtagn::size_t _size)
      : size(_size - sizeof(segment))
      , status(FREE)
    {
      marker = LAST_SEGMENT;
    }

    inline fhtagn::size_t full_size() const
    {
      return size + sizeof(segment);
    }

    static inline fhtagn::size_t header_size()
    {
      return block_alignmentT::adjust_size(sizeof(segment));
    }

    fhtagn::size_t  size;
    char            status;
    union {
      segment *       next;
      fhtagn::size_t  marker;
    };
  };

  /**
   * Finds and allocates a free segment of the given size, splitting larger
   * segments if necessary.
   **/
  inline segment * allocate_segment(fhtagn::size_t size);

  /**
   * Finds the segment in which ptr resides, or throws if the ptr can't be
   * found.
   **/
  inline segment * find_segment_for(void * ptr) const;

  /**
   * Merges contiguous free segments.
   **/
  inline void defragment_free_list();

  void *          m_memblock;
  fhtagn::size_t  m_size;

  segment *       m_start;

  mutable mutex_t m_mutex;
};


}} // namespace fhtagn::memory

#include <fhtagn/memory/detail/fixed_pool.tcc>

#endif // guard
