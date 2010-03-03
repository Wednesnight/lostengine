/**
 * $Id: fixed_pool.tcc 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_MEMORY_DETAIL_FIXED_POOL_TCC
#define FHTAGN_MEMORY_DETAIL_FIXED_POOL_TCC

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <sstream>
#include <stdexcept>

namespace fhtagn {
namespace memory {

template <
  typename mutexT,
  typename block_alignmentT,
  template <typename> class adoption_policyT
>
fixed_pool<mutexT, block_alignmentT, adoption_policyT>::fixed_pool(
    void * memblock, fhtagn::size_t size)
  : adoption_policyT<char>(static_cast<char*>(memblock))
  , m_memblock(memblock)
  , m_size(size)
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
  fhtagn::size_t adjusted_size = m_size - size_diff;

  size_diff = adjusted_size % block_alignment_t::BLOCK_SIZE;
  adjusted_size -= size_diff;

  m_memblock = adjusted_start;
  m_size = adjusted_size;

  // Initialize the memblock with one free list entry spanning the whole block.
  m_start = new (m_memblock) segment(m_size);
}



template <
  typename mutexT,
  typename block_alignmentT,
  template <typename> class adoption_policyT
>
typename fixed_pool<mutexT, block_alignmentT, adoption_policyT>::segment *
fixed_pool<mutexT, block_alignmentT, adoption_policyT>::allocate_segment(
    fhtagn::size_t size)
{
  // We search for a suitable segment in two passes. In the first pass, we try
  // to find segments with exactly the requested size, so that we don't need to
  // fragment our free memory further.
  segment * seg = m_start;
  while (static_cast<char>(segment::ALLOCATED) == seg->status
      || seg->size != size)
  {
    if (segment::LAST_SEGMENT == seg->marker) {
      // Reached the end of the list, end now.
      break;
    }
    seg = seg->next;
  }

  // If the segment we've ended up with is the exact size we want, let's use
  // it!
  if (static_cast<char>(segment::FREE) == seg->status && seg->size == size) {
    seg->status = segment::ALLOCATED;
    return seg;
  }

  // If the segment isn't exactly the size we need, we'll try and find one that's
  // larger, that we can then split in two.
  //
  // The size segment we're looking for is larger than the required size plus
  // a header for the split off part.
  // XXX There's a bit of an edge case here; if the segment we find is exactly
  // full_size below, then on the one hand it'd make no sense to split off a
  // zero-sized segment. On the other hand, doing so
  //   a) keeps the free list accurate (even if it includes zero-sized
  //      segments), and
  //   b) avoids a third pass where we'd choose such a segment anyway because
  //      it's our last chance for finding enough space.
  // For the time being, we'll consider creating zero-sized segments the
  // lesser evil.
  fhtagn::size_t full_size = size + segment::header_size();

  seg = m_start;
  while (static_cast<char>(segment::ALLOCATED) == seg->status
      || seg->size < full_size)
  {
    if (segment::LAST_SEGMENT == seg->marker) {
      break;
    }
    seg = seg->next;
  }

  if (static_cast<char>(segment::ALLOCATED) == seg->status
      || seg->size < full_size)
  {
    // If this is still the case, we didn't find a suitably sized segment and
    // need to give up.
    return NULL;
  }

  // Create a new segment header for the part we split off.
  void * new_seg_addr = pointer(seg).char_ptr + full_size;
  segment * new_seg = new (new_seg_addr) segment(seg->size - size);

  new_seg->next = seg->next;
  seg->next = new_seg;

  seg->status = segment::ALLOCATED;
  seg->size = size;

  return seg;
}




template <
  typename mutexT,
  typename block_alignmentT,
  template <typename> class adoption_policyT
>
void *
fixed_pool<mutexT, block_alignmentT, adoption_policyT>::alloc(
    fhtagn::size_t size)
{
  if (!size) {
    return NULL;
  }

  size = block_alignmentT::adjust_size(size);

  typename mutex_t::scoped_lock lock(m_mutex);

  segment * seg = allocate_segment(size);
  if (!seg) {
    return NULL;
  }

  return pointer(seg).char_ptr + segment::header_size();
}



template <
  typename mutexT,
  typename block_alignmentT,
  template <typename> class adoption_policyT
>
void *
fixed_pool<mutexT, block_alignmentT, adoption_policyT>::realloc(
    void * ptr, fhtagn::size_t new_size)
{
  if (!new_size) {
    return NULL;
  }

  new_size = block_alignmentT::adjust_size(new_size);

  typename mutex_t::scoped_lock lock(m_mutex);

  if (!ptr) {
    segment * seg = allocate_segment(new_size);
    if (!seg) {
      return NULL;
    }

    return pointer(seg).char_ptr + segment::header_size();
  }

  // Find segment for this pointer.
  segment * seg = find_segment_for(ptr);

  // If the new size is smaller than the segment size, that'll mean we can
  // serve the request from the segment itself.
  if (new_size <= seg->size) {
    // If the segment's use shrinks by a segment header size or more, we'll
    // split the segment.
    if (new_size <= seg->size - segment::header_size()) {

      fhtagn::size_t new_seg_size = seg->size - new_size;
      seg->size = new_size;

      void * new_seg_addr = pointer(seg).char_ptr + seg->full_size();
      segment * new_seg = new (new_seg_addr) segment(new_seg_size);

      new_seg->next = seg->next;
      seg->next = new_seg;

      defragment_free_list();
    }
    return pointer(seg).char_ptr + segment::header_size();
  }

  // The best case would be if ptr's segment was followed by a free segment
  // large enough to hold the new size. Given that free segments get
  // defragmented, we only need to check the following segment.
  if (segment::LAST_SEGMENT != seg->marker
      && static_cast<char>(segment::FREE) == seg->next->status)
  {
    // The next segment may be a candidate. Calculate the combined size for
    // both, and check whether that'd be enough.
    fhtagn::size_t combined_size = seg->size + seg->next->full_size();
    if (combined_size >= new_size) {
      // This is awesome, the new size would fit. There are however a few cases
      // here, that need careful handling.
      //
      // a) The combined size is exactly the new size. That's the easiest one.
      if (combined_size == new_size) {
        seg->size = new_size;
        seg->next = seg->next->next;
        return pointer(seg).char_ptr + segment::header_size();
      }

      // b) The combined size is large enough to fit the new_size and a new
      //    segment header, so we can split off unused memory.
      if (combined_size - new_size > segment::header_size()) {
        seg->size = new_size;

        void * new_seg_addr = pointer(seg).char_ptr + seg->full_size();
        segment * new_seg = new (new_seg_addr) segment(combined_size - seg->size);

        new_seg->next = seg->next;
        seg->next = new_seg;

        defragment_free_list();
        return pointer(seg).char_ptr + segment::header_size();
      }

      // c) The combined size is large enough to fit new_size, but not large
      //    enough to also fit a segment header. Given that we can't allocate
      //    *anything* in memory blocks this small, we might as well merge that
      //    as unused memory onto the current segment.
      seg->size = combined_size;
      seg->next = seg->next->next;
      return pointer(seg).char_ptr + segment::header_size();
    }
  }

  // Apparently we could not merge the currently used segment with it's
  // neighbouring one. So let's find some new space for this pointer to point
  // to.
  segment * new_seg = allocate_segment(new_size);
  if (!new_seg) {
    return NULL;
  }

  // If alloc returned a new segment, we'll move over the old data and free the
  // old segment. Since freeing also defragments, there's no need to defragment
  // manually here.
  void * old_data = pointer(seg).char_ptr + segment::header_size();
  void * new_data = pointer(new_seg).char_ptr + segment::header_size();

  ::memcpy(new_data, old_data, std::min(seg->size, new_size));

  // Same as free(), but without an additional lock.
  seg->status = segment::FREE;
  defragment_free_list();

  return new_data;
}



template <
  typename mutexT,
  typename block_alignmentT,
  template <typename> class adoption_policyT
>
typename fixed_pool<mutexT, block_alignmentT, adoption_policyT>::segment *
fixed_pool<mutexT, block_alignmentT, adoption_policyT>::find_segment_for(
    void * ptr) const
{
  void * end = pointer(m_memblock).char_ptr + m_size;
  if (ptr < m_memblock || ptr >= end) {
    std::stringstream s;
    s << "fixed_pool: can't free pointer " << std::hex << ptr
      << ", it's not in pool " << m_memblock << " of size " << std::dec
      << m_size;
    throw std::logic_error(s.str());
  }

  // We'll traverse the whole segment list for safety reasons: this lets us
  // detect whether ptr is pointing to the start of a segment's data, as it
  // always should.
  segment * seg = m_start;

  while (ptr != pointer(seg).char_ptr + segment::header_size()) {
    if (segment::LAST_SEGMENT == seg->marker) {
      break;
    }
    seg = seg->next;
  }

  if (ptr != pointer(seg).char_ptr + segment::header_size()) {
    std::stringstream s;
    s << "fixed_pool: can't free pointer " << std::hex << ptr
      << "; it's in pool " << m_memblock << " of size " << std::dec << m_size
      << " but not in any known segment.";
    throw std::logic_error(s.str());
  }

  return seg;
}



template <
  typename mutexT,
  typename block_alignmentT,
  template <typename> class adoption_policyT
>
void
fixed_pool<mutexT, block_alignmentT, adoption_policyT>::free(void * ptr)
{
  if (!ptr) {
    return;
  }

  typename mutex_t::scoped_lock lock(m_mutex);

  // Find segment for pointer, and mark the memory freed.
  segment * seg = find_segment_for(ptr);
  seg->status = segment::FREE;

  // Lastly, defragment the free list.
  defragment_free_list();
}



template <
  typename mutexT,
  typename block_alignmentT,
  template <typename> class adoption_policyT
>
bool
fixed_pool<mutexT, block_alignmentT, adoption_policyT>::in_use() const
{
  typename mutex_t::scoped_lock lock(m_mutex);

  return (segment::LAST_SEGMENT != m_start->marker);
}



template <
  typename mutexT,
  typename block_alignmentT,
  template <typename> class adoption_policyT
>
void
fixed_pool<mutexT, block_alignmentT, adoption_policyT>::defragment_free_list()
{
  // Since all fragments are in sequence, all we need to do is find two or more
  // free segments in a row, and merge them.
  segment * contiguous = NULL;

  segment * seg = m_start;
  do {

    if (static_cast<char>(segment::FREE) == seg->status) {
      // If there's no current contiguous segment, start one now. That's all
      // we need to do at this point.
      if (!contiguous) {
        contiguous = seg;
      }
      // All other free segments are part of the current contiguous segment
    }

    else if (static_cast<char>(segment::ALLOCATED) == seg->status) {
      // If there's a contiguous segment, then this current segments signals
      // the end of it.
      if (contiguous) {
        if (contiguous->next == seg) {
          // Found a single free segment, no merging necessary or possible.
          contiguous = NULL;
        }
        else {
          // Since the segments are contiguous, all merging is is adjusting
          // the size and next members of the first segment in the list.
          contiguous->next = seg;
          contiguous->size = (pointer(seg).char_ptr
              - pointer(contiguous).char_ptr) - segment::header_size();

          contiguous = NULL;
        }
      }

    }

    // Break after last segment.
    if (segment::LAST_SEGMENT == seg->marker) {
      break;
    }
    seg = seg->next;
  } while (true);

  // If the contiguous segment is set here, that means the segment reaches
  // up to the end of the memory block. Let's merge stuff, then.
  if (contiguous) {
    contiguous->marker = segment::LAST_SEGMENT;
    contiguous->size = ((pointer(m_memblock).char_ptr + m_size)
        - pointer(contiguous).char_ptr) - segment::header_size();
  }
}



template <
  typename mutexT,
  typename block_alignmentT,
  template <typename> class adoption_policyT
>
fhtagn::size_t
fixed_pool<mutexT, block_alignmentT, adoption_policyT>::alloc_size(
    void * ptr) const
{
  if (!ptr) {
    return 0;
  }

  typename mutex_t::scoped_lock lock(m_mutex);
  segment * seg = find_segment_for(ptr);
  return seg->size;
}

}} // namespace fhtagn::memory


#endif // guard
