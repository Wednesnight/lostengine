/**
 * $Id: dynamic_pool.tcc 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_MEMORY_DETAIL_DYNAMIC_POOL_TCC
#define FHTAGN_MEMORY_DETAIL_DYNAMIC_POOL_TCC

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <sstream>
#include <stdexcept>

namespace fhtagn {
namespace memory {

template <
  typename poolT,
  int MEMORY_BLOCK_SIZE,
  typename mutexT
>
void *
dynamic_pool<poolT, MEMORY_BLOCK_SIZE, mutexT>::alloc(fhtagn::size_t size)
{
  typename mutex_t::scoped_lock lock(m_mutex);
  return lockfree_alloc(size);
}



template <
  typename poolT,
  int MEMORY_BLOCK_SIZE,
  typename mutexT
>
void *
dynamic_pool<poolT, MEMORY_BLOCK_SIZE, mutexT>::lockfree_alloc(
    fhtagn::size_t size)
{

  // Iterate over all pools and try to allocate from each of them. If one
  // succeeds, then we're done.
  typename pool_map_t::const_iterator map_end = m_pool_map.end();
  for (typename pool_map_t::const_iterator map_iter = m_pool_map.begin()
      ; map_iter != map_end ; ++map_iter)
  {
    void * ptr = map_iter->second->alloc(size);
    if (ptr) {
      return ptr;
    }
  }

  // Uh-oh, existing pools don't have enough memory. Seems like we have to
  // create a new one.
  char * memblock = new char[MEMORY_BLOCK_SIZE];
  if (!memblock) {
    return NULL;
  }
  pool_ptr new_pool = pool_ptr(new pool_t(memblock, MEMORY_BLOCK_SIZE));
  if (!new_pool) {
    return NULL;
  }

  // We'll assume that each newly allocated memblock exists at a unique location
  // (a rather safe assumption). Thus that location + size is also unique,
  // making the whole pool_key_t unique. Thus we do not have to worry about the
  // insert below failing.
  pool_key_t key = std::make_pair(pointer(memblock).void_ptr,
      pointer(memblock + MEMORY_BLOCK_SIZE).void_ptr);
  m_pool_map.insert(std::make_pair(key, new_pool));

  // Now we have a new pool, return memory from there. It hardly can fail, but
  // if it does, we can't really help it any longer.
  return new_pool->alloc(size);
}



template <
  typename poolT,
  int MEMORY_BLOCK_SIZE,
  typename mutexT
>
typename dynamic_pool<poolT, MEMORY_BLOCK_SIZE, mutexT>::pool_map_t::iterator
dynamic_pool<poolT, MEMORY_BLOCK_SIZE, mutexT>::find_pool(void * ptr)
{
  typename pool_map_t::iterator map_end = m_pool_map.end();
  for (typename pool_map_t::iterator map_iter = m_pool_map.begin()
      ; map_iter != map_end ; ++map_iter)
  {
    void * range_start = map_iter->first.first;
    void * range_end = map_iter->first.second;
    if (range_start <= ptr && ptr < range_end) {
      // Found the pool.
      return map_iter;
    }
  }

  return map_end;
}



template <
  typename poolT,
  int MEMORY_BLOCK_SIZE,
  typename mutexT
>
typename dynamic_pool<poolT, MEMORY_BLOCK_SIZE, mutexT>::pool_map_t::const_iterator
dynamic_pool<poolT, MEMORY_BLOCK_SIZE, mutexT>::find_pool(void * ptr) const
{
  typename pool_map_t::const_iterator map_end = m_pool_map.end();
  for (typename pool_map_t::const_iterator map_iter = m_pool_map.begin()
      ; map_iter != map_end ; ++map_iter)
  {
    void * range_start = map_iter->first.first;
    void * range_end = map_iter->first.second;
    if (range_start <= ptr && ptr < range_end) {
      // Found the pool.
      return map_iter;
    }
  }

  return map_end;
}



template <
  typename poolT,
  int MEMORY_BLOCK_SIZE,
  typename mutexT
>
void *
dynamic_pool<poolT, MEMORY_BLOCK_SIZE, mutexT>::realloc(void * ptr,
    fhtagn::size_t new_size)
{
  if (!ptr) {
    return alloc(new_size);
  }

  if (!new_size) {
    return NULL;
  }

  typename mutex_t::scoped_lock lock(m_mutex);

  // First, try to find the pool this pointer is from. There's no helping it,
  // we have to iterate over the map.
  typename pool_map_t::iterator iter = find_pool(ptr);

  // If we didn't find a pool above, the pointer isn't managed by us.
  if (iter == m_pool_map.end()) {
    return NULL;
  }
  pool_ptr current_pool = iter->second;

  // See if the current pool can handle the reallocation. If so, we're done.
  void * new_ptr = current_pool->realloc(ptr, new_size);
  if (new_ptr) {
    return new_ptr;
  }

  // Apparently that was not the case, so we'll allocate new memory via our own
  // alloc function. It shouldn't come from the same pool, as realloc() above
  // failed - but we don't really care.
  new_ptr = lockfree_alloc(new_size);
  if (!new_ptr) {
    return NULL;
  }

  // Now copy the contents of the old pointer to the new pointer, then we can
  // free the old pointer.
  ::memcpy(new_ptr, ptr, std::min(current_pool->alloc_size(ptr), new_size));
  current_pool->free(ptr);

  return new_ptr;
}



template <
  typename poolT,
  int MEMORY_BLOCK_SIZE,
  typename mutexT
>
void
dynamic_pool<poolT, MEMORY_BLOCK_SIZE, mutexT>::free(void * ptr)
{
  if (!ptr) {
    return;
  }

  typename mutex_t::scoped_lock lock(m_mutex);

  typename pool_map_t::iterator iter = find_pool(ptr);
  if (iter == m_pool_map.end()) {
    // That's odd, apparently we weren't responsible.
    return;
  }
  pool_ptr current_pool = iter->second;

  current_pool->free(ptr);

  // Finally, if the pool is now empty, remove it.
  if (!current_pool->in_use()) {
    m_pool_map.erase(iter);
  }
}



template <
  typename poolT,
  int MEMORY_BLOCK_SIZE,
  typename mutexT
>
bool
dynamic_pool<poolT, MEMORY_BLOCK_SIZE, mutexT>::in_use() const
{
  typename mutex_t::scoped_lock lock(m_mutex);

  return !m_pool_map.empty();
}



template <
  typename poolT,
  int MEMORY_BLOCK_SIZE,
  typename mutexT
>
fhtagn::size_t
dynamic_pool<poolT, MEMORY_BLOCK_SIZE, mutexT>::alloc_size(void * ptr) const
{
  if (!ptr) {
    return 0;
  }

  typename mutex_t::scoped_lock lock(m_mutex);

  typename pool_map_t::const_iterator iter = find_pool(ptr);
  if (iter == m_pool_map.end()) {
    // That's odd, apparently we weren't responsible.
    return 0;
  }
  pool_ptr pool = iter->second;

  return pool->alloc_size(ptr);
}


}} // namespace fhtagn::memory


#endif // guard
