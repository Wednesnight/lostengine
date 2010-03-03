/**
 * $Id: fhtagn.h 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_H
#define FHTAGN_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

#include <fhtagn/fhtagn-config.h>

#ifndef HAVE_BOOST_CSTDINT_HPP
#error Fhtagn! requires definitions for standard integer types
#endif
#include <boost/cstdint.hpp>

namespace fhtagn {

/**
 * Define size_t again, just to have a ssize_t and size_t in the same
 * namespace
 **/
typedef ::size_t size_t;

/**
 * Define ssize_t, if it doesn't exist yet.
 **/
namespace detail {

template <int SIZE_T_SIZE>
struct ssize_t_size_detector
{
};

template <>
struct ssize_t_size_detector<2>
{
  typedef boost::int16_t ssize_t;
};

#if defined(HAVE_BOOST__INT32_T)
template <>
struct ssize_t_size_detector<4>
{
  typedef boost::int32_t ssize_t;
};
#endif

#if defined(HAVE_BOOST__INT64_T)
template <>
struct ssize_t_size_detector<8>
{
  typedef boost::int32_t ssize_t;
};
#endif

} // namespace detail

typedef detail::ssize_t_size_detector<sizeof(size_t)>::ssize_t ssize_t;

} // namespace fhtagn

#endif // guard
