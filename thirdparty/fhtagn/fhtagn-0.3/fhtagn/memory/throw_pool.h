/**
 * $Id: throw_pool.h 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_MEMORY_THROW_POOL_H
#define FHTAGN_MEMORY_THROW_POOL_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <new>

#include <fhtagn/fhtagn.h>

#include <fhtagn/memory/memory_pool.h>

namespace fhtagn {
namespace memory {

/**
 * The throw_pool class is a simple wrapper around a MemoryPool pool, that
 * catches when NULL is returned from alloc or realloc, and throws an
 * exception.
 *
 * The reason behind this is that while it's common for the C-style malloc/
 * realloc functions on which the MemoryPool is modelled to return NULL in
 * out-of-memory conditions (or on other errors), C++ code using STL-style
 * allocators will expect an exception.
 **/
template <
  typename memory_poolT
>
struct throw_pool
{
  BOOST_CLASS_REQUIRE(memory_poolT, ::fhtagn::memory::concepts,
      MemoryPoolConcept);


  throw_pool(memory_poolT & pool)
    : m_pool(pool)
  {
  }



  inline void * alloc(fhtagn::size_t size)
  {
    void * ret = m_pool.alloc(size);
    if (!ret) {
      throw std::bad_alloc();
    }
    return ret;
  }



  inline void * realloc(void * ptr, fhtagn::size_t new_size)
  {
    void * ret = m_pool.realloc(ptr, new_size);
    if (!ret) {
      throw std::bad_alloc();
    }
    return ret;
  }



  inline void free(void * ptr)
  {
    return m_pool.free(ptr);
  }



  inline bool in_use() const
  {
    return m_pool.in_use();
  }



  inline fhtagn::size_t alloc_size(void * ptr) const
  {
    return m_pool.alloc_size(ptr);
  }

private:

  memory_poolT &  m_pool;
};


}} // namespace fhtagn::memory

#endif // guard
