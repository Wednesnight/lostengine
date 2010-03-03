/**
 * $Id: shared_ptr.h 319 2009-06-08 22:17:47Z unwesen $
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

#ifndef FHTAGN_SHARED_PTR_H
#define FHTAGN_SHARED_PTR_H

/**
 * Defines the following classes, either from the TR1 namespace (if available),
 * or from boost (if availble).
 *
 * It makes sense to use Fhtagn's definitions (if you're using Fhtagn! anyway),
 * as they'll use defintions from the std namespace as they become available.
 **/


// Prefer the TR1 definition
#if defined(HAVE_TR1_MEMORY)

#include <tr1/memory>
namespace fhtagn {
using std::tr1::shared_ptr;
using std::tr1::weak_ptr;
using std::tr1::enable_shared_from_this;

using std::tr1::static_pointer_cast;
using std::tr1::dynamic_pointer_cast;
using std::tr1::const_pointer_cast;

} // namespace fhtagn

#else // HAVE_TR1_MEMORY

// Only if tr1::shared_ptr is not available, use boost::shared_ptr
#if defined(HAVE_BOOST_SHARED_PTR_HPP)

#include <boost/shared_ptr.hpp>
#include <boost/pointer_cast.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace fhtagn {
using boost::shared_ptr;
using boost::weak_ptr;
using boost::enable_shared_from_this;

using boost::static_pointer_cast;
using boost::dynamic_pointer_cast;
using boost::const_pointer_cast;

} // namespace fhtagn

#else // HAVE_BOOST_SHARED_PTR_HPP

// Can't really build this, error out.
#error Neither tr1::shared_ptr nor boost::shared_ptr available.

#endif // HAVE_BOOST_SHARED_PTR_HPP

#endif // HAVE_TR1_MEMORY

#endif // guard
