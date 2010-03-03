/**
 * $Id: stdvariant.h 319 2009-06-08 22:17:47Z unwesen $
 *
 * This file is part of the Fhtagn! C++ Library.
 * Copyright (C) 2009 Jens Finkhaeuser <unwesen@users.sourceforge.net>.
 *
 * Author: Henning Pfeiffer <slashgod@users.sourceforge.net>
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
#ifndef FHTAGN_STDVARIANT_H
#define FHTAGN_STDVARIANT_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <fhtagn/fhtagn.h>

#include <string>

#include <fhtagn/variant.h>

// specialization for builtin bool type
FHTAGN_VARIANT_SPECIALIZE(bool)

// specializations for builtin integer types
FHTAGN_VARIANT_SPECIALIZE(boost::int8_t)
FHTAGN_VARIANT_SPECIALIZE(boost::uint8_t)
FHTAGN_VARIANT_SPECIALIZE(boost::int16_t)
FHTAGN_VARIANT_SPECIALIZE(boost::uint16_t)
#if defined(HAVE_BOOST__INT32_T)
FHTAGN_VARIANT_SPECIALIZE(boost::int32_t)
FHTAGN_VARIANT_SPECIALIZE(boost::uint32_t)
#endif
#if defined(HAVE_BOOST__INT64_T)
FHTAGN_VARIANT_SPECIALIZE(boost::int64_t)
FHTAGN_VARIANT_SPECIALIZE(boost::uint64_t)
#endif

#ifdef _WIN32
// On win32, int appears to be none of the above
FHTAGN_VARIANT_SPECIALIZE(signed int)
FHTAGN_VARIANT_SPECIALIZE(unsigned int)
#endif


// floating point types
FHTAGN_VARIANT_SPECIALIZE(float)
FHTAGN_VARIANT_SPECIALIZE(double)

// std::string may not be a builtin type, but it's often used
FHTAGN_VARIANT_SPECIALIZE(std::string)
FHTAGN_VARIANT_SPECIALIZE_HOLDER(std::string::value_type *, std::string)
FHTAGN_VARIANT_SPECIALIZE_HOLDER(std::string::value_type const *, std::string)
FHTAGN_VARIANT_SPECIALIZE_HOLDER(std::string::value_type const * const, std::string)

// This is a specialization for character arrays of varying lengths, such as
// string literals. I've no idea how portable this is, but on my GCC, this works
// like a charm.
// i686-apple-darwin8-gcc-4.0.1 (GCC) 4.0.1 (Apple Computer, Inc. build 5363)
namespace fhtagn {
    template <int N>
    struct variant::specialization_traits<char [N]>
    {
        typedef std::string holder_type;
    };

    template <int N>
    struct variant::specialization_traits<const char [N]>
    {
        typedef std::string holder_type;
    };

} // namespace fhtagn

#endif // guard
