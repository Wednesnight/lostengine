/**
 * $Id: block_pool.tcc 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_MEMORY_DETAIL_BLOCK_POOL_TCC
#define FHTAGN_MEMORY_DETAIL_BLOCK_POOL_TCC

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <sstream>
#include <stdexcept>
#include <cmath>

namespace fhtagn {
namespace memory {

template <
  fhtagn::size_t BLOCK_SIZE,
  typename mutexT,
  typename block_alignmentT,
  template <typename> class adoption_policyT
>
block_pool<BLOCK_SIZE, mutexT, block_alignmentT, adoption_policyT>::block_pool(
    void * memblock, fhtagn::size_t size)
  : adoption_policyT<char>(static_cast<char *>(memblock))
  , m_memblock(memblock)
{
  // We don't really need to know the beginning of the memory block and it's
  // full size; all we need is the block-aligned pointer, and a size that takes
  // into account both a possibly shifted start pointer, and the size as
  // multiples of blocks.
  // |----|----|----|----|  | are block boundaries, - and x are bytes
  // |---x|xxxx|xxxx|xx--|  input memory block
  // |----|xxxx|xxxx|xx--|  memory block with adjusted start
  // |----|xxxx|xxxx|----|  fully adjusted memory block

  void * adjusted_start = block_alignment_t::adjust_pointer(m_memblock);
  fhtagn::size_t size_diff = pointer(adjusted_start).char_ptr
    - pointer(m_memblock).char_ptr;
  fhtagn::size_t adjusted_size = size - size_diff;

  size_diff = adjusted_size % block_alignment_t::BLOCK_SIZE;
  adjusted_size -= size_diff;

  m_memblock = adjusted_start;

  // Now that the memory block we're using is aligned and sized, figure out
  // how much of it we'll be using for metadata. We'll use a fairly primitve
  // algorithm to figure that out:
  // 1. Assume all of the block is for data.
  // 2. Calculate the size of metadata we'd need if we wanted to manage this
  //    assumed amount of data.
  // 3. While the sum of metadata size and assumed data size is larger than
  //    the memory block's size, reduce the assumed data size by one BLOCK_SIZE,
  //    and repeat at 2.

  // Count the assumed data in units of BLOCK_SIZE.
  fhtagn::size_t assumed_size = adjusted_size / BLOCK_SIZE;
  fhtagn::size_t remainder = adjusted_size - (assumed_size * BLOCK_SIZE);
  fhtagn::size_t metadata_size = block_alignment<sizeof(fhtagn::size_t)>::adjust_size(
      static_cast<fhtagn::size_t>(
        std::ceil((assumed_size * 8) / float(BITS_PER_SIZE_T))));
  do {
    --assumed_size;
    remainder = adjusted_size - (assumed_size * BLOCK_SIZE);
    metadata_size = block_alignment<sizeof(fhtagn::size_t)>::adjust_size(
        static_cast<fhtagn::size_t>(
          std::ceil((assumed_size * 8) / float(BITS_PER_SIZE_T))));
  } while (metadata_size > remainder);

  // Throw bad_alloc if we can't split off enough metadata.
  if (!metadata_size) {
    throw std::bad_alloc();
  }

  // On the assumption that BLOCK_SIZE is a multiple of fhtagn::size_t, we'll place
  // the metadata behind the data, because that automatically aligns it properly.
  m_size = assumed_size;
  m_metadata = reinterpret_cast<fhtagn::size_t *>(
      pointer(m_memblock).char_ptr + (m_size * BLOCK_SIZE));
  ::memset(m_metadata, 0, metadata_size);
}



template <
  fhtagn::size_t BLOCK_SIZE,
  typename mutexT,
  typename block_alignmentT,
  template <typename> class adoption_policyT
>
void *
block_pool<BLOCK_SIZE, mutexT, block_alignmentT, adoption_policyT>::alloc(
    fhtagn::size_t size)
{
  if (!size) {
    return NULL;
  }

  if (BLOCK_SIZE != size) {
    return NULL;
  }

  typename mutex_t::scoped_lock lock(m_mutex);

  // Find the segment (described by how many bits fit into a size_t) for the
  // first free block of memory.
  fhtagn::size_t * metadata = m_metadata;
  fhtagn::size_t segment = 0;
  do {
    if (*metadata ^ ~fhtagn::size_t(0)) {
      // Got a metadata where not all bits are set. We can allocate from here!
      break;
    }

    ++metadata;
    ++segment;
  } while (segment < (m_size / BITS_PER_SIZE_T));


  // Within the segment, search backwards (easier/more portable) for a free bit
  fhtagn::size_t mask = 1;
  fhtagn::size_t offs = BITS_PER_SIZE_T;
  do {
    if (!(*metadata & mask)) {
      // Got a zero bit, that's the one we want to allocate.
      break;
    }

    mask <<= 1;
    --offs;
  } while (offs);

  // Translate to index. The offset is (BITS_PER_SIZE_T - offs) because we
  // searched backwards above.
  fhtagn::size_t index = (segment * BITS_PER_SIZE_T) + (BITS_PER_SIZE_T - offs);

  // We *may* have more metadata than data, i.e. if we have a number of entries
  // not divisible by the number of bits in a size_t. So let's first check whether
  // the index we found is valid before.
  if (0 > index || index >= m_size) {
    // Ok, not a valid index, we're out of memory.
    return NULL;
  }

  // Flag metadata as allocated
  *metadata |= mask;

  return pointer(m_memblock).char_ptr + (BLOCK_SIZE * index);
}



template <
  fhtagn::size_t BLOCK_SIZE,
  typename mutexT,
  typename block_alignmentT,
  template <typename> class adoption_policyT
>
void *
block_pool<BLOCK_SIZE, mutexT, block_alignmentT, adoption_policyT>::realloc(
    void * ptr, fhtagn::size_t new_size)
{
  typename mutex_t::scoped_lock lock(m_mutex);

  if (m_memblock > ptr || ptr >= m_metadata) {
    // Invalid pointer, we don't handle it.
    return NULL;
  }

  if (ptr && BLOCK_SIZE == new_size) {
    // Not really a realloc, the size hasn't changed. We can allow that.
    return ptr;
  }
  return NULL;
}



template <
  fhtagn::size_t BLOCK_SIZE,
  typename mutexT,
  typename block_alignmentT,
  template <typename> class adoption_policyT
>
void
block_pool<BLOCK_SIZE, mutexT, block_alignmentT, adoption_policyT>::free(
    void * ptr)
{
  if (!ptr) {
    return;
  }

  typename mutex_t::scoped_lock lock(m_mutex);

  if (m_memblock > ptr || ptr >= m_metadata) {
    // Invalid pointer, we don't handle it.
    return;
  }

  // Calculate index of the ptr into m_memblock.
  fhtagn::size_t index = (pointer(ptr).char_ptr - pointer(m_memblock).char_ptr)
    / BLOCK_SIZE;

  // Translate that into segment and offset. Note that the offset should really
  // be (BITS_PER_SIZE_T - (index % BITS_PER_SIZE_T)), but in order to use it
  // to shift the mask (below), we'd need to invert it yet again.
  fhtagn::size_t segment = index / BITS_PER_SIZE_T;
  fhtagn::size_t offset = index % BITS_PER_SIZE_T;

  fhtagn::size_t mask = 1;
  mask <<= offset;

  // Toggle bit for the pointer to mark it free.
  fhtagn::size_t * metadata = m_metadata + segment;
  *metadata ^= mask;
}



template <
  fhtagn::size_t BLOCK_SIZE,
  typename mutexT,
  typename block_alignmentT,
  template <typename> class adoption_policyT
>
bool
block_pool<BLOCK_SIZE, mutexT, block_alignmentT, adoption_policyT>::in_use() const
{
  typename mutex_t::scoped_lock lock(m_mutex);

  // The pool is in use if any of the metadata items has any bit set. Simple,
  // really.
  fhtagn::size_t * metadata = m_metadata;
  fhtagn::size_t segment = 0;
  do {
    if (*metadata) {
      return true;
    }

    ++metadata;
    ++segment;
  } while (segment < (m_size / BITS_PER_SIZE_T));

  // We survived, the pool must be empty.
  return false;
}



template <
  fhtagn::size_t BLOCK_SIZE,
  typename mutexT,
  typename block_alignmentT,
  template <typename> class adoption_policyT
>
fhtagn::size_t
block_pool<BLOCK_SIZE, mutexT, block_alignmentT, adoption_policyT>::alloc_size(
    void * ptr) const
{
  if (!ptr) {
    return 0;
  }

  typename mutex_t::scoped_lock lock(m_mutex);

  if (m_memblock > ptr || ptr >= m_metadata) {
    // Invalid pointer, we don't handle it.
    return 0;
  }

  return BLOCK_SIZE;
}



}} // namespace fhtagn::memory


#endif // guard
