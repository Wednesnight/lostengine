/**
 * $Id: compressed_value.tcc 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_DETAIL_COMPRESSED_VALUE_TCC
#define FHTAGN_DETAIL_COMPRESSED_VALUE_TCC

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <fhtagn/compressed_value.h>

namespace fhtagn {

namespace detail {

/*****************************************************************************
 * Implementation for compressed_value_holder for when T is held by value
 **/
template <
  typename T,
  bool VALUE
>
compressed_value_holder<T, VALUE>::compressed_value_holder()
  : m_t()
{
}



template <
  typename T,
  bool VALUE
>
compressed_value_holder<T, VALUE>::compressed_value_holder(T const & t)
  : m_t(t)
{
}



template <
  typename T,
  bool VALUE
>
compressed_value_holder<T, VALUE>::~compressed_value_holder()
{
}



template <
  typename T,
  bool VALUE
>
T *
compressed_value_holder<T, VALUE>::get()
{
  return &m_t;
}



template <
  typename T,
  bool VALUE
>
T const *
compressed_value_holder<T, VALUE>::get() const
{
  return &m_t;
}


/*****************************************************************************
 * Specialization for compressed_value_holder for when T is held via a pointer
 **/
template <
  typename T
>
struct compressed_value_holder<T, false>
{
  inline compressed_value_holder()
    : m_t(new T())
  {
  }



  inline compressed_value_holder(T const & t)
    : m_t(new T(t))
  {
  }



  inline ~compressed_value_holder()
  {
    delete m_t;
  }



  inline T * get()
  {
    return m_t;
  }



  inline T const * get() const
  {
    return m_t;
  }



  T * m_t;
};



} // namespace detail

/*****************************************************************************
 * Implementation for compressed_value
 **/
template <typename T>
compressed_value<T>::compressed_value()
  : m_holder()
{
}



template <typename T>
compressed_value<T>::compressed_value(T const & t)
  : m_holder(t)
{
}



template <typename T>
compressed_value<T>::~compressed_value()
{
}



template <typename T>
compressed_value<T>::compressed_value(compressed_value<T> const & other)
  : m_holder(*other.m_holder.get())
{
}



template <typename T>
compressed_value<T> &
compressed_value<T>::operator=(compressed_value<T> const & other)
{
  *m_holder.get() = *other.m_holder.get();
  return *this;
}



template <typename T>
T *
compressed_value<T>::operator->()
{
  return m_holder.get();
}



template <typename T>
T * const
compressed_value<T>::operator->() const
{
  return m_holder.get();
}


template <typename T>
T &
compressed_value<T>::operator*()
{
  return *m_holder.get();
}



template <typename T>
T const &
compressed_value<T>::operator*() const
{
  return *m_holder.get();
}


} // namespace fhtagn

#endif // guard
