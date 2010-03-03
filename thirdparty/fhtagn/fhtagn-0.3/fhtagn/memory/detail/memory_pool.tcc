/**
 * $Id: memory_pool.tcc 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_MEMORY_DETAIL_MEMORY_POOL_TCC
#define FHTAGN_MEMORY_DETAIL_MEMORY_POOL_TCC

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#if defined(HAVE_MALLOC_SIZE)
#include <malloc/malloc.h>
#endif

namespace fhtagn {
namespace memory {

void *
heap_pool::alloc(fhtagn::size_t size)
{
  if (!size) {
    return NULL;
  }

  return ::malloc(size);

}



void *
heap_pool::realloc(void * ptr, fhtagn::size_t new_size)
{
  if (!new_size) {
    return NULL;
  }

  return ::realloc(ptr, new_size);
}



void
heap_pool::free(void * ptr)
{
  if (!ptr) {
    return;
  }

  ::free(ptr);
}



bool
heap_pool::in_use() const
{
  return true;
}



fhtagn::size_t
heap_pool::alloc_size(void * ptr) const
{
  if (!ptr) {
    return 0;
  }

#if defined(HAVE_MALLOC_SIZE)
  return malloc_size(ptr);
#else
  return 0;
#endif
}


}} // namespace fhtagn::memory


#endif // guard
