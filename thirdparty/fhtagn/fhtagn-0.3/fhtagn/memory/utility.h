/**
 * $Id: utility.h 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_MEMORY_UTILITY_H
#define FHTAGN_MEMORY_UTILITY_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <fhtagn/fhtagn.h>

namespace fhtagn {
namespace memory {


/**
 * Utility class; adjusts size_t and pointers according to a block size.
 *
 * The size_t adjustment increases the size_t parameter until it is a multiple
 * of the block size.
 *
 * The adjustment for pointers advances the pointer until it lies on a block
 * boundary.
 **/
template <
  fhtagn::size_t T_BLOCK_SIZE = sizeof(fhtagn::size_t)
>
struct block_alignment
{
  enum {
    BLOCK_SIZE = T_BLOCK_SIZE,
  };

  static inline fhtagn::size_t adjust_size(fhtagn::size_t size)
  {
    fhtagn::size_t remainder = size % BLOCK_SIZE;
    if (!remainder) {
      return size;
    }

    return size - remainder + BLOCK_SIZE;
  }

  static inline void * adjust_pointer(void * ptr)
  {
    return reinterpret_cast<void *>(
        adjust_size(reinterpret_cast<fhtagn::size_t>(ptr)));
  }
};



/**
 * Helper struct for conveniently intepreting void * as char * and vice
 * versa.
 **/
struct pointer
{
  pointer(void * ptr)
  {
    void_ptr = ptr;
  }

  union {
    void * void_ptr;
    char * char_ptr;
  };
};


}} // namespace fhtagn::memory

#endif // guard
