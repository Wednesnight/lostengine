/**
 * $Id: size_based_pool.h 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_MEMORY_SIZE_BASED_POOL_H
#define FHTAGN_MEMORY_SIZE_BASED_POOL_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <new>

#include <fhtagn/fhtagn.h>

#include <fhtagn/memory/memory_pool.h>
#include <fhtagn/memory/dynamic_pool.h>
#include <fhtagn/memory/block_pool.h>
#include <fhtagn/meta/for.h>

namespace fhtagn {
namespace memory {

/**
 * The size_based_pool class is inspired by Python's memory allocator. Both
 * create pools for objects of different sizes, with sizes doubling up to a
 * maximum size - there'll be a pool for objects of size 1, 2, 4, 8, 16, etc.
 *
 * Objects of a non-power-of-two size are allocated from the pool for objects
 * for the next larger power-of-two size, e.g. objects of size 5 would be
 * alloced from the pool for objects of size 8.
 *
 * Each pool is a fixed number of items in size, and conceptually identical to
 * block_pool (this implementation in fact uses block_pool). If the pool runs
 * out of space, it's available memory is extended (this implementation uses
 * dynamic_pool wrapped around block_pool).
 *
 * If an object is larger than the largest pool size accommodates, it's
 * allocated directly from the heap (this implementation uses heap_pool).
 *
 * With this implementation, you can specify the minimum object size, the
 * maximum object size, an incrementor (see fhtagn/meta/for.h for details)
 * and the number of objects per each pool grows by if it runs out of space -
 * at compile time.
 *
 * Note that due to implementation details of block_pool, the OBJECTS_PER_POOL
 * number is approximate; there'll usually be a few less objects available per
 * pool than the specified number.
 **/
template <
  fhtagn::size_t OBJECTS_PER_POOL = 256,
  fhtagn::size_t MIN_OBJECT_SIZE = 1,
  fhtagn::size_t MAX_OBJECT_SIZE = 256,
  template <int> class incrementorT = ::fhtagn::meta::multi_double,
  typename mutexT = ::fhtagn::threads::fake_mutex
>
struct size_based_pool
{
  typedef mutexT mutex_t;

  inline size_based_pool();

  inline void * alloc(fhtagn::size_t size);
  inline void * realloc(void * ptr, fhtagn::size_t new_size);
  inline void free(void * ptr);
  inline bool in_use() const;
  inline fhtagn::size_t alloc_size(void * ptr) const;

private:

  struct virtual_pool_base
  {
    virtual ~virtual_pool_base() {};

    virtual void * alloc(fhtagn::size_t size) = 0;
    virtual void * realloc(void * ptr, fhtagn::size_t new_size) = 0;
    virtual void free(void * ptr) = 0;
    virtual bool in_use() const = 0;
    virtual fhtagn::size_t alloc_size(void * ptr) const = 0;
  };

  template <fhtagn::size_t OBJECT_SIZE>
  struct virtual_pool : virtual_pool_base
  {
    virtual ~virtual_pool() {};

    virtual void * alloc(fhtagn::size_t size)
    {
      return m_pool.alloc(size);
    }

    virtual void * realloc(void * ptr, fhtagn::size_t new_size)
    {
      return m_pool.realloc(ptr, new_size);
    }

    virtual void free(void * ptr)
    {
      return m_pool.free(ptr);
    }

    virtual bool in_use() const
    {
      return m_pool.in_use();
    }

    virtual fhtagn::size_t alloc_size(void * ptr) const
    {
      return m_pool.alloc_size(ptr);
    }

    ::fhtagn::memory::dynamic_pool<
      ::fhtagn::memory::block_pool<OBJECT_SIZE>,
      OBJECT_SIZE * OBJECTS_PER_POOL
    > m_pool;
  };


  typedef fhtagn::shared_ptr<virtual_pool_base>       virtual_pool_ptr;
  typedef std::map<fhtagn::size_t, virtual_pool_ptr>  pool_map_t;
  typedef std::map<void *, fhtagn::size_t>            pointer_size_map_t;

  template <int I>
  struct pool_creator
  {
    void operator()(pool_map_t & map)
    {
      map[I] = virtual_pool_ptr(new virtual_pool<I>());
    }
  };

  inline fhtagn::size_t pool_size(fhtagn::size_t in_size) const;


  pool_map_t          m_pool_map;
  heap_pool           m_heap;

  pointer_size_map_t  m_pointer_size_map;

  mutable mutex_t     m_mutex;
};


}} // namespace fhtagn::memory

#include <fhtagn/memory/detail/size_based_pool.tcc>

#endif // guard
