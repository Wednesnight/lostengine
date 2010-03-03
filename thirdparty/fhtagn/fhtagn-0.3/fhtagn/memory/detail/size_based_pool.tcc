/**
 * $Id: size_based_pool.tcc 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_MEMORY_DETAIL_SIZE_BASED_POOL_TCC
#define FHTAGN_MEMORY_DETAIL_SIZE_BASED_POOL_TCC

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

namespace fhtagn {
namespace memory {



template <
  fhtagn::size_t OBJECTS_PER_POOL,
  fhtagn::size_t MIN_OBJECT_SIZE,
  fhtagn::size_t MAX_OBJECT_SIZE,
  template <int> class incrementorT,
  typename mutexT
>
size_based_pool<OBJECTS_PER_POOL, MIN_OBJECT_SIZE, MAX_OBJECT_SIZE,
    incrementorT, mutexT>::size_based_pool()
{
  // Multiplied with 2 because static_for iterates over [begin, end)
  ::fhtagn::meta::static_for<MIN_OBJECT_SIZE, MAX_OBJECT_SIZE * 2, incrementorT,
    pool_creator>(m_pool_map);
}



template <
  fhtagn::size_t OBJECTS_PER_POOL,
  fhtagn::size_t MIN_OBJECT_SIZE,
  fhtagn::size_t MAX_OBJECT_SIZE,
  template <int> class incrementorT,
  typename mutexT
>
fhtagn::size_t
size_based_pool<OBJECTS_PER_POOL, MIN_OBJECT_SIZE, MAX_OBJECT_SIZE,
    incrementorT, mutexT>::pool_size(fhtagn::size_t in_size) const
{
  fhtagn::size_t s = MIN_OBJECT_SIZE;
  for ( ; s < (MAX_OBJECT_SIZE * 2) ; s *= 2) {
    if (s >= in_size) {
      break;
    }
  }
  return s > MAX_OBJECT_SIZE ? 0 : s;
}



template <
  fhtagn::size_t OBJECTS_PER_POOL,
  fhtagn::size_t MIN_OBJECT_SIZE,
  fhtagn::size_t MAX_OBJECT_SIZE,
  template <int> class incrementorT,
  typename mutexT
>
void *
size_based_pool<OBJECTS_PER_POOL, MIN_OBJECT_SIZE, MAX_OBJECT_SIZE,
    incrementorT, mutexT>::alloc(fhtagn::size_t size)
{
  if (!size) {
    return NULL;
  }


  typename mutexT::scoped_lock lock(m_mutex);

  fhtagn::size_t adjusted_size = pool_size(size);

  if (adjusted_size) {
    void * result = m_pool_map[adjusted_size]->alloc(adjusted_size);
    if (result) {
      m_pointer_size_map[result] = adjusted_size;
    }
    return result;
  }

  void * result = m_heap.alloc(size);
  if (result) {
    m_pointer_size_map[result] = size;
  }
  return result;
}



template <
  fhtagn::size_t OBJECTS_PER_POOL,
  fhtagn::size_t MIN_OBJECT_SIZE,
  fhtagn::size_t MAX_OBJECT_SIZE,
  template <int> class incrementorT,
  typename mutexT
>
void *
size_based_pool<OBJECTS_PER_POOL, MIN_OBJECT_SIZE, MAX_OBJECT_SIZE,
    incrementorT, mutexT>::realloc(void * ptr, fhtagn::size_t new_size)
{
  if (!ptr) {
    return alloc(new_size);
  }

  if (!new_size) {
    return NULL;
  }

  fhtagn::size_t adjusted_size = pool_size(new_size);

  typename mutexT::scoped_lock lock(m_mutex);

  pointer_size_map_t::iterator iter = m_pointer_size_map.find(ptr);
  if (iter == m_pointer_size_map.end()) {
    return NULL;
  }

  if (iter->second == adjusted_size) {
    // No reallocation required, the backing store is the same size as before.
    return ptr;
  }

  // If we've reached here, there's not much helping it: we'll need to allocate
  // new memory and memcpy.
  void * new_ptr = NULL;
  if (adjusted_size) {
    // Allocate
    new_ptr = m_pool_map[adjusted_size]->alloc(adjusted_size);
    if (!new_ptr) {
      return NULL;
    }
    m_pointer_size_map[new_ptr] = adjusted_size;

    // Memcopy
    ::memcpy(new_ptr, ptr, std::min(iter->second, adjusted_size));
  }
  else {
    // Allocate
    new_ptr = m_heap.alloc(new_size);
    if (!new_ptr) {
      return NULL;
    }
    m_pointer_size_map[new_ptr] = new_size;

    // Memcopy
    ::memcpy(new_ptr, ptr, std::min(iter->second, new_size));
  }

  // Now release the old pointer.
  fhtagn::size_t old_size = iter->second;
  m_pointer_size_map.erase(iter);

  fhtagn::size_t old_adjusted_size = pool_size(old_size);
  if (old_size == old_adjusted_size) {
    m_pool_map[old_size]->free(ptr);
  }
  else {
    m_heap.free(ptr);
  }

  return new_ptr;
}



template <
  fhtagn::size_t OBJECTS_PER_POOL,
  fhtagn::size_t MIN_OBJECT_SIZE,
  fhtagn::size_t MAX_OBJECT_SIZE,
  template <int> class incrementorT,
  typename mutexT
>
void
size_based_pool<OBJECTS_PER_POOL, MIN_OBJECT_SIZE, MAX_OBJECT_SIZE,
    incrementorT, mutexT>::free(void * ptr)
{
  if (!ptr) {
    return;
  }

  typename mutexT::scoped_lock lock(m_mutex);

  pointer_size_map_t::const_iterator iter = m_pointer_size_map.find(ptr);
  if (iter == m_pointer_size_map.end()) {
    // Not managed by us
    return;
  }

  fhtagn::size_t adjusted_size = pool_size(iter->second);
  if (iter->second == adjusted_size) {
    m_pool_map[iter->second]->free(ptr);
  }
  else {
    m_heap.free(ptr);
  }

  m_pointer_size_map.erase(ptr);
}



template <
  fhtagn::size_t OBJECTS_PER_POOL,
  fhtagn::size_t MIN_OBJECT_SIZE,
  fhtagn::size_t MAX_OBJECT_SIZE,
  template <int> class incrementorT,
  typename mutexT
>
bool
size_based_pool<OBJECTS_PER_POOL, MIN_OBJECT_SIZE, MAX_OBJECT_SIZE,
    incrementorT, mutexT>::in_use() const
{
  typename mutexT::scoped_lock lock(m_mutex);
  return !m_pointer_size_map.empty();
}



template <
  fhtagn::size_t OBJECTS_PER_POOL,
  fhtagn::size_t MIN_OBJECT_SIZE,
  fhtagn::size_t MAX_OBJECT_SIZE,
  template <int> class incrementorT,
  typename mutexT
>
fhtagn::size_t
size_based_pool<OBJECTS_PER_POOL, MIN_OBJECT_SIZE, MAX_OBJECT_SIZE,
    incrementorT, mutexT>::alloc_size(void * ptr) const
{
  typename mutexT::scoped_lock lock(m_mutex);

  pointer_size_map_t::const_iterator iter = m_pointer_size_map.find(ptr);
  if (iter == m_pointer_size_map.end()) {
    // Pointer not managed here.
    return 0;
  }

  fhtagn::size_t adjusted_size = pool_size(iter->second);
  if (iter->second != adjusted_size) {
    return m_heap.alloc_size(ptr);
  }

  // Can return 0 if the pool does not manage the ptr - that'd be a bug, though.
  typename pool_map_t::const_iterator pool_iter = m_pool_map.find(iter->second);
  if (pool_iter == m_pool_map.end()) {
    // Pretty much impossible.
    throw std::bad_alloc();
  }
  fhtagn::size_t ret = pool_iter->second->alloc_size(ptr);
  assert(ret);
  return ret;
}

}} // namespace fhtagn::memory


#endif // guard
