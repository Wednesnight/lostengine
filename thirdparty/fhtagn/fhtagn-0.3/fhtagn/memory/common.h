/**
 * $Id: common.h 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_MEMORY_COMMON_H
#define FHTAGN_MEMORY_COMMON_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <fhtagn/fhtagn.h>

namespace fhtagn {
namespace memory {

/**
 * Used in both block_pool and fixed_pool: two policies that either adopt a
 * pointer (that is, delete it in the dtor) or not.
 **/
template <typename T>
struct adopt_policy
{
  inline adopt_policy(T * obj)
    : m_obj(obj)
  {
  }

  inline ~adopt_policy()
  {
    delete m_obj;
  }

  T * m_obj;
};



template <typename T>
struct adopt_array_policy
{
  inline adopt_array_policy(T * array)
    : m_array(array)
  {
  }

  inline ~adopt_array_policy()
  {
    delete [] m_array;
  }

  T * m_array;
};



template <typename T>
struct ignore_policy
{
  inline ignore_policy(T *)
  {
  }
};



template <typename T>
struct ignore_array_policy
{
  inline ignore_array_policy(T *)
  {
  }
};

}} // namespace fhtagn::memory

#endif // guard
