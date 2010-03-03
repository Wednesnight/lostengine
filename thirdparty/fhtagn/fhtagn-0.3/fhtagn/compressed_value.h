/**
 * $Id: compressed_value.h 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_COMPRESSED_VALUE_H
#define FHTAGN_COMPRESSED_VALUE_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <fhtagn/fhtagn.h>

namespace fhtagn {

/**
 * The compressed_value class provides a thin wrapper around a value type that
 * provides an interface much like a pointer. It will either encapsulate an
 * instance of the value type directly, or a pointer to the value type, picking
 * the smaller of the two so you don't have to.
 *
 * Note that of course the total memory usage of a compresssed_value<T> where
 * sizeof(T) is larger than sizeof(T*) rises, so the usefulness of this class
 * is fairly restricted.
 **/
namespace detail {

template <
  typename T,
  bool VALUE
>
struct compressed_value_holder
{
  inline compressed_value_holder();
  inline compressed_value_holder(T const & t);
  inline ~compressed_value_holder();

  inline T * get();
  inline T const * get() const;

  T m_t;
};

} // namespace detail

template <typename T>
class compressed_value
{
public:
  /**
   * Construction, destruction.
   **/
  inline compressed_value();
  inline compressed_value(T const & t);
  inline ~compressed_value();

  /**
   * Copying.
   **/
  inline compressed_value(compressed_value<T> const & other);
  inline compressed_value & operator=(compressed_value<T> const & other);

  /**
   * Pointer functions
   **/
  inline T * operator->();
  inline T * const operator->() const;

  inline T & operator*();
  inline T const & operator*() const;

private:
  typename detail::compressed_value_holder<
    T,
    sizeof(T) <= sizeof(T*)
  > m_holder;
};

} // namespace fhtagn

#include <fhtagn/detail/compressed_value.tcc>

#endif // guard
