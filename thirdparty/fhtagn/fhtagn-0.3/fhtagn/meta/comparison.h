/**
 * $Id: comparison.h 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_META_COMPARISON_H
#define FHTAGN_META_COMPARISON_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <fhtagn/fhtagn.h>

namespace fhtagn {
namespace meta {

/**
 * This file defines constructs for comparing integers at compile-time. Usage
 * of all these constructs follows the same basic pattern, e.g.:
 *
 *    bool b = compare_construct<42, 666>::value;
 *
 * Depending on the comparison performed by the construct, the resulting value
 * evaluates either to true or to false.
 **/
template <int I1, int I2>
struct equal
{
  enum {
    value = (I1 == I2),
  };
};



template <int I1, int I2>
struct not_equal
{
  enum {
    value = (I1 != I2),
  };
};



template <int I1, int I2>
struct greater
{
  enum {
    value = (I1 > I2),
  };
};



template <int I1, int I2>
struct less
{
  enum {
    value = (I1 < I2),
  };
};



template <int I1, int I2>
struct greater_equal
{
  enum {
    value = (I1 >= I2),
  };
};



template <int I1, int I2>
struct less_equal
{
  enum {
    value = (I1 <= I2),
  };
};

}} // namespace fhtagn::meta


#endif // guard
